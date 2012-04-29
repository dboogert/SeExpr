
ADD_LIBRARY (Texture SHARED texture.cpp)
target_link_libraries(Texture ${SEEXPR_LIBRARIES})
SET_TARGET_PROPERTIES(Texture PROPERTIES OUTPUT_NAME "Texture" PREFIX "SeExpr" SUFFIX ".so")

INSTALL (TARGETS Texture  DESTINATION plugins)

