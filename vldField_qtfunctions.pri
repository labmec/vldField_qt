
# Copied and modified from qt_functions.prf

defineReplace(vldField_qtLibraryTarget) {

    unset(LIBRARY_NAME)
    LIBRARY_NAME = $$1

    mac:contains(VLDFIELD_QT_CONFIG, vldField_qtFramework) {

        QMAKE_FRAMEWORK_BUNDLE_NAME = $$LIBRARY_NAME
        export(QMAKE_FRAMEWORK_BUNDLE_NAME)
    }

    contains(TEMPLATE, .*lib):CONFIG(debug, debug|release) {

        !debug_and_release|build_pass {

            mac:RET = $$member(LIBRARY_NAME, 0)_debug
            win32:RET = $$member(LIBRARY_NAME, 0)d
        }
    }

    isEmpty(RET):RET = $$LIBRARY_NAME
    return($$RET)
}

defineTest(vldField_qtAddLibrary) {

    LIB_PATH = $$1
    LIB_NAME = $$2

    mac:contains(VLDFIELD_QT_CONFIG, vldField_qtFramework) {

        LIBS      *= -F$${LIB_PATH}
    }
    else {

        unix:lessThan(QT_MAJOR_VERSION, 5) {

            # Many Linux distributions install validatefield_qt in the same directory
            # as the Qt libs and thus we need to prepend the path for the local build
            # to avoid conflicting with the installed version.
            # Qt5 qmake appends ( instead of prepending ) the path to the Qt libs
            # to LIBS, but for Qt4 we need to use the QMAKE_LIBDIR_FLAGS.

            QMAKE_LIBDIR_FLAGS *= -L$${LIB_PATH}
        }
        else {
            LIBS *= -L$${LIB_PATH}
        }
    }

    unset(LINKAGE)

    mac:contains(VLDFIELD_QT_CONFIG, vldField_qtFramework) {

        LINKAGE = -framework $${LIB_NAME}
    }

    isEmpty(LINKAGE) {

        if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {

            mac:LINKAGE = -l$${LIB_NAME}_debug
            win32:LINKAGE = -l$${LIB_NAME}d
        }
    }

    isEmpty(LINKAGE) {

        LINKAGE = -l$${LIB_NAME}
    }

    !isEmpty(QMAKE_LSB) {

        QMAKE_LFLAGS *= --lsb-shared-libs=$${LIB_NAME}
    }

    LIBS += $$LINKAGE
    export(LIBS)
    export(QMAKE_LFLAGS)
    export(QMAKE_LIBDIR_FLAGS)

    return(true)
}
