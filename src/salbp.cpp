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

#include "salbp.h"

mt::SALBP::SALBP( const std::vector<std::string> &argTokens ) :
    Problem{ problemTypes_t::SALBP, argTokens }
{
}

mt::SALBP::~SALBP() {
}

mt::SolutionBase *mt::SALBP::GenerateRandomSolution( const std::size_t &argSize ) const {
    ( void )argSize;
    return nullptr;
}

double mt::SALBP::GetOFV( const SolutionBase * const argSolution ) const {
    ( void )argSolution;
    return 0.0;
}

void mt::SALBP::UpdateTabooTenures( const SolutionBase * const argNewSolution,
                                    const long &argSwapI, const long &argSwapJ,
                                    const unsigned long &argTabooTenure,
                                    mt::Matrix<unsigned long> &argTTMatrix ) const {
    ( void )argNewSolution;
    ( void )argSwapI;
    ( void )argSwapJ;
    ( void )argTabooTenure;
    ( void )argTTMatrix;
}
