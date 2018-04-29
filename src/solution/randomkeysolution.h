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

#ifndef RANDOMKEYSOLUTION_H
#define RANDOMKEYSOLUTION_H

#include "perm_solution.h"
#include "solution.h"

#include <memory>
#include <random>
#include <vector>

namespace mt {

class RandomKeySolution final : public Solution<double>
{
public:
    RandomKeySolution(const unsigned int argSeed, const std::size_t argSize);
    RandomKeySolution(std::vector<double> * const argSolution);
    RandomKeySolution(const RandomKeySolution &argRandomKeySolution);
    RandomKeySolution(RandomKeySolution &&argRandomKeySolution);
    ~RandomKeySolution() override;

    SolutionBase *Copy() const override;
    std::vector<double> *GenerateRandomSolution(const unsigned int argSeed,
                                                const std::size_t argSize) const override;
    PermSolution *GetPermSolution() override;
    SolutionBase *GetSwappedVariant(const unsigned long argSwapIndexI,
                                    const unsigned long argSwapIndexJ) const override;
    void Mutate(std::mt19937_64 &argEngine) override;
    SolutionBase *ReproduceWithOtherParent(
            const unsigned long argCrossoverPoint,
            const SolutionBase * const argOtherParent) const override;

private:
    void UpdatePermutationVector();

    std::vector<unsigned long> *permutationVec = nullptr;
};

} // namespace mt

#endif // RANDOMKEYSOLUTION_H
