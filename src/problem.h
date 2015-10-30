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

#ifndef PROBLEM_H
#define PROBLEM_H

#include "randomkeysolution.h"

#include <iostream>
#include <vector>

enum class problemTypes_t : unsigned short {
    QAP,
    SALBP
};

namespace mt {

class Problem {
public:
    Problem( const problemTypes_t &argType, const std::vector<std::string> &argTokens );
    Problem( const Problem &argProblem ) = delete;
    Problem( Problem &&argProblem ) = delete;
    virtual ~Problem();

    virtual double GetOFV( const mt::RandomKeySolution * const argSolution ) const = 0;

    const std::string name;
    const unsigned long size = 0;
    const problemTypes_t type;
};

}

#endif // PROBLEM_H
