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

#ifndef TABOOSEARCHREFERENCESET_H
#define TABOOSEARCHREFERENCESET_H

#include "problem.h"
#include "randomkeysolution.h"

#include <iostream>
#include <vector>

namespace mt {

class TabooSearchReferenceSet {
public:
    TabooSearchReferenceSet( const mt::Problem * const argProblem,
                             const unsigned short &argTSInstanceAmount );
    ~TabooSearchReferenceSet();

    mt::RandomKeySolution *GetStartSolution( const unsigned short &argIndex ) const
        { return solutions[ argIndex ]; }
    void SetSolution( const unsigned short &argIndex, mt::RandomKeySolution *argSolution );

private:
    std::vector< mt::RandomKeySolution* > solutions;
};

}

#endif // TABOOSEARCHREFERENCESET_H
