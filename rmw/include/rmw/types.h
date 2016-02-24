// Copyright 2014 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__TYPES_H_
#define RMW__TYPES_H_

#if __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rmw/visibility_control.h"

typedef int rmw_ret_t;
#define RMW_RET_OK 0
#define RMW_RET_ERROR 1
#define RMW_RET_TIMEOUT 2

// 24 bytes is the most memory needed to represent the GID by any current
// implementation. It may need to be increased in the future.
#define RMW_GID_STORAGE_SIZE 24

typedef struct RMW_PUBLIC_TYPE rmw_node_t
{
  const char * implementation_identifier;
  void * data;
  const char * name;
  const size_t domain_id;
} rmw_node_t;

typedef struct RMW_PUBLIC_TYPE rmw_publisher_t
{
  const char * implementation_identifier;
  void * data;
  const char * topic_name;
} rmw_publisher_t;

typedef struct RMW_PUBLIC_TYPE rmw_subscription_t
{
  const char * implementation_identifier;
  void * data;
  const char * topic_name;
} rmw_subscription_t;

typedef struct RMW_PUBLIC_TYPE rmw_service_t
{
  const char * implementation_identifier;
  void * data;
  const char * service_name;
} rmw_service_t;

typedef struct RMW_PUBLIC_TYPE rmw_client_t
{
  const char * implementation_identifier;
  void * data;
  const char * service_name;
} rmw_client_t;

typedef struct RMW_PUBLIC_TYPE rmw_guard_condition_t
{
  const char * implementation_identifier;
  void * data;
} rmw_guard_condition_t;

typedef struct RMW_PUBLIC_TYPE rmw_subscriptions_t
{
  size_t subscriber_count;
  void ** subscribers;
} rmw_subscriptions_t;

typedef struct RMW_PUBLIC_TYPE rmw_services_t
{
  size_t service_count;
  void ** services;
} rmw_services_t;

typedef struct RMW_PUBLIC_TYPE rmw_clients_t
{
  size_t client_count;
  void ** clients;
} rmw_clients_t;

typedef struct RMW_PUBLIC_TYPE rmw_guard_conditions_t
{
  size_t guard_condition_count;
  void ** guard_conditions;
} rmw_guard_conditions_t;

typedef struct RMW_PUBLIC_TYPE rmw_waitset_t
{
  const char * implementation_identifier;
  rmw_guard_conditions_t * fixed_guard_conditions;
  void * data;
} rmw_waitset_t;

typedef struct RMW_PUBLIC_TYPE rmw_request_id_t
{
  int8_t writer_guid[16];
  int64_t sequence_number;
} rmw_request_id_t;

typedef struct RMW_PUBLIC_TYPE rmw_time_t
{
  uint64_t sec;
  uint64_t nsec;
} rmw_time_t;

enum RMW_PUBLIC_TYPE rmw_qos_reliability_policy_t
{
  RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT,
  RMW_QOS_POLICY_RELIABLE,
  RMW_QOS_POLICY_BEST_EFFORT
};

enum RMW_PUBLIC_TYPE rmw_qos_history_policy_t
{
  RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT,
  RMW_QOS_POLICY_KEEP_LAST_HISTORY,
  RMW_QOS_POLICY_KEEP_ALL_HISTORY
};

enum RMW_PUBLIC_TYPE rmw_qos_durability_policy_t
{
  RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT,
  RMW_QOS_POLICY_TRANSIENT_LOCAL_DURABILITY,
  RMW_QOS_POLICY_VOLATILE_DURABILITY
};

typedef struct RMW_PUBLIC_TYPE rmw_qos_profile_t
{
  enum rmw_qos_history_policy_t history;
  size_t depth;
  enum rmw_qos_reliability_policy_t reliability;
  enum rmw_qos_durability_policy_t durability;
} rmw_qos_profile_t;

typedef struct RMW_PUBLIC_TYPE rmw_topic_names_and_types_t
{
  size_t topic_count;
  char ** topic_names;
  char ** type_names;
} rmw_topic_names_and_types_t;

typedef struct RMW_PUBLIC_TYPE rmw_gid_t
{
  const char * implementation_identifier;
  uint8_t data[RMW_GID_STORAGE_SIZE];
} rmw_gid_t;

typedef struct RMW_PUBLIC_TYPE rmw_message_info_t
{
  // const rmw_time_t received_timestamp;
  rmw_gid_t publisher_gid;
  bool from_intra_process;
} rmw_message_info_t;

enum {RMW_QOS_POLICY_DEPTH_SYSTEM_DEFAULT = 0};

#if __cplusplus
}
#endif

#endif  // RMW__TYPES_H_
