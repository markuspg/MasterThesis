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

#ifndef SALBP_H
#define SALBP_H

#include "problem.h"
#include "task.h"
#include "task_storage.h"
#include "../helper_functions.h"
#include "../solution/perm_solution.h"
#include "../solution/solution.h"

#include <list>

namespace mt {

class TaskStorage;

class SALBP final : public Problem {
public:
    SALBP(const std::vector<std::string> &argTokens);
    ~SALBP() override;

    bool CheckIfTaboo(const unsigned int argIterationCount,
                      SolutionBase * const argSolution,
                      const unsigned long argSwapIndexI,
                      const unsigned long argSwapIndexJ,
                      const Matrix<unsigned long> &argTTMatrix) const override;
    SolutionBase *GenerateRandomSolution(const unsigned int argSeed) const override;
    double GetOFV(SolutionBase * const argSolution) const override;
    void UpdateFrequenciesMatrix(Matrix<unsigned long> &argFrequenciesMatrix,
                                 SolutionBase * const argNewSolution) const override;
    void UpdateTabooTenures(SolutionBase * const argNewSolution,
                            const long argSwapI, const long argSwapJ,
                            const unsigned long argTabooTenure,
                            mt::Matrix<unsigned long> &argTTMatrix) const override;

    const unsigned long cycleTime = 0;

private:
    const TaskStorage * const tasks;
};

} // namespace mt

#endif // SALBP_H
