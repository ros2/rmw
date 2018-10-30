//
// Created by Ross Desmond on 10/30/18.
//

#ifndef RMW_GET_NODE_INFO_AND_TYPES_H
#define RMW_GET_NODE_INFO_AND_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "rmw/macros.h"
#include "rmw/names_and_types.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Return a list of subscribed topic names and their types.
/**
 * This function returns a list of subscribed topic names and their types.
 *
 * The node parameter must not be `NULL`, and must point to a valid node.
 *
 * The topic_names_and_types parameter must be allocated and zero initialized.
 * The topic_names_and_types is the output for this function, and contains
 * allocated memory.
 * Therefore, it should be passed to rmw_names_and_types_fini() when
 * it is no longer needed.
 * Failing to do so will result in leaked memory.
 *
 * \param[in] node the handle to the node being used to query the ROS graph
 * \param[in] allocator allocator to be used when allocating space for strings
 * \param[in] node_name the name of the node to get information for
 * \param[out] topic_names_and_types list of topic names and their types the node_name is subscribed to
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if the node is invalid, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_subscriber_names_and_types_by_node(
        const rmw_node_t * node,
        rcutils_allocator_t * allocator,
        const char * node_name,
        bool no_demangle,
        rmw_names_and_types_t * topics_names_and_types);

/// Return a list of published topic names and their types.
/**
 * This function returns a list of published topic names and their types.
 *
 * The node parameter must not be `NULL`, and must point to a valid node.
 *
 * The topic_names_and_types parameter must be allocated and zero initialized.
 * The topic_names_and_types is the output for this function, and contains
 * allocated memory.
 * Therefore, it should be passed to rmw_names_and_types_fini() when
 * it is no longer needed.
 * Failing to do so will result in leaked memory.
 *
 * \param[in] node the handle to the node being used to query the ROS graph
 * \param[in] allocator allocator to be used when allocating space for strings
 * \param[in] node_name the name of the node to get information for
 * \param[out] topic_names_and_types list of topic names and their types the node_name is publishing
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if the node is invalid, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_publisher_names_and_types_by_node(
        const rmw_node_t * node,
        rcutils_allocator_t * allocator,
        const char * node_name,
        bool no_demangle,
        rmw_names_and_types_t * topic_names_and_types);

/// Return a list of service names and their types.
/**
 * This function returns a list of service topic names and their types.
 *
 * The node parameter must not be `NULL`, and must point to a valid node.
 *
 * The service_names_and_types parameter must be allocated and zero initialized.
 * The service_names_and_types is the output for this function, and contains
 * allocated memory.
 * Therefore, it should be passed to rmw_names_and_types_fini() when
 * it is no longer needed.
 * Failing to do so will result in leaked memory.
 *
 * \param[in] node the handle to the node being used to query the ROS graph
 * \param[in] allocator allocator to be used when allocating space for strings
 * \param[in] node_name the name of the node to get information for
 * \param[out] service_names_and_types list of service names and their types the node_name has created a service for
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if the node is invalid, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_service_names_and_types_by_node(
        const rmw_node_t * node,
        rcutils_allocator_t * allocator,
        const char * node_name,
        rmw_names_and_types_t * service_names_and_types);

#ifdef __cplusplus
}
#endif
#endif //RMW_GET_NODE_INFO_AND_TYPES_H
