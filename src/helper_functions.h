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

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "matrix.h"
#include "problem.h"
#include "qap.h"
#include "salbp.h"
#include "settings.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

extern mt::Settings *settings;

namespace mt {

unsigned int GetTabooTenure( const unsigned int &argPS );
mt::Problem *LoadProblem( const std::string &argLine );
int ParseCommandLine( int argC, char *argV[] );
std::vector<std::string> Split( const std::string &argString, char argDelim );
void Split( const std::string &argString, char argDelim, std::vector<std::string> &argElements );

}

#endif // HELPER_FUNCTIONS_H
