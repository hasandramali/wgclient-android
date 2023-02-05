#
# Copyright (c) 2015 Pavlo Lavrenenko
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 2.6.0)
project (SVDLL)

set (SVDLL_LIBRARY server)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions -w -frtti -Wl,--no-undefined")
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

include(../cmake/cotire.cmake)

file( GLOB_RECURSE SVDLL_SOURCES
	*.cpp
	../game_shared/*.cpp
	../regamedll/*.cpp
	../version/*.cpp
	../engine/*.cpp
	../pm_shared/*.cpp
	../public/interface.cpp
	../public/MemPool.cpp )
list( APPEND SVDLL_SOURCES ${PM_SOURCES} )

include_directories (. .. hostage bot ../common ../engine/common ../engine ../public ../public/tier1 ../pm_shared ../game_shared ../game_shared/bot ../regamedll ../public/regamedll)

add_definitions( -D_LINUX -DCLIENT_WEAPONS -D_access=access -D_vsnwprintf=vswprintf -Dstricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -D_snprintf=snprintf -D_stricmp=strcasecmp -D_vsnprintf=vsnprintf -D_strdup=strdup -D_write=write -D_close=close -DREGAMEDLL_FIXES -DREGAMEDLL_CHECKS -DREGAMEDLL_ADD -DDEDICATED -DREGAMEDLL_SELF -DREGAMEDLL_API -DCLIENT_WEAPONS -fpermissive -DAPP_VERSION="none" -DAPP_COMMIT_TIME="none" -DAPP_COMMIT_DATE="none" -DAPP_COMMIT_SHA="none" -DAPP_COMMIT_URL="none" -DIGNORE_APPVERSION_H )

add_library (${SVDLL_LIBRARY} SHARED ${SVDLL_SOURCES})

target_link_libraries(${SVDLL_LIBRARY} ${CMAKE_DL_LIBS})

set_target_properties (${SVDLL_SHARED} PROPERTIES
	POSITION_INDEPENDENT_CODE 1
	COTIRE_CXX_PREFIX_HEADER_INIT "precompiled.h")

if( CS16CLIENT_ANALYZE )
	include(../cmake/PVS-Studio.cmake)
	pvs_studio_add_target(TARGET analyze ALL
		FORMAT tasklist
		LOG "${CMAKE_CURRENT_LIST_DIR}/regamedll.tasks"
		ANALYZE ${SVDLL_LIBRARY})
endif()

cotire( ${SVDLL_LIBRARY} )
