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

namespace mt {

class GAThread {
public:
    GAThread( const unsigned short &argIndex, std::mutex &argMutex,
              const mt::Problem * const argProblem, mt::TSReferenceSet &argReferenceSet );

    bool IsFinished() const { return finished; }
    void Iteration();

private:
    bool finished = false;
    const unsigned short index = 0;
    unsigned int iterationCount = 0;
    const mt::Problem * const problem = nullptr;
    std::mutex &tsReferenceSetMutex;
    mt::TSReferenceSet &referenceSet;
};

}

#endif // GATHREAD_H