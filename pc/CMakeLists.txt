set(qtgui_SOURCES main.cpp)
set(qtgui_HEADERS main.h ../openglqt/render.h ../util/camera.h
	menubutton.h gamecontroller.h gamebutton.h
	../util/cursordefines.h ../util/client.h
	botselector.h main.h gameslider.h)
set(qtgui_FORMS ui.ui)
set(qtgui_RESOURCES res.qrc)

set(QT_USE_QTOPENGL TRUE)

qt4_wrap_cpp(qtgui_HEADERS_MOC ${qtgui_HEADERS})
qt4_wrap_ui(qtgui_FORMS_HEADERS ${qtgui_FORMS})
qt4_add_resources(qtgui_RESOURCES_RCC ${qtgui_RESOURCES})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR}/simulation/)

add_executable(../qtgui ${qtgui_SOURCES} ${qtgui_HEADERS_MOC} ${qtgui_FORMS_HEADERS} ${qtgui_RESOURCES_RCC})

target_link_libraries(../qtgui ${QT_LIBRARIES} AWSim)
