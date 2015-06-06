# CMakeLists.txt generated 2015/06/02 14:25:36
# by vcproj05.pl from F:\Projects\npptidy2\Tidy2.sln

# local CMake Scripts.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules )

cmake_minimum_required (VERSION 2.8.8)

project (Tidy2)

option( USE_EXTERNAL_TIDY "Set OFF to not search for an installed lib tidy."         ON )
option( USE_SUBDIRECTORY "Set OFF to comile directly from this root CMakeLists.txt." ON )

# Setup MSVC 3rd party directories if available and needed
# include( ConfigureMsvc3rdParty )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(WARNING_FLAGS_CXX "-Wall")
    set(WARNING_FLAGS_C   "-Wall")
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set (WARNING_FLAGS_CXX "-Wall -Wno-overloaded-virtual")
   set (WARNING_FLAGS_C   "-Wall")
endif() 

if(WIN32 AND MSVC)
    set(MSVC_FLAGS "-DNOMINMAX -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D__CRT_NONSTDC_NO_WARNINGS")
    # turn off various warnings
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305 4996)
    #     set(MSVC_FLAGS "${MSVC_FLAGS} /wd${warning}")
    # endforeach()
    #if (${MSVC_VERSION} EQUAL 1600)
    #    set( MSVC_LD_FLAGS "/FORCE:MULTIPLE" )
    #endif ()
    #set(NOMINMAX 1)
    # to distinguish between debug and release lib in windows
    set( CMAKE_DEBUG_POSTFIX "d" )
    list( APPEND add_LIBS Shlwapi )
endif()

# Uncomment to REDUCE the Windows configurations buildable
# set(CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "" FORCE) # Disables MinSizeRel & MaxSpeedRel

set (BOOST_CXX_FLAGS "-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_BIMAP_DISABLE_SERIALIZATION")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS_C} ${MSVC_FLAGS} -D_REENTRANT")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS_CXX} ${MSVC_FLAGS} -D_REENTRANT ${BOOST_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}")

if (USE_EXTERNAL_TIDY)
    #  TIDY_FOUND          - True if tidy found.
    #  TIDY_INCLUDE_DIRS   - where to find tidy.h, etc.
    #  TIDY_LIBRARIES      - List of libraries when using tidy.
    find_package( Tidy )
    if (TIDY_FOUND)
        message(STATUS "*** Found EXTERNAL Tidy lib ${TIDY_LIBRARIES} inc ${TIDY_INCLUDE_DIRS}")
        list( APPEND add_LIBS ${TIDY_LIBRARIES} )
        include_directories( ${TIDY_INCLUDE_DIRS} )
    else ()
        message(STATUS "*** External Tidy library NOT FOUND" )
    endif ()
endif ()

if (NOT TIDY_FOUND)
# =======================================================================
# =======================================================================
if (USE_SUBDIRECTORY)
###########################################################################    
    add_subdirectory( tidy-html5 ) # Needed modifcation to remove CPack etc...
###########################################################################    
else ()
###########################################################################    
set(LIB_NAME tidy5)

file(READ tidy-html5/version.txt versionFile)
if (NOT versionFile)
    message(FATAL_ERROR "Unable to determine libtidy version. version.txt file is missing.")
endif()
string(STRIP "${versionFile}" LIBTIDY_VERSION)
string(REPLACE "." ";" VERSION_LIST ${LIBTIDY_VERSION})
list(GET VERSION_LIST 0 TIDY_MAJOR_VERSION)
list(GET VERSION_LIST 1 TIDY_MINOR_VERSION)
list(GET VERSION_LIST 2 TIDY_POINT_VERSION)

add_definitions ( -DSUPPORT_UTF16_ENCODINGS=1 )
add_definitions ( -DSUPPORT_ASIAN_ENCODINGS=1 )
add_definitions ( -DSUPPORT_ACCESSIBILITY_CHECKS=1 )
add_definitions ( -DLIBTIDY_VERSION="${LIBTIDY_VERSION}" )

# Issue #188 - Support user items in platform.h
if (TIDY_CONFIG_FILE)
    add_definitions( -DTIDY_CONFIG_FILE="${TIDY_CONFIG_FILE}" )
endif ()
if (TIDY_USER_CONFIG_FILE)
    add_definitions( -DTIDY_USER_CONFIG_FILE="${TIDY_USER_CONFIG_FILE}" )
endif ()
if (SUPPORT_GETPWNAM)
    add_definitions( -DSUPPORT_GETPWNAM=1 )
endif ()

