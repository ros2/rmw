// Copyright 2025 Minju Lee (이민주).
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

#include "gmock/gmock.h"
#include "osrf_testing_tools_cpp/scope_exit.hpp"
#include "rcutils/allocator.h"

#include "rmw/error_handling.h"
#include "rmw/service_endpoint_info.h"
#include "rmw/types.h"

char * get_mallocd_string(const char * s)
{
  size_t size = strlen(s) + 1;
  char * c = reinterpret_cast<char *>(malloc(size));
  memcpy(c, s, size);
  return c;
}

TEST(test_service_endpoint_info, set_service_type) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  char * val = get_mallocd_string("test_service_type");
  rmw_ret_t ret = rmw_service_endpoint_info_set_service_type(&service_endpoint_info, val, nullptr);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null allocator";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_service_type(&service_endpoint_info, nullptr, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null service_type";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_service_type(nullptr, val, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    allocator.deallocate(const_cast<char *>(service_endpoint_info.service_type), allocator.state);
  });
  ret = rmw_service_endpoint_info_set_service_type(&service_endpoint_info, val, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  // free the mallocd string and verify that the string was copied by value
  free(val);
  EXPECT_STREQ(
    service_endpoint_info.service_type,
    "test_service_type") << "Service Type value is not as expected";
}

TEST(test_service_endpoint_info, set_service_type_hash) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  rosidl_type_hash_t type_hash = rosidl_get_zero_initialized_type_hash();
  type_hash.version = 22;
  for (uint8_t i = 0; i < ROSIDL_TYPE_HASH_SIZE; i++) {
    type_hash.value[i] = i;
  }

  rmw_ret_t ret = rmw_service_endpoint_info_set_service_type_hash(nullptr, &type_hash);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_service_type_hash(&service_endpoint_info, nullptr);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null type_hash";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_service_type_hash(&service_endpoint_info, &type_hash);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";

  EXPECT_EQ(service_endpoint_info.service_type_hash.version, 22);
  for (size_t i = 0; i < ROSIDL_TYPE_HASH_SIZE; i++) {
    EXPECT_EQ(i, service_endpoint_info.service_type_hash.value[i]);
  }
}

TEST(test_service_endpoint_info, set_node_name) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  char * val = get_mallocd_string("test_node_name");
  rmw_ret_t ret = rmw_service_endpoint_info_set_node_name(&service_endpoint_info, val, nullptr);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null allocator";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_node_name(&service_endpoint_info, nullptr, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null node_name";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_node_name(nullptr, val, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    allocator.deallocate(const_cast<char *>(service_endpoint_info.node_name), allocator.state);
  });
  ret = rmw_service_endpoint_info_set_node_name(&service_endpoint_info, val, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  // free the mallocd string and verify that the string was copied by value
  free(val);
  EXPECT_STREQ(service_endpoint_info.node_name, "test_node_name") <<
    "Node name value is not as expected";
}

TEST(test_service_endpoint_info, set_node_namespace) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  char * val = get_mallocd_string("test_node_namespace");
  rmw_ret_t ret = rmw_service_endpoint_info_set_node_namespace(
    &service_endpoint_info,
    val,
    nullptr);

  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null allocator";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_node_namespace(&service_endpoint_info, nullptr, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null node_namespace";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_node_namespace(nullptr, val, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    allocator.deallocate(const_cast<char *>(service_endpoint_info.node_namespace), allocator.state);
  });
  ret = rmw_service_endpoint_info_set_node_namespace(&service_endpoint_info, val, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  // free the mallocd string and verify that the string was copied by value
  free(val);
  EXPECT_STREQ(service_endpoint_info.node_namespace, "test_node_namespace") <<
    "Node namespace value is not as expected";
}

TEST(test_service_endpoint_info, set_endpoint_type) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();

  rmw_ret_t ret = rmw_service_endpoint_info_set_endpoint_type(nullptr, RMW_ENDPOINT_CLIENT);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_endpoint_type(
    &service_endpoint_info,
    RMW_ENDPOINT_CLIENT);

  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  EXPECT_EQ(service_endpoint_info.endpoint_type, RMW_ENDPOINT_CLIENT) <<
    "endpoint_type is not as expected";
}

TEST(test_service_endpoint_info, set_endpoint_count) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();

  rmw_ret_t ret = rmw_service_endpoint_info_set_endpoint_count(nullptr, 1);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_endpoint_count(&service_endpoint_info, 0);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for endpoint_count not being 1 or 2";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_endpoint_count(&service_endpoint_info, 3);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for endpoint_count not being 1 or 2";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_endpoint_count(&service_endpoint_info, 1);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  EXPECT_EQ(service_endpoint_info.endpoint_count, 1) << "endpoint_count is not as expected";

  ret = rmw_service_endpoint_info_set_endpoint_count(&service_endpoint_info, 2);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  EXPECT_EQ(service_endpoint_info.endpoint_count, 2) << "endpoint_count is not as expected";
}

