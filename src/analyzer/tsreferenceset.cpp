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

#include "tsreferenceset.h"

mt::TSReferenceSet::TSReferenceSet( const mt::Problem * const argProblem,
                                    const unsigned short &argTSInstanceAmount ) :
    problem{ argProblem },
    tsInstanceQuantity{ argTSInstanceAmount }
{
    bestSolutions.resize( tsInstanceQuantity, nullptr );
    bestSolutionValues.resize( tsInstanceQuantity, std::numeric_limits< double >::max() );
    solutions.resize( tsInstanceQuantity, nullptr );
    solutionValues.resize( tsInstanceQuantity, std::numeric_limits< double >::max() );
    std::cout << "    Constructing TabooSearchReferenceSet" << std::endl;
    for ( unsigned short i = 0; i < tsInstanceQuantity; ++i ) {
        solutions[ i ] = problem->GenerateRandomSolution( i );
        solutionValues[ i ] = problem->GetOFV( solutions[ i ] );
    }
}

mt::TSReferenceSet::~TSReferenceSet() {
    for ( auto it = bestSolutions.begin(); it != bestSolutions.end(); ++it) {
        delete *it;
    }
    for ( auto it = solutions.begin(); it != solutions.end(); ++it) {
        delete *it;
    }

    std::lock_guard< std::mutex > lockMeasure{ measureMutex };
    measure.SetOverallIterationCount( iterationCounter );
}

void mt::TSReferenceSet::PromoteBestSolution( const unsigned short &argIndex ) {
    // Check, if the previously set new solution is better than the best one found by this thread
    if ( solutionValues[ argIndex ] < bestSolutionValues[ argIndex ] ) {
        // If yes, update the best solution for this thread
        delete bestSolutions[ argIndex ];
        bestSolutions[ argIndex ] = solutions[ argIndex ]->Copy();
        bestSolutionValues[ argIndex ] = solutionValues[ argIndex ];

        // Check if and update, if a new global optimum (accross all threads) was found
        if ( bestSolutionValues[ argIndex ] < globalBestSolutionV ) {
            updatedGlobalBestSolution = true;
            globalBestSolution.reset( solutions[ argIndex ]->Copy() );
            globalBestSolutionV = bestSolutionValues[ argIndex ];
        }
    }
}

void mt::TSReferenceSet::RotateSolutions() {
    // If whilst the last iteration a new global best solution was found, ...
    if ( updatedGlobalBestSolution ) {
        // promote it to all odd taboo search thread start solutions
        for ( unsigned short i = 0; i < tsInstanceQuantity; ++i ) {
            if ( i % 2 == 1 ) {
                delete solutions[ i ];
                solutions[ i ] = globalBestSolution->Copy();
                solutionValues[ i ] = globalBestSolutionV;
            }
        }
    }

    // Reference set rotation according to 'james2009cooperative' p. 817
    std::rotate( solutions.begin(), solutions.begin() + 1, solutions.end() );
    std::rotate( solutionValues.begin(), solutionValues.begin() + 1, solutionValues.end() );

    // Reset the inidicator for the next iteration
    updatedGlobalBestSolution = false;
}

void mt::TSReferenceSet::SetSolution( const unsigned short &argIndex,
                                      mt::SolutionBase *argSolution ) {
    if ( argSolution == solutions[ argIndex ] ) {
        return;
    }
    delete solutions[ argIndex ];
    solutions[ argIndex ] = argSolution;
}
