include(${PROJECT_SOURCE_DIR}/cmake.macros)
subdirlist(children_dirs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(child ${children_dirs})
  message(${child})
  get_filename_component(module ${child} NAME)
  file(GLOB ${module}_source_files "${CMAKE_CURRENT_SOURCE_DIR}/${module}/*.cpp")

  if(NOT "${${module}_source_files}" STREQUAL "")
    include_directories(${PROJECT_SOURCE_DIR}/include)
    include_directories(${PROJECT_BINARY_DIR}/include)

    add_executable(${module} ${${module}_source_files})
    target_link_libraries(${module})
  else()
    message("No source files have been discovered for ${module}")
  endif()
endforeach()
