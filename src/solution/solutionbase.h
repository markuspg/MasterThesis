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

#ifndef SOLUTIONBASE_H
#define SOLUTIONBASE_H

#include <vector>

namespace mt {

class SolutionBase
{
public:
    SolutionBase();
    SolutionBase( const SolutionBase &argSolutionBase );
    SolutionBase( SolutionBase &&argSolutionBase ) = delete;
    virtual ~SolutionBase();

    virtual SolutionBase *Copy() const = 0;
    virtual std::vector< unsigned long > *GetAssignmentVectorCopy() const = 0;
    virtual SolutionBase *GetQAPSolution() const = 0;
    virtual std::vector< double > *GetSolutionVectorCopy() const = 0;
    virtual SolutionBase *GetSwappedVariant( const unsigned long &argSwapIndexI,
                                             const unsigned long &argSwapIndexJ ) const = 0;
    virtual SolutionBase *ReproduceWithOtherParent( const unsigned long &argCrossoverPoint,
                                                    const SolutionBase * const argOtherParent ) const = 0;
};

}

#endif // SOLUTIONBASE_H