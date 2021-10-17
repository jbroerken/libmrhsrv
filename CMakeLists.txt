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
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 99)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(libmrhsrv VERSION 1.0.0
                  DESCRIPTION "MRH Platform Server Communication Library"
                  LANGUAGES C)
set(LIB_SO_VERSION 1)

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
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MRH_CommunicationAuth.cpp"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MRH_CommunicationAuth.h"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MRH_CommunicationClient.cpp"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_ServerConnectionInternal.h"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_Server.c"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_ServerRevision.c"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Error/MRH_ServerError.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Communication/MRH_CommunicationClient.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Communication/MRH_CommunicationOpCode.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_Server.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerConnection.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerActor.h"
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
add_library(libmrhsrv_Shared SHARED ${SRC_LIST_GLOBAL})
set_target_properties(libmrhsrv_Shared
                      PROPERTIES
                      PREFIX "lib"
                      OUTPUT_NAME "mrhsrv"
                      SUFFIX ".so"
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${LIB_SO_VERSION})
					  
add_library(libmrhsrv_Static STATIC ${SRC_LIST_GLOBAL})
set_target_properties(libmrhsrv_Static
    				  PROPERTIES
					  PREFIX "lib"
					  OUTPUT_NAME "mrhsrv"
					  SUFFIX ".a"
                      VERSION ${PROJECT_VERSION})

###
#  Install
#  -------
#  Library installation.
###
install(TARGETS libmrhsrv_Shared libmrhsrv_Static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${PUBLIC_API_DIR_PATH}/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.h")