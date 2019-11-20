.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. SPDX-License-Identifier: CC-BY-4.0
.. Copyright (C) 2019 SAMSUNG 

Release-Notes
=============


This document provides the release notes for O-RAN SC.

.. contents::
   :depth: 3
   :local:


Version history
---------------

+--------------------+--------------------+--------------------+--------------------+
| **Date**           | **Ver.**           | **Author**         | **Comment**        |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2019-11-20         | 1.0.0              | Md Waseem Ahamed   | First draft        |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+



Summary
-------

The Amber release of the KPI Mon xAPP supports full closed loop control 
for CuCpResourceStatusReport from multiple gNBs 
and stores the metrics in Redis DB.


Release Data
------------

+--------------------------------------+--------------------------------------+
| **Project**                          | RAN Intelligent Controller           |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Repo/commit-ID**                   |        ric-app/kpimon                |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release designation**              |              Amber                   |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release date**                     |      2019-11-20                      |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Purpose of the delivery**          | open-source xAPP for KPI Monitoring  |
|                                      |                                      |
|                                      |                                      |
+--------------------------------------+--------------------------------------+


Limitations
-----------

- The xAPP currently does not support A1 Interface, so policy updates is not implemented in the xAPP. All the GNB to be subscribed needs to be specified initially.
- The xAPP currently only supports messages from CU.
- The subscription request parameters (RAN Function ID etc) cannot be changed.
