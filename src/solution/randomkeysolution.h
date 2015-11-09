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

#ifndef RANDOMKEYSOLUTION_H
#define RANDOMKEYSOLUTION_H

#include "solution.h"
#include "qap_solution.h"

#include <memory>
#include <random>
#include <vector>

namespace mt {

class RandomKeySolution : public Solution
{
public:
    RandomKeySolution( const std::size_t &argSize );
    RandomKeySolution( const std::vector< double > * const argSolution );
    RandomKeySolution( const RandomKeySolution &argRandomKeySolution );
    RandomKeySolution( RandomKeySolution &&argRandomKeySolution );
    ~RandomKeySolution();

    virtual Solution *Copy() const;
    static std::vector< double > *GenerateRandomSolution( const std::size_t &argSize );
    virtual mt::QAPSolution *GetQAPSolution() const;
    virtual Solution *GetSwappedVariant( const unsigned long &argSwapIndexI,
                                         const unsigned long &argSwapIndexJ ) const;

    const std::vector< double > * solutionVec = nullptr;
};

}

#endif // RANDOMKEYSOLUTION_H
