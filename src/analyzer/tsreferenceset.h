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

#ifndef TABOOSEARCHREFERENCESET_H
#define TABOOSEARCHREFERENCESET_H

#include "../helper_functions.h"
#include "../matrix.h"
#include "../measure.h"
#include "../problem/problem.h"
#include "../solution/solutionbase.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <mutex>
#include <tuple>
#include <vector>

using dSol = std::pair<double, mt::SolutionBase*>;
using solTup = std::tuple<mt::SolutionBase*, double, unsigned long, bool>;

extern mt::Measure measure;
extern std::mutex measureMutex;

namespace mt {

// For diversification and intensification (james2009cooperative, p. 814)
class TSReferenceSet final {
public:
    TSReferenceSet(const Problem * const argProblem);
    TSReferenceSet(const TSReferenceSet &argTSReferenceSet) = delete;
    TSReferenceSet(TSReferenceSet &&argTSReferenceSet) = delete;
    ~TSReferenceSet();

    double GetGlobalMinimumSolV() const;
    double GetLocalMinimumSolV(const unsigned short argIndex) const;
    unsigned int GetIterationCount() const;
    SolutionBase *GetStartSolution(const unsigned short argIndex) const;
    double GetStartSolutionValue(const unsigned short argIndex) const;
    void PrepareOptimizationRun();
    void RotateSolutions();
    void SetFinalStatistics();
    void SetSolution(const std::string &argAlgorithm, const unsigned short argIndex,
                     SolutionBase *argSolution, const double argV);

    TSReferenceSet &operator++();

    const mt::Problem * const problem;

private:
    void DiversifyUnchangedSolutions();
    double GetSolutionsMedian() const;
    void PromoteNewSolution(const std::string &argAlgo,
                            const unsigned short argIndex);
    void SpreadGlobalBestSolution();

    std::vector<SolutionBase*> bestSolutions;
    std::vector<double> bestSolutionValues;
    mt::Matrix<unsigned long> frequenciesMatrix;
    bool initializationRun = true;
    std::unique_ptr<SolutionBase> globalBestSolution = nullptr;
    double globalBestSolutionV = std::numeric_limits<double>::max();
    std::stringstream globalBestSS;
    unsigned int iterationCounter = 0;
    // Store solutions, their OFV and their update status (james2009cooperative, p.816)
    std::vector<solTup> solutions;
    bool updatedGlobalBestSolution = false;
};

} // namespace mt

inline mt::TSReferenceSet &mt::TSReferenceSet::operator++() {
    ++iterationCounter; return *this;
}

inline unsigned int mt::TSReferenceSet::GetIterationCount() const {
    return iterationCounter;
}

inline double mt::TSReferenceSet::GetLocalMinimumSolV(const unsigned short argIndex) const {
    return bestSolutionValues[argIndex];
}

inline mt::SolutionBase *mt::TSReferenceSet::GetStartSolution(
        const unsigned short argIndex) const {
    return std::get<SolutionBase*>(solutions[argIndex])->Copy();
}

inline double mt::TSReferenceSet::GetStartSolutionValue(
        const unsigned short argIndex) const {
    return std::get<double>(solutions[argIndex]);
}

#endif // TABOOSEARCHREFERENCESET_H
