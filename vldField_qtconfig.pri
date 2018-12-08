
VLDFIELD_QT_VER_MAJ      = 1
VLDFIELD_QT_VER_MIN      = 0
VLDFIELD_QT_VER_PAT      = 0
VLDFIELD_QT_VERSION      = $${VLDFIELD_QT_VER_MAJ}.$${VLDFIELD_QT_VER_MIN}.$${VLDFIELD_QT_VER_PAT}

######################################################################
# Install paths
######################################################################

VLDFIELD_QT_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

unix {
    VLDFIELD_QT_INSTALL_PREFIX    = /usr/local/vldField_qt-$$VLDFIELD_QT_VERSION
    # VLDFIELD_QT_INSTALL_PREFIX = /usr/local/vldField_qt-$$VLDFIELD_QT_VERSION-qt-$$QT_VERSION
}

win32 {
    VLDFIELD_QT_INSTALL_PREFIX    = C:/vldField_qt-$$VLDFIELD_QT_VERSION
    # VLDFIELD_QT_INSTALL_PREFIX = C:/vldField_qt-$$VLDFIELD_QT_VERSION-qt-$$QT_VERSION
}

VLDFIELD_QT_INSTALL_HEADERS   = $${VLDFIELD_QT_INSTALL_PREFIX}/include
VLDFIELD_QT_INSTALL_LIBS      = $${VLDFIELD_QT_INSTALL_PREFIX}/lib

######################################################################
# Designer plugin
# creator/designer load designer plugins from certain default
# directories ( f.e the path below QT_INSTALL_PREFIX ) and the 
# directories listed in the QT_PLUGIN_PATH environment variable.
# When using the path below VLDFIELD_QT_INSTALL_PREFIX you need to
# add $${VLDFIELD_QT_INSTALL_PREFIX}/plugins to QT_PLUGIN_PATH in the 
# runtime environment of designer/creator.
######################################################################

VLDFIELD_QT_INSTALL_PLUGINS   = $${VLDFIELD_QT_INSTALL_PREFIX}/plugins/designer

# linux distributors often organize the Qt installation
# their way and QT_INSTALL_PREFIX doesn't offer a good
# path. Also QT_INSTALL_PREFIX is only one of the default
# search paths of the designer - not the Qt creator

#VLDFIELD_QT_INSTALL_PLUGINS   = $$[QT_INSTALL_PREFIX]/plugins/designer

######################################################################
# Features
# When building a VLDFIELD_QT application with qmake you might want to load
# the compiler/linker flags, that are required to build a VLDFIELD_QT application
# from VLDFIELD_QT.prf. Therefore all you need to do is to add "CONFIG += VLDFIELD_QT" 
# to your project file and take care, that VLDFIELD_QT.prf can be found by qmake.
# ( see http://doc.trolltech.com/4.7/qmake-advanced-usage.html#adding-new-configuration-features )
# I recommend not to install the VLDFIELD_QT features together with the
# Qt features, because you will have to reinstall the VLDFIELD_QT features,
# with every Qt upgrade. 
######################################################################

VLDFIELD_QT_INSTALL_FEATURES  = $${VLDFIELD_QT_INSTALL_PREFIX}/features
# VLDFIELD_QT_INSTALL_FEATURES  = $$[QT_INSTALL_PREFIX]/features

######################################################################
# Build the static/shared libraries.
# If VLDFIELD_QTDll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

#VLDFIELD_QT_CONFIG           += VLDFIELD_QTDll

######################################################################
# VLDFIELD_QTPlot enables all classes, that are needed to use the VLDFIELD_QTPlot 
# widget. 
######################################################################

#VLDFIELD_QT_CONFIG       += VLDFIELD_QTPlot

######################################################################
# VLDFIELD_QTWidgets enables all classes, that are needed to use the all other
# widgets (sliders, dials, ...), beside VLDFIELD_QTPlot. 
######################################################################

#VLDFIELD_QT_CONFIG     += VLDFIELD_QTWidgets

######################################################################
# If you want to display svg images on the plot canvas, or
# export a plot to a SVG document
######################################################################

#VLDFIELD_QT_CONFIG     += VLDFIELD_QTSvg

######################################################################
# If you want to use a OpenGL plot canvas
######################################################################

#VLDFIELD_QT_CONFIG     += VLDFIELD_QTOpenGL

######################################################################
# You can use the MathML renderer of the Qt solutions package to 
# enable MathML support in VLDFIELD_QT. Because of license implications
# the ( modified ) code of the MML Widget solution is included and
# linked together with the VLDFIELD_QTMathMLTextEngine into an own library. 
# To use it you will have to add "CONFIG += VLDFIELD_QTmathml"
# to your qmake project file.
######################################################################

#VLDFIELD_QT_CONFIG     += VLDFIELD_QTMathML

######################################################################
# If you want to build the VLDFIELD_QT designer plugin, 
# enable the line below.
# Otherwise you have to build it from the designer directory.
######################################################################

VLDFIELD_QT_CONFIG     += VLDFIELD_QTDesigner

######################################################################
# Compile all VLDFIELD_QT classes into the designer plugin instead
# of linking it against the shared VLDFIELD_QT library. Has no effect
# when VLDFIELD_QTDesigner or VLDFIELD_QTDll are not both enabled.
#
# On systems where rpath is supported ( all Unixoids ) the 
# location of the installed VLDFIELD_QT library is compiled into the plugin,
# but on Windows it might be easier to have a self contained
# plugin to avoid any hassle with configuring the runtime
# environment of the designer/creator.
######################################################################

win32 {
    VLDFIELD_QT_CONFIG     += VLDFIELD_QTDesignerSelfContained
}

######################################################################
# When Qt has been built as framework qmake wants 
# to link frameworks instead of regular libs
######################################################################

macx:!static:CONFIG(qt_framework, qt_framework|qt_no_framework) {

    VLDFIELD_QT_CONFIG += VLDFIELD_QTFramework
}  

######################################################################
# Create and install pc files for pkg-config
# See http://www.freedesktop.org/wiki/Software/pkg-config/
######################################################################

unix {

    #VLDFIELD_QT_CONFIG     += VLDFIELD_QTPkgConfig
}
