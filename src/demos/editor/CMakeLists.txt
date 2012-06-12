cmake_minimum_required(VERSION 2.8)

SET(RENDERMAN_FOUND TRUE)

IF(RENDERMAN_FOUND)
	MESSAGE(STATUS "Found renderman: " $ENV{RMANTREE}) 
	SET(RMAN_INC_DIR $ENV{RMANTREE}/include)
	SET(RMAN_LIB_DIR $ENV{RMANTREE}/lib)

	SET(RMAN_LIBS 3delight)

	INCLUDE_DIRECTORIES(${RMAN_INC_DIR})
	LINK_DIRECTORIES(${RMAN_LIB_DIR})
ENDIF() 

set(QT_USE_QTWEBKIT TRUE)
find_package(Qt4 REQUIRED)

set(src main.cpp mainwindow.cpp highlighter.cpp imager.cpp autocomplete.cpp autocompletelistwidget.cpp codeeditor.cpp)
set(inc mainwindow.h highlighter.h imager.h autocomplete.h autocompletelistwidget.h codeeditor.h)

qt4_wrap_cpp(inc_moc ${inc})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(editor ${src} ${inc_moc})
target_link_libraries(editor ${QT_LIBRARIES} ${SEEXPR_LIBRARIES} ${RMAN_LIBS})
install(TARGETS editor DESTINATION demo)

ADD_SUBDIRECTORY (rman)