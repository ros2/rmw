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

}

#endif  // __ros_middleware_interface__handles__h__
