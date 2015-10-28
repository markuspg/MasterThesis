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

extern mt::Settings *settings;

mt::Problem *LoadProblem( const std::string &argLine ) {
    std::vector< std::string > problemTokens = Split( argLine, '|' );
    if ( problemTokens[ 1 ] == "QAP" ) {
        return new mt::QAP{ problemTokens };
    }
    if ( problemTokens[ 1 ] == "SALBP" ) {
    }
    return nullptr;
}

int ParseCommandLine( int argC, char *argV[] ) {
    // A temporary storage for the extracted strings
    std::vector< std::string > commandLineArguments{};
    commandLineArguments.reserve( argC );
    
    // The variables for the data
    unsigned short tempGAInstances = 0;
    unsigned int tempMaxFailures = 0;
    std::string tempOutputFile;
    std::vector< std::string > tempProblemFiles;
    bool tempRandomizedTabooTenures = false;
    unsigned short tempTabooTenureFac = 0;
    unsigned short tempTSInstances = 0;
    
    // Convert all arguments to strings and organize them in a vector
    for ( unsigned short i = 0; i < argC; ++i ) {
        commandLineArguments.emplace_back( argV[ i ] );
    }
    
    unsigned int lastActiveIndex = 0;
    // Store relevant options to the settings
    for ( unsigned short i = 0; i < argC; ++i ) {
        if ( argC < 2 || commandLineArguments[ i ] == "--help" || commandLineArguments[ i ] == "-h" ) {
            std::cout << "MasterThesis is a parallelized adaptive memory heuristic implementation\n"
                         "for my master thesis. The command line arguments are the following:\n\n"
                         "\t--ga <gaInstances>    The desired number of genetic algorithm threads\n"
                         "\t--ts <tsInstances>    The desired number of taboo search threads\n"
                         "\t--mf <maxFailures>    The number of allowed improvement failures,\n"
                         "\t                      before the search terminates\n"
                         "\t--rtt <randTTenures>  If the taboo tenures shall be randomized {0;1}\n"
                         "\t--ttf <tTenureFac>    How many times the problem size\n"
                         "\t                      the taboo tenure shall last" << std::endl;
            return 1;
        }
        if ( commandLineArguments[ i ] == "--ga" ) {
            tempGAInstances = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--mf" ) {
            tempMaxFailures = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--rtt" ) {
            tempRandomizedTabooTenures = static_cast< bool >( std::stoul( commandLineArguments[ i + 1 ] ) );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--ttf" ) {
            tempTabooTenureFac = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--ts" ) {
            tempTSInstances = std::stoul( commandLineArguments[ i + 1 ] );
            lastActiveIndex = i + 1;
            continue;
        }
    }

    for ( unsigned int i = lastActiveIndex + 1; i < static_cast< unsigned int >( argC ) - 1; ++i ) {
        tempProblemFiles.emplace_back( argV[ i ] );
    }

    tempOutputFile = commandLineArguments.back();
    
    settings = new mt::Settings{ tempGAInstances, tempMaxFailures, tempOutputFile,
                                 std::move( tempProblemFiles ), tempRandomizedTabooTenures,
                                 tempTabooTenureFac, tempTSInstances };
    
    return 0;
}

std::vector<std::string> Split( const std::string &argString, char argDelim ) {
    std::vector<std::string> elements;
    Split( argString, argDelim, elements );
    return elements;
}

void Split( const std::string &argString, char argDelim, std::vector<std::string> &argElements ) {
    std::stringstream stringStream{ argString };
    std::string item;
    while (std::getline( stringStream, item, argDelim ) ) {
        argElements.push_back( item );
    }
}
