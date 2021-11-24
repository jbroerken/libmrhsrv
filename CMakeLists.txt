#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
#set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 11)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(libmrhsrv VERSION 1.0.0
                  DESCRIPTION "MRH Platform Server Communication Library"
                  LANGUAGES C)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Install Paths
#  -------------
#  GNU system paths for installation.
###
include(GNUInstallDirs)

###
#  Build Paths
#  -----------
#  The paths for the cmake build.
###
set(BUILD_DIR_PATH "${CMAKE_SOURCE_DIR}/build/")
file(MAKE_DIRECTORY ${BUILD_DIR_PATH})

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")
set(INCLUDE_DIR_PATH "${CMAKE_SOURCE_DIR}/include/")

set(SRC_LIST_GLOBAL "${SRC_DIR_PATH}/libmrhsrv/Error/MRH_ServerError.c"
					"${SRC_DIR_PATH}/libmrhsrv/Error/MRH_ServerErrorInternal.h"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MsQuic/MRH_MsQuic.c"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MsQuic/MRH_MsQuic.h"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MsQuic/MRH_MsQuicContext.c"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MsQuic/MRH_MsQuicContext.h"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/NetMessage/MRH_NetMessageV1.c"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/NetMessage/MRH_NetMessageV1.h"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MRH_ServerCommunication.c"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_Server.c"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_ServerTypesInternal.h"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_ServerRevision.c"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Error/MRH_ServerError.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Communication/MRH_ServerCommunication.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Communication/MRH_NetMessage.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_Server.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerTypes.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerSizes.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerRevision.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv.h")
					
###
#  Public API Path
#  ---------------
#  The path to the public api include folder.
###
set(PUBLIC_API_DIR_PATH "${INCLUDE_DIR_PATH}/libmrhsrv")
					 
#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
###
add_library(libmrhsrv_Static STATIC ${SRC_LIST_GLOBAL})
set_target_properties(libmrhsrv_Static
    				  PROPERTIES
					  PREFIX "lib"
					  OUTPUT_NAME "mrhsrv"
					  SUFFIX ".a"
                      VERSION ${PROJECT_VERSION})

###
#  Required Libraries
#  ------------------
#  Libraries required by this platform service.
###
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

include_directories ("/usr/local/msquic/include")

find_library(libmsquic NAMES msquic HINTS "/usr/local/msquic/lib" REQUIRED)
find_library(libsodium NAMES sodium REQUIRED)

target_link_libraries(libmrhsrv_Static PUBLIC msquic)
target_link_libraries(libmrhsrv_Static PUBLIC sodium)

###
#  Install
#  -------
#  Library installation.
###
install(TARGETS libmrhsrv_Static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${PUBLIC_API_DIR_PATH}/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.h")