find_path(VALGRIND_INCLUDE_DIR valgrind/memcheck.h
  /usr/include /usr/local/include ${VALGRIND_PREFIX}/include)
find_program(VALGRIND_PROGRAM NAMES valgrind PATH 
  /usr/bin /usr/local/bin ${VALGRIND_PREFIX}/bin)

find_package_handle_standard_args(VALGRIND DEFAULT_MSG
    VALGRIND_INCLUDE_DIR
    VALGRIND_PROGRAM)

mark_as_advanced(VALGRIND_INCLUDE_DIR VALGRIND_PROGRAM)