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
    // Tighter stopping criterion in the initialization run (james2009cooperative, p. 814)
    maxFailures{ *settings->maxFailures / 100 },
    mutex{ argMutex },
    problem{ argProblem },
    referenceSet{ argReferenceSet },
    tabooTenure{ GetTabooTenure( problem->size ) },
    tabooTenureCounter{ tabooTenure },
    tabooTenures{ problem->size, 0 }
{
    // std::cout << "      Constructing TSThread with id " << index << std::endl;
}

void mt::TSThread::CleanUpMatrix( mt::Matrix< dSol > &argMatrix ) const {
    for ( unsigned long i = 0; i < problem->size; ++i ) {
        for ( unsigned long j = i + 1; j < problem->size; ++j ) {
            delete argMatrix( i, j ).second;
        }
    }
}

mt::SolutionBase *mt::TSThread::GetBestNeigh( double &argBestNeighV,
                                              const std::unique_ptr< const SolutionBase > &argTempSol ) {
    mt::Matrix< dSol >costs =
            { problem->size, dSol{ std::numeric_limits< double >::max(), nullptr } };
    // Iterate over all lines
    for ( unsigned long i = 0; i < problem->size; ++i ) {
        // Iterate over all columns above the main diagonal (because swaps are symmetrical)
        for ( unsigned long j = i + 1; j < problem->size; ++j ) {
            mt::SolutionBase *tempSolution =  argTempSol->GetSwappedVariant( i, j );
            costs( i, j ) = dSol{ problem->GetOFV( tempSolution ), tempSolution };
        }
    }

    // Get the local or global minimum found so far
    double minimumSoFar = std::numeric_limits< double >::max();
    if ( *settings->globalBestAspiration ) {
        std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
        minimumSoFar = referenceSet.GetGlobalMinimumSolV();
    } else {
        std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
        minimumSoFar = referenceSet.GetLocalMinimumSolV( index );
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
             && argBestNeighV > minimumSoFar ) {
            costs( swapI, swapJ ).first = std::numeric_limits< double >::max();
        } else {
            // Taillard's taboo evaluation => constant time
            mt::SolutionBase *bestNeigh = argTempSol->GetSwappedVariant( swapI, swapJ );
            CleanUpMatrix( costs );
            unsigned int tabooFinish = iterationCount + tabooTenure;
            problem->UpdateTabooTenures( bestNeigh, swapI, swapJ, tabooFinish, tabooTenures );
            return bestNeigh;
        }
    }
}

void mt::TSThread::Iteration() {
    ++iterationCount;

    // Taboo tenure shuffling as proposed by taillard1991robust, p. 448
    --tabooTenureCounter;
    if ( !tabooTenureCounter && *settings->randomizedTabooTenures && *settings->tabooTenureShuffling ) {
        tabooTenure = GetTabooTenure( problem->size );
        tabooTenureCounter = tabooTenure;
    }

    std::unique_ptr< const SolutionBase > tempSol = nullptr;
    double tempSolV = 0.0;
    {
        std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
        tempSol.reset( referenceSet.GetStartSolution( index ) );
        tempSolV = referenceSet.GetStartSolutionValue( index );
    }

    double bestNeighV = std::numeric_limits< double >::max();
    mt::SolutionBase *bestNeigh = GetBestNeigh( bestNeighV, tempSol );

    if ( !bestNeigh ) {
        ++failures;

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

void mt::TSThread::PrepareOptimizationRun() {
    // These settings are for the optimization run (after the initialization run)
    failures = 0;
    finished = false;
    iterationCount = 0;
    // Tighter stopping criterion in the initialization run (james2009cooperative, p. 814)
    maxFailures *= 100;
    tabooTenures.ResetWithValue( 0 );
}
