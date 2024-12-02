cmake_minimum_required(VERSION 3.12)
project(ChatServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_TOOLCHAIN_FILE /root/software/vcpkg-2024.09.30/scripts/buildsystems/vcpkg.cmake)
set(gRPC_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/grpc)
set(Protobuf_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/protobuf)
set(protobuf_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/protobuf)
set(upb_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/upb)
set(utf8_range_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/utf8_range)
set(absl_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/absl)
set(re2_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/re2)
set(OpenSSL_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/openssl)
set(OPENSSL_ROOT_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux)
set(c-ares_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/c-ares)
set(hiredis_DIR /root/software/vcpkg-2024.09.30/installed/x64-linux/share/hiredis)

file(GLOB CPP_SRC ${CMAKE_SOURCE_DIR}/*.cpp)
file(GLOB CC_SRC ${CMAKE_SOURCE_DIR}/*.cc)
set(SRC ${CPP_SRC} ${CC_SRC})
message(STATUS ${SRC})
add_executable(a.out ${SRC})
target_link_libraries(a.out PUBLIC boost_system boost_filesystem pthread jsoncpp)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(a.out PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(a.out PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

find_package(upb CONFIG REQUIRED)
target_link_libraries(a.out PRIVATE upb::upb)

find_package(utf8_range CONFIG REQUIRED)
target_link_libraries(a.out PRIVATE utf8_range::utf8_range utf8_range::utf8_validity)

find_package(c-ares REQUIRED)
find_package(OpenSSL CONFIG REQUIRED)
target_link_libraries(a.out PRIVATE OpenSSL::SSL)
target_link_libraries(a.out PRIVATE OpenSSL::Crypto)

find_package(re2 CONFIG REQUIRED)
target_link_libraries(a.out PRIVATE re2::re2)

find_package(hiredis CONFIG REQUIRED)
target_link_libraries(a.out PRIVATE hiredis)

target_link_libraries(a.out PRIVATE mysqlcppconn)