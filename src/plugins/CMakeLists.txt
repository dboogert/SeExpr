
# find ExrBase headers & OIIO headers and libs
# If they're found then build the OIIO texture sample plugin 
INCLUDE_DIRECTORIES(/Users/dboogert/work/oiio/dist/macosx/include /opt/local/include)
LINK_DIRECTORIES(/Users/dboogert/work/oiio/dist/macosx/lib)

ADD_LIBRARY (OIIOTexture SHARED texture.cpp)
TARGET_LINK_LIBRARIES(OIIOTexture ${SEEXPR_LIBRARIES} OpenImageIO)
SET_TARGET_PROPERTIES(OIIOTexture PROPERTIES OUTPUT_NAME "OIIOTexture" PREFIX "SeExpr" SUFFIX ".so")

INSTALL (TARGETS OIIOTexture  DESTINATION plugins)

