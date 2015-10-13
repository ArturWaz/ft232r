
set(ft232r_source_files wrapper_ftd2xx.h ftd2xx.cpp ftd2xx.h)

add_library(ftdi ${ft232r_source_files})

if (WIN32)
    target_link_libraries(ftdi ${CMAKE_CURRENT_SOURCE_DIR}/ftd2xx_from_FTDI/ftd2xx64.dll)
elseif (UNIX)
    target_link_libraries(ftdi ${CMAKE_CURRENT_SOURCE_DIR}/ftd2xx_from_FTDI/libftd2xx.so.1.1.12)
else ()
    message( FATAL_ERROR "\nSystem is not supported, cannot compile ftd2xx library." )
endif()