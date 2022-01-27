# uncomment to enable the gui examples

# sdl
# add_subdirectory(sdl)

# qt
# add_subdirectory(qt)

# fltk test
# add_subdirectory(fltk)

# gtkmm test
# add_subdirectory(gtkmm)

# imgui example
# add_subdirectory(imgui)

# Nana
# add_subdirectory(nana)

# Generic test that uses conan libs

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TARGETS central pares)

find_package(gRPC)
find_package(Protobuf)
include_directories(
        PRIVATE
        ${CMAKE_BINARY_DIR}/generated
        ${CMAKE_SOURCE_DIR}/include
    )

foreach(_target_file ${TARGETS})

    add_library(${_target_file}_client ${_target_file}_client.cc)

    target_link_libraries(
        ${_target_file}_client
        PUBLIC
        gRPC::grpc++
        gRPC::grpc++_reflection
        ${_target_file}_grpc_proto
      )
    foreach(_target
            cli_${_target_file} serv_${_target_file})
        add_executable(${_target} ${_target}.cc)
        target_link_libraries(${_target}
            PUBLIC
            gRPC::grpc++
            gRPC::grpc++_reflection
            ${_target_file}_grpc_proto
            central_client
            pares_client
            )
        add_custom_target(
            run_${_target}
            COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_target} $(arg)
            DEPENDS ${_target}
        )

    endforeach()
endforeach()

