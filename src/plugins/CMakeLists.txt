
ADD_LIBRARY (texture SHARED texture.cpp)
target_link_libraries(texture ${SEEXPR_LIBRARIES})

INSTALL (TARGETS texture  DESTINATION plugins)

