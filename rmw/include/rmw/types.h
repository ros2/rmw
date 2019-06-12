// Copyright 2014-2017 Open Source Robotics Foundation, Inc.
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

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// map rcutils specific log levels to rmw speicfic type
#include <rcutils/logging.h>

#include "rmw/init.h"
#include "rmw/ret_types.h"
#include "rmw/serialized_message.h"
#include "rmw/visibility_control.h"

// 24 bytes is the most memory needed to represent the GID by any current
// implementation. It may need to be increased in the future.
#define RMW_GID_STORAGE_SIZE 24

typedef struct RMW_PUBLIC_TYPE rmw_node_t
{
  const char * implementation_identifier;
  void * data;
  const char * name;
  const char * namespace_;
  rmw_context_t * context;
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
  rmw_context_t * context;
} rmw_guard_condition_t;

typedef struct RMW_PUBLIC_TYPE rmw_publisher_allocation_t
{
  const char * implementation_identifier;
  void * data;
} rmw_publisher_allocation_t;

typedef struct RMW_PUBLIC_TYPE rmw_subscription_allocation_t
{
  const char * implementation_identifier;
  void * data;
} rmw_subscription_allocation_t;

/// Array of subscriber handles.
/**
 * An array of void * pointers representing type-erased middleware-specific subscriptions.
 * The number of non-null entries may be smaller than the allocated size of the array.
 * The number of subscriptions represented may be smaller than the allocated size of the array.
 * The creator of this struct is responsible for allocating and deallocating the array.
 */
typedef struct RMW_PUBLIC_TYPE rmw_subscriptions_t
{
  /// The number of subscribers represented by the array.
  size_t subscriber_count;
  /// Pointer to an array of void * pointers of subscriptions.
  void ** subscribers;
} rmw_subscriptions_t;

/// Array of service handles.
/**
 * An array of void * pointers representing type-erased middleware-specific services.
 * The number of non-null entries may be smaller than the allocated size of the array.
 * The number of services represented may be smaller than the allocated size of the array.
 * The creator of this struct is responsible for allocating and deallocating the array.
 */
typedef struct RMW_PUBLIC_TYPE rmw_services_t
{
  /// The number of services represented by the array.
  size_t service_count;
  /// Pointer to an array of void * pointers of services.
  void ** services;
} rmw_services_t;

/// Array of client handles.
/**
 * An array of void * pointers representing type-erased middleware-specific clients.
 * The number of non-null entries may be smaller than the allocated size of the array.
 * The number of clients represented may be smaller than the allocated size of the array.
 * The creator of this struct is responsible for allocating and deallocating the array.
 */
typedef struct RMW_PUBLIC_TYPE rmw_clients_t
{
  /// The number of clients represented by the array.
  size_t client_count;
  /// Pointer to an array of void * pointers of clients.
  void ** clients;
} rmw_clients_t;

typedef struct RMW_PUBLIC_TYPE rmw_events_t
{
  /// The number of events represented by the array.
  size_t event_count;
  /// Pointer to an array of void * pointers of events.
  void ** events;
} rmw_events_t;

/// Array of guard condition handles.
/**
 * An array of void * pointers representing type-erased middleware-specific guard conditions.
 * The number of non-null entries may be smaller than the allocated size of the array.
 * The number of guard conditions represented may be smaller than the allocated size of the array.
 * The creator of this struct is responsible for allocating and deallocating the array.
 */
typedef struct RMW_PUBLIC_TYPE rmw_guard_conditions_t
{
  /// The number of guard conditions represented by the array.
  size_t guard_condition_count;
  /// Pointer to an array of void * pointers of guard conditions.
  void ** guard_conditions;
} rmw_guard_conditions_t;

typedef struct RMW_PUBLIC_TYPE rmw_wait_set_t
{
  const char * implementation_identifier;
  rmw_guard_conditions_t * guard_conditions;
  void * data;
} rmw_wait_set_t;

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

enum RMW_PUBLIC_TYPE rmw_security_enforcement_policy_t
{
  RMW_SECURITY_ENFORCEMENT_PERMISSIVE,
  RMW_SECURITY_ENFORCEMENT_ENFORCE,
};

typedef struct RMW_PUBLIC_TYPE rmw_node_security_options_t
{
  enum rmw_security_enforcement_policy_t enforce_security;
  const char * security_root_path;
} rmw_node_security_options_t;

enum RMW_PUBLIC_TYPE rmw_qos_reliability_policy_t
{
  RMW_QOS_POLICY_RELIABILITY_SYSTEM_DEFAULT,
  RMW_QOS_POLICY_RELIABILITY_RELIABLE,
  RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT,
  RMW_QOS_POLICY_RELIABILITY_UNKNOWN
};

enum RMW_PUBLIC_TYPE rmw_qos_history_policy_t
{
  RMW_QOS_POLICY_HISTORY_SYSTEM_DEFAULT,
  RMW_QOS_POLICY_HISTORY_KEEP_LAST,
  RMW_QOS_POLICY_HISTORY_KEEP_ALL,
  RMW_QOS_POLICY_HISTORY_UNKNOWN
};

enum RMW_PUBLIC_TYPE rmw_qos_durability_policy_t
{
  RMW_QOS_POLICY_DURABILITY_SYSTEM_DEFAULT,
  RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL,
  RMW_QOS_POLICY_DURABILITY_VOLATILE,
  RMW_QOS_POLICY_DURABILITY_UNKNOWN
};

