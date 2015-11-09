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

#ifndef TSTHREAD_H
#define TSTHREAD_H

#include "helper_functions.h"
#include "tsreferenceset.h"

#include <iostream>
#include <mutex>

namespace mt {

class TSThread {
public:
    TSThread( const unsigned short &argIndex, std::mutex &argMutex,
              mt::TSReferenceSet &argReferenceSet );

    mt::Solution *GetBestNeigh( double &argBestNeighV,
                                mt::Solution *argTempSol );
    bool IsFinished() const { return finished; }
    void Iteration();
    void PrepareSecondTSRun();

private:
    unsigned int failures = 0;
    bool finished = false;
    const unsigned short index = 0;
    unsigned int invalidSolutions = 0;
    unsigned int iterationCount = 0;
    unsigned int maxFailures = 0;
    std::mutex &mutex;
    mt::TSReferenceSet &referenceSet;
    unsigned int tabooTenure = 0;
    mt::Matrix< unsigned long > tabooTenures;
};

}

#endif // TSTHREAD_H
