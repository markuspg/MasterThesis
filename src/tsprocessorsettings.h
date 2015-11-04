#ifndef TABOOSEARCHPROCESSORSETTINGS_H
#define TABOOSEARCHPROCESSORSETTINGS_H

#include "matrix.h"

namespace mt {

struct TSProcessorSettings {
public:
    TSProcessorSettings( const unsigned int &argTabooTenure,
                         mt::Matrix< unsigned int > * const argTabooTenures );

    const unsigned int tabooTenure = 0;
    mt::Matrix< unsigned int > * const tabooTenures = nullptr;
};

}

#endif // TABOOSEARCHPROCESSORSETTINGS_H
