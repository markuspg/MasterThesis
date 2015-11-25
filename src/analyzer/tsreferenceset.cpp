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

mt::TSReferenceSet::TSReferenceSet( const mt::Problem * const argProblem ) :
    problem{ argProblem },
    frequenciesMatrix{ argProblem->size, 0 }
{
    bestSolutions.resize( *settings->gaInstances + *settings->tsInstances, nullptr );
    bestSolutionValues.resize( *settings->gaInstances + *settings->tsInstances, std::numeric_limits< double >::max() );
    solutions.resize( *settings->gaInstances + *settings->tsInstances, solTup{ nullptr, 0.0, 0, true } );
    std::cout << "    Constructing TabooSearchReferenceSet" << std::endl;
    double iniMedium = 0.0;
    for ( unsigned short i = 0; i < *settings->gaInstances + *settings->tsInstances; ++i ) {
        std::get< SolutionBase* >( solutions[ i ] ) = problem->GenerateRandomSolution( i );
        std::get< double >( solutions[ i ] ) = problem->GetOFV( std::get< SolutionBase* >( solutions[ i ] ) );
        iniMedium += std::get< double >( solutions[ i ] );
    }
    iniMedium /= *settings->gaInstances + *settings->tsInstances;

    std::lock_guard< std::mutex > lockMeasure{ measureMutex };
    measure.SetInitializationStats( GetSolutionsMedian(), iniMedium );
}

mt::TSReferenceSet::~TSReferenceSet() {
    for ( auto it = bestSolutions.begin(); it != bestSolutions.end(); ++it) {
        delete *it;
    }
    for ( auto it = solutions.begin(); it != solutions.end(); ++it) {
        delete std::get< SolutionBase* >( *it );
    }

    std::lock_guard< std::mutex > lockMeasure{ measureMutex };
    measure.SetOverallIterationCount( iterationCounter );
}

void mt::TSReferenceSet::DiversifyUnchangedSolutions() {
    // Diversify all solutions not being updated in the preceding iteration (james2009cooperative, p. 816)
    for ( unsigned short i = 0; i < *settings->gaInstances + *settings->tsInstances; ++i ) {
        if ( std::get< bool >( solutions[ i ] ) == false ) {
            std::get< SolutionBase* >( solutions[ i ] )->Diversify( std::get< unsigned long >(
                                                                            solutions[ i ] ) );
            std::get< double >( solutions[ i ] ) = problem->GetOFV( std::get< SolutionBase* >(
                                                                            solutions[ i ] ) );
            std::get< unsigned long >( solutions[ i ] ) += 1;
            std::get< bool >( solutions[ i ] ) = true;
        }
    }
}

double mt::TSReferenceSet::GetSolutionsMedian() const {
    std::vector< solTup > tempSolutions{ solutions };
    std::sort( tempSolutions.begin(), tempSolutions.end(),
        []( const solTup &a, const solTup &b )
            { return std::get< double >( a ) < std::get< double >( b ); } );
    if ( solutions.size() % 2 == 0 ) {
        double belowMedian = std::get< double >( tempSolutions[ tempSolutions.size() / 2 - 1 ] );
        double aboveMedian = std::get< double >( tempSolutions[ tempSolutions.size() / 2 ] );
        return ( belowMedian + aboveMedian ) / 2;
    } else {
        return std::get< double >( tempSolutions[ tempSolutions.size() / 2 ] );
    }
}

mt::SolutionBase *mt::TSReferenceSet::GetStartSolution( const unsigned short &argIndex ) const {
    return std::get< SolutionBase* >( solutions[ argIndex ] );
}

double mt::TSReferenceSet::GetStartSolutionValue( const unsigned short &argIndex ) const {
    return std::get< double >( solutions[ argIndex ] );
}

void mt::TSReferenceSet::PrepareOptimizationRun() {
    frequenciesMatrix.ResetWithValue( 0 );
    initializationRun = false;
    iterationCounter = 0;

    double optMedium = 0.0;
    for ( unsigned short i = 0; i < *settings->gaInstances + *settings->tsInstances; ++i ) {
        std::get< double >( solutions[ i ] ) = problem->GetOFV( std::get< SolutionBase* >( solutions[ i ] ) );
        optMedium += std::get< double >( solutions[ i ] );
    }
    optMedium /= *settings->gaInstances + *settings->tsInstances;

    std::lock_guard< std::mutex > lockMeasure{ measureMutex };
    measure.SetOptimizationStats( GetSolutionsMedian(), optMedium );
}

