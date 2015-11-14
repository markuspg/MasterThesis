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

unsigned int mt::GetTabooTenure( const unsigned int &argPS ) {
    unsigned int tabooTenure = 0;
    if ( *settings->randomizedTabooTenures ) {
        std::random_device randomDevice;
#ifdef Q_PROCESSOR_X86_64
        std::mt19937_64 engine{ randomDevice() };
#else
        std::mt19937 engine{ randomDevice() };
#endif
        std::uniform_int_distribution<> distribution{
            static_cast< int >( *settings->tabooTenuresFac * argPS
                                * ( 1.0 - *settings->tabooTenureDeviation ) ),
            static_cast< int >( *settings->tabooTenuresFac * argPS
                                * ( 1.0 + *settings->tabooTenureDeviation ) ) };

        tabooTenure = distribution( engine );
    } else {
        tabooTenure = *settings->tabooTenuresFac * argPS;
    }

    return tabooTenure;
}

mt::Problem *mt::LoadProblem( const std::string &argLine ) {
    std::vector< std::string > problemTokens = mt::Split( argLine, '|' );
    if ( problemTokens.size() < 2 ) {
        return nullptr;
    }
    if ( problemTokens[ 1 ] == "QAP" ) {
        if ( problemTokens.size() < 5 ) {
            throw std::runtime_error{ "The QAP " + problemTokens[ 0 ] + " is malformed" };
        }
        return new mt::QAP{ problemTokens };
    }
    if ( problemTokens[ 1 ] == "SALBP" ) {
        if ( problemTokens.size() < 6 ) {
            throw std::runtime_error{ "The SALBP " + problemTokens[ 0 ] + " is malformed" };
        }
        return new mt::SALBP{ problemTokens };
    }
    return nullptr;
}

int mt::ParseCommandLine( int argC, char *argV[] ) {
    // A temporary storage for the extracted strings
    std::vector< std::string > commandLineArguments{};
    commandLineArguments.reserve( argC );
    
    // The variables for the data
    unsigned short tempGAInstances = 1;
    unsigned int tempMaxFailures = 1000000;
    std::string tempOutputFile;
    std::vector< std::string > tempProblemFiles;
    bool tempRandomizedTabooTenures = false;
    bool tempRandomKeys = false;
    double tempTabooTenureDeviation = 0.1;
    unsigned short tempTabooTenureFac = 1;
    unsigned short tempTSInstances = 1;
    
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
                         "\t--ga <gaInstances>    The desired number of threads running the\n"
                         "\t                      genetic algorithm (default: 1)\n"
                         "\t--ts <tsInstances>    The desired number of threads running taboo\n"
                         "\t                      search instances (default: 1)\n"
                         "\t--mf <maxFailures>    The number of allowed improvement failures\n"
                         "\t                      before the search terminates (default: 1000000)\n"
                         "\t--rk <UseRandomKeys>  If solutions shall be encoded as random keys (any non-zero\n"
                         "\t                      integer value will be interpreted as true; default: 0)\n"
                         "\t--rtt <randTTenures>  If the taboo tenures shall be randomized (any non-zero\n"
                         "\t                      integer value will be interpreted as true; default: 0)\n"
                         "\t--ttd <tTenureDev>    The spread factor for randomized taboo tenures [0.1,0.9]\n"
                         "\t                      (default: 0.1)\n"
                         "\t--ttf <tTenureFac>    How many times the problem size\n"
                         "\t                      the taboo tenure shall last (default: 1)" << std::endl;
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
        if ( commandLineArguments[ i ] == "--rk" ) {
            tempRandomKeys = static_cast< bool >( std::stoul( commandLineArguments[ i + 1 ] ) );
            lastActiveIndex = i + 1;
            continue;
        }
        if ( commandLineArguments[ i ] == "--ttd" ) {
            tempTabooTenureDeviation = std::stod( commandLineArguments[ i + 1 ] );
            if ( tempTabooTenureDeviation < 0.1 || tempTabooTenureDeviation > 0.9 ) {
                throw std::runtime_error{ "The taboo deviation is not in the valid range of [0.1,0.9]" };
            }
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
                                 std::move( tempProblemFiles ), tempRandomizedTabooTenures, tempRandomKeys,
                                 tempTabooTenureDeviation, tempTabooTenureFac, tempTSInstances };
    
    return 0;
}

std::vector<std::string> mt::Split( const std::string &argString, char argDelim ) {
    std::vector<std::string> elements;
    mt::Split( argString, argDelim, elements );
    return elements;
}

void mt::Split( const std::string &argString, char argDelim, std::vector<std::string> &argElements ) {
    std::stringstream stringStream{ argString };
    std::string item;
    while (std::getline( stringStream, item, argDelim ) ) {
        argElements.push_back( item );
    }
}
