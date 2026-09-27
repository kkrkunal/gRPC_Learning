cmake_minimum_required(VERSION 3.20)

project(gRPC_Test_01)

set(CMAKE_CXX_STANDARD 20)

# Prevent Windows headers from breaking Protobuf min/max templates
if (WIN32)
    add_compile_definitions(NOMINMAX)
endif()

# Find packages automatically
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Collect all utility files without listing them one-by-one
file(GLOB UTILS_SOURCES "Utilities/*.cpp")
file(GLOB PROTO_SOURCES "gRPC_stuff/*.cc")

# Define server executable
add_executable(server Server.cpp ${UTILS_SOURCES} ${PROTO_SOURCES})
target_link_libraries(server PRIVATE gRPC::grpc++ protobuf::libprotobuf)

# Define client executable
add_executable(client Client.cpp ${UTILS_SOURCES} ${PROTO_SOURCES})
target_link_libraries(client PRIVATE gRPC::grpc++ protobuf::libprotobuf)

# Link Windows Sockets library if on Windows
if (WIN32)
    target_link_libraries(server PRIVATE ws2_32)
    target_link_libraries(client PRIVATE ws2_32)
endif()