// Copyright 2025 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <utility>
#include <unordered_map>
#include <vector>

#include "rmw/error_handling.h"
#include "rmw/types.h"

#include "rmw_security_common/security.hpp"

// Processor for security attributes with FILE URI
static bool process_file_uri_security_file(
  bool /*supports_pkcs11*/,
  const std::string & prefix,
  const std::filesystem::path & full_path,
  std::string & result)
{
  if (!std::filesystem::is_regular_file(full_path)) {
    return false;
  }
  result = prefix + full_path.generic_string();
  return true;
}

// Processor for security attributes with PKCS#11 URI
static bool process_pkcs_uri_security_file(
  bool supports_pkcs11,
  const std::string & /*prefix*/,
  const std::filesystem::path & full_path,
  std::string & result)
{
  if (!supports_pkcs11) {
    return false;
  }

  const std::string p11_prefix("pkcs11:");

  std::ifstream ifs(full_path);
  if (!ifs.is_open()) {
    return false;
  }

  if (!(ifs >> result)) {
    return false;
  }
  if (result.find(p11_prefix) != 0) {
    return false;
  }

  return true;
}

rmw_ret_t get_security_files(
  const char * prefix,
  const char * secure_root,
  rcutils_string_map_t * result)
{
  return get_security_files_support_pkcs(false, prefix, secure_root, result);
}

rmw_ret_t get_security_files_support_pkcs(
  bool supports_pkcs11,
  const char * prefix,
  const char * secure_root,
  rcutils_string_map_t * result)
{
  // check arguments that could be null
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(prefix, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(secure_root, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(result, RMW_RET_INVALID_ARGUMENT);

  std::string prefix_std(prefix);
  std::string secure_root_std(secure_root);

  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  using std::placeholders::_4;
  using security_file_processor =
    std::function<bool (bool, const std::string &, const std::filesystem::path &, std::string &)>;
  using processor_vector =
    std::vector<std::pair<std::string, security_file_processor>>;

  // Key: the security attribute
  // Value: ordered sequence of pairs. Each pair contains one possible file name
  //        for the attribute and the corresponding processor method
  // Pairs are ordered by priority: the first one matching is used.
  const std::unordered_map<std::string, processor_vector> required_files{
    {"IDENTITY_CA", {
        {"identity_ca.cert.p11", std::bind(process_pkcs_uri_security_file, _1, _2, _3, _4)},
        {"identity_ca.cert.pem", std::bind(process_file_uri_security_file, _1, _2, _3, _4)}}},
    {"CERTIFICATE", {
        {"cert.p11", std::bind(process_pkcs_uri_security_file, _1, _2, _3, _4)},
        {"cert.pem", std::bind(process_file_uri_security_file, _1, _2, _3, _4)}}},
    {"PRIVATE_KEY", {
        {"key.p11", std::bind(process_pkcs_uri_security_file, _1, _2, _3, _4)},
        {"key.pem", std::bind(process_file_uri_security_file, _1, _2, _3, _4)}}},
    {"PERMISSIONS_CA", {
        {"permissions_ca.cert.p11", std::bind(process_pkcs_uri_security_file, _1, _2, _3, _4)},
        {"permissions_ca.cert.pem", std::bind(process_file_uri_security_file, _1, _2, _3, _4)}}},
    {"GOVERNANCE", {
        {"governance.p7s", std::bind(process_file_uri_security_file, _1, _2, _3, _4)}}},
    {"PERMISSIONS", {
        {"permissions.p7s", std::bind(process_file_uri_security_file, _1, _2, _3, _4)}}},
  };

  const std::unordered_map<std::string, std::string> optional_files{
    {"CRL", "crl.pem"},
  };

  std::unordered_map<std::string, std::string> result_std;

  for (const std::pair<const std::string,
    std::vector<std::pair<std::string, security_file_processor>>> & el : required_files)
  {
    std::string attribute_value;
    bool processed = false;
    for (auto & proc : el.second) {
      std::filesystem::path full_path(secure_root_std);
      full_path /= proc.first;
      if (proc.second(supports_pkcs11, prefix_std, full_path, attribute_value)) {
        processed = true;
        break;
      }
    }
    if (!processed) {
      result_std.clear();
      return RMW_RET_ERROR;
    }
    result_std[el.first] = attribute_value;
  }

  for (const std::pair<const std::string, std::string> & el : optional_files) {
    std::filesystem::path full_path(secure_root_std);
    full_path /= el.second;
    if (std::filesystem::is_regular_file(full_path)) {
      result_std[el.first] = prefix_std + full_path.generic_string();
    }
  }

  rmw_ret_t ret;
  ret = rcutils_string_map_reserve(result, result_std.size());

  if (ret != RMW_RET_OK) {
    RMW_SET_ERROR_MSG("failed to reserve memory for the string map");
    return RMW_RET_ERROR;
  }

  for (const auto & [key, value] : result_std) {
    ret = rcutils_string_map_set(result, key.c_str(), value.c_str());
    if (ret != RMW_RET_OK) {
      RMW_SET_ERROR_MSG("failed to insert value in the string map");
      return RMW_RET_ERROR;
    }
  }

  return RMW_RET_OK;
}
