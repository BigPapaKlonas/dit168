Releasing
=========

Update the Changes page
-----------------------

Make sure that [Changes](../Changes) is up to date. Update if necessary and update
the submodule in the repository.

Change the version number
-------------------------

The version number should be updated in the `maint` branch for a maintenance release
and from the `master` branch for a major release. Maintenance releases should only
change the build version, not the major and minor version.

Update `VERA_MAJOR`, `VERA_MINOR` and/or `VERA_BUILD` from the main `CMakeLists.txt`.
Rebuild vera++, run all the tests and commit this single change in the expected
branch.

If this is a major release, merge the `master` branch in the `maint` branch with

    git checkout maint
    git merge master

Push the changes in `maint` and `master` in the main repository.

It's better to wait for the nightly builds to make sure that all the tests are
still passing.

Tag the new version
-------------------

Either from the `maint` or `master` branch, run the command

    git tag vVERSION

where version is the new version number.

Generate the packages
---------------------

Checkout the expected version with

    git checkout vVERSION
    git submodule update

Make sure that the source tree is clean with

    git status

then create a build directory and configure the build with CMake.
If a binary package is also generated a on that system, also make sure to set
`CMAKE_BUILD_TYPE` to `Release`.

Generate the `packages_source` and `package_doc` targets.

Generate the [binary package on Mac OS X](Building#markdown-header-mac-os-x-package).

Generate the [binary package on Microsoft Windows](Building#markdown-header-microsoft-windows).

Upload the four packages (source, doc, win32 and Darwin) packages to https://bitbucket.org/verateam/vera/downloads

Update the Install page
-----------------------

Update the links and version number in the [Installation](../Installation) page.

Make an announcement
--------------------

todo
