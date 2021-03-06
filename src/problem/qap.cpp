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

#include "qap.h"

mt::QAP::QAP(const std::vector<std::string> &argTokens) :
    Problem{problemTypes_t::QAP, argTokens},
    distances{mt::Matrix<int>::ConvertStringVecToIntVec(tools::Split(argTokens[4], ';'))},
    flows{mt::Matrix<int>::ConvertStringVecToIntVec(tools::Split(argTokens[3], ';'))}
{
    std::cout << "      > Constructing QAP " << argTokens[0] << "\n";
}

bool mt::QAP::CheckIfTaboo(const unsigned int argIterationCount,
                           SolutionBase * const argSolution,
                           const unsigned long argSwapIndexI,
                           const unsigned long argSwapIndexJ,
                           const Matrix<unsigned long> &argTTMatrix) const {
    // taillard1991robust, p. 447
    const auto tempSol = dynamic_cast<PermSolution*>(argSolution->GetPermSolution());
    if (argTTMatrix((*tempSol)(argSwapIndexI), argSwapIndexI) >= argIterationCount
        && argTTMatrix((*tempSol)(argSwapIndexJ), argSwapIndexJ) >= argIterationCount ) {
        delete tempSol;
        return true;
    }
    delete tempSol;
    return false;
}

mt::SolutionBase *mt::QAP::GenerateRandomSolution(const unsigned int argSeed) const {
    if (*settings->randomKeys) {
        return new RandomKeySolution{argSeed, size};
    } else {
        return new PermSolution{argSeed, size};
    }
}

double mt::QAP::GetOFV(mt::SolutionBase * const argSolution) const {
    // Stores the converted solution
    const auto tempSol = dynamic_cast<mt::PermSolution*>(argSolution->GetPermSolution());
    assert(tempSol);

    unsigned long ofv = 0;
    const auto size = tempSol->GetSize();
    for (auto i = decltype(size){0}; i < size; ++i) {
        for (auto j = decltype(size){0}; j < size; ++j ) {
            const int flow = flows((*tempSol)(i), (*tempSol)(j));
            const int distance = distances(i, j);
            ofv += flow * distance;
        }
    }

    delete tempSol;

    return ofv;
}

void mt::QAP::UpdateFrequenciesMatrix(Matrix<unsigned long> &argFrequenciesMatrix,
                                      SolutionBase * const argNewSolution) const {
    const auto tempSol = dynamic_cast<mt::PermSolution*>(argNewSolution->GetPermSolution());
    std::vector< unsigned long >::size_type solSize = tempSol->GetSize();
    for (unsigned long i = 0; i < solSize; ++i) {
        argFrequenciesMatrix((*tempSol)(i), i) += 1;
    }
    delete tempSol;
}

void mt::QAP::UpdateTabooTenures(mt::SolutionBase * const argNewSolution,
                                 const long argSwapI, const long argSwapJ,
                                 const unsigned long argTabooTenure,
                                 mt::Matrix<unsigned long> &argTTMatrix) const {
    const auto tempSol = dynamic_cast<mt::PermSolution*>(argNewSolution->GetPermSolution());
    // Forbid the  re-assignment of the swapped units to the locations
    argTTMatrix((*tempSol)(argSwapI), argSwapI) = argTabooTenure;
    argTTMatrix((*tempSol)(argSwapJ), argSwapJ) = argTabooTenure;
    delete tempSol;
}
