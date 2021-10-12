// Copyright 2021 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__SUBSCRIPTION_CONTENT_FILTERED_TOPIC_OPTIONS_H_
#define RMW__SUBSCRIPTION_CONTENT_FILTERED_TOPIC_OPTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"
#include "rcutils/types.h"
#include "rmw/macros.h"
#include "rmw/ret_types.h"
#include "rmw/visibility_control.h"

typedef struct RMW_PUBLIC_TYPE rmw_subscription_content_filtered_topic_options_s
{
  /**
   * specify the criteria to select the data samples of interest.
   *
   * It is similar to the WHERE part of an SQL clause.
   */
  char * filter_expression;
  /**
   * give values to the tokens placeholder ‘parameters’ (i.e., "%n" tokens begin from 0) in the
   * filter_expression. The number of supplied parameters must fit with the requested values.
   *
   * It can be NULL if there is no "%n" tokens placeholder in filter_expression.
   * The maximun size allowance depends on concrete DDS vendor.
   * (i.e., it cannot be greater than 100 on RTI_Connext.)
   */
  rcutils_string_array_t * expression_parameters;
} rmw_subscription_content_filtered_topic_options_t;


/// Get zero initialized content filtered topic options.
RMW_PUBLIC
rmw_subscription_content_filtered_topic_options_t
rmw_get_zero_initialized_content_filtered_topic_options();


/// Initialize the given content filtered topic options.
/**
 * \param[in] filter_expression The filter expression.
 * \param[in] expression_parameters_argc The expression parameters argc.
 * \param[in] expression_parameter_argv The expression parameters argv.
 * \param[in] allocator The allocator used when copying data to the content filtered topic options.
 * \param[out] options The content filtered topic options to be set.
 * \returns RMW_RET_INVALID_ARGUMENT, or
 * \returns RMW_RET_BAD_ALLOC, or
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t
rmw_subscription_content_filtered_topic_options_init(
  const char * filter_expression,
  size_t expression_parameters_argc,
  const char * expression_parameter_argv[],
  const rcutils_allocator_t * allocator,
  rmw_subscription_content_filtered_topic_options_t * options);

/// Set the given content filtered topic options.
/**
 * \param[in] filter_expression The filter expression.
 * \param[in] expression_parameters_argc The expression parameters argc.
 * \param[in] expression_parameter_argv The expression parameters argv.
 * \param[in] allocator The allocator used when copying data to the content filtered topic options.
 * \param[out] options The content filtered topic options to be set.
 * \returns RMW_RET_INVALID_ARGUMENT, or
 * \returns RMW_RET_BAD_ALLOC, or
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t
rmw_subscription_content_filtered_topic_options_set(
  const char * filter_expression,
  size_t expression_parameters_argc,
  const char * expression_parameter_argv[],
  const rcutils_allocator_t * allocator,
  rmw_subscription_content_filtered_topic_options_t * options);

/// Copy the given content filtered topic options.
/**
 * \param[in] src content filtered topic options to be copied.
 * \param[in] allocator allocator used when copying data to the new content filtered topic options.
 * \param[out] dst content filtered topic options to be set.
 * \returns RMW_RET_INVALID_ARGUMENT, or
 * \returns RMW_RET_BAD_ALLOC, or
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t
rmw_subscription_content_filtered_topic_options_copy(
  const rmw_subscription_content_filtered_topic_options_t * src,
  const rcutils_allocator_t * allocator,
  rmw_subscription_content_filtered_topic_options_t * dst);


/// Finalize the content filtered topic options.
/**
 * \param[in] options content filtered topic options to be finalized.
 * \param[in] allocator allocator used to deallocate the content filtered topic options.
 * \returns RMW_RET_INVALID_ARGUMENT, or
 * \returns RMW_RET_ERROR, or
 * \returns RMW_RET_OK
 */
RMW_PUBLIC
rmw_ret_t
rmw_subscription_content_filtered_topic_options_fini(
  rmw_subscription_content_filtered_topic_options_t * options,
  const rcutils_allocator_t * allocator);

#ifdef __cplusplus
}
#endif

#endif  // RMW__SUBSCRIPTION_CONTENT_FILTERED_TOPIC_OPTIONS_H_
