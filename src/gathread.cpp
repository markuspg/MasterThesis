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
    problem{ argProblem },
    tsReferenceSetMutex{ argMutex },
    referenceSet{ argReferenceSet }
{
    std::cout << "      Constructing GAThread with id " << index << std::endl;
}

void mt::GAThread::Iteration() {
    ++iterationCount;
}