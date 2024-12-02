PROTOC=/root/software/vcpkg-2024.09.30/installed/x64-linux/tools/protobuf/protoc #protoc 路径

$PROTOC -I=$(pwd) --grpc_out=$(pwd) --plugin=protoc-gen-grpc="/root/software/vcpkg-2024.09.30/installed/x64-linux/tools/grpc/grpc_cpp_plugin" message.proto #生成.grpc.pb.h/cpp

$PROTOC --cpp_out=. message.proto #生成.pb.h/cpp