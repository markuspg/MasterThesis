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

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

namespace mt {

class QAPSolution;

class Solution
{
public:
    Solution();
    Solution( const Solution &argSolution );
    Solution( Solution &&argSolution ) = delete;
    virtual ~Solution();

    virtual Solution *Copy() const = 0;
    virtual mt::QAPSolution *GetQAPSolution() const = 0;
    virtual Solution *GetSwappedVariant( const unsigned long &argSwapIndexI,
                                         const unsigned long &argSwapIndexJ ) const = 0;
};

}

#endif // SOLUTION_H
