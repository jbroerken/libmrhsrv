# libmrhsrv

<p align="center">
<img width="100%" height="100%" src="/doc/source/banner.png">
</p>


## About

The libmrhsrv library is used to connect to and interact with the MRH connection and communication servers. 
It provides a way to manage the connection and handles the opcodes used during communication.


## Requirements

#### Compilation

This project is built using CMake. You can find CMake here:

https://cmake.org/

#### Library Dependencies

This project requires other libraries and headers to function:

Dependency | Source
---------- | ------
mrhshared | https://github.com/jbroerken/mrhshared/
libsodium | https://github.com/jedisct1/libsodium/
lsquic | https://github.com/litespeedtech/lsquic

For more information about the requirements, check the "Building" section found in the documentation.


## Documentation

All documentation is build with sphinx-doc using the Read The Docs theme.
To build the documentation, grab the requirements for it:

#### sphinx-doc
https://www.sphinx-doc.org/en/master/

#### Read The Docs Theme
https://sphinx-rtd-theme.readthedocs.io/en/stable/

## Licence

This project is licenced under the ZLib licence. 
Please read the included LICENCE.txt for the exact terms.


## Directories

This project supplies multiple directories for the development of said project. 
Their names and descriptions are as follows:

Directory | Description
--------- | -----------
build | CMake build directory.
doc | Documentation files.
include | Library header files used by the library as well as the project headers.
lib | Library files used by the project as well as the created build.
src | Project source code.
