#include "tsprocessorsettings.h"

mt::TSProcessorSettings::TSProcessorSettings( const unsigned int &argTabooTenure,
                                              mt::Matrix< unsigned int > * const argTabooTenures ) :
    tabooTenure{ argTabooTenure },
    tabooTenures{ argTabooTenures }
{
}
