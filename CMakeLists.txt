cmake_minimum_required(VERSION 2.8.7)
project(shot)

include_directories($ENV{BOOST_ROOT})
link_directories($ENV{BOOST_ROOT}/libs/program_options/src)

include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/shot)

include_directories($ENV{GMOCK_HOME}/include $ENV{GMOCK_HOME}/gtest/include)
link_directories($ENV{GMOCK_HOME}/mybuild $ENV{GMOCK_HOME}/gtest/mybuild)

set(MONGO_DIR $ENV{HOME}/build/mongo-cxx-driver-legacy-0.0-26compat-2.6.2)
include_directories(${MONGO_DIR}/src)
include_directories(${MONGO_DIR}/build/linux2/use-system-boost/)
link_directories(${MONGO_DIR})
link_directories(${MONGO_DIR}/src)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-Wall -O2 -std=c++11")

set(sources
  shot/utils.cpp
  shot/translit.cpp
  shot/Config.cpp
  shot/url.cpp
  shot/Logger.cpp
  shot/Request.cpp
  shot/Response.cpp
  shot/Handler.cpp
  shot/Filer.cpp
  shot/HtmlTemplate.cpp
  shot/SitemapTemplate.cpp
  shot/system_handlers.cpp
  shot/AppHandler.cpp
  shot/Parser.cpp
  shot/Server.cpp
  shot/DbClient.cpp
)

set(test_sources
  shot/utils_test.cpp
  shot/translit_test.cpp
  shot/ConfigTest.cpp
  shot/RequestTest.cpp
  shot/ParserTest.cpp
  shot/HandlerTest.cpp
  shot/ServerTest.cpp
)

add_library(${PROJECT_NAME} SHARED ${sources})
#target_link_libraries(${PROJECT_NAME} mongoclient)
target_link_libraries(${PROJECT_NAME}
  boost_program_options
  boost_system
  boost_filesystem
  boost_thread
  pthread
  mongoclient
  crypto
)

add_executable(${PROJECT_NAME}_main shot/main.cpp)
target_link_libraries(${PROJECT_NAME}_main 
  boost_program_options
  boost_system
  pthread
  mongoclient
  crypto
  ${PROJECT_NAME}
)

add_executable(${PROJECT_NAME}_test ${test_sources} shot/test.cpp)
target_link_libraries(${PROJECT_NAME}_test gmock gtest pthread crypto ${PROJECT_NAME})

# copy static test folder
add_custom_target(${PROJECT_NAME}_static ALL)
add_custom_command(TARGET ${PROJECT_NAME}_static
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/static ${PROJECT_BINARY_DIR}/static
)
