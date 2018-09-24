Number input validation library.

This library uses Qt version 5.10 classes. To build it, you will need Qt and a C++ toolchain in
either of these 64 bit platforms: Linux , Windows or Mac OS.

To build it, follow these steps:

1. cd <vldField_qt root dir>
2. qmake vldField_qt.pro

3. One of the following commands, depending on platform:

make                                 (In Linux/Mac OS)

nmake                                (Or, if in Windows and assuming you have MSVC 2015 or later)

4. One of the following commands, depending on platform:

make install                          (In Linux/Mac OS)


nmake install                      (Or, if in Windows and assuming you have MSVC 2015 or later)

Once this library is installed, just include its headers in your projects and have its lib file path visible to linker.


