#ifndef SETTINGS_H
#define SETTINGS_H

#include "ezgravercore_global.h"

namespace Ez {

struct EZGRAVERCORESHARED_EXPORT Specifications {
    /*! The time required to erase the EEPROM in milliseconds. */
    static int const EraseTimeMs{6000};

    /*! The image width */
    static int const ImageWidth{512};

    /*! The image height */
    static int const ImageHeight{512};

private:
    Specifications() = delete;
};

}

#endif // SETTINGS_H
