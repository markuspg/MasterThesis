/*
 * Copyright 2015-2018 Markus Prasser
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
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <vector>

namespace mt {

template<typename T>
class Matrix final
{
public:
    Matrix() = delete;
    Matrix(const std::vector<T> &argItems);
    Matrix(const unsigned long argDim, const T &argDefaultValue);
    ~Matrix() = default;

    static std::vector<int> ConvertStringVecToIntVec(
            const std::vector<std::string> &argStrList);
    static std::vector<unsigned long> ConvertStringVecToULongVec
        (const std::vector<std::string> &argStrList);
    T GetMinimumValueIndizes(long &argI, long &argJ, bool(*argComp)(T a, T b));
    void ResetWithValue(const T &argValue);

    T& operator() (const unsigned int argI, const unsigned int argJ);
    const T& operator() (const unsigned int argI, const unsigned int argJ) const;

private:
    const unsigned long dimension = 0;
    std::vector<T> A;
};

} // namespace mt

template<typename T>
mt::Matrix<T>::Matrix(const std::vector<T> &argItems) :
    dimension{static_cast<unsigned int>(std::round(std::sqrt(argItems.size())))},
    A{argItems}
{
    if (A.size() != dimension * dimension) {
        throw std::runtime_error{"Size of new mt::Matrix's vector does not match"};
    }
}

template<typename T>
mt::Matrix<T>::Matrix(const unsigned long argDim, const T &argDefaultValue) :
    dimension{argDim},
    A(dimension * dimension, argDefaultValue)
{
}

template <typename T>
inline T& mt::Matrix<T>::operator() (const unsigned int argI,
                                     const unsigned int argJ) {
    return A.at(argI * dimension + argJ);
}

template <typename T>
inline const T& mt::Matrix<T>::operator() (const unsigned int argI,
                                           const unsigned int argJ) const {
    return A.at(argI * dimension + argJ);
}

template<typename T>
std::vector<int> mt::Matrix<T>::ConvertStringVecToIntVec(
        const std::vector<std::string> &argStrList) {
    std::vector<int> intVec;
    intVec.reserve(argStrList.size());
    for (auto cit = argStrList.cbegin(); cit != argStrList.cend(); ++cit) {
        intVec.emplace_back(std::stoi(*cit));
    }
    return intVec;
}

template<typename T>
std::vector<unsigned long> mt::Matrix<T>::ConvertStringVecToULongVec
        (const std::vector<std::string> &argStrList) {
    std::vector<unsigned long> uLongVec;
    uLongVec.reserve(argStrList.size());
    for (auto cit = argStrList.cbegin(); cit != argStrList.cend(); ++cit) {
        uLongVec.emplace_back(std::stoul(*cit));
    }
    return uLongVec;
}

template<typename T>
T mt::Matrix<T>::GetMinimumValueIndizes(long &argI, long &argJ,
                                        bool(*argComp)(T a, T b)) {
    const auto minElem = std::min_element(A.begin(), A.end(), argComp);
    auto offset = std::distance(A.begin(), minElem);
    std::ldiv_t result = std::div(static_cast<long>(offset),
                                  static_cast<long>(dimension));
    argI = result.quot;
    argJ = result.rem;

    return *minElem;
}

template<typename T>
void mt::Matrix<T>::ResetWithValue(const T &argValue) {
    A.clear();
    A.resize(dimension * dimension, argValue);
}

#endif // MATRIX_H
