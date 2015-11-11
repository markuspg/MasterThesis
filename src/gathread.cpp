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
        population[ i ].second = problem->GenerateRandomSolution( problem->size );
        population[ i ].first = problem->GetOFV( population[ i ].second );
    }
}

void mt::GAThread::Iteration() {
    ++iterationCount;

    Reproduce();

    Mutate();
}

void mt::GAThread::Mutate() {

}

void mt::GAThread::Reproduce() {

}
