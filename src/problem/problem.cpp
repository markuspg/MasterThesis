/*
 * Copyright 2015-2018 Markus Prasser
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

#include "problem.h"

mt::Problem::Problem(const problemTypes_t argType,
                     const std::vector<std::string> &argTokens) :
    name{argTokens[0]},
    size{std::stoul(argTokens[2])},
    type{argType}
{
    std::cout << "      Constructing Problem " << argTokens[0] << "\n";

    std::lock_guard<std::mutex> lockMeasure{measureMutex};
    measure.SetProblemParameters(name, size, argTokens[1]);
}
