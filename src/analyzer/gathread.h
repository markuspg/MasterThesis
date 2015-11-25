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

#ifndef GATHREAD_H
#define GATHREAD_H

#include "tsreferenceset.h"

#include <iostream>
#include <mutex>
#include <vector>

typedef std::pair< double, mt::SolutionBase* > dSol;

namespace mt {

class GAThread final {
public:
    GAThread( const unsigned short &argIndex, std::mutex &argMutex,
              const mt::Problem * const argProblem, mt::TSReferenceSet &argReferenceSet );
    ~GAThread();

    bool IsFinished() const { return finished; }
    void Iteration();

private:
    void CreateInitialPopulation();
    void Immigrate();
    void Mutate();
    void Reproduce();
    void SelectParentsMonteCarlo( std::pair< unsigned long, unsigned long > &argParents );

    bool finished = false;
    const unsigned short index = 0;
    unsigned int iterationCount = 0;
    unsigned long popSize = 0;
    const unsigned long immigrationsQuantity = 0;   //! How many chromosomes shall immigrate per iteration
    std::vector< dSol > population;
    const mt::Problem * const problem = nullptr;
    const unsigned long mutationsQuantity = 0;      //! How many chromosomes shall be mutated per iteration
    const unsigned long reproductionQuantity = 0;   //! How many reproductions shall happen per iteration
    std::mutex &tsReferenceSetMutex;
    mt::TSReferenceSet &referenceSet;
};

}

#endif // GATHREAD_H
