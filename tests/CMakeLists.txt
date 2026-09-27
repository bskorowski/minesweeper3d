cmake_minimum_required(VERSION 3.19)

project(minesweeper3d-tests)


function(Register_test name)
      add_executable(${name} ${name}.cpp)
      target_link_libraries(${name} PRIVATE minesweeper3d-lib tasty::tasty)
      target_compile_features(${name} PRIVATE cxx_std_23)
      add_test(NAME ${name} COMMAND ${name})
endfunction()

Register_test(input_test)
Register_test(format_test)
Register_test(trig_test)

message("Tests registered")
