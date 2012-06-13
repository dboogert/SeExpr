cmake_minimum_required(VERSION 2.8)

set(src display.cpp)

add_library(ed SHARED ${src})
set_target_properties(ed PROPERTIES PREFIX "" SUFFIX ".dpy")
target_link_libraries(ed ${RMAN_LIBS})
install(TARGETS ed DESTINATION demo)
