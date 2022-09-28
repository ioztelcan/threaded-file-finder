include(${PROJECT_SOURCE_DIR}/cmake.macros)
subdirlist(children_dirs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(child ${children_dirs})
    get_filename_component(module ${child} NAME)
    file(GLOB ${module}_source_files "${CMAKE_SOURCE_DIR}/src/${module}/${module}_*.cpp")

    if (NOT "${${module}_source_files}" STREQUAL "")
      include_directories("${PROJECT_SOURCE_DIR}/include")
      include_directories("${PROJECT_BINARY_DIR}/include")

      # build source files under test as a shared library.
      add_library(lib_test_${module} SHARED ${${module}_source_files})

      if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        target_link_libraries(lib_test_${module})
      else()
        target_link_libraries(lib_test_${module})
      endif()

      file(GLOB test_files_${module} "${child}/*.cpp")
      add_executable(test_${module} ${test_files_${module}})

      add_dependencies(build_and_test test_${module})
      target_link_libraries(
        test_${module}
        lib_test_${module}
        gtest_main)
        gtest_discover_tests(test_${module} DISCOVERY_MODE PRE_TEST)
    else()
        message("No test source files discovered for ${module}")
    endif()
endforeach()