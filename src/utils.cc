#include "utils.h"

std::string bufr_path()
{
	const char* s = ::getenv("METEO_VM2_BUFR_SOURCE");
	if (!s)
		return METEO_VM2_BUFR_SOURCE;
	return s;
}
