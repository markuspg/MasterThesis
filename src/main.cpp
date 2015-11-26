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

#include "helper_functions.h"
#include "measure.h"
#include "settings.h"
#include "analyzer/analyzer.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <istream>

std::mutex measureMutex;
mt::Measure measure;
mt::Settings *settings = nullptr;

int main( int argc, char *argv[] ) {
    std::cout << "<---- MasterThesis ---->" << std::endl;
    
    if ( mt::tools::ParseCommandLine( argc, argv ) ) {
        return 1;
    }

    for ( auto cit = settings->problemFiles->cbegin(); cit != settings->problemFiles->cend(); ++cit ) {
        std::fstream inputFile;
        inputFile.open( *cit, std::ios::in );
        for ( std::string line; std::getline( inputFile, line ); ) {
            const mt::Problem * const problem = mt::tools::LoadProblem( line );
            if ( problem ) {
                mt::Analyzer analyzer{ problem };
                // Start measuring the wall-clock time of the optimization part
                measure.StartTimer();
                analyzer.Analyze();
                delete problem;
            } else {
                throw std::runtime_error{ "  Invalid problem encountered in line '" + line + "' of"
                                          " file '" + *cit + "'"};
            }
            measure.WriteToDisk();
        }
    }
    
    delete settings;
    settings = nullptr;
    
    std::cout << "Finished MasterThesis" << std::endl;
    
    return 0;
}
