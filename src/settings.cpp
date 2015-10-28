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

#include "settings.h"

mt::Settings::Settings( const unsigned short &argGAInstances, const unsigned int &argMaxFailures,
                        const std::string &argOutputFile, std::vector< std::string > &&argProblemFiles,
                        const bool &argRandomizedTabooTenures, const unsigned short &argTabooTenureFac,
                        const unsigned short &argTSInstances ) :
    gaInstances{ new unsigned short{ argGAInstances } },
    maxFailures{ new unsigned int{ argMaxFailures } },
    outputFile{ new std::string{ argOutputFile } },
    problemFiles{ new std::vector< std::string >{ argProblemFiles } },
    randomizedTabooTenures{ new bool{ argRandomizedTabooTenures } },
    tabooTenuresFac{ new unsigned short{ argTabooTenureFac } },
    tsInstances{ new unsigned short{ argTSInstances } }
{
    std::cout << "  Settings constructor" << std::endl;
    std::cout << "   gaInstances:\t\t\t" << *gaInstances
              << "\n   maxFailures:\t\t\t" << *maxFailures
              << "\n   outputFile:\t\t\t" << *outputFile
              << "\n   problemFiles:\t\t" << CreateStringOfProblemFiles()
              << "\n   randomizedTabooTenures:\t" << *randomizedTabooTenures
              << "\n   tabooTenuresFac:\t\t" << *tabooTenuresFac
              << "\n   tsInstances:\t\t\t" << *tsInstances << std::endl;
}

mt::Settings::~Settings() {
    std::cout << " Settings destructor" << std::endl;
    delete gaInstances;
    delete maxFailures;
    delete outputFile;
    delete problemFiles;
    delete randomizedTabooTenures;
    delete tabooTenuresFac;
    delete tsInstances;
}

std::string mt::Settings::CreateStringOfProblemFiles() const {
    std::string output;
    for ( auto cit = problemFiles->cbegin(); cit != problemFiles->cend() - 1; ++cit ) {
        output.append( *cit );
        output.append( 1, ';' );
    }
    output.append( problemFiles->back() );

    return output;
}
