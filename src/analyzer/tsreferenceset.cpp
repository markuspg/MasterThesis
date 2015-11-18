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
    solutions.resize( tsInstanceQuantity, solTup{ nullptr, 0.0, true } );
    std::cout << "    Constructing TabooSearchReferenceSet" << std::endl;
    for ( unsigned short i = 0; i < tsInstanceQuantity; ++i ) {
        std::get< 0 >( solutions[ i ] ) = problem->GenerateRandomSolution( i );
        std::get< 1 >( solutions[ i ] ) = problem->GetOFV( std::get< 0 >( solutions[ i ] ) );
    }
}

mt::TSReferenceSet::~TSReferenceSet() {
    for ( auto it = bestSolutions.begin(); it != bestSolutions.end(); ++it) {
        delete *it;
    }
    for ( auto it = solutions.begin(); it != solutions.end(); ++it) {
        delete std::get< 0 >( *it );
    }

    std::lock_guard< std::mutex > lockMeasure{ measureMutex };
    measure.SetOverallIterationCount( iterationCounter );
}

mt::SolutionBase *mt::TSReferenceSet::GetStartSolution( const unsigned short &argIndex ) const {
    return std::get< 0 >( solutions[ argIndex ] );
}

double mt::TSReferenceSet::GetStartSolutionValue( const unsigned short &argIndex ) const {
    return std::get< 1 >( solutions[ argIndex ] );
}

void mt::TSReferenceSet::PrepareOptimizationRun() {
    initializationRun = false;
    iterationCounter = 0;
}

void mt::TSReferenceSet::PromoteBestSolution( const unsigned short &argIndex ) {
    // Check, if the previously set new solution is better than the best one found by this thread
    if ( std::get< 1 >( solutions[ argIndex ] ) < bestSolutionValues[ argIndex ] ) {
        // If yes, update the best solution for this thread
        delete bestSolutions[ argIndex ];
        bestSolutions[ argIndex ] = std::get< 0 >( solutions[ argIndex ] )->Copy();
        bestSolutionValues[ argIndex ] = std::get< 1 >( solutions[ argIndex ] );

        // Check if and update, if a new global optimum (accross all threads) was found
        if ( bestSolutionValues[ argIndex ] < globalBestSolutionV ) {
            updatedGlobalBestSolution = true;
            globalBestSolution.reset( std::get< 0 >( solutions[ argIndex ] )->Copy() );
            globalBestSolutionV = bestSolutionValues[ argIndex ];
        }
    }
}

void mt::TSReferenceSet::RotateSolutions() {
    if ( initializationRun ) {
        return;
    }
    // If whilst the last iteration a new global best solution was found, ...
    if ( updatedGlobalBestSolution ) {
        // promote it to all odd taboo search thread start solutions (james2009cooperative, p. 816)
        for ( unsigned short i = 0; i < tsInstanceQuantity; ++i ) {
            if ( i % 2 == 1 ) {
                delete std::get< 0 >( solutions[ i ] );
                solutions[ i ] = solTup{ globalBestSolution->Copy(), globalBestSolutionV, true };
            }
        }
    }

    // Diversify all solutions not being updated in the preceding iteration (james2009cooperative, p. 816)
    for ( unsigned short i = 0; i < tsInstanceQuantity; ++i ) {
        if ( std::get< 2 >( solutions[ i ] ) == false ) {
            std::get< 0 >( solutions[ i ] )->Diversify( i );
            std::get< 1 >( solutions[ i ] ) = problem->GetOFV( std::get< 0 >( solutions[ i ] ) );
            std::get< 2 >( solutions[ i ] ) = true;
        }
    }

    // Reference set rotation according to 'james2009cooperative' p. 817
    std::rotate( solutions.begin(), solutions.begin() + 1, solutions.end() );

    // Reset the inidicator for the next iteration
    updatedGlobalBestSolution = false;
}

void mt::TSReferenceSet::SetSolution( const unsigned short &argIndex,
                                      mt::SolutionBase *argSolution,
                                      const double &argV ) {
    // If no valid solution could be found, just set the 'updated' flag to false
    if ( !argSolution ) {
        std::get< 2 >( solutions[ argIndex ] ) = false;
        return;
    }
    // Check if the solution changed and replace if so, otherwise set 'updated' flag to false
    if ( argSolution == std::get< 0 >( solutions[ argIndex ] ) ) {
        std::get< 2 >( solutions[ argIndex ] ) = false;
        return;
    }
    delete std::get< 0 >( solutions[ argIndex ] );
    solutions[ argIndex ] = solTup{ argSolution, argV, true };

    PromoteBestSolution( argIndex );
}
