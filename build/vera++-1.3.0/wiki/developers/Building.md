# Compiling Vera++ #

## Getting the sources ##

### From the download section ###

Just download the vera++ sources from the [downloads section](https://bitbucket.org/verateam/vera/downloads/)
and extract it.

The source tarball is called `vera++-X.Y.Z.tar.gz` where `X.Y.Z` is the version number.

The documentation can be downloaded in the tarball `vera++-X.Y.Z-doc.tar.gz`. It is useful to build vera++ from the source without the need to install pandoc to build the documentation.

### With git ###

To get a copy of Vera++'s repository, run:

    git clone https://bitbucket.org/verateam/vera.git
    cd vera
    git submodule update --init

To update that repository, run:

    git pull
    git submodule update

Note that the submodule is only needed to build the documentation. If it is not there, a warning is displayed and the documentation is not generated.

## Dependencies ##

Vera++ is implemented in C++ and depends on the [Boost](http://www.boost.org/) and [Tcl](http://www.tcl.tk/) libraries. It uses [CMake](http://cmake.org/ CMake) as build system.
The documentation is generated with [pandoc](http://johnmacfarlane.net/pandoc/).

Note that pandoc is only needed to build the documentation. If it is not found, a warning is displayed and the documentation is not generated.

### Installing Boost on Windows ###

Download boost to some place. Unzip the archive.

 * Go to the directory tools\build\v2\.
 * Run bootstrap.bat
 * Run b2 install --prefix=PREFIX where PREFIX is the directory where you want Boost.Build to be installed
 * Add PREFIX\bin to your PATH environment variable.

More documentation can be found at the [Boost website](http://www.boost.org/doc/libs/1_56_0/more/getting_started/windows.html#install-boost-build)

### Installing Boost.Python on Windows ###

Once boost is installed on windows you can add the python support for vera++ by installing the Boost.Python module.

 * Go again to the directory tools\build\v2\.
 * Run bjam --with-python
 * Run b2 install --prefix=PREFIX where PREFIX is the directory where you want Boost.Build to be installed

## Configuring and building ##

To build vera++, as with any CMake project, run either, `cmake`, `ccmake` or `cmake-gui`, configure and generate the project, and build the project with your build environment. You can then install vera++ by building the `install` target, or generate a package by building the `package` target.

On unix compatible systems (Linux, *BSD, Mac OS X, ...), this is done this way:

    cd vera
    mkdir build
    cd build
    cmake ..
    make -j
    make install

See [Running CMake](http://www.cmake.org/cmake/help/runningcmake.html) for more information.

## Testing ##

Vera++ uses CTest, the testing tool that comes with CMake. To run the tests, simply type

    ctest

in your build directory.

The test results can be submitted to the [vera++'s dashboard](http://my.cdash.org/index.php?project=vera%2B%2B) with the command

    ctest -D Experimental

To submit nightly tests, it is easier to create a test directory, copy `vera.ctest` from [vera++'s sources](https://bitbucket.org/ThArGos/vera/src) to that directory and create a new file `vera.cmake` like this one:

    set(CTEST_SITE "host.site")
    set(CTEST_BUILD_NAME "compiler name")
    set(CTEST_BUILD_CONFIGURATION Debug)
    set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
    include(${CTEST_SCRIPT_DIRECTORY}/vera.ctest)

adjust the values, and run every nights:

    ctest -S /path/to/vera.cmake

The test results will appear on [vera++'s dashboard](http://my.cdash.org/index.php?project=vera%2B%2B). See [CMake Scripting Of CTest](http://www.cmake.org/Wiki/CMake_Scripting_Of_CTest) for more details.

## Packages ##

Vera++ uses `CPack` to generate the binary and source packages.

### Source package ###

Just build the `package_source` target to generate the source archive as a `tar.gz` file.
Make sure your repository is clean with `git status` before generating the package.

### Documentation package ###

Build the `package_doc` target to generate the documentation archive as a `tar.gz` file.

### Mac OS X package ###

`CPack` depends on [PackageMaker](https://developer.apple.com/downloads/index.action) to generate this package.
Set the CMake options:

* `CMAKE_BUILD_TYPE` to `Release`,
* `VERA_USE_SYSTEM_LUA` to `OFF`,
* `CMAKE_INSTALL_PREFIX` to `/usr`.

and build the target `package`.

### Microsoft Windows ###

`CPack` depends on [NSIS](http://nsis.sourceforge.net/Main_Page) to generate this package.
Set the CMake options:

* `VERA_USE_SYSTEM_BOOST` to `OFF`,
* `VERA_USE_SYSTEM_LUA` to `OFF`,
* `VERA_USE_SYSTEM_PYTHON` to `OFF`,

and build the target `package` in `Release` mode.

### Linux Debian ###

Set the CMake option `CMAKE_BUILD_TYPE` to `Release` and build the target `package`.

### Linux RPM ###

Set the CMake option `CMAKE_BUILD_TYPE` to `Release` and build the target `package`.

Note: this package generation has not yet been tested.