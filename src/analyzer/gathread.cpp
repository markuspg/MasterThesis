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

#include "gathread.h"

mt::GAThread::GAThread( const unsigned short &argIndex, std::mutex &argMutex,
                        const mt::Problem * const argProblem, mt::TSReferenceSet &argReferenceSet ) :
    index{ argIndex },
    popSize{ argProblem->size * argProblem->size > 100 ? 100 : argProblem->size * argProblem->size },
    population{ popSize, { 0.0, nullptr } },
    problem{ argProblem },
    mutationGeneQuantity{ *settings->mutationImpact * problem->size < 1.0 ?
            1 : static_cast< unsigned long >( *settings->mutationImpact * problem->size ) },
    mutationsQuantity{ *settings->mutationRate * popSize < 1 ?
            1 : static_cast< unsigned long >( *settings->mutationRate * popSize ) },
    reproductionQuantity{ *settings->reproductionRate * popSize / 2 < 1 ?
            1 : static_cast< unsigned long >( *settings->reproductionRate * popSize / 2 ) },
    tsReferenceSetMutex{ argMutex },
    referenceSet{ argReferenceSet }
{
    std::cout << "      Constructing GAThread with id " << index << std::endl;

    CreateInitialPopulation();
}

mt::GAThread::~GAThread() {
    for ( auto &s: population ) {
        delete s.second;
    }
}

void mt::GAThread::CreateInitialPopulation() {
    for ( unsigned long i = 0; i < popSize; ++i ) {
        population[ i ].second = problem->GenerateRandomSolution( i );
        population[ i ].first = problem->GetOFV( population[ i ].second );
    }
    std::sort( population.begin(), population.end(),
            []( const dSol &a, const dSol &b ){ return a.first < b.first; } );
}

void mt::GAThread::Iteration() {
    ++iterationCount;

    Reproduce();

    Mutate();

    // Re-sort the population for the next iteration
    std::sort( population.begin(), population.end(),
            []( const dSol &a, const dSol &b ){ return a.first < b.first; } );
}

void mt::GAThread::Mutate() {

}

void mt::GAThread::Reproduce() {
    for ( unsigned long i = 0; i < reproductionQuantity; ++i ) {
        std::pair< unsigned long, unsigned long > parentIndices;
        SelectParentsMonteCarlo( parentIndices );

        std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
        std::mt19937_64 engine{ randomDevice() };
#else
        std::mt19937 engine{ randomDevice() };
#endif
        std::uniform_int_distribution<> distribution{ 0, static_cast< int >( problem->size ) };
        const unsigned long crossoverPoint = distribution( engine );
        mt::SolutionBase *childA = population[ parentIndices.first ].second->ReproduceWithOtherParent(
                crossoverPoint, population[ parentIndices.second ].second );
        mt::SolutionBase *childB = population[ parentIndices.second ].second->ReproduceWithOtherParent(
                crossoverPoint, population[ parentIndices.first ].second );
        delete population[ parentIndices.first ].second;
        population[ parentIndices.first ].second = childA;
        population[ parentIndices.first ].first = problem->GetOFV( population[ parentIndices.first ].second );
        delete population[ parentIndices.second ].second;
        population[ parentIndices.second ].second = childB;
        population[ parentIndices.second ].first = problem->GetOFV( population[ parentIndices.second ].second );
    }
}

void mt::GAThread::SelectParentsMonteCarlo( std::pair< unsigned long , unsigned long > &argParentIndices ) {
    std::vector< double > probabilities;
    probabilities.resize( popSize, 0.0 );
    double sum = 0.0;
    // Use the inverted values as probabilities, since smaller values should be chosen more likely
    for ( unsigned long i = 0; i < popSize; ++i ) {
        probabilities[ i ] = 1 / population[ i ].first;
        sum += probabilities[ i ];
    }
    std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
    std::mt19937_64 engine{ randomDevice() };
#else
    std::mt19937 engine{ randomDevice() };
#endif
    std::uniform_real_distribution<> distribution{ 0.0, sum };
    // Turn the roulette wheel
    double parentAValue = distribution( engine );
    double parentBValue = distribution( engine );
    // Find the corresponding solutions
    double accum = 0.0;
    for ( unsigned long i = 0; i < popSize; ++i ) {
        accum += probabilities[ i ];
        if ( accum > parentAValue ) {
            argParentIndices.first = i;
            break;
        }
    }
    accum = 0.0;
    for ( unsigned long i = 0; i < popSize; ++i ) {
        accum += probabilities[ i ];
        if ( accum > parentBValue ) {
            argParentIndices.second = i;
            break;
        }
    }
}