#ifndef __ros_middleware_interface__functions__h__
#define __ros_middleware_interface__functions__h__

#include "rosidl_generator_cpp/MessageTypeSupport.h"

#include "handles.h"

namespace ros_middleware_interface
{

void init();

NodeHandle create_node();

template<typename T>
const rosidl_generator_cpp::MessageTypeSupportHandle& get_type_support_handle();

PublisherHandle create_publisher(const NodeHandle& node_handle, const rosidl_generator_cpp::MessageTypeSupportHandle & type_support_handle, const char * topic_name);

void publish(const PublisherHandle& publisher_handle, const void * ros_message);

SubscriberHandle create_subscriber(const NodeHandle& node_handle, const rosidl_generator_cpp::MessageTypeSupportHandle & type_support_handle, const char * topic_name);

bool take(const SubscriberHandle& subscriber_handle, void * ros_message);

GuardConditionHandle create_guard_condition();

void trigger_guard_condition(const GuardConditionHandle& guard_condition_handle);

void wait(SubscriberHandles& subscriber_handles, GuardConditionHandles& guard_condition_handles, bool non_blocking);

}

#endif  // __ros_middleware_interface__functions__h__
