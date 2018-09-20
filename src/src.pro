
# qmake project file for building the vldField_qt library

VLDFIELD_QT_ROOT = $${PWD}/..
include( $${VLDFIELD_QT_ROOT}/vldField_qtconfig.pri )
include( $${VLDFIELD_QT_ROOT}/vldField_qtbuild.pri )
include( $${VLDFIELD_QT_ROOT}/vldField_qtfunctions.pri )

VLDFIELD_QT_OUT_ROOT = $${OUT_PWD}/..

TEMPLATE          = lib
TARGET            = $$vldField_qtLibraryTarget(vldField_qt)

DESTDIR           = $${VLDFIELD_QT_OUT_ROOT}/lib

contains(VLDFIELD_QT_CONFIG, vldField_qtDll) {

    CONFIG += dll
    win32|symbian: DEFINES += QT_DLL VLDFIELD_QT_DLL VLDFIELD_QT_MAKEDLL
}
else {
    CONFIG += staticlib
} 

contains(VLDFIELD_QT_CONFIG, vldField_qtFramework) {

    CONFIG += lib_bundle
}

include ( $${PWD}/src.pri )

# Install directives

target.path    = $${VLDFIELD_QT_INSTALL_LIBS}
INSTALLS       = target 

CONFIG(lib_bundle) {

    FRAMEWORK_HEADERS.version = Versions
    FRAMEWORK_HEADERS.files = $${HEADERS}
    FRAMEWORK_HEADERS.path = Headers
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}
else {

    headers.files  = $${HEADERS}
    headers.path   = $${VLDFIELD_QT_INSTALL_HEADERS}
    INSTALLS += headers
}

contains(VLDFIELD_QT_CONFIG, vldField_qtPkgConfig) {

    CONFIG     += create_pc create_prl no_install_prl

    QMAKE_PKGCONFIG_NAME = vldField_qt$${VLDFIELD_QT_VER_MAJ}
    QMAKE_PKGCONFIG_DESCRIPTION = Qt Widgets for Technical Applications

    QMAKE_PKGCONFIG_LIBDIR = $${VLDFIELD_QT_INSTALL_LIBS}
    QMAKE_PKGCONFIG_INCDIR = $${VLDFIELD_QT_INSTALL_HEADERS}

    QMAKE_PKGCONFIG_DESTDIR = pkgconfig

    greaterThan(QT_MAJOR_VERSION, 4) {

        QMAKE_PKGCONFIG_FILE = Qt$${QT_MAJOR_VERSION}$${QMAKE_PKGCONFIG_NAME}
        QMAKE_PKGCONFIG_REQUIRES = Qt5Widgets Qt5Concurrent Qt5PrintSupport

        contains(VLDFIELD_QT_CONFIG, vldField_qtSvg) {
            QMAKE_PKGCONFIG_REQUIRES += Qt5Svg
        }

        contains(VLDFIELD_QT_CONFIG, vldField_qtOpenGL) {
            QMAKE_PKGCONFIG_REQUIRES += Qt5OpenGL
        }

        QMAKE_DISTCLEAN += $${DESTDIR}/$${QMAKE_PKGCONFIG_DESTDIR}/$${QMAKE_PKGCONFIG_FILE}.pc
    }
    else {

        # there is no QMAKE_PKGCONFIG_FILE fo Qt4
        QMAKE_PKGCONFIG_REQUIRES = QtGui 

        contains(VLDFIELD_QT_CONFIG, vldField_qtSvg) {
            QMAKE_PKGCONFIG_REQUIRES += QtSvg
        }

        contains(VLDFIELD_QT_CONFIG, vldField_qtOpenGL) {
            QMAKE_PKGCONFIG_REQUIRES += QtOpenGL
        }

        QMAKE_DISTCLEAN += $${DESTDIR}/$${QMAKE_PKGCONFIG_DESTDIR}/$${TARGET}.pc
    }

    QMAKE_DISTCLEAN += $${DESTDIR}/libvldField_qt.prl
}
