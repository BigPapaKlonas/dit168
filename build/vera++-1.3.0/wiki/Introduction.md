Introduction
============

Vera++ is a programmable tool for verification, analysis and transformation of C++ source code.

The main usage scenarios that are foreseen for Vera++ are:

* Ensure that the source code complies with the given _coding standards and conventions_.
* Provide source code _metrics and statistics_.
* Perform automated _transformations_ of the source code, which can range from _pretty-printing_ to _diagnostics_ to _fault injection_ and advanced testing.

The main design idea of Vera++ is to create a generic engine that will be able to parse the C++ code and present it in the form of collections of various objects to user provided _scripts_ that will define the concrete actions to be executed.

Currently the following object collections are provided:

* Collection of source _file names_.
* Collection of _source lines_ for each file.
* Collection of identified _tokens_ in each file.

__Note:__ It is foreseen that future versions of Vera++ will provide also the semantic view on the code.

The most important feature of Vera++ is that all activities other than code parsing are defined by scripts.
This means that Vera++ is _flexible_ and _extensible_.

For example, compliance with coding standards can be expressed in terms of _rules_, each being defined by a separate script. The scripts can access all collections listed above and perform actions related to the given rule. The user can ask to run any given script or some defined set of scripts in a single program execution.

As a simple example, a coding convention that limits the length of the source line can be implemented as a script that traverses the collection of files and the collection of source lines and checks whether each source line fits within the given limits. A report can be generated for each non-conforming line of code so that the user gets a clear information about where the problem is located.

All existing rules present their reports in the format that is compatible with regular compiler's output, so that it is easy to integrate Vera++ with the existing build framework.

Similarly, automated transformation procedures are implemented as separate scripts that scan the above collections and produce another source files according to their algorithms. A simple example of such transformation might be a script that removes empty lines from source code.

The Tcl programming language is currently supported for scripts that run within Vera++.
