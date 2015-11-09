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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace mt {

class Settings {
public:
    Settings( const unsigned short &argGAInstances, const unsigned int &argMaxFailures,
              const std::string &argOutputFile, std::vector< std::string > &&argProblemFiles,
              const bool &argRandomizedTabooTenures, const bool &argRandomKeys,
              const double &argTabooTenureDeviation, const unsigned short &argTabooTenureFac,
              const unsigned short &argTSInstances );
    Settings( const Settings &argSettings ) = delete;
    Settings( Settings &&argSettings ) = delete;
    ~Settings();

    const unsigned short * const gaInstances = nullptr;
    const unsigned int * const maxFailures = nullptr;
    const std::string * const outputFile = nullptr;
    const std::vector< std::string > * const problemFiles = nullptr;
    const bool * const randomizedTabooTenures = nullptr;
    const bool * const randomKeys = nullptr;
    const double * const tabooTenureDeviation = nullptr;
    const unsigned short * const tabooTenuresFac = nullptr;
    const unsigned short * const tsInstances = nullptr;

private:
    std::string CreateStringOfProblemFiles() const;
};

}

#endif // SETTINGS_H
