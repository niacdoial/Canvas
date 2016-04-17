#include "ststpstdlib.h"

// standard header, includes platform dependent stuff and standard stuff

#include <map>
#ifndef WIN32

#ifdef _WIN32
#define WIN32 _WIN32
#elif defined __WIN32__
#define WIN32 __WIN32__
#endif

#endif

#ifdef WIN32
#include "../WIN32_HEAD.h"
#endif