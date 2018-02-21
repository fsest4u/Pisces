3rdparty
========

Bundled copies of 3rd party libraries.

To prevent using bundled copies in favor of system libraries use the build flag:

USE_SYSTEM_LIBS

To require the use of System libraries and not allow falling back to bundled
copies if a system library is not found use the build flag:

SYSTEM_LIBS_REQUIRED

Both of the above libraries should be constructed like so:

-D...=1

Each copy is a pristine (unchanged) copy without any modification. Any files
that need to be or are modified will be in the extra directory.


Versions
========

httpserver     - 1.0.0

extra
=====

The extra directory is for files that are custom files and not part of the
3rdparty source directly. Such as preconfigured .config files.
