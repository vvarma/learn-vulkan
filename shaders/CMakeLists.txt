FILE(GLOB VERT_SRCS *.vert)
FILE(GLOB FRAG_SRCS *.frag)
generate_spv(TARGET simple_shader SRCS ${VERT_SRCS} ${FRAG_SRCS} SPV_SRCS SPV_FILES)

add_custom_target(simple_shader ALL DEPENDS ${SPV_FILES})
