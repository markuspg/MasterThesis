/*
 * Copyright 2015-2018 Markus Prasser
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

#include "tsreferenceset.h"
#include "../helper_functions.h"
#include "../measure.h"

#include <iostream>
#include <mutex>

using dSol = std::pair<double, mt::SolutionBase*>;

extern mt::Measure measure;
extern std::mutex measureMutex;

namespace mt {

class TSThread final {
public:
    TSThread(const unsigned short argIndex, std::mutex &argMutex,
             const mt::Problem * const argProblem,
             mt::TSReferenceSet &argReferenceSet);

    void CleanUpMatrix(mt::Matrix<dSol> &argMatrix) const;
    SolutionBase *GetBestNeigh(double argBestNeighV,
                               const std::unique_ptr<const SolutionBase> &argTempSol);
    bool IsFinished() const;
    void Iteration();
    void PrepareOptimizationRun();

private:
    unsigned int failures = 0;
    bool finished = false;
    const unsigned short index = 0;
    unsigned int iterationCount = 0;
    unsigned int maxFailures = 0;
    std::mutex &mutex;
    const mt::Problem * const problem = nullptr;
    mt::TSReferenceSet &referenceSet;
    // taboo tenures and aspiration criterion local to each thread
    // (james2009cooperative, p. 814)
    unsigned int tabooTenure = 0;
    unsigned int tabooTenureCounter = 0;
    mt::Matrix<unsigned long> tabooTenures;
};

} // namespace mt

inline bool mt::TSThread::IsFinished() const {
    return finished;
}

#endif // TSTHREAD_H
