//
//  Config.h
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef Broadway_core_Config_h
#define Broadway_core_Config_h

#define BROADWAY_DEBUG


#ifndef USE_JSON_PARSER
#define USE_JSON_PARSER
#endif

#ifndef USE_NETWORK
#define USE_NETWORK
#endif

#ifdef USE_JAVA_INTERPRETER
#undef USE_JAVA_INTERPRETER
#endif

#ifdef USE_WEB_SERVER
#undef USE_WEB_SERVER
#endif

#ifndef USE_GPIO
#define USE_GPIO
#endif


#ifdef USE_GRAPHICS
#undef USE_GRAPHICS
#endif

#ifdef USE_GRAPHICS_HELPERS
#undef USE_GRAPHICS_HELPERS
#endif

#ifdef USE_GRAPHICS_EXTRA
#undef USE_GRAPHICS_EXTRA
#endif

#ifndef USE_XML_PARSER
#define USE_XML_PARSER
#endif

#include "../../Broadway/Core/Core.h"


#endif
