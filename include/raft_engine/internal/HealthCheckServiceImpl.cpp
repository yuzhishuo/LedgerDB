/*
 * @Author: Leo
 * @Date: 2022-01-31 14:26:15
 * @LastEditTime: 2022-07-22 09:50:23
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/src/raft_engine/HealthCheckServiceImpl.cpp
 */

#include <grpc/grpc.h>

#include <raft_engine/internal/HealthCheckServiceImpl.h>

using namespace yuzhi::raft_engine;
using grpc::health::v1::HealthCheckRequest;
using grpc::health::v1::HealthCheckResponse;

Status HealthCheckServiceImpl::Check(ServerContext *context, const HealthCheckRequest *request,
                                     HealthCheckResponse *response)
{
  std::lock_guard<std::mutex> lock(mu_);
  auto iter = status_map_.find(request->service());
  if (iter == status_map_.end())
  {
    return Status(StatusCode::NOT_FOUND, "");
  }
  response->set_status(iter->second);
  return Status::OK;
}

Status HealthCheckServiceImpl::Watch(ServerContext *context, const HealthCheckRequest *request,
                                     ::grpc::ServerWriter<HealthCheckResponse> *writer)
{
  auto last_state = HealthCheckResponse::UNKNOWN;
  while (!context->IsCancelled())
  {
    {
      std::lock_guard<std::mutex> lock(mu_);
      HealthCheckResponse response;
      auto iter = status_map_.find(request->service());
      if (iter == status_map_.end())
      {
        response.set_status(response.SERVICE_UNKNOWN);
      }
      else
      {
        response.set_status(iter->second);
      }
      if (response.status() != last_state)
      {
        writer->Write(response, ::grpc::WriteOptions());
      }
    }
    gpr_sleep_until(gpr_time_add(gpr_now(GPR_CLOCK_MONOTONIC), gpr_time_from_millis(1000, GPR_TIMESPAN)));
  }
  return Status::OK;
}

void HealthCheckServiceImpl::SetStatus(const grpc::string &service_name, HealthCheckResponse::ServingStatus status)
{
  std::lock_guard<std::mutex> lock(mu_);
  if (shutdown_)
  {
    status = HealthCheckResponse::NOT_SERVING;
  }
  status_map_[service_name] = status;
}

void HealthCheckServiceImpl::SetAll(HealthCheckResponse::ServingStatus status)
{
  std::lock_guard<std::mutex> lock(mu_);
  if (shutdown_)
  {
    return;
  }
  for (auto iter = status_map_.begin(); iter != status_map_.end(); ++iter)
  {
    iter->second = status;
  }
}

void HealthCheckServiceImpl::Shutdown()
{
  std::lock_guard<std::mutex> lock(mu_);
  if (shutdown_)
  {
    return;
  }
  shutdown_ = true;
  for (auto iter = status_map_.begin(); iter != status_map_.end(); ++iter)
  {
    iter->second = HealthCheckResponse::NOT_SERVING;
  }
}
