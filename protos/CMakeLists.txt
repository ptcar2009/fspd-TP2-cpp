project(TP2)

set(_PROTO_FILES central pares)
set(_PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(_PROTO_GENERATED_FILES)

find_package(gRPC)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated)
set(PROTO_OUT_DIR ${CMAKE_BINARY_DIR}/generated)
foreach(_proto_file ${_PROTO_FILES})
    set(${_proto_file}_proto_srcs ${PROTO_OUT_DIR}/${_proto_file}.pb.cc)
    set(${_proto_file}_proto_hdrs ${PROTO_OUT_DIR}/${_proto_file}.pb.h)
    set(${_proto_file}_grpc_srcs ${PROTO_OUT_DIR}/${_proto_file}.grpc.pb.cc)
    set(${_proto_file}_grpc_hdrs ${PROTO_OUT_DIR}/${_proto_file}.grpc.pb.h)

    set(${_proto_file}_generated ${${_proto_file}_proto_srcs} ${${_proto_file}_proto_hdrs} ${${_proto_file}_grpc_srcs} ${${_proto_file}_grpc_hdrs})

    add_custom_command(
        OUTPUT ${${_proto_file}_generated}
        COMMAND ${CMAKE_BINARY_DIR}/build_tools/protoc
            --grpc_out "${PROTO_OUT_DIR}"
            --cpp_out "${PROTO_OUT_DIR}"
            -I "${_PROTO_DIR}"
            --plugin=protoc-gen-grpc="${CMAKE_BINARY_DIR}/build_tools/grpc_cpp_plugin"
            ${_proto_file}.proto
        DEPENDS ${_PROTO_DIR}/${_proto_file}.proto
        )
    set(_PROTO_GENERATED_FILES ${_PROTO_GENERATED_FILES} ${${_proto_file}_generated})

    add_library(${_proto_file}_grpc_proto
      ${${_proto_file}_generated}
    )
    target_link_libraries(${_proto_file}_grpc_proto
        PRIVATE gRPC::grpc++
        )

endforeach()

add_custom_target(proto
    ALL
    DEPENDS ${_PROTO_GENERATED_FILES}
    )

# Include generated *.pb.h files
include_directories(${_PROTO_DIR})
