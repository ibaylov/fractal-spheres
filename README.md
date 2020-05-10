# The Application
The application visualizes the SphereFlake fractal using OpenGL

Proudly powered by GNU C/C++, GNU Autotools, VIM, Microsoft Code, QT Creator, Mercurial, Valgrind and Doxygen
... and Git

# What's the buzz

I have created this application on a prrof of qualification request.  Thus it was made under some restrictions:

* it was done in a week
* it has to be portable
* it has to use bare C++, no STL, no Boost
* it has to work /near/ real-time on a decent workstation


I'm providing the code to you in hope you'll find some use of it's compnnents or can use it as a case study of OpenGL,Computer Graphics,  MVC and general C++ programming.

# Inslallation

## Prerequisites

You should have installed autotools, g++, OpenGL libGL and libGLU,  and GLUT (freeglut).

## Configuring
 
Run configure with the default arguments for your system, for example on RHEL/Fedora that should look like:

    ./configure --prefix=/usr --libdir=/usr/lib64 --sysconfdir=/etc

On Debian derivates tis may look as this:

    ./configure --prefix=/usr  --sysconfdir=/etc

If you don't want to mess up your system and don't have anything special about the required libs and/or build platform, just run

    ./configure

This will happily place everything in /usr/local if you do  __make install__ afterwards

Look at the configure output, it should contain lines

    checking for sqrt... yes
    checking for glPushMatrix... yes
    checking for gluSphere... yes
    checking for glutInit... yes

If sometring's not here, you don't have all the required libraries installed

## Building and installiation

After the successful configuration just type

    make

And, if you want the aplication installed, 

    make install

# Configure options

## Release

The application has to be built with a  optimization level to perform smoothly. For GCC it will be -O3

    CXFLAGS="-O3" ./configure --prefix=/usr --libdir=/usr/lib64 --sysconfdir=/etc

The CXXFLAGS are not set by configure deliberately, so you can play with defines and profiling options

## For debugging

    CXFLAGS="-O0 -ggdb -D_DEBUG_DUMP_" ./configure --prefix=/usr --libdir=/usr/lib64 --sysconfdir=/etc

## For profiling

    CXFLAGS="-O0 -ggdb -pg" ./configure --prefix=/usr --libdir=/usr/lib64 --sysconfdir=/etc

__Don't forget to `make clean` after each reconfiguration__

You can also passs any other preprocessor definition by adding it with -D to CXXFLAGS

## Build control macros

    _GEOM_FLOAT_ 		- define to use float scalar
    _NO_CXX_11_  		- define to exclude the c++ 11 + specific code
    _DEBUG_DUMP_ 		- define to enable debug features and dumping
    _FV_CACHE_SIZE_		- define the size of element's cache Set to 0 to disable the cache

### If sometring goes wrong with configure

Recreate the configure script by typing in

    aclocal && autoconf && automake -a

# The documentation

The documentation is extracted from source code comments by Doxygen. To recreate it install doxygen. 
Once having doxygen installed run

    cd doc && doxygen

# Running

After the application has been built type in

	src/fractal-spheres
or, if you have already installed it with __make install__, simply run

	fractal-spheres

The user interface is keyboard-based with no special keys used. The key commnds are:

    a - Camera orbit left
    d - Camera orbit right
    w - Camera orbit up
    s - Camera orbit down
    A - Capera yaw left
    D - Camera yaw right
    W - Camera pitch up
    S - Camera pitch down
    [ - Camera mowe forward
    ] - Camera move backward
    { - Camera zoom in
    } - Camera zoom out
    Space - reset the wiewport
    1 - Select gold model paint 
    2 - Select Pierot multi-color model paint
    3 - Select solver glass model paint
    ESC - Exits the application

__For convenience the Zoom FOV is restricted betwenn 9 and 90 degrees. This can be removed in viewport.cc__ 


Happy hacking!
