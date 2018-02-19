Changes
=======

Vera++ 1.3.0
----------

The development version of vera++ differs from 1.2.1 in the following ways:

- Python and Lua support
- exclusions are now based on regular expressions - the old tcl format is deprecated but still supported for backward compatibility
- profiles are using a new format that allows to put the same options than on the command line - the old tcl format is deprecated but still supported for backward compatibility
- rule and transform scripts are now directly in the vera root directory - the old location in the 'scripts' directory is deprecated but still supported for backward compatibility
- vera++ executable is now relocatable - the files are located based on the current executable location
- the 'no new line at end of file' is not hard coded any more
- invalid tokens are reported with the standard report mechanism and are no more blocking the execution
- IO error reports contain a description of the failure
- error reports are all prefixed with `vera++: ` so we can know which program produces the message when using vera++ in a pipe
- test suite enhanced code coverage 
- BUGFIX: T009 and T019 work properly with OpenMP for directive
- BUGFIX: tests are no more failing when run from the distribution archive
- BUGFIX: fix segmentation fault when passing an invalid number to the 'report' function in tcl

Vera++ 1.2.1
------------

Vera++ 1.2.1 differs from 1.2.0 in the following ways:

- BUGFIX: fix `--inputs` in order to be able to read the inputs from a file

Vera++ 1.2.0
------------

Vera++ 1.2.0 differs from 1.1.2 in the following ways:

- Full Tcl stack printed when a rule fail.
- New command line interface that support long and short options. The old style
  command line is still usable for backward compatibility.
- Produce output to standard output by default so the output can easily be piped
  to another program. The options `--warning` and `--error` make vera++ produce its
  output on the error output.
- CMake macros to easily run vera++ in any CMake project.
- Easier integration in a test chain by return an error code when at least one
  report is produced and the `--error` option is used. `--quiet` and `--summary`
  can also help to better integrate vera++ in the test chain.
- The standard output format match gcc's output format for a better integration
  in a build chain.
- Can read the list of files to check from one or more files.
- Can read the source code to check from the standard input.
- Can write the several reports in differents formats and in different places.
- Added `--root` option to point the the vera root directory from the command
  line and ease the usage of custom rules.
- Reports can be produced in [checkstyle](http://checkstyle.sourceforge.net/) XML format.
- Vera++ no more impose the extension of the source files to check.
- Several exclusion files can be used.
- Several parameter files can be used.
- Build system now uses CMake.
- Builds with TCL 8.6.
- Don't require Boost sources to build.
- New documentation generation process to unify the wiki, the html doc and the
  manpage.
- Binary packages for MS Windows and Mac OS X (and others).
- Nightly tests to avoid regressions.
- New website.
- BUGFIX: the rule T019 now works properly with `do ... while` blocks.

Vera++ 1.1.2
------------

Vera++ 1.1.2 differs from 1.1.1 in the following ways:

- Added `-xmlreport` option.

Vera++ 1.1.1
------------

Vera++ 1.1.1 differs from 1.1.0 in the following ways:

- Added `-help` option.
- Updated code for compatibility with newer versions of Boost.
  The reference version of the Boost library is now 1.35 or 1.36.
- BUGFIX: Corrected handling of current directory when neither `HOME`
  nor `VERA_ROOT` is specified (this affects Windows users only).

Vera++ 1.1.0
------------

Vera++ 1.1.0 differs from 1.0.0 in the following ways:

- Updated rules:
    - T002: additionally recognizes redefinition (`#define`) of keywords
    - T009: recognizes comment adjacent to colon as an exception to the rule
- Added rules:
    - F001: Source files should not use the `\r` (CR) character
    - F002: File names should be well-formed
            Note: F002 is not part of the default profile.
    - T012: Negation operator should not be used in its short form
    - T013: Source files should contain the copyright notice
    - T014: Source files should refer the Boost Software License
            Note: T014 is not part of the default profile.
    - T015: HTML links in comments and string literals should be correct
    - T016: Calls to `min`/`max` should be protected against accidental
            macro substitution
    - T017: Unnamed namespaces are not allowed in header files
    - T018: Using namespace is not allowed in header files
    - T019: Control structures should have complete curly-braced block of code
- Added predefined boost profile to emulate the original Boost inspect tool.
- Added transformations:
    - move_namespace: Changes the given identifier, useful for moving the whole
      project from one namespace to another.
    - move_macros: Changes the given prefix in all identifiers,
      useful for moving the whole set of macros that have common prefix.
    - move_includes: Changes the given part of `#include "..."` directives,
      useful for moving libraries and whole sets of header files.
- Added documentation for all available transformations.
- Makefiles modified to better support Windows make users.
- Extension `.ipp` added to the list of recognized source file extensions.
- New option `-showrules` includes name of rules in each report line.
- Changed the profile definition to be an active Tcl script
  instead of passive text file.
- Added the possibility to define exclusions to rule checks.
- BUGFIX: Corrected handling of newline tokens.