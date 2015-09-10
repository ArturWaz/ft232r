
set(ft232r_source_files _ftd2xx.h FT232R.cpp FT232R.h)

add_library(ft232r ${ft232r_source_files})

if (WIN32)
    target_link_libraries(ft232r ${CMAKE_CURRENT_SOURCE_DIR}/ftdi/ftd2xx64.dll)
elseif (UNIX)
    target_link_libraries(ft232r ${CMAKE_CURRENT_SOURCE_DIR}/ftdi/libftd2xx.so.1.1.12)
else ()
    message( FATAL_ERROR "\nSystem is not supported, cannot compile FT232R library." )
endif()