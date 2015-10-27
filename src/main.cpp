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

#include "analyzer.h"
#include "helper_functions.h"
#include "settings.h"

#include <iostream>

mt::Settings *settings = nullptr;

int main( int argc, char *argv[] ) {
    std::cout << "<---- MasterThesis ---->" << std::endl;
    
    if ( ParseCommandLine( argc, argv ) ) {
        return 1;
    }
    
    mt::Analyzer analyzer;
    analyzer.Analyze();
    
    delete settings;
    settings = nullptr;
    
    std::cout << "Finished MasterThesis" << std::endl;
    
    return 0;
}
