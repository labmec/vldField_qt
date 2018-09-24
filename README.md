Number input validation library.

This library uses Qt version 5.10 classes. To build it, you will need Qt and a C++ toolchain in
either of these 64 bit platforms: Linux , Windows or Mac OS.

To build it, follow these steps:

1. cd <vldField_qt root dir>
2. qmake vldField_qt.pro

In Linux/Mac OS:
3. make 

Or, if in Windows (assuming you have MSVC 2015 or later):
3. nmake

In Linux/Mac OS (this might need 'sudo' if installation folder belongs to root user):
4. make install 

Or, if in Windows (assuming you have MSVC 2015 or later):
4. nmake install

Once this library is installed, just include its headers in your projects and have its lib file path visible to linker.


