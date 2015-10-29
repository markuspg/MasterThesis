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

#ifndef QAP_H
#define QAP_H

#include "helper_functions.h"
#include "matrix.h"
#include "problem.h"

#include <iostream>
#include <vector>

namespace mt {

class QAP : public Problem {
public:
    QAP( const std::vector<std::string> &argTokens );
    QAP( const Problem &argProblem ) = delete;
    QAP( Problem &&argProblem ) = delete;

    const mt::Matrix< int > distances;
    const mt::Matrix< int > flows;
};

}

#endif // QAP_H