enum RMW_PUBLIC_TYPE rmw_qos_liveliness_policy_t
{
  RMW_QOS_POLICY_LIVELINESS_SYSTEM_DEFAULT,
  RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
  RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_NODE,
  RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC,
  RMW_QOS_POLICY_LIVELINESS_UNKNOWN
};

#define RMW_QOS_DEADLINE_DEFAULT {0, 0}
#define RMW_QOS_LIFESPAN_DEFAULT {0, 0}
#define RMW_QOS_LIVELINESS_LEASE_DURATION_DEFAULT {0, 0}

/// ROS MiddleWare quality of service profile.
typedef struct RMW_PUBLIC_TYPE rmw_qos_profile_t
{
  enum rmw_qos_history_policy_t history;
  /// Size of the message queue.
  size_t depth;
  /// Reliabiilty QoS policy setting
  enum rmw_qos_reliability_policy_t reliability;
  /// Durability QoS policy setting
  enum rmw_qos_durability_policy_t durability;
  /// The period at which messages are expected to be sent/received
  struct rmw_time_t deadline;
  /// The age at which messages are considered expired and no longer valid
  struct rmw_time_t lifespan;
  /// Liveliness QoS policy setting
  enum rmw_qos_liveliness_policy_t liveliness;
  /// The time within which the RMW node or publisher must show that it is alive
  struct rmw_time_t liveliness_lease_duration;

  /// If true, any ROS specific namespacing conventions will be circumvented.
  /**
   * In the case of DDS and topics, for example, this means the typical
   * ROS specific prefix of `rt` would not be applied as described here:
   *
   *   http://design.ros2.org/articles/topic_and_service_names.html#ros-specific-namespace-prefix
   *
   * This might be useful when trying to directly connect a native DDS topic
   * with a ROS 2 topic.
   */
  bool avoid_ros_namespace_conventions;
} rmw_qos_profile_t;

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

/// Type mapping of rcutils log severity types to rmw specific types.
typedef enum RMW_PUBLIC_TYPE
{
  RMW_LOG_SEVERITY_DEBUG = RCUTILS_LOG_SEVERITY_DEBUG,
  RMW_LOG_SEVERITY_INFO = RCUTILS_LOG_SEVERITY_INFO,
  RMW_LOG_SEVERITY_WARN = RCUTILS_LOG_SEVERITY_WARN,
  RMW_LOG_SEVERITY_ERROR = RCUTILS_LOG_SEVERITY_ERROR,
  RMW_LOG_SEVERITY_FATAL = RCUTILS_LOG_SEVERITY_FATAL
} rmw_log_severity_t;

/// QoS Liveliness Changed information provided by a subscription.
typedef struct RMW_PUBLIC_TYPE rmw_liveliness_changed_status_t
{
  /**
   * The total number of currently active Publishers which publish to the topic associated with
   * the Subscription.
   * This count increases when a newly matched Publisher asserts its liveliness for the first time
   * or when a Publisher previously considered to be not alive reasserts its liveliness.
   * The count decreases when a Publisher considered alive fails to assert its liveliness and
   * becomes not alive, whether because it was deleted normally or for some other reason.
   */
  int32_t alive_count;
  /**
   * The total count of current Publishers which publish to the topic associated with the
   * Subscription that are no longer asserting their liveliness.
   * This count increases when a Publisher considered alive fails to assert its liveliness and
   * becomes not alive for some reason other than the normal deletion of that Publisher.
   * It decreases when a previously not alive Publisher either reasserts its liveliness or is
   * deleted normally.
   */
  int32_t not_alive_count;
  /// The change in the alive_count since the status was last read.
  int32_t alive_count_change;
  /// The change in the not_alive_count since the status was last read.
  int32_t not_alive_count_change;
} rmw_liveliness_changed_status_t;

/// QoS Requested Deadline Missed information provided by a subscription.
typedef struct RMW_PUBLIC_TYPE rmw_requested_deadline_missed_status_t
{
  /**
   * Lifetime cumulative number of missed deadlines detected for any instance read by the
   * subscription.
   * Missed deadlines accumulate; that is, each deadline period the total_count will be incremented
   * by one for each instance for which data was not received.
   */
  int32_t total_count;
  /// The incremental number of deadlines detected since the status was read.
  int32_t total_count_change;
} rmw_requested_deadline_missed_status_t;

/// QoS Liveliness Lost information provided by a publisher.
typedef struct RMW_PUBLIC_TYPE rmw_liveliness_lost_status_t
{
  /**
   * Lifetime cumulative number of times that a previously-alive Publisher became not alive due to
   * a failure to actively signal its liveliness within its offered liveliness period.
   * This count does not change when an already not alive Publisher simply remains not alive for
   * another liveliness period.
   */
  int32_t total_count;
  /// The change in total_count since the last time the status was last read.
  int32_t total_count_change;
} rmw_liveliness_lost_status_t;

/// QoS Deadline Missed information provided by a publisher.
typedef struct RMW_PUBLIC_TYPE rmw_offered_deadline_missed_status_t
{
  /**
   * Lifetime cumulative number of offered deadline periods elapsed during which a Publisher failed
   * to provide data.
   * Missed deadlines accumulate; that is, each deadline period the total_count will be incremented
   * by one.
   */
  int32_t total_count;
  /// The change in total_count since the last time the status was last read.
  int32_t total_count_change;
} rmw_offered_deadline_missed_status_t;

#ifdef __cplusplus
}
#endif

#endif  // RMW__TYPES_H_
