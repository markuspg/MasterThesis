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

#include "task_storage.h"

mt::TaskStorage::TaskStorage(const TaskStorage &argTaskStorage) :
    tasks{ argTaskStorage.tasks.size(), nullptr }
{
    std::vector< Task* >::size_type size = argTaskStorage.tasks.size();
    for ( unsigned long i = 0; i < size; ++i ) {
        std::vector< Task* > *predecessors = new std::vector< Task* >;
        for ( auto cit = argTaskStorage.tasks[ i ]->predecessors->cbegin();
              cit != argTaskStorage.tasks[ i ]->predecessors->cend(); ++cit ) {
            predecessors->emplace_back( tasks[ ( *cit )->index - 1 ] );
        }
        tasks[ i ] = new Task{ argTaskStorage.tasks[ i ]->duration, argTaskStorage.tasks[ i ]->index,
                               predecessors };
    }
}

mt::TaskStorage::TaskStorage( const unsigned long &argSize, const std::vector< std::string > &argTokens ) :
    tasks{ argSize, nullptr }
{
    // Split the tokens for their usage whilst the Tasks' construction
    std::vector< std::string > durationStrings = tools::Split( argTokens[ 3 ], ';' );
    std::vector< std::string > precedenceStrings = tools::Split( argTokens[ 4 ], ';' );

    // Construct all tasks, one after another
    for ( unsigned long i = 0; i < argSize; ++i ) {
        // First declare and initialize the needed variables
        std::vector< Task* > *predecessors = new std::vector< Task* >;
        unsigned long taskDuration = 0;
        unsigned long taskID = 0;

        // Then convert the string values to the needed data items
        taskDuration = std::stoul( durationStrings[ i ] );
        std::vector< std::string > precedenceString = tools::Split( precedenceStrings[ i ], ':' );
        taskID = std::stoul( precedenceString[ 0 ] );
        if ( precedenceString.size() > 1 ) {
            std::vector< std::string > predecessorIndicesString = tools::Split( precedenceString[ 1 ], ',' );
            for ( std::size_t j = 0; j < predecessorIndicesString.size(); ++j ) {
                const unsigned long predecessorIndexString = std::stoul( predecessorIndicesString[ j ] ) - 1;
                // Ensure lexicographical ordering of the tasks
                assert( predecessorIndexString < i );
                predecessors->emplace_back( tasks[ predecessorIndexString ] );
            }
        }
        tasks[ i ] = new Task{ taskDuration, taskID, predecessors };
    }
}

mt::TaskStorage::~TaskStorage() {
    for ( auto &s: tasks ) {
        delete s;
    }
}
