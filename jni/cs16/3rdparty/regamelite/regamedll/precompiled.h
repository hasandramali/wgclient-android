#pragma once

#ifndef IGNORE_APPVERSION_H
#include "version/appversion.h"
#endif

#include "osconfig.h"
#include "basetypes.h"
#include "archtypes.h"

#if defined(__ARM_NEON__) || defined(__NEON__)

#define SIMD_AVAILABLE
#define SIMD_AVAILABLE_NEON
#include "neon_mathfun.h"

#elif defined(__SSE2__) || defined(_M_IX86_FP)

#define SIMD_AVAILABLE
#define SIMD_AVAILABLE_SSE2
#include "sse_mathfun.h"
#include "asmlib.h"

#endif

#include "MemPool.h"
#include "engine.h"
#include "platform.h"
#include "regamedll_debug.h"

#ifdef HOOK_GAMEDLL
#include "memory.h"
#include "hooker.h"
#include "RegameDLLRuntimeConfig.h"

// STL containers Visual Studio 6.0
#include "stl/vector"
#include "stl/list"
#endif // HOOK_GAMEDLL

//valve libs stuff
#include "tier0/platform.h"
#include "tier0/dbg.h"

#include "dlls.h"
#include "interface.h"
#include "hookchains_impl.h"
#include "regamedll.h"

#include "regamedll_interfaces.h"
#include "regamedll_api.h"

#include "regamedll_interfaces_impl.h"
#include "regamedll_api_impl.h"
