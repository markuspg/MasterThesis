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

#ifndef MATRIX_H
#define MATRIX_H

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

namespace mt {

template< typename T>
class Matrix
{
public:
    Matrix() = delete;
    Matrix ( const Matrix &argMatrix ) = delete;
    Matrix ( Matrix &&argMatrix );
    Matrix( const std::vector< T > &argItems );
    Matrix( const unsigned long &argDim, const T &argDefaultValue );
    ~Matrix();

    static std::vector< int > ConvertStringVecToIntVec( const std::vector< std::string > &argStrList );
    static std::vector< unsigned long > ConvertStringVecToULongVec
        ( const std::vector< std::string > &argStrList );
    T GetMinimumValueIndizes( long &argI, long &argJ );

    T& operator() ( const unsigned int &argI, const unsigned int &argJ ) const
        { return A->at( argI * dimension + argJ ); }

private:
    const unsigned long dimension = 0;
    std::vector< T > *A = nullptr;
};

}

template< typename T >
mt::Matrix< T >::Matrix( Matrix &&argMatrix ) :
    dimension{ argMatrix.dimension },
    A{ argMatrix.A }
{
    argMatrix.A = nullptr;
}

template< typename T>
mt::Matrix< T >::Matrix( const std::vector< T > &argItems ) :
    dimension{ static_cast< unsigned int >( std::round( std::sqrt( argItems.size() ) ) ) },
    A{ new std::vector< T > }
{
    A->resize( dimension * dimension );
    for ( std::size_t i = 0; i < argItems.size(); i++ ) {
        ( *A )[ i ] = argItems.at( i );
    }
    A->shrink_to_fit();

    if ( A->size() != dimension * dimension ) {
        throw std::runtime_error{ "Quantity of Matrix items does not fit its dimensionality" };
    }
}

template< typename T>
mt::Matrix< T >::Matrix( const unsigned long &argDim, const T &argDefaultValue ) :
    dimension{ argDim },
    A{ new std::vector< T > }
{
    A->resize( dimension * dimension, argDefaultValue );
}

template< typename T>
mt::Matrix< T >::~Matrix() {
    delete A;
}

template< typename T >
std::vector< int > mt::Matrix< T >::ConvertStringVecToIntVec
( const std::vector< std::string > &argStrList ) {
    std::vector< int > intList;
    for ( auto cit = argStrList.cbegin(); cit != argStrList.cend(); ++cit ) {
        intList.emplace_back( std::stoi( *cit ) );
    }
    return intList;
}

template< typename T >
std::vector< unsigned long > mt::Matrix< T >::ConvertStringVecToULongVec
    ( const std::vector< std::string > &argStrList ) {
    std::vector< unsigned long > uLongList;
    for ( auto cit = argStrList.cbegin(); cit != argStrList.cend(); ++cit ) {
        uLongList.emplace_back( std::stoul( *cit ) );
    }
    return uLongList;
}

template< typename T >
T mt::Matrix< T >::GetMinimumValueIndizes( long &argI, long &argJ ) {
    typename std::vector< T >::iterator minElem = std::min_element( A->begin(), A->end() );
    auto offset = std::distance( A->begin(), minElem );
    std::ldiv_t result = std::div( static_cast< long >( offset ),
                                   static_cast< long >( dimension ) );
    argI = result.quot;
    argJ = result.rem;

    return *minElem;
}

#endif // MATRIX_H
