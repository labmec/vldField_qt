
include( vldField_qtconfig.pri )

TEMPLATE = subdirs
CONFIG   += ordered

SUBDIRS = \
    src 

 
vldField_qtspec.files  = vldField_qtconfig.pri vldField_qtfunctions.pri vldField_qt.prf
vldField_qtspec.path  = $${VLDFIELD_QT_INSTALL_FEATURES}

#INSTALLS += vldField_qtspec
