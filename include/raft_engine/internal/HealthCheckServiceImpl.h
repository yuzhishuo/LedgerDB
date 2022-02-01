/*
 * @Author: your name
 * @Date: 2022-01-31 14:09:00
 * @LastEditTime: 2022-01-31 14:28:26
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath:
 * /example-authority-cpp/include/raft_engine/internal/HealthCheckServiceImpl.h
 */

#pragma once

#include <grpc/health/v1/health.grpc.pb.h>

namespace yuzhi::raft_engine {

class HealthCheckServiceImpl : public grpc::Health::v1::Health::Service {
public:
  Status Check(ServerContext *context,
               const health::v1::HealthCheckRequest *request,
               health::v1::HealthCheckResponse *response) override;
  Status Watch(ServerContext *context,
               const health::v1::HealthCheckRequest *request,
               ServerWriter<health::v1::HealthCheckResponse> *writer) override;
  void SetStatus(const grpc::string &service_name,
                 health::v1::HealthCheckResponse::ServingStatus status);
  void SetAll(health::v1::HealthCheckResponse::ServingStatus status);

  void Shutdown();

private:
  std::mutex mu_;
  bool shutdown_ = false;
  std::map<const grpc::string, health::v1::HealthCheckResponse::ServingStatus>
      status_map_;
};

} // namespace yuzhi::raft_engine