find_package(Qt4 COMPONENTS QtCore QtOpenGL QtGui)

set(COMP_GUI true CACHE BOOL "Compile GUI")

if(QT4_FOUND AND COMP_GUI)
	set(qtgui_SOURCES main.cpp botselector.cpp gamebutton.cpp
		gamecontroller.cpp gameslider.cpp menubutton.cpp)
	set(qtgui_HEADERS main.h ../openglqt/render.h ../util/camera.h
		menubutton.h gamecontroller.h gamebutton.h
		../util/cursordefines.h ../util/client.h
		botselector.h main.h gameslider.h ../openglqt/skeletalsystem.h
		../Qt-based-common-libs/globj.h)

	set(QT_USE_QTOPENGL TRUE)

	qt4_wrap_cpp(qtgui_HEADERS_MOC ${qtgui_HEADERS})
	qt4_wrap_ui(qtgui_FORMS_HEADERS ${qtgui_FORMS})

	include(${QT_USE_FILE})
	add_definitions(${QT_DEFINITIONS})
	include_directories(${CMAKE_SOURCE_DIR}/simulation/)

	add_executable(../qtgui ${qtgui_SOURCES} ${qtgui_HEADERS_MOC} ${qtgui_FORMS_HEADERS} ${qtgui_RESOURCES_RCC})

	target_link_libraries(../qtgui ${QT_LIBRARIES} AWSim AWExtSim nepeta)
endif()
