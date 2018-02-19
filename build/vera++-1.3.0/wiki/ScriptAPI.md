Script API
==========

The scripts (rules and transformations) are written in Tcl, Lua or Python, and are
executed by the embedded interpreter that has access to relevant state
of the program. A set of commands is provided to enable easy read-only
operation on the information that was gathered by parsing given source
files.

In Tcl and Lua, the commands are usable without any prefix. In Python, the commands
are available in the `vera` module. The vera module is already imported and does
not need to be imported again in the rule code.

The following commands are provided:

-   **`getSourceFileNames`** - returns the list of file names that were
    provided to Vera++ as program parameters.
-   **`getLineCount fileName`** - returns the number of lines in the
    given source file.
-   **`getAllLines fileName`** - returns the list of lines, in their
    natural order, that form a give source file.
-   **`getLine fileName lineNumber`** - returns the selected line; line
    numbers are counted from 1.
-   **`getTokens fileName fromLine fromColumn toLine toColumn filter`** -
    returns the list of tokens, in their natural order, from the given source file and that match the given selection criteria.
    
    The meaning of arguments for selecting tokens is:

    -   **`fromLine`** - the lowest line number (counted from 1),
        inclusive
    -   **`fromColumn`** - the lowest column number (counted from 0),
        inclusive
    -   **`toLine`** - the highest line number, inclusive; -1 means that
        the selected range spans to the end of the file
    -   **`toColumn`** - the highest column number, exclusive; -1 means
        that the selected range spans to the end of the line defined by
        `toLine`.
    -   **`filter`** - the list of selected token types, the recognized
        token types are listed below; if this list is empty, then all
        token types are allowed.

    With Tcl, the `getTokens` command returns a list of lists - the nested lists
    have the following elements:

    -   *value* - the literal text of the token
    -   *lineNumber* - the line number (from 1) where the token appears
    -   *columnNumber* - the column number (from 0) where the token
        appears
    -   *name* - the name or type of the token; see below for the list
        of recognized token types

    With Python and Lua, the `getTokens` command returns a list of token objects
    that have the following attributes:

    -   *value* - the literal text of the token
    -   *line* - the line number (from 1) where the token appears
    -   *column* - the column number (from 0) where the token
        appears
    -   *name* - the name or type of the token; see below for the list
        of recognized token types
    -   *type* - same as `name`

-   **`getParameter name defaultValue`** - returns the value of the
    given parameter or the provided default value if no such parameter
    is defined.
-   **`report fileName lineNumber message`** - registers a report for
    the given file and line; this report is printed at the end of the
    program execution, sorted by file and line number. Use this command
    to generate output that is compatible with the warning/error output
    format of popular compilers.

Examples:
---------

To process all lines from all source files, use the following code
pattern:

Tcl:

    foreach fileName [getSourceFileNames] {
        foreach line [getAllLines $fileName] {
            # ...
        }
    }

Python:

    for f in vera.getSourceFileNames():
      for line in vera.getAllLines(f):
        # ...

Lua:

    for file in getSourceFileNames() do
      for line in getAllLines(file) do
        -- ...
      end
    end

To process all tokens from all source files, use:

Tcl:

    foreach fileName [getSourceFileNames] {
        foreach token [getTokens $fileName 1 0 -1 -1 {}] {
            set tokenValue [lindex $token 0]
            set lineNumber [lindex $token 1]
            set columnNumber [lindex $token 2]
            set tokenType [lindex $token 3]
            # ...
        }
    }

Python:

    for f in vera.getSourceFileNames():
      for t in for t in vera.getTokens(fileName, 1, 0, -1, -1, []):
        # print t.line, t.type, ...

Lua:

    for fileName in getSourceFileNames() do
      for t in getTokens(fileName, 1, 0, -1, -1, filter) do
        -- t.line, t.type, ...
      end
    end

To process only curly braces from the given source file, use:

