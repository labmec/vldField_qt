
HEADERS += \
    VLDFintegerlineedit.h \
	VLDFvalidationcontroller.h \
    VLDFdoublelineedit.h \
	VLDFunitinput.h \
	VLDFvalidationinput.h \
	VLDFpopupmessage.h

	
	

SOURCES += \
    VLDFintegerlineedit.cpp \
	VLDFvalidationcontroller.cpp \
	VLDFdoublelineedit.cpp \
	VLDFunitinput.cpp \
	VLDFvalidationinput.cpp \
    VLDFpopupmessage.cpp	
		
 
greaterThan(QT_MAJOR_VERSION, 4) {

    QT += printsupport
    QT += concurrent
} 

contains(VLDFIELD_QT_CONFIG, vldField_qtSvg) {

    QT += svg

    HEADERS += qwt_plot_svgitem.h
    SOURCES += qwt_plot_svgitem.cpp 
}
else {

    DEFINES += VLDFIELD_QT_NO_SVG
}

contains(VLDFIELD_QT_CONFIG, vldField_qtOpenGL) {

    QT += opengl

    HEADERS += qwt_plot_glcanvas.h
    SOURCES += qwt_plot_glcanvas.cpp
}
else {

    DEFINES += VLDFIELD_QT_NO_OPENGL
}

contains(VLDFIELD_QT_CONFIG, vldField_qtWidgets) {

    HEADERS += \
        qwt_abstract_slider.h \
        qwt_abstract_scale.h \
        qwt_arrow_button.h \
        qwt_analog_clock.h \
        qwt_compass.h \
        qwt_compass_rose.h \
        qwt_counter.h \
        qwt_dial.h \
        qwt_dial_needle.h \
        qwt_knob.h \
        qwt_slider.h \
        qwt_thermo.h \
        qwt_wheel.h
    
    SOURCES += \
        qwt_abstract_slider.cpp \
        qwt_abstract_scale.cpp \
        qwt_arrow_button.cpp \
        qwt_analog_clock.cpp \
        qwt_compass.cpp \
        qwt_compass_rose.cpp \
        qwt_counter.cpp \
        qwt_dial.cpp \
        qwt_dial_needle.cpp \
        qwt_knob.cpp \
        qwt_slider.cpp \
        qwt_thermo.cpp \
        qwt_wheel.cpp
}
