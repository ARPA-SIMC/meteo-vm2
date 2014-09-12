#ifndef METEO_VM2_TOOLS_UTILS
#define METEO_VM2_TOOLS_UTILS

#include <cstdlib>
#include <string>

/**
 * @brief Default source path.
 * The path is the value of the environment variable `METEO_VM2_BUFR_SOURCE` or, if
 * unset, `$localstatedir/lib/meteo-vm2/source/bufr.luac`.
 */
std::string bufr_path();

#endif
