if(DEFINED CMAKE_LINKER_TYPE) 
   return()
endif()

include(CheckLinkerFlag)

check_linker_flag(CXX "-fuse-ld=mold" HAS_MOLD)
check_linker_flag(CXX "-fuse-ld=lld" HAS_LLD)

if(HAS_MOLD AND NOT WIN32)
   set(CMAKE_LINKER_TYPE MOLD)
elseif(HAS_LLD)
   set(CMAKE_LINKER_TYPE LLD)
endif()