### tidy library
# file locations
set ( SRCDIR tidy-html5/src )
set ( INCDIR tidy-html5/include )
# file lists
set ( CFILES
        ${SRCDIR}/access.c       ${SRCDIR}/attrs.c        ${SRCDIR}/istack.c
        ${SRCDIR}/parser.c       ${SRCDIR}/tags.c         ${SRCDIR}/entities.c
        ${SRCDIR}/lexer.c        ${SRCDIR}/pprint.c       ${SRCDIR}/charsets.c ${SRCDIR}/clean.c
        ${SRCDIR}/localize.c     ${SRCDIR}/config.c       ${SRCDIR}/alloc.c
        ${SRCDIR}/attrask.c      ${SRCDIR}/attrdict.c     ${SRCDIR}/attrget.c
        ${SRCDIR}/buffio.c       ${SRCDIR}/fileio.c       ${SRCDIR}/streamio.c
        ${SRCDIR}/tagask.c       ${SRCDIR}/tmbstr.c       ${SRCDIR}/utf8.c
        ${SRCDIR}/tidylib.c      ${SRCDIR}/mappedio.c     ${SRCDIR}/gdoc.c )
set ( HFILES
        ${INCDIR}/platform.h     ${INCDIR}/tidy.h         ${INCDIR}/tidyenum.h
        ${INCDIR}/buffio.h )
set ( LIBHFILES
        ${SRCDIR}/access.h       ${SRCDIR}/attrs.h        ${SRCDIR}/attrdict.h ${SRCDIR}/charsets.h
        ${SRCDIR}/clean.h        ${SRCDIR}/config.h       ${SRCDIR}/entities.h
        ${SRCDIR}/fileio.h       ${SRCDIR}/forward.h      ${SRCDIR}/lexer.h
        ${SRCDIR}/mappedio.h     ${SRCDIR}/message.h      ${SRCDIR}/parser.h
        ${SRCDIR}/pprint.h       ${SRCDIR}/streamio.h     ${SRCDIR}/tags.h
        ${SRCDIR}/tmbstr.h       ${SRCDIR}/utf8.h         ${SRCDIR}/tidy-int.h
        ${SRCDIR}/version.h      ${SRCDIR}/gdoc.h  )
if (MSVC)
    list(APPEND CFILES ${SRCDIR}/sprtf.c)
    list(APPEND LIBHFILES ${SRCDIR}/sprtf.h)
endif ()
#######################################
# Always build the STATIC library
set(name tidy-static)
add_library ( ${name} STATIC ${CFILES} ${HFILES} ${LIBHFILES} )
set_target_properties( ${name} PROPERTIES 
    OUTPUT_NAME ${LIB_NAME}s
    )
list ( APPEND add_LIBS ${name} )
###########################################################################    
endif ()
include_directories( tidy-html5/include )

# =======================================================================
# =======================================================================
endif ()

# Adding 2 of 7 defines indicated.
add_definitions( 
    -DTIDY2_EXPORTS
    -DTIDY_STATIC
    -DUNICODE
    -D_UNICODE
    )

# Add 3 include directories indicated
include_directories( 
    Tidy2
    Tidy2/NppInterface
    )

### 1 DLL #############################################
# Project [Tidy2], type [Application], 6 C/C++, 12 Hdrs, 0 Other, 0 INCS, 7 DEFS
# File dir [F:\Projects\npptidy2\Tidy2\], out [F:\Projects\npptidy2], rel = [Tidy2\]
#  tidy-html5/include/tidy.h
set(name Tidy2)
set(dir Tidy2)
set( ${name}_SRCS
    ${dir}/AboutDialog.cpp
    ${dir}/NppInterface/StaticDialog.cpp
    ${dir}/PluginMain.cpp
    ${dir}/VersionAndAbout.rc
    ${dir}/dllmain.cpp
    ${dir}/stdafx.cpp
    )
set( ${name}_HDRS
    ${dir}/AboutDialog.h
    ${dir}/NppInterface/Notepad_plus_msgs.h
    ${dir}/NppInterface/PluginInterface.h
    ${dir}/NppInterface/Scintilla.h
    ${dir}/NppInterface/StaticDialog.h
    ${dir}/NppInterface/Window.h
    ${dir}/PluginDetails.h
    ${dir}/PluginMain.h
    ${dir}/resource.h
    ${dir}/stdafx.h
    ${dir}/targetver.h )
add_library( ${name} SHARED ${${name}_SRCS} ${${name}_HDRS}  )
target_link_libraries( ${name} ${add_LIBS} )
message(STATUS "*** target_link_libraries( ${name} ${add_LIBS} )")
###list(APPEND add_LIBS ${name})

# deal with install, if required
# install(TARGETS ${inst_LIBS}
#     RUNTIME DESTINATION bin
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib )
# install(TARGETS ${inst_BINS} DESTINATION bin)
# install(FILES ${inst_HDRS} DESTINATION include)

# eof
