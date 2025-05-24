#ifndef GW2_BOSSES_VERSION_H
#define GW2_BOSSES_VERSION_H

    #include "../BuildVersion.h"

    #define VERSION_MAJOR 1
    #define VERSION_MINOR 4
    #define VERSION_BUILD 3
    #define VERSION_REVISION_BASE 0

    #define STRINGIZE2(x) #x
    #define STRINGIZE(x) STRINGIZE2(x)

    #ifdef _DEBUG
        #define GET_REVISION() VERSION_REVISION
    #else
        #define GET_REVISION() VERSION_REVISION_BASE
    #endif

    #define VERSION_STRING  \
        STRINGIZE(VERSION_MAJOR) "." \
        STRINGIZE(VERSION_MINOR) "." \
        STRINGIZE(VERSION_BUILD) "." \
        STRINGIZE(GET_REVISION())


#endif // !GW2_BOSSES_VERSION_H
