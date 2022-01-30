/*
 * @Author: your name
 * @Date: 2022-01-30 15:01:30
 * @LastEditTime: 2022-01-30 15:32:40
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/internal/HeathCheck.h
 */
#pragma once
#include <grpcpp/health_check_service_interface.h>

namespace yuzhi::raft_engine
{

    class HeathCheckServerImpl : public ::grpc::HealthCheckServiceInterface
    {
    public:
        HeathCheck() = default;
        ~HeathCheck() = default;
    };

} // namespace yuzhi
