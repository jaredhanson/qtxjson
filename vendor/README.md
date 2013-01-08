This directory contains third-party dependencies used by QtxJson.

## yajl

#### Source Code

The source code of yajl has been added to this repository using `git subtree`:

    $ git subtree add -P vendor/src/yajl --squash git://github.com/lloyd/yajl.git 2.0.4

To subsequently update to a newer version of yajl, pull the code:

    $ git subtree pull -P vendor/src/yajl --squash git://github.com/lloyd/yajl.git <version>

#### Compiling

##### Mac OS X

Commands executed when building yajl 2.0.4:

    $ cd vendor/yajl
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

Refer to "vendor/yajl/BUILDING" for further information.

#### References

[GitHub](http://lloyd.github.com/yajl/)
