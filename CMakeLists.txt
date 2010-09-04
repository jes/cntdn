cmake_minimum_required(VERSION 2.6)
project(cntdn)

add_executable(letters letters.c letters_main.c)
add_executable(numbers numbers.c numbers_main.c)
add_executable(cntdn cntdn.c dictcorner.c games.c letters.c numbers.c opts.c players.c strings.c)

install(TARGETS letters numbers cntdn
        RUNTIME DESTINATION bin)
