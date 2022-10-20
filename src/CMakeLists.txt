cmake_minimum_required(VERSION 2.8.9)

# Maybe stop from CMAKEing in the wrong place
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same. Go use the /build directory.")
  endif()

# Matlab
# https://www.mathworks.com/matlabcentral/answers/472865-calling-matlab-functions-from-c-application-built-with-cmake-in-visual-studio
macro(link_matlab PROJECT_NAME)
# TODO: check system rather than compiler
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message(STATUS "Linking apple Matlab")
    target_link_libraries(${PROJECT_NAME} ${Matlab_ROOT_DIR}/extern/bin/maci64/libMatlabEngine.dylib)
    target_link_libraries(${PROJECT_NAME} ${Matlab_ROOT_DIR}/extern/bin/maci64/libMatlabDataArray.dylib)
    find_package(Threads)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using Clang (linux or apple) or GCC
  message(STATUS "Linking linux Matlab")
  target_link_libraries(${PROJECT_NAME} ${Matlab_ROOT_DIR}/extern/bin/glnxa64/libMatlabEngine.so)
  target_link_libraries(${PROJECT_NAME} ${Matlab_ROOT_DIR}/extern/bin/glnxa64/libMatlabDataArray.so)
  find_package(Threads)
  target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
  message(STATUS "Linking Windows Matlab")
  target_link_libraries(${PROJECT_NAME} ${Matlab_ROOT_DIR}/extern/lib/win64/microsoft/libMatlabEngine.lib)
  target_link_libraries(${PROJECT_NAME} ${Matlab_ROOT_DIR}/extern/lib/win64/microsoft/libMatlabDataArray.lib)
else()
  # unrecognized
  message( FATAL_ERROR "Unrecognized compiler [${CMAKE_CXX_COMPILER_ID}]" )
endif()
endmacro()

add_executable(
  run
  main.cpp
  utils.cpp
  MatlabHelpers.cpp
)
target_include_directories(run PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
# borrow args.hxx directly from polyscope
target_include_directories(run PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../deps/polyscope/deps/args/")
target_link_libraries(run polyscope geometry-central)
link_matlab(run)




option(PACKAGE_TESTS "Build the tests" ON)

if(PACKAGE_TESTS)
  enable_testing()
  add_subdirectory(
    tests
  )
endif()
