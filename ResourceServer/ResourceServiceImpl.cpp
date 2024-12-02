#include "ResourceServiceImpl.h"
#include "ErrorCode.h"
#include <fstream>
#include <filesystem>
grpc::Status ResourceServiceImpl::AddIcon(grpc::ServerContext* context, 
                                          message::AddIconReq* req, 
                                          message::AddIconRsp* rsp) {
    // Step 1: 提取 UID
    int uid = req->uid();

    // Step 2: 创建分层目录路径 (例如： ./icons/00/12/)
    std::string base_dir = "./icons/";
    //2,147,483,648
    int temp = uid / 100;
    int dir2 = temp % 10000;
    temp /= 10000;
    int dir1 = temp % 10000;

    std::string sub_dir = base_dir + std::to_string(dir1) + "/" + std::to_string(dir2) + "/";

    // 创建目录
    if (!std::filesystem::exists(sub_dir)) {
        std::filesystem::create_directories(sub_dir);
    }

    // Step 3: 文件路径
    std::string icon_path = sub_dir + std::to_string(uid) + ".icon";

    // Step 4: 打开文件并写入
    std::ofstream ofs(icon_path, std::ios::binary | std::ios::out);
    if (!ofs) {
        rsp->set_error(ErrorCode::RPC_FAILED);
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to open file for writing.");
    }

    ofs.write(req->contents().data(), req->contents().size());
    ofs.close();

    if (ofs.fail()) {
        rsp->set_error(ErrorCode::RPC_FAILED);
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to write to file.");
    }

    // Step 5: 响应成功
    rsp->set_error(ErrorCode::SUCCESS);
    rsp->set_url(icon_path); // 返回文件的完整路径
    return grpc::Status::OK;
}

grpc::Status ResourceServiceImpl::GetIcon(grpc::ServerContext*context,message::GetIconReq*req,message::GetIconRsp*rsp)
{
    std::string url = req->url();
    std::ifstream ifs(url,std::ios::in);
    if(ifs.is_open()==false){
        rsp->set_error(ErrorCode::RPC_FAILED);
        return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to open file for reading.");
    }
    // 一次性读取文件内容到 std::string
    std::string content((std::istreambuf_iterator<char>(ifs)),
                         std::istreambuf_iterator<char>());
    ifs.close();
    rsp->set_error(ErrorCode::SUCCESS);
    rsp->set_icon_pixmap(content);
    return grpc::Status::OK;
}