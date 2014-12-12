#ifndef __ros_middleware_interface__functions__h__
#define __ros_middleware_interface__functions__h__

#include "rosidl_generator_cpp/MessageTypeSupport.h"
#include "rosidl_generator_cpp/ServiceTypeSupport.h"

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

void wait(SubscriberHandles& subscriber_handles, GuardConditionHandles& guard_condition_handles, ServiceHandles& service_handles, bool non_blocking);

ClientHandle create_client(const NodeHandle& node_handle, const rosidl_generator_cpp::ServiceTypeSupportHandle & service_type_support_handle, const char * service_name);

void send_request(const ClientHandle& client_handle, const void * ros_request);

bool receive_response(const ClientHandle& client_handle, void * ros_response);

ServiceHandle create_service(const NodeHandle& node_handle, const rosidl_generator_cpp::ServiceTypeSupportHandle & service_type_support_handle, const char * service_name);

bool take_request(const ServiceHandle& service_handle, void * ros_request);

void send_response(const ServiceHandle& service_handle, void * ros_request, void * ros_response);
}

#endif  // __ros_middleware_interface__functions__h__
