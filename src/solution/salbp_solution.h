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

#ifndef SALBPSOLUTION_H
#define SALBPSOLUTION_H

#include "solution.h"
#include "randomkeysolution.h"

#include <algorithm>
#include <memory>
#include <random>
#include <vector>

namespace mt {

class RandomKeySolution;

class SALBPSolution final : public Solution< unsigned long > {
public:
    SALBPSolution( const unsigned int &argSeed, const std::size_t &argSize );
    SALBPSolution( std::vector< unsigned long > * const argSolution );
    SALBPSolution( const SALBPSolution &argSALBPSolution );
    SALBPSolution( SALBPSolution &&argSALBPSolution );
    virtual ~SALBPSolution();

    static SALBPSolution *ComputeFromRandomKeys( const RandomKeySolution *argSol );
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

#endif // SALBPSOLUTION_H
