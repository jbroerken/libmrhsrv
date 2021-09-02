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
project(libmrhsrv VERSION 1.0
                  DESCRIPTION "MRH Platform Server Communication Library"
                  LANGUAGES C)


#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Binary Paths
#  ------------
#  The paths for our created binary file(s).
###
set(BIN_DIR_PATH "${CMAKE_SOURCE_DIR}/lib/")
file(MAKE_DIRECTORY ${BIN_DIR_PATH})

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
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MRH_CommunicationApp.cpp"
					"${SRC_DIR_PATH}/libmrhsrv/Communication/MRH_CommunicationCommunication.cpp"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_ServerOpCode.h"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_ServerConnectionInternal.h"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_Server.c"
					"${SRC_DIR_PATH}/libmrhsrv/MRH_ServerRevision.c"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Error/MRH_ServerError.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Communication/MRH_CommunicationClient.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Communication/MRH_CommunicationApp.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/Communication/MRH_CommunicationPlatform.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_Server.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerConnection.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerActor.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv/MRH_ServerRevision.h"
					"${INCLUDE_DIR_PATH}/libmrhsrv/libmrhsrv.h")
					
###
#  Library Paths
#  -------------
#  The paths to the external libraries to use. 
#  Both the binary and include paths should be set here.
#  Add OS specific libraries in their own lists.
###
set(LIB_INCLUDE_LIST "${INCLUDE_DIR_PATH}/mrhshared")
					 
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
    				  ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    				  LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    				  RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})
					  
add_library(libmrhsrv_Static STATIC ${SRC_LIST_GLOBAL})
set_target_properties(libmrhsrv_Static
    				  PROPERTIES
					  PREFIX "lib"
					  OUTPUT_NAME "mrhsrv"
					  SUFFIX ".a"
    				  ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    				  LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    				  RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})

###
#  Required Libraries
#  ------------------
#  Libraries and / or headers required by this library.
###
include_directories(${LIB_INCLUDE_LIST})