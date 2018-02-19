Running Vera++
=============

Vera++ needs to know where the rules and transformation scripts are located.
The following rules are applied:

* If the `--root` option is used, its argument is used as the name of the directory where the `scripts` subdirectory with scripts should be located, otherwise
* If the `VERA_ROOT` environment variable is defined, it is used as the name of the directory where the `scripts` subdirectory with scripts should be located, otherwise
* If the `HOME` environment variable is defined, then the `~/.vera++` directory is used (and it should contain the `scripts` subdirectory with scripts), otherwise
* The current directory should contain the `scripts` subdirectory.

Options
-------

Vera++ recognizes the following parameters:

`-`
:   (a single minus) indicates that the source code to check will be provided on the stdin.

`-p` `--profile` _profilename_
:   instructs the program to execute all rules defined in the given profile; the profile name is just a name of the file that will be found under the `profiles` directory. There is always a `default` profile that lists many of the existing rules - it is used when no profile is named explicitly.

`-R` `--rule` _rulename_
:   instructs the program to execute the given rule; note that the name of the rule should not contain the file extension of the script implementing the rule - this is added automatically, so that for example `--rule my_rule` means that Vera++ will find the `my_rule.tcl` script and will run it.

`--transform` _transformationname_
:   instructs the program to execute a single named transformation; the naming scheme is the same as for the `--rule` option.

`-o` `--std-report` _filename_
:   writes the standard (gcc-like) report to this file. A single dash `-` means that the standard output or the error output will be used, depending on the usage of the `--warning` or `--error` option. This option may be used several times in order to produce the reports in several locations - for example on the standard output and in a file. Default value is `-`.

`-v` `--vc-report` _filename_
:   writes the Visual C report to this file. A single dash `-` means that the standard output or the error output will be used, depending on the usage of the `--warning` or `--error` option. This option may be used several times in order to produce the reports in several locations - for example on the standard output and in a file. This report is not produced by default.

`-x` `--xml-report` _filename_
:   writes the XML report to this file. Not used by default. A single dash `-` means that the standard output or the error output will be used, depending on the usage of the `--warning` or `--error` option. This option may be used several times in order to produce the reports in several locations - for example on the standard output and in a file. This report is not produced by default.

`-c` `--checkstyle-report` _filename_
:   writes the checkstyle report to this file. Not used by default. A single dash `-` means that the standard output or the error output will be used, depending on the usage of the `--warning` or `--error` option. This option may be used several times in order to produce the reports in several locations - for example on the standard output and in a file. This report is not produced by default.

`-s` `--show-rule`
:   includes the name of the rule in each report line.

`-d` `--no-duplicate`
:   instructs the program to omit duplicated messages in the final report (the duplicates can be a result of violating the same rule many times in the same line of source code).

`-w` `--warning`
:   reports are marked as warning and generated on the error output.

`-e` `--error`
:   reports are marked as error and generated on the error output. An non zero exit code is used when one or more reports are generated.

`-q` `--quiet`
:   don't display the reports. This option is best used with `--summary` and/or with `--error`.

`-S` `--summary`
:   displays the number of reports and the number of processed files.

`--parameters` _filename_
:   instructs the program to read parameter values from the given file; each parameter association should be placed in a separate line of this file. This option may be used several times.

`-P` `--parameter` _parameterassociation_
:   provides the value of the named parameter to the scripts (see the documentation of each script to see whether it recognizes any parameters); the parameter association has the form `name=value`.

`--exclusions` _exclusionsfilename_
:   instructs the program to exclude some source files from rule checks, as described in the given file; the content of this file 
is a set of regular expressions that match the log output in the standard format, with the rule name. If the regular
expression matches, the report is excluded. For example, to exclude all the `T003` report for the `case` and  `delete`
keywords, just add this line to an exclusion file

    :\d+: T003: keyword '(case|delete)' not followed by a single space

The empty lines are ignored in the exclusion files as well as the lines beginning
with a `#`, so the file can be commented.

`-i` `--inputs` _filename_
:   the inputs are read from that file. A single dash `-` means that the files to check will be read from the standard input. This option may be used several times.

`-r` `--root` _path_
:   uses the given path as the vera++ root directory

`--version`
:   prints the program version information and exits.

`-h` `--help`
:   prints the list of recognized options and exits.

`--`
:   (a double dash) do not interpret any more arguments as options.

Arguments that are not starting with a dash `-` are treated as source files to check. Files starting with a dash can be checked by prefixing them with the current directory shortcut `./`.

When no input file is provided either as an argument or with the `--input` option, the list of source file names is read from the standard input.

Profile file
------------

The profile file contains a list of options and their value. The available options are the same as the options used on the command line in their long form and without the leading `--`. The options noted as being usable several times are also usable several times in the profile file. An example profile definition that groups three rules (L001, L002 and L003) and changes the maximum line length to 120 might look like:

    rule=L001
    rule=L002
    rule=L003
    parameter=max-line-length=120

Examples of executing Vera++ with rules
---------------------------------------

To execute all default verification rules against the file `file.cpp`, run:

    vera++ file.cpp

To execute only rule `L001` (this rule ensures that there is no trailing whitespace in each source line) against the same file, run:

    vera++ -R L001 file.cpp

To execute rule `L004` (this rule checks for too long source lines) with the parameter value providing 78 as the maximum line length, run:

    vera++ -R L004 -P max-line-length=78 file.cpp

To execute all rules from your favorite profile (assuming that the `my_favorite` profile definition is stored in the `profiles` directory) against all header files in the current filesystem subtree, run:

    find . -name '*.h' | vera++ --profile my_favorite

__Note:__ Vera++ collects the reports generated by each rule and prints them out sorted and after all rules were executed. If there were no problem reports, the output of the program is empty.

__Note:__ Vera++ reports are generated on the standard output by default, making them easy to use with a pipe. The `--warning` and `--error` options are changing the output to the standard error. The options `--std-report`, `--vc-report`, `--xml-report` and `--quiet` may be used to disable the output to the standard or error output.

Examples of executing Vera++ with transformations
-------------------------------------------------

To execute the `trim_right` source code transformation (it removes the trailing whitespace that the rule `L001` above complained about) on all `.cpp` files in the current directory run:

    vera++ --transform trim_right *.cpp

As a result, each `.cpp` file will be backed up with the additional extension `.bak` and the files will be trimmed by removing trailing whitespace. The exact behavior is defined by the script named `trim_right.tcl` in the `scripts/transformations` directory.

Running Vera++ as a test with CMake
-----------------------------------

CMake offers the possibility to run tests that are considered to pass when they return a 0 value and to fail otherwise. Fortunately, vera++, when used with the `--error` option, has exactly this behavior.
Creating the test is just a matter of listing the sources to check:

    file(GLOB_RECURSE srcs
      ${CMAKE_SOURCE_DIR}/src/*.cpp
      ${CMAKE_SOURCE_DIR}/src/*.h)
    add_test(NAME VeraStyle
      COMMAND vera++
      --error
      ${srcs})

Running Vera++ during the build with CMake
------------------------------------------

Running vera++ in a test integrates quite badly with the IDEs or with [CDash](http://cdash.org): the reports are hidden in the test log, and it is not easy to look at the problematic code. Moreover, a failure in the coding style is not the same as a failure in a unit or functional test, and shouldn't appear in the same way.
Another option is to run vera++ during the build and make it generate warnings that are well interpreted by the IDEs and CDash. In QtCreator for instance, it is then possible to click on the warning to go to the problematic code.

Running vera++ during the build can be done in a similar way to the previous section, by replacing the `add_test()` call with a `add_custom_target()` that will run the style check every time the custom target is built.

    file(GLOB_RECURSE srcs
      ${CMAKE_SOURCE_DIR}/src/*.cpp
      ${CMAKE_SOURCE_DIR}/src/*.h)
    add_custom_target(VeraStyle ALL
      vera++
      --warning
      ${srcs})

For large projects, running the style check every time can be quite time consuming and uncomfortable for the developer. It is then more convenient to split the style check in several parts that can be run in parallel, and to avoid rerunning the check if the files to check have not been modified. A vera++ macro is available to do that very easily:

    find_package(vera++)
    include(${VERA++_USE_FILE})
    add_vera_targets(*.h *.cpp
      RECURSE
      ROOT "${CMAKE_SOURCE_DIR}")

This macro adds a new `style_reports` target that is run every time a source file
is modified. A `style` target is still available to force the style check.
The target names can be configured with the parameters `NAME` and `NAME_ALL`.
This macro is the recommended way to use vera++ with CMake.

Backward compatibility with vera++ 1.2
--------------------------------------

* Tcl exclusion files are still supported, but deprecated
* Tcl profiles are still supported, but deprecated

Backward compatibility with vera++ 1.1
--------------------------------------

Vera++ is still mostly compatible with the vera++ 1.1 command line interface,
but this feature is planned for removal and its usage is not recommended.

Vera++ tries to detect if the old command line style is used by searching for
the old options in the arguments. If no old style option is found, vera++ uses
the new command line parser.

The command line style can be forced to the old style by setting the environment
variable `VERA_LEGACY` to `on`, `true` or `1`. Any other value will force vera++
to use the new command line style.

Note: the behavior of vera++ is not backward compatible with vera++ 1.1 when no
option is passed to vera++ and `VERA_LEGACY` is not set:

* the reports are generated on the standard output instead of the error output;
* a single dash `-` means that the source code to check is read from the
  standard input instead of reading the list of files to check;
* the lack of input files makes vera++ read the standard input instead of
  generating an error.
