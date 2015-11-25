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

#ifndef PERMSOLUTION_H
#define PERMSOLUTION_H

#include "solution.h"
#include "randomkeysolution.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <random>
#include <vector>

#include "../settings.h"

extern mt::Settings *settings;

namespace mt {

class RandomKeySolution;

class PermSolution final : public Solution< unsigned long >
{
public:
    PermSolution( const unsigned int &argSeed, const std::size_t &argSize );
    PermSolution( std::vector< unsigned long > * const argSolution );
    PermSolution( const PermSolution &argPermSolution );
    PermSolution( PermSolution &&argPermSolution );
    virtual ~PermSolution();

    virtual SolutionBase *Copy() const override;
    virtual std::vector< unsigned long > *GenerateRandomSolution( const unsigned int &argSeed,
                                                                  const std::size_t &argSize ) const override;
    virtual PermSolution *GetPermSolution() override;
    virtual SolutionBase *GetSwappedVariant( const unsigned long &argSwapIndexI,
                                             const unsigned long &argSwapIndexJ ) const override;
#ifdef Q_PROCESSOR_X86_64
    virtual void Mutate( std::mt19937_64 &argEngine ) override;
#else
    virtual void Mutate( std::mt19937 &argEngine ) override;
#endif
    virtual SolutionBase *ReproduceWithOtherParent( const unsigned long &argCrossoverPoint,
                                                    const SolutionBase * const argOtherParent ) const override;
};

}

#endif // PERMSOLUTION_H
