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
                        mt::TSReferenceSet &argReferenceSet ) :
    index{ argIndex },
    maxFailures{ *settings->maxFailures / 100 },
    mutex{ argMutex },
    referenceSet{ argReferenceSet },
    tabooTenure{ mt::GetTabooTenure( referenceSet.problem->size ) },
    tabooTenures{ referenceSet.problem->size, 0 }
{
    std::cout << "      Constructing TSThread with id " << index << std::endl;
}

mt::RandomKeySolution *mt::TSThread::GetBestNeigh( double &argBestNeighV,
                                                   mt::RandomKeySolution *argTempSol ) {
    mt::Matrix< double > costs{ referenceSet.problem->size, std::numeric_limits< double >::max() };
    // Iterate over all lines
    for ( unsigned long i = 0; i < referenceSet.problem->size; ++i ) {
        // Iterate over all columns above the main diagonal (because swaps are symmetrical)
        for ( unsigned long j = i + 1; j < referenceSet.problem->size; ++j ) {
            mt::RandomKeySolution *tempSolution =  argTempSol->GetSwappedVariant( i, j );
            costs( i, j ) = referenceSet.problem->GetOFV( tempSolution );
            delete tempSolution;
        }
    }
    // Analyze all swaps
    long swapI = 0, swapJ = 0;
    while ( true ) {
        argBestNeighV = costs.GetMinimumValueIndizes( swapI, swapJ );
        // If no improvement could be achieved, leave with false, indicating improvement failure
        if ( argBestNeighV == std::numeric_limits< double >::max() ) {
            return nullptr;
        }
        // If a swap is taboo, exclude it from consideration by settings its cost to the maximum double value
        unsigned int iterationCount = referenceSet.GetIterationCount();
        if ( tabooTenures( swapI, swapJ ) >= iterationCount
             // New global optimum as aspiration criterion
             && argBestNeighV > referenceSet.GetGlobalMinimumSolV() ) {
            costs( swapI, swapJ ) = std::numeric_limits< double >::max();
        } else {
            // Taillard's taboo evaluation => constant time
            tabooTenures( swapI, swapJ ) = iterationCount + tabooTenure;
            return argTempSol->GetSwappedVariant( swapI, swapJ );
        }
    }
}

void mt::TSThread::Iteration() {
    mt::RandomKeySolution *tempSol = nullptr;
    double tempSolV = 0.0;
    {
        std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
        tempSol = referenceSet.GetStartSolution( index );
        tempSolV = referenceSet.GetStartSolutionValue( index );
    }

    double bestNeighV = std::numeric_limits< double >::max();
    mt::RandomKeySolution *bestNeigh = GetBestNeigh( bestNeighV, tempSol );

    if ( !bestNeigh ) {
        ++failures;
        ++invalidSolutions;
        return;
    }

    if ( bestNeighV > tempSolV ) {
        ++failures;
    }

    {
        std::lock_guard< std::mutex > lockTSReferenceSet{ mutex };
        referenceSet.SetSolution( index, bestNeigh );
        referenceSet.SetSolutionValue( index, bestNeighV );

        referenceSet.PromoteBestSolution( index );
    }

    if ( failures >= maxFailures ) {
        finished = true;
        // These options are for the second run (after the initialization run)
        firstRun = false;
        maxFailures *= 100;

        ResetIndizes();
    }
}

void mt::TSThread::ResetIndizes() {
    failures = 0;
    invalidSolutions = 0;
    tabooTenures.ResetWithValue( 0 );
}
