#pragma once
#include "parameter_conflict_check.hpp"
#include <libcamera/controls.h>
#include <mutex>
#include <rclcpp/node_interfaces/node_parameters_interface.hpp>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace rclcpp
{
class Node;
} // namespace rclcpp


class ParameterHandler
{
public:
  // re-definition of the private ControlList::ControlListMap
  // typedef std::unordered_map<unsigned int, libcamera::ControlValue> ControlValueMap;

  ParameterHandler(rclcpp::Node *const node);

  void
  declare(const libcamera::ControlInfoMap &controls);

  libcamera::ControlList &
  get();

  void
  clear();

  // std::tuple<ControlValueMap, std::vector<std::string>>
  // parameterCheckAndConvert(const std::vector<rclcpp::Parameter> &parameters);

private:
  rclcpp::Node *const node;

  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_cb_on;

#ifdef RCLCPP_HAS_PARAM_EXT_CB
  rclcpp::node_interfaces::PreSetParametersCallbackHandle::SharedPtr param_cb_pre;
  rclcpp::node_interfaces::PostSetParametersCallbackHandle::SharedPtr param_cb_post;
#endif

  // TODO: consider a map<string, {ControlId, ControlInfo}>
  // std::vector<std::string> parameter_names;
  std::unordered_map<std::string, const libcamera::ControlId *> parameter_ids;
  std::unordered_map<std::string, libcamera::ControlInfo> parameter_info;
  // keep track of set parameters
  // ParameterMap parameters_full;

  // ControlValueMap control_values;
  libcamera::ControlList control_values;
  std::mutex parameters_lock;
  std::mutex parameters_consumed_lock;

  void
  adjust(std::vector<rclcpp::Parameter> &parameters);

  std::vector<std::string>
  validate(const std::vector<rclcpp::Parameter> &parameters);

  void
  apply(const std::vector<rclcpp::Parameter> &parameters);

  rcl_interfaces::msg::SetParametersResult
  OnSetValidate(const std::vector<rclcpp::Parameter> &parameters);

#ifdef RCLCPP_HAS_PARAM_EXT_CB
  void
  PreSetResolve(std::vector<rclcpp::Parameter> &parameters);

  void
  PostSetApply(const std::vector<rclcpp::Parameter> &parameters);
#endif
};