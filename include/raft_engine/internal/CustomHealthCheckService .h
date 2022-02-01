/*
 * @Author: Leo
 * @Date: 2022-01-30 15:01:30
 * @LastEditTime: 2022-01-31 14:23:51
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/internal/HeathCheck.h
 */

#pragma once
#include <grpcpp/health_check_service_interface.h>

#include <raft_engine/internal/HealthCheckServiceImpl.h>

namespace yuzhi::raft_engine {

class CustomHealthCheckService : public grpc::HealthCheckServiceInterface {
public:
  explicit CustomHealthCheckService(HealthCheckServiceImpl *impl)
      : impl_(impl) {
    impl_->SetStatus("", HealthCheckResponse::SERVING);
  }
  void SetServingStatus(const grpc::string &service_name,
                        bool serving) override {
    impl_->SetStatus(service_name, serving ? HealthCheckResponse::SERVING
                                           : HealthCheckResponse::NOT_SERVING);
  }

  void SetServingStatus(bool serving) override {
    impl_->SetAll(serving ? HealthCheckResponse::SERVING
                          : HealthCheckResponse::NOT_SERVING);
  }

  void Shutdown() override { impl_->Shutdown(); }
};

} // namespace yuzhi::raft_engine