TEST(test_service_endpoint_info, set_gids) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  uint8_t gid[1][RMW_GID_STORAGE_SIZE];
  for (uint8_t i = 0; i < RMW_GID_STORAGE_SIZE; i++) {
    gid[0][i] = i;
  }
  rmw_ret_t ret = rmw_service_endpoint_info_set_gids(
    nullptr,
    (const uint8_t *) gid,
    1,
    RMW_GID_STORAGE_SIZE,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_gids(
    &service_endpoint_info,
    (const uint8_t *) gid,
    0,
    RMW_GID_STORAGE_SIZE,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for endpoint_count not being 1 or 2";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_gids(
    &service_endpoint_info,
    (const uint8_t *) gid,
    3,
    RMW_GID_STORAGE_SIZE,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for endpoint_count not being 1 or 2";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_gids(
    &service_endpoint_info,
    (const uint8_t *) gid,
    1,
    RMW_GID_STORAGE_SIZE + 1,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for size greater than RMW_GID_STORAGE_SIZE";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_gids(
    &service_endpoint_info,
    (const uint8_t *) gid,
    1,
    RMW_GID_STORAGE_SIZE,
    nullptr);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null allocator";
  rmw_reset_error();

  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    allocator.deallocate(service_endpoint_info.endpoint_gids, allocator.state);
  });
  ret = rmw_service_endpoint_info_set_gids(
    &service_endpoint_info,
    (const uint8_t *) gid,
    1,
    RMW_GID_STORAGE_SIZE,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  for (uint8_t i = 0; i < RMW_GID_STORAGE_SIZE; i++) {
    EXPECT_EQ(service_endpoint_info.endpoint_gids[0][i], gid[0][i]) <<
      "Gid value is not as expected";
  }

  uint8_t reader_writer_gid[2][RMW_GID_STORAGE_SIZE];
  for (uint8_t i = 0; i < RMW_GID_STORAGE_SIZE; i++) {
    reader_writer_gid[0][i] = i + 1;
    reader_writer_gid[1][i] = i + 2;
  }
  ret = rmw_service_endpoint_info_set_gids(
    &service_endpoint_info,
    (const uint8_t *) reader_writer_gid,
    2,
    RMW_GID_STORAGE_SIZE,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  for (uint8_t i = 0; i < RMW_GID_STORAGE_SIZE; i++) {
    EXPECT_EQ(service_endpoint_info.endpoint_gids[0][i], reader_writer_gid[0][i]) <<
      "Gid value is not as expected";
    EXPECT_EQ(service_endpoint_info.endpoint_gids[1][i], reader_writer_gid[1][i]) <<
      "Gid value is not as expected";
  }
}

TEST(test_service_endpoint_info, set_qos_profiles) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmw_qos_profile_t qos_profile;
  qos_profile.history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  qos_profile.depth = 0;
  qos_profile.reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  qos_profile.durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  qos_profile.deadline = {1, 0};
  qos_profile.lifespan = {2, 0};
  qos_profile.liveliness = RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
  qos_profile.liveliness_lease_duration = {3, 0};
  qos_profile.avoid_ros_namespace_conventions = false;

  auto expect_qos_equal = [](const rmw_qos_profile_t & actual, const rmw_qos_profile_t & expected) {
      EXPECT_EQ(actual.history, expected.history) << "Unequal history";
      EXPECT_EQ(actual.depth, expected.depth) << "Unequal depth";
      EXPECT_EQ(actual.reliability, expected.reliability) << "Unequal reliability";
      EXPECT_EQ(actual.durability, expected.durability) << "Unequal durability";
      EXPECT_EQ(actual.deadline.sec, expected.deadline.sec) << "Unequal deadline sec";
      EXPECT_EQ(actual.deadline.nsec, expected.deadline.nsec) << "Unequal deadline nsec";
      EXPECT_EQ(actual.lifespan.sec, expected.lifespan.sec) << "Unequal lifespan sec";
      EXPECT_EQ(actual.lifespan.nsec, expected.lifespan.nsec) << "Unequal lifespan nsec";
      EXPECT_EQ(actual.liveliness, expected.liveliness) << "Unequal liveliness";
      EXPECT_EQ(actual.liveliness_lease_duration.sec, expected.liveliness_lease_duration.sec)
        << "Unequal liveliness lease duration sec";
      EXPECT_EQ(actual.liveliness_lease_duration.nsec, expected.liveliness_lease_duration.nsec)
        << "Unequal liveliness lease duration nsec";
      EXPECT_EQ(actual.avoid_ros_namespace_conventions, expected.avoid_ros_namespace_conventions)
        << "Unequal avoid namespace conventions";
    };

  rmw_ret_t ret = rmw_service_endpoint_info_set_qos_profiles(nullptr, &qos_profile, 1, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_qos_profiles(
    &service_endpoint_info,
    nullptr,
    1,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null qos_profiles";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_qos_profiles(
    &service_endpoint_info,
    &qos_profile,
    0,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for endpoint_count not being 1 or 2";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_set_qos_profiles(
    &service_endpoint_info,
    &qos_profile,
    3,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for endpoint_count not being 1 or 2";
  rmw_reset_error();

  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    allocator.deallocate(service_endpoint_info.qos_profiles, allocator.state);
  });
  ret = rmw_service_endpoint_info_set_qos_profiles(
    &service_endpoint_info,
    &qos_profile,
    1,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";

  expect_qos_equal(service_endpoint_info.qos_profiles[0], qos_profile);

  rmw_qos_profile_t reader_qos_profile = qos_profile;
  rmw_qos_profile_t writer_qos_profile;
  writer_qos_profile.history = RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT;
  writer_qos_profile.depth = 2;
  writer_qos_profile.reliability = RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
  writer_qos_profile.durability = RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL;
  writer_qos_profile.deadline = {2, 0};
  writer_qos_profile.lifespan = {3, 0};
  writer_qos_profile.liveliness = RMW_QOS_POLICY_LIVELINESS_AUTOMATIC;
  writer_qos_profile.liveliness_lease_duration = {4, 0};
  writer_qos_profile.avoid_ros_namespace_conventions = true;

  rmw_qos_profile_t qos_profiles[2] = {reader_qos_profile, writer_qos_profile};
  ret = rmw_service_endpoint_info_set_qos_profiles(
    &service_endpoint_info,
    qos_profiles,
    2,
    &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";

  expect_qos_equal(service_endpoint_info.qos_profiles[0], reader_qos_profile);
  expect_qos_equal(service_endpoint_info.qos_profiles[1], writer_qos_profile);
}

TEST(test_service_endpoint_info, zero_init) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  EXPECT_FALSE(service_endpoint_info.node_name);
  EXPECT_FALSE(service_endpoint_info.node_namespace);
  EXPECT_FALSE(service_endpoint_info.service_type);
  EXPECT_EQ(service_endpoint_info.endpoint_type, RMW_ENDPOINT_INVALID) <<
    "Endpoint type value should be invalid";
  EXPECT_EQ(service_endpoint_info.endpoint_gids, nullptr);
  EXPECT_EQ(service_endpoint_info.qos_profiles, nullptr);
}

TEST(test_service_endpoint_info, fini) {
  rmw_service_endpoint_info_t service_endpoint_info =
    rmw_get_zero_initialized_service_endpoint_info();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmw_qos_profile_t qos_profile;
  qos_profile.history = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
  qos_profile.depth = 0;
  qos_profile.reliability = RMW_QOS_POLICY_RELIABILITY_RELIABLE;
  qos_profile.durability = RMW_QOS_POLICY_DURABILITY_VOLATILE;
  qos_profile.deadline = {1, 0};
  qos_profile.lifespan = {2, 0};
  qos_profile.liveliness = RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC;
  qos_profile.liveliness_lease_duration = {3, 0};
  qos_profile.avoid_ros_namespace_conventions = false;

  uint8_t gid[1][RMW_GID_STORAGE_SIZE];
  for (uint8_t i = 0; i < RMW_GID_STORAGE_SIZE; i++) {
    gid[0][i] = i * 12;
  }

  rmw_ret_t ret = rmw_service_endpoint_info_set_qos_profiles(
    &service_endpoint_info, &qos_profile, 1, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  ret = rmw_service_endpoint_info_set_endpoint_type(&service_endpoint_info, RMW_ENDPOINT_CLIENT);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid rmw_endpoint_type_t arguments";
  ret = rmw_service_endpoint_info_set_gids(
    &service_endpoint_info, (const uint8_t *) gid, 1, RMW_GID_STORAGE_SIZE, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid gids arguments";
  ret = rmw_service_endpoint_info_set_node_namespace(
    &service_endpoint_info, "namespace", &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid node_namespace arguments";
  ret = rmw_service_endpoint_info_set_node_name(&service_endpoint_info, "name", &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid node_name arguments";
  ret = rmw_service_endpoint_info_set_service_type(&service_endpoint_info, "type", &allocator);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid service_type arguments";
  ret = rmw_service_endpoint_info_fini(&service_endpoint_info, nullptr);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) << "Expected invalid argument for null allocator";
  rmw_reset_error();
  ret = rmw_service_endpoint_info_fini(nullptr, &allocator);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null service_endpoint_info";
  rmw_reset_error();

  ret = rmw_service_endpoint_info_fini(&service_endpoint_info, &allocator);
  // Verify that the values inside the struct are zero-ed out and finished.
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid fini arguments";
  EXPECT_FALSE(service_endpoint_info.node_name);
  EXPECT_FALSE(service_endpoint_info.node_namespace);
  EXPECT_FALSE(service_endpoint_info.service_type);
  EXPECT_EQ(service_endpoint_info.endpoint_type, RMW_ENDPOINT_INVALID) <<
    "Endpoint type value should be invalid";
  EXPECT_EQ(service_endpoint_info.endpoint_gids, nullptr);
  EXPECT_EQ(service_endpoint_info.qos_profiles, nullptr);
}
