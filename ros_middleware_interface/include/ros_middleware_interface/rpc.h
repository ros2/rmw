#ifndef __ros_middleware_interface__rpc__h__
#define __ros_middleware_interface__rpc__h__

#include <cstdint>

namespace ros_middleware_interface
{

typedef struct RequestId {
  int8_t writer_guid[16];
  int64_t sequence_number;
} RequestId;

}

#endif  // __ros_middleware_interface__rpc__h__
