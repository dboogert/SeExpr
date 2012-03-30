cmake_minimum_required(VERSION 2.8)

set(QT_USE_QTWEBKIT TRUE)
find_package(Qt4 REQUIRED)

set(src main.cpp mainwindow.cpp highlighter.cpp imager.cpp)
set(inc mainwindow.h highlighter.h imager.h)

qt4_wrap_cpp(inc_moc ${inc})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(editor ${src} ${inc_moc})
target_link_libraries(editor ${QT_LIBRARIES} ${SEEXPR_LIBRARIES})
install(TARGETS editor DESTINATION demo)