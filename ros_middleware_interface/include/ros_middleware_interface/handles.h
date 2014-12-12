#ifndef __ros_middleware_interface__handles__h__
#define __ros_middleware_interface__handles__h__

namespace ros_middleware_interface
{

typedef struct NodeHandle {
  const char * _implementation_identifier;
  void * _data;
} NodeHandle;

typedef struct PublisherHandle {
  const char * _implementation_identifier;
  void * _data;
} PublisherHandle;

typedef struct SubscriberHandle {
  const char * implementation_identifier_;
  void * data_;
} SubscriberHandle;

typedef struct GuardConditionHandle {
  const char * implementation_identifier_;
  void * data_;
} GuardConditionHandle;

typedef struct SubscriberHandles {
  unsigned long subscriber_count_;
  void * * subscribers_;
} SubscriberHandles;

typedef struct GuardConditionHandles {
  unsigned long guard_condition_count_;
  void * * guard_conditions_;
} GuardConditionHandles;

typedef struct ClientHandle {
  const char * implementation_identifier_;
  void * data_;
} ClientHandle;

typedef struct ServiceHandle {
  const char * implementation_identifier_;
  void * data_;
} ServiceHandle;

typedef struct ServiceHandles {
  unsigned long service_count_;
  void * * services_;
} ServiceHandles;

}

#endif  // __ros_middleware_interface__handles__h__
