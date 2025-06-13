cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH /usr/local/cmake) # ??

project(full_duplex_project)
enable_testing()

include(quick_project/Test OPTIONAL RESULT_VARIABLE quick_project)
if (quick_project)
  quick_project_add_check_target()
endif(quick_project)

add_definitions(-std=c++23)
add_definitions(-DBOOST_HANA_CONFIG_ENABLE_STRING_UDL)
add_definitions(-ftemplate-backtrace-limit=0)
add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-DASIO_STANDALONE -DASIO_NO_TYPEID -DASIO_HAS_STD_STRING_VIEW)

include_directories(${full_duplex_project_SOURCE_DIR}/include)

install(FILES include/full_duplex.hpp DESTINATION include)
install(DIRECTORY include/full_duplex DESTINATION include)

if (quick_project)
  add_subdirectory(test)
endif(quick_project)
