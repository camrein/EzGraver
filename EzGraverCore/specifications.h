#ifndef SETTINGS_H
#define SETTINGS_H

#include "ezgravercore_global.h"

namespace Ez {
namespace Specifications {

/*! The time required to erase the EEPROM in milliseconds. */
int const EraseTimeMs{6000};

/*! The image width */
int const ImageWidth{512};

/*! The image height */
int const ImageHeight{512};

}
}

#endif // SETTINGS_H
