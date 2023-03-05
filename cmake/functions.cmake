function(generate_spv)
    cmake_parse_arguments(gen_spv "TARGET" "" "SRCS;SPV_SRCS" ${ARGN})
  find_program(_GLSLC glslc REQUIRED)
  foreach(FIL ${gen_spv_SRCS})
    get_filename_component(spv_name ${FIL} NAME)
    get_filename_component(spv_path ${FIL} ABSOLUTE)

    set(spv_gen "${CMAKE_CURRENT_BINARY_DIR}/${spv_name}.spv")
    list(APPEND ${gen_spv_SPV_SRCS} ${spv_gen})

    add_custom_command(
      OUTPUT "${spv_gen}"
      COMMAND ${_GLSLC} ARGS ${spv_path} -o ${spv_gen}
      COMMENT "Running glslc generate"
      VERBATIM
      DEPENDS ${spv_path})

  #target_sources("${gen_spv_TARGET}" PRIVATE ${spv_gen})
  endforeach()
  #set(${gen_spv_SPV_SRCS} ${test_list} PARENT_SCOPE)
  set_source_files_properties(${${gen_spv_SPV_SRCS}} PROPERTIES GENERATED TRUE)
  set(${gen_spv_SPV_SRCS}
      ${${gen_spv_SPV_SRCS}}
      PARENT_SCOPE)
endfunction()