Tcl:

    foreach token [getTokens $fileName 1 0 -1 -1 {leftbrace rightbrace}] {
        # ...
    }

Python:

    for t in for t in vera.getTokens(fileName, 1, 0, -1, -1, ["leftbrace", "rightbrace"]):
      # ...

Lua:

    for t in getTokens(fileName, 1, 0, -1, -1, {"leftbrace", "rightbrace"}) do
      -- ...
    end

The complete rule script for verifying that the lines are no longer than
some limit (the limit can be provided as a parameter, but the default
value is defined in by the script itself):

    # Line cannot be too long

    set maxLength [getParameter "max-line-length" 100]

    foreach f [getSourceFileNames] {
        set lineNumber 1
        foreach line [getAllLines $f] {
            if {[string length $line] > $maxLength} {
                report $f $lineNumber "line is longer than ${maxLength} characters"
            }
            incr lineNumber
        }
    }

The above script is actually the implementation of rule L004.

Notes about line splicing
-------------------------

As required by the C++ ISO standard, the line splicing (with the
backslash at the end of the line) is performed before tokenizing. This
means that the lists of tokens might not strictly fit the list of lines.

Due to the internal mechanisms of the parser, the line splicing freezes
the line counter and forces the column counter to continue until the
last line in the spliced block. This means that there might be physical
non-empty lines that apparently don't have any tokens, as well as tokens
that have column numbers not matching the physical source line lengths.

Recognized token types
----------------------

The following token types are recognized by the parser and can be used
for filter selection in the `getTokens` command (some of these token
types are related to compiler extensions):

    and
    andand
    andassign
    any
    arrow
    arrowstar
    asm
    assign
    auto
    bool
    break
    case
    catch
    ccomment
    char
    charlit
    class
    colon
    colon_colon
    comma
    compl
    const
    constcast
    continue
    contline
    cppcomment
    decimalint
    default
    delete
    divide
    divideassign
    do
    dot
    dotstar
    double
    dynamiccast
    ellipsis
    else
    enum
    eof
    eoi
    equal
    explicit
    export
    extern
    false
    float
    floatlit
    for
    friend
    goto
    greater
    greaterequal
    hexaint
    identifier
    if
    inline
    int
    intlit
    leftbrace
    leftbracket
    leftparen
    less
    lessequal
    long
    longintlit
    minus
    minusassign
    minusminus
    msext_asm
    msext_based
    msext_cdecl
    msext_declspec
    msext_endregion
    msext_except
    msext_fastcall
    msext_finally
    msext_inline
    msext_int16
    msext_int32
    msext_int64
    msext_int8
    msext_leave
    msext_region
    msext_stdcall
    msext_try
    mutable
    namespace
    new
    newline
    not
    notequal
    octalint
    operator
    or
    orassign
    oror
    percent
    percentassign
    plus
    plusassign
    plusplus
    pound
    pound_pound
    pp_define
    pp_elif
    pp_else
    pp_endif
    pp_error
    pp_hheader
    pp_if
    pp_ifdef
    pp_ifndef
    pp_include
    pp_line
    pp_number
    pp_pragma
    pp_qheader
    pp_undef
    pp_warning
    private
    protected
    public
    question_mark
    register
    reinterpretcast
    return
    rightbrace
    rightbracket
    rightparen
    semicolon
    shiftleft
    shiftleftassign
    shiftright
    shiftrightassign
    short
    signed
    sizeof
    space
    space2
    star
    starassign
    static
    staticcast
    stringlit
    struct
    switch
    template
    this
    throw
    true
    try
    typedef
    typeid
    typename
    union
    unsigned
    using
    virtual
    void
    volatile
    wchart
    while
    xor
    xorassign

Note
----

There is a predefined rule named `DUMP` that prints on the screen all
tokens with their types and position. This rule can be helpful as a
guideline for creating custom filtering criteria:

    vera++ --rule DUMP myfile.cpp
