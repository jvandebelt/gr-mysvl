INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_MYSVL mysvl)

FIND_PATH(
    MYSVL_INCLUDE_DIRS
    NAMES mysvl/api.h
    HINTS $ENV{MYSVL_DIR}/include
        ${PC_MYSVL_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    MYSVL_LIBRARIES
    NAMES gnuradio-mysvl
    HINTS $ENV{MYSVL_DIR}/lib
        ${PC_MYSVL_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MYSVL DEFAULT_MSG MYSVL_LIBRARIES MYSVL_INCLUDE_DIRS)
MARK_AS_ADVANCED(MYSVL_LIBRARIES MYSVL_INCLUDE_DIRS)

