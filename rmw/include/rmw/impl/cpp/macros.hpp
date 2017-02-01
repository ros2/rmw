// Copyright 2015 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__IMPL__CPP__MACROS_HPP_
#define RMW__IMPL__CPP__MACROS_HPP_

#include <iostream>
#include <sstream>
#include <string>

#include "rmw/error_handling.h"
#include "rmw/impl/config.h"  // For RMW_AVOID_MEMORY_ALLOCATION
#include "rmw/impl/cpp/demangle.hpp"  // For demangle.

// *INDENT-OFF* (prevent uncrustify from using four space indention here)
#define RMW_TRY_PLACEMENT_NEW(Destination, BufferForNew, FailureAction, Type, ...) try { \
  Destination = new(BufferForNew) Type(__VA_ARGS__); \
} catch(const std::exception & exception) { \
  RMW_SET_ERROR_MSG(( \
    std::string("caught C++ exception ") + rmw::impl::cpp::demangle(exception) + \
    " constructing " #Type ": " + exception.what() \
  ).c_str()); \
  FailureAction; \
} catch(...) { \
  RMW_SET_ERROR_MSG("caught unknown C++ exception constructing " #Type); \
  FailureAction; \
}

#define RMW_TRY_DESTRUCTOR(Statement, Type, FailureAction) try { \
  Statement; \
} catch(const std::exception & exception) { \
  RMW_SET_ERROR_MSG(( \
    std::string("caught C++ exception in destructor of " #Type ": ") + \
    rmw::impl::cpp::demangle(exception) + ": " + exception.what() \
  ).c_str()); \
  FailureAction; \
} catch(...) { \
  RMW_SET_ERROR_MSG("caught unknown C++ exception in destructor of " #Type); \
  FailureAction; \
}

#define RMW_TRY_DESTRUCTOR_FROM_WITHIN_FAILURE(Statement, Type) try { \
  Statement; \
} catch(const std::exception & exception) { \
  std::stringstream ss; \
  ss << "caught C++ exception in destructor of " #Type " while handling a failure: " \
     << rmw::impl::cpp::demangle(exception) << ": " << exception.what() \
     << ", at: " << __FILE__ << ":" << __LINE__ << '\n'; \
  (std::cerr << ss.str()).flush(); \
} catch(...) { \
  std::stringstream ss; \
  ss << "caught unknown C++ exception in destructor of " #Type \
     << " while handling a failure at: " << __FILE__ << ":" << __LINE__ << '\n'; \
  (std::cerr << ss.str()).flush(); \
}

#if RMW_AVOID_MEMORY_ALLOCATION
#define PRINT_TYPE_SUPPORT_MISMATCH() #ElementName \
  " implementation '%s'(%p) does not match type support implementation '%s'(%p)", \
  ElementTypeID, ElementTypeID, ExpectedTypeID, ExpectedTypeID

#define PRINT_RMW_IMPL_MISMATCH() #ElementName \
  " implementation '%s'(%p) does not match rmw implementation '%s'(%p)", \
  ElementTypeID, ElementTypeID, ExpectedTypeID, ExpectedTypeID

#define RMW_CHECK_TYPE_IDENTIFIERS_MATCH(ElementName, ElementTypeID, ExpectedTypeID, OnFailure) { \
  if (ElementTypeID != ExpectedTypeID) { \
    char __msg[1024]; \
    if(!strcmp(#ElementName, "type support")) { \
      snprintf( \
        __msg, 1024, \
        PRINT_TYPE_SUPPORT_MISMATCH()); \
    } else { \
      snprintf( \
        __msg, 1024, \
        PRINT_RMW_IMPL_MISMATCH()); \
    } \
    RMW_SET_ERROR_MSG(__msg); \
    OnFailure; \
  } \
}
#else  // RMW_AVOID_MEMORY_ALLOCATION
#define RMW_CHECK_TYPE_IDENTIFIERS_MATCH(ElementName, ElementTypeID, ExpectedTypeID, OnFailure) { \
  if (ElementTypeID != ExpectedTypeID) { \
    size_t __bytes_that_would_have_been_written; \
    if(!strcmp(#ElementName, "type support")) { \
      __bytes_that_would_have_been_written = snprintf( \
        NULL, 0, \
        PRINT_TYPE_SUPPORT_MISMATCH()); \
    } else { \
      __bytes_that_would_have_been_written = snprintf( \
        NULL, 0, \
        PRINT_RMW_IMPL_MISMATCH()); \
    } \
    char * __msg = \
      reinterpret_cast<char *>(rmw_allocate(__bytes_that_would_have_been_written + 1)); \
    if(!strcmp(#ElementName, "type support")) { \
      snprintf( \
        __msg, __bytes_that_would_have_been_written + 1, \
        PRINT_TYPE_SUPPORT_MISMATCH()); \
    } else{ \
      snprintf( \
        __msg, __bytes_that_would_have_been_written + 1, \
        PRINT_RMW_IMPL_MISMATCH()); \
    } \
    RMW_SET_ERROR_MSG(__msg); \
    rmw_free(__msg); \
    OnFailure; \
  } \
}
#endif  // RMW_AVOID_MEMORY_ALLOCATION
// *INDENT-ON*

#endif  // RMW__IMPL__CPP__MACROS_HPP_
