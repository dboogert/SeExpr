

include_directories(/Users/dboogert/work/oiio/dist/macosx/include /opt/local/include)
link_directories(/Users/dboogert/work/oiio/dist/macosx/lib)

ADD_LIBRARY (Texture SHARED texture.cpp)
target_link_libraries(Texture ${SEEXPR_LIBRARIES} OpenImageIO)
SET_TARGET_PROPERTIES(Texture PROPERTIES OUTPUT_NAME "Texture" PREFIX "SeExpr" SUFFIX ".so")

INSTALL (TARGETS Texture  DESTINATION plugins)