void mt::TSReferenceSet::PromoteBestSolution( const unsigned short &argIndex ) {
    // Check, if the previously set new solution is better than the best one found by this thread
    if ( std::get< double >( solutions[ argIndex ] ) < bestSolutionValues[ argIndex ] ) {
        // If yes, update the best solution for this thread
        delete bestSolutions[ argIndex ];
        bestSolutions[ argIndex ] = std::get< SolutionBase* >( solutions[ argIndex ] )->Copy();
        bestSolutionValues[ argIndex ] = std::get< double >( solutions[ argIndex ] );

        // Check if and update, if a new global optimum (accross all threads) was found
        if ( bestSolutionValues[ argIndex ] < globalBestSolutionV ) {
            updatedGlobalBestSolution = true;
            globalBestSolution.reset( std::get< SolutionBase* >( solutions[ argIndex ] )->Copy() );
            globalBestSolutionV = bestSolutionValues[ argIndex ];
        }
    }
}

void mt::TSReferenceSet::RotateSolutions() {
    if ( initializationRun ) {
        // Deacitivate solution circulation and promotion whilst the initialization run
        // (james2009cooperative, p. 814)
        return;
    }

    SpreadGlobalBestSolution();

    DiversifyUnchangedSolutions();

    // Reference set rotation according to 'james2009cooperative' p. 817
    std::rotate( solutions.begin(), solutions.begin() + 1, solutions.end() );
}

void mt::TSReferenceSet::SetFinalStatistics() {
    double finMedium = 0.0;
    for ( unsigned short i = 0; i < *settings->gaInstances + *settings->tsInstances; ++i ) {
        std::get< double >( solutions[ i ] ) = problem->GetOFV( std::get< SolutionBase* >( solutions[ i ] ) );
        finMedium += std::get< double >( solutions[ i ] );
    }
    finMedium /= *settings->gaInstances + *settings->tsInstances;

    std::lock_guard< std::mutex > lockMeasure{ measureMutex };
    measure.SetFinalStats( GetSolutionsMedian(), finMedium );
}

void mt::TSReferenceSet::SetSolution( const unsigned short &argIndex,
                                      mt::SolutionBase *argSolution,
                                      const double &argV ) {
    // If no valid solution could be found, just set the 'updated' flag to false
    if ( !argSolution ) {
        std::get< bool >( solutions[ argIndex ] ) = false;
        return;
    }

    // Update the solutions before the rotation, because otherwise the best solution would me massively
    // overrated because of its promotion which would lead to much too high convergence
    problem->UpdateFrequenciesMatrix( frequenciesMatrix, argSolution );

    // Check if the solution changed and replace if so, otherwise set 'updated' flag to false
    if ( argSolution == std::get< SolutionBase* >( solutions[ argIndex ] ) ) {
        std::get< bool >( solutions[ argIndex ] ) = false;
        return;
    }
    unsigned long tempStepWidth = std::get< unsigned long >( solutions[ argIndex ] );
    delete std::get< SolutionBase* >( solutions[ argIndex ] );
    solutions[ argIndex ] = solTup{ argSolution, argV, tempStepWidth, true };

    PromoteBestSolution( argIndex );
}

void mt::TSReferenceSet::SpreadGlobalBestSolution() {
    // If whilst the last iteration a new global best solution was found, ...
    if ( *settings->promoteGlobalBestSol && updatedGlobalBestSolution ) {
        // promote it to all odd taboo search thread start solutions (james2009cooperative, p. 816)
        for ( unsigned short i = 0; i < *settings->gaInstances + *settings->tsInstances; ++i ) {
            if ( i % 2 == 1 ) {
                unsigned long tempStepWidth = std::get< unsigned long >( solutions[ i ] );
                delete std::get< SolutionBase* >( solutions[ i ] );
                solutions[ i ] = solTup{ globalBestSolution->Copy(),
                                         globalBestSolutionV,
                                         tempStepWidth,
                                         true };
            }
        }
    }

    // Reset the inidicator for the next iteration
    updatedGlobalBestSolution = false;
}
