Transformations
===============

move\_includes Change prefix of \#include paths
-----------------------------------------------

This transformation allows one to modify the prefix of file paths in
\#include directives.

The motivation for this transformation is to help move whole libraries
from one file tree to another.

Please use this transformation as a boilerplate for your own customized
version.

For example, the following file:

~~~~
#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"
~~~~

will be transformed into:

~~~~
#include "boom/shared_ptr.hpp"
#include "boom/bind.hpp"
~~~~

Note: The transformation is performed in place, which means that the
source files are modified.

move\_macros Change prefix in macros
------------------------------------

This transformation allows one to modify the prefix of macros.

The motivation for this transformation is to help move whole libraries
or source sets from one naming conventioin to another.

Please use this transformation as a boilerplate for your own customized
version.

For example, the following file:

~~~~
#define BOOST_SOME_MACRO 1
// ...
#ifdef BOOST_SOME_MACRO
// ...
#endif
~~~~

will be transformed into:

~~~~
#define BOOM_SOME_MACRO 1
// ...
#ifdef BOOM_SOME_MACRO
// ...
#endif
~~~~

Note: This transformation actually does not check whether the given
identifier is indeed a macro name and the prefix replacement is
performed systematically on all identifiers that match.

Note: The transformation is performed in place, which means that the
source files are modified.


move\_namespace Change namespace name
-------------------------------------

This transformation allows one to consistently change the namespace
name.

The motivation for this transformation is to help move whole libraries
or source sets from one namespace to another, for example to allow the
coexistence of two different version of the same library.

Please use this transformation as a boilerplate for your own customized
version.

For example, the following file:

~~~~
namespace boost
{
void foo();
}

void boost::foo() {/* ... */}
~~~~

will be transformed into:

~~~~
namespace boom
{
void foo();
}

void boom::foo() {/* ... */}
~~~~

Note: This transformation actually does not check whether the given
identifier is indeed a namespace name and the replacement is performed
systematically on all identifiers that match. Do not use it on code that
overloads namespace names for other purposes.

Note: The transformation is performed in place, which means that the
source files are modified.


to\_lower Change identifier naming convention from CamelCase to standard\_lowercase
-----------------------------------------------------------------------------------

This transformation allows one to modify the naming convention of all
identifiers from CamelCase to standard\_lowercase, as used by the
standard library or Boost.

For example, the following code:

~~~~
namespace MyTools
{

class MyClass
{
public:
    void myFunction();
};

}
~~~~

will be transformed into this:

~~~~
namespace my_tools
{

class my_class
{
public:
    void my_function();
};

}
~~~~

Note: The transformation is performed in place, which means that the
source files are modified.

Note: This transformation does not modify comments and string literals.

to\_xml Transform C++ code into XML
-----------------------------------

This transformation generates a XML tree where nodes relate to C++
source code tokens.

For example, the following file (file.cpp):

~~~~
#include <iostream>

int main()
{
    std::cout << "Hello World\n";
}
~~~~

will be transformed into new file named file.cpp.xml:

~~~~
<?xml version="1.0" encoding="ISO-8859-1"?>
<cpp-source file-name="test.cpp">
    <token name="pp_hheader" line="1" column="0">#include &lt;iostream&gt;</token>
    <token name="newline" line="1" column="19">![CDATA[
]]</token>
    <token name="newline" line="2" column="0">![CDATA[
]]</token>
    <token name="int" line="3" column="0">int</token>
    <token name="space" line="3" column="3"> </token>
    <token name="identifier" line="3" column="4">main</token>
    <token name="leftparen" line="3" column="8">(</token>
    <token name="rightparen" line="3" column="9">)</token>
    <token name="newline" line="3" column="10">![CDATA[
]]</token>
    <token name="leftbrace" line="4" column="0">{</token>
    <token name="newline" line="4" column="1">![CDATA[
]]</token>
    <token name="space" line="5" column="0">    </token>
    <token name="identifier" line="5" column="4">std</token>
    <token name="colon_colon" line="5" column="7">::</token>
    <token name="identifier" line="5" column="9">cout</token>
    <token name="space" line="5" column="13"> </token>
    <token name="shiftleft" line="5" column="14">&lt;&lt;</token>
    <token name="space" line="5" column="16"> </token>
    <token name="stringlit" line="5" column="17">"Hello World\n"</token>
    <token name="semicolon" line="5" column="32">;</token>
    <token name="newline" line="5" column="33">![CDATA[
]]</token>
    <token name="rightbrace" line="6" column="0">}</token>
    <token name="newline" line="6" column="1">![CDATA[
]]</token>
    <token name="eof" line="7" column="0"></token>
</cpp-source>
~~~~

Note: If the source code does not use line splicing, then concatenation
of all XML node values is equivalent to the original C++ code.


to\_xml2 Transform C++ code into XML (another variant)
------------------------------------------------------

This transformation generates a XML tree where nodes relate to C++
source code tokens.

The difference between this version and the one named to\_xml is that
here nodes have names related to token types, which can make it easier
for some further XML transformations.

For example, the following file (file.cpp):

~~~~
#include <iostream>

int main()
{
    std::cout << "Hello World\n";
}
~~~~

will be transformed into new file named file.cpp.xml:

~~~~
<?xml version="1.0" encoding="ISO-8859-1"?>
<cpp-source file-name="test.cpp">
    <pp_hheader line="1" column="0">#include &lt;iostream&gt;</pp_hheader>
    <newline line="1" column="19">![CDATA[
]]</newline>
    <newline line="2" column="0">![CDATA[
]]</newline>
    <int line="3" column="0">int</int>
    <space line="3" column="3"> </space>
    <identifier line="3" column="4">main</identifier>
    <leftparen line="3" column="8">(</leftparen>
    <rightparen line="3" column="9">)</rightparen>
    <newline line="3" column="10">![CDATA[
]]</newline>
    <leftbrace line="4" column="0">{</leftbrace>
    <newline line="4" column="1">![CDATA[
]]</newline>
    <space line="5" column="0">    </space>
    <identifier line="5" column="4">std</identifier>
    <colon_colon line="5" column="7">::</colon_colon>
    <identifier line="5" column="9">cout</identifier>
    <space line="5" column="13"> </space>
    <shiftleft line="5" column="14">&lt;&lt;</shiftleft>
    <space line="5" column="16"> </space>
    <stringlit line="5" column="17">"Hello World\n"</stringlit>
    <semicolon line="5" column="32">;</semicolon>
    <newline line="5" column="33">![CDATA[
]]</newline>
    <rightbrace line="6" column="0">}</rightbrace>
    <newline line="6" column="1">![CDATA[
]]</newline>
    <eof line="7" column="0"></eof>
</cpp-source>
~~~~

Note: If the source code does not use line splicing, then concatenation
of all XML node values is equivalent to the original C++ code.


trim\_right Remove trailing white space
---------------------------------------

This transformation removes the trailing whitespace from each line of
code.

It can be treated as a quick remedy for problems reported by rule L001.

Note: The transformation is performed in place, which means that the
source files are modified.
