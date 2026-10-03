
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
   set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")  
   set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")  
else()
   message(WARNING "No ccache in path. It is worth installing.")
endif()
