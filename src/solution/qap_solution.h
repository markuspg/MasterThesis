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

#ifndef QAPSOLUTION_H
#define QAPSOLUTION_H

#include "randomkeysolution.h"
#include "solution.h"

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

namespace mt {

class QAPSolution : public Solution
{
public:
    QAPSolution( const std::size_t &argSize );
    QAPSolution( const std::vector< unsigned long > * const argSolution );
    QAPSolution( const QAPSolution &argQAPSolution ) = delete;
    QAPSolution( QAPSolution &&argQAPSolution );
    ~QAPSolution();

    static std::vector< unsigned long > *ComputeFromRandomKeys( const mt::RandomKeySolution *argSol );
    static std::vector< unsigned long > *GenerateRandomSolution( const std::size_t &argSize );
    QAPSolution *GetSwappedVariant( const unsigned long &argSwapIndexI,
                                    const unsigned long &argSwapIndexJ ) const;

    const std::vector< unsigned long > * assignment = nullptr;
};

}

#endif // QAPSOLUTION_H
