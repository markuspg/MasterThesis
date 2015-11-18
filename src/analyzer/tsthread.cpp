/*
 * Copyright 2015 Markus Prasser
 *
 * This file is part of MasterThesis.
 *
 *  MasterThesis is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MasterThesis is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with MasterThesis.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tsthread.h"

mt::TSThread::TSThread( const unsigned short &argIndex, std::mutex &argMutex,
                        const mt::Problem * const argProblem, mt::TSReferenceSet &argReferenceSet ) :
    index{ argIndex },
    maxFailures{ *settings->maxFailures / 100 },
    mutex{ argMutex },
    problem{ argProblem },
    referenceSet{ argReferenceSet },
    tabooTenure{ mt::GetTabooTenure( referenceSet.problem->size ) },
    tabooTenures{ referenceSet.problem->size, 0 }
{
    std::cout << "      Constructing TSThread with id " << index << std::endl;
}

void mt::TSThread::CleanUpMatrix( mt::Matrix< dSol > *argMatrix ) const {
    for ( unsigned long i = 0; i < referenceSet.problem->size; ++i ) {
        for ( unsigned long j = i + 1; j < referenceSet.problem->size; ++j ) {
            delete ( *argMatrix )( i, j ).second;
        }
    }
}

mt::SolutionBase *mt::TSThread::GetBestNeigh( double &argBestNeighV,
                                              mt::SolutionBase *argTempSol ) {
    mt::Matrix< dSol >costs =
            { referenceSet.problem->size, dSol{ std::numeric_limits< double >::max(), nullptr } };
    // Iterate over all lines
    for ( unsigned long i = 0; i < referenceSet.problem->size; ++i ) {
        // Iterate over all columns above the main diagonal (because swaps are symmetrical)
        for ( unsigned long j = i + 1; j < referenceSet.problem->size; ++j ) {
            mt::SolutionBase *tempSolution =  argTempSol->GetSwappedVariant( i, j );
            costs( i, j ) = dSol{ referenceSet.problem->GetOFV( tempSolution ), tempSolution };
        }
    }
    // Analyze all swaps
    long swapI = 0, swapJ = 0;
    while ( true ) {
        // taillard1991robust, p. 445
        argBestNeighV = costs.GetMinimumValueIndizes( swapI, swapJ, []( dSol a, dSol b ){ return a.first < b.first; } ).first;
        // If no improvement could be achieved, leave with false, indicating improvement failure
        if ( argBestNeighV == std::numeric_limits< double >::max() ) {
            return nullptr;
        }
        // If a swap is taboo, exclude it from consideration by settings its cost to the maximum double value
        if ( problem->CheckIfTaboo( iterationCount, costs( swapI, swapJ ).second, swapI, swapJ, tabooTenures )
             // New global optimum as aspiration criterion
             && argBestNeighV > referenceSet.GetGlobalMinimumSolV() ) {
            costs( swapI, swapJ ).first = std::numeric_limits< double >::max();
        } else {
            // Taillard's taboo evaluation => constant time
            mt::SolutionBase *bestNeigh = argTempSol->GetSwappedVariant( swapI, swapJ );
            CleanUpMatrix( &costs );
            unsigned int tabooFinish = iterationCount + tabooTenure;
            problem->UpdateTabooTenures( bestNeigh, swapI, swapJ, tabooFinish, tabooTenures );
            return bestNeigh;
        }
    }
}

void mt::TSThread::Iteration() {
    ++iterationCount;
    mt::SolutionBase *tempSol = nullptr;
    double tempSolV = 0.0;
    {
        std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
        tempSol = referenceSet.GetStartSolution( index );
        tempSolV = referenceSet.GetStartSolutionValue( index );
    }

    double bestNeighV = std::numeric_limits< double >::max();
    mt::SolutionBase *bestNeigh = GetBestNeigh( bestNeighV, tempSol );

    if ( !bestNeigh ) {
        ++failures;
        ++invalidSolutions;

        std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
        referenceSet.SetSolution( index, nullptr, 0.0 );
    } else {
        if ( bestNeighV >= tempSolV ) {
            ++failures;
        }

        {
            std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
            referenceSet.SetSolution( index, bestNeigh, bestNeighV );
        }
    }

    if ( failures >= maxFailures ) {
        finished = true;

        std::stringstream measureStream;
        measureStream << index << ':' << iterationCount;
        std::string measureString = measureStream.str();
        std::lock_guard< std::mutex > lockMeasure{ measureMutex };
        measure.AddTSThreadIterations( measureString );
    }
}

void mt::TSThread::PrepareSecondTSRun() {
    // These options are for the second run (after the initialization run)
    failures = 0;
    finished = false;
    invalidSolutions = 0;
    iterationCount = 0;
    maxFailures *= 100;
    tabooTenures.ResetWithValue( 0 );
}
