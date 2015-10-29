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

#ifndef TABOOSEARCH_H
#define TABOOSEARCH_H

#include "taboosearchreferenceset.h"

#include <iostream>
#include <mutex>

extern std::mutex tsMutex;

namespace mt {

void TabooSearchCycle( const unsigned short argIndex, mt::TabooSearchReferenceSet &argTSReferenceSet );

}

#endif // TABOOSEARCH_H
