^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package rmw
^^^^^^^^^^^^^^^^^^^^^^^^^

0.8.0 (2019-09-24)
------------------
* Added specific return type for non existent node (`#182 <https://github.com/ros2/rmw/issues/182>`_)
* Added function for getting clients by node (`#179 <https://github.com/ros2/rmw/issues/179>`_)
* Added get_actual_qos() feature to subscriptions (`#177 <https://github.com/ros2/rmw/issues/177>`_)
* Added ``RMW_QOS_POLICY_LIVELINESS_UNKNOWN`` enum (`#175 <https://github.com/ros2/rmw/issues/175>`_)
* Contributors: Jacob Perron, M. M, ivanpauno

0.7.1 (2019-05-08)
------------------

* Implement QoS: liveliness, deadline, lifespan (`#171 <https://github.com/ros2/rmw/issues/171>`_)
* Rmw preallocate (`#160 <https://github.com/ros2/rmw/issues/160>`_)
* Add new QoS policy data types to rmw (`#173 <https://github.com/ros2/rmw/issues/173>`_)
* Contributors: M. M, Michael Carroll, Ross Desmond

0.7.0 (2019-04-13)
------------------
* Add function to get publisher actual qos settings (`#169 <https://github.com/ros2/rmw/issues/169>`_)
* fix checking boolean variable which might contain a string (`#165 <https://github.com/ros2/rmw/issues/165>`_)
* change parameter events to use KEEP_LAST (`#162 <https://github.com/ros2/rmw/issues/162>`_)
* Fix typo error (`#164 <https://github.com/ros2/rmw/issues/164>`_)
* pass context to wait set, and provide fini function for context (`#163 <https://github.com/ros2/rmw/issues/163>`_)
* minor notes explaining rmw_qos_profile_t (`#156 <https://github.com/ros2/rmw/issues/156>`_)
* Contributors: Dirk Thomas, Lalit Begani, Mike Lautman, William Woodall, ivanpauno

0.6.1 (2018-12-06)
------------------
* Add node graph functions (`#158 <https://github.com/ros2/rmw/issues/158>`_)
* refactor init to allow options to be passed and to not be global (`#154 <https://github.com/ros2/rmw/issues/154>`_)
* Methods to retrieve matched counts on publisher and subscriber (`#155 <https://github.com/ros2/rmw/issues/155>`_)
* use uint8_t instead of char for serialized message (`#161 <https://github.com/ros2/rmw/issues/161>`_)
* Contributors: Karsten Knese, Michael Carroll, Ross Desmond, William Woodall

0.6.0 (2018-11-16)
------------------
* use new error handling API (`#153 <https://github.com/ros2/rmw/issues/153>`_)
* Add semicolons to RCLCPP and RCUTILS macros. (`#150 <https://github.com/ros2/rmw/issues/150>`_)
* Include node namespaces in get_node_names() (`#148 <https://github.com/ros2/rmw/issues/148>`_)
* add missing doc for parameter (`#149 <https://github.com/ros2/rmw/issues/149>`_)
* rcutils_serialized_message -> rcutils_char_array (`#146 <https://github.com/ros2/rmw/issues/146>`_)
* rmw serialized to rcutils serialized (`#145 <https://github.com/ros2/rmw/issues/145>`_)
* rcutil -> rcutils (`#147 <https://github.com/ros2/rmw/issues/147>`_)
* get serialization format (`#143 <https://github.com/ros2/rmw/issues/143>`_)
* Contributors: Chris Lalancette, Karsten Knese, Michael Carroll, Mikael Arguedas, William Woodall

0.5.0 (2018-06-23)
------------------
* use rcutils allocator in allocators.c to avoid direct use of malloc/free (`#140 <https://github.com/ros2/rmw/issues/140>`_)
* check the return value of rcutils_snprintf (`#138 <https://github.com/ros2/rmw/issues/138>`_)
* _raw function (`#125 <https://github.com/ros2/rmw/issues/125>`_)
* Merge pull request `#137 <https://github.com/ros2/rmw/issues/137>`_ from ros2/misra_fixup
* Change #if to #ifdef.
* add function to parse key-value user_data (`#132 <https://github.com/ros2/rmw/issues/132>`_)
* Add validation functions accepting string length (`#135 <https://github.com/ros2/rmw/issues/135>`_)
* Clarify that NULL blocks forever in rmw_wait (`#134 <https://github.com/ros2/rmw/issues/134>`_)
* Enable setting log levels in DDS implementation (`#124 <https://github.com/ros2/rmw/issues/124>`_)
* Optimize namespace node and topic validation (`#130 <https://github.com/ros2/rmw/issues/130>`_)
* Contributors: Dirk Thomas, Ethan Gao, Karsten Knese, Michael Carroll, Shane Loretz, Sriram Raghunathan, William Woodall
