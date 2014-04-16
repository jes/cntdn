cmake_minimum_required(VERSION 2.6)
project(cntdn)

add_executable(letters letters.c letters_main.c)
add_executable(lettersd letters.c lettersd.c)
add_executable(numbers numbers.c numbers_main.c)

target_link_libraries(lettersd pthread)

set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -O3")

set(DEFAULT_DICT ${CMAKE_INSTALL_PREFIX}/share/dict/cntdn-dictionary)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

install(TARGETS letters lettersd numbers
        RUNTIME DESTINATION bin)
install(PROGRAMS gletters.py RENAME gletters
        DESTINATION bin)
install(PROGRAMS tkletters.pl RENAME tkletters
        DESTINATION bin)
install(FILES dictionary RENAME cntdn-dictionary
        DESTINATION share/dict)
