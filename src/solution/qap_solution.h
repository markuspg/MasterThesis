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

#include "solution.h"
#include "randomkeysolution.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <random>
#include <vector>

namespace mt {

class RandomKeySolution;

class QAPSolution final : public Solution< unsigned long >
{
public:
    QAPSolution( const unsigned int &argSeed, const std::size_t &argSize );
    QAPSolution( std::vector< unsigned long > * const argSolution );
    QAPSolution( const QAPSolution &argQAPSolution );
    QAPSolution( QAPSolution &&argQAPSolution );
    virtual ~QAPSolution();

    static QAPSolution *ComputeFromRandomKeys( const RandomKeySolution *argSol );
    virtual SolutionBase *Copy() const override;
    virtual std::vector< unsigned long > *GenerateRandomSolution( const unsigned int &argSeed,
                                                                  const std::size_t &argSize ) const override;
    virtual QAPSolution *GetQAPSolution() const override;
    virtual SALBPSolution *GetSALBPSolution() const override;
    virtual SolutionBase *GetSwappedVariant( const unsigned long &argSwapIndexI,
                                             const unsigned long &argSwapIndexJ ) const override;
    virtual SolutionBase *ReproduceWithOtherParent( const unsigned long &argCrossoverPoint,
                                                    const SolutionBase * const argOtherParent ) const;
};

}

#endif // QAPSOLUTION_H
