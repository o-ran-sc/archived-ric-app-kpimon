.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. SPDX-License-Identifier: CC-BY-4.0
.. Copyright (C) 2019 SAMSUNG




KPI Monitoring xAPP Overview
============================

LTE eNB and 5G NR gNB produce data that can be monitored and analyzed to measure the system's effectiveness.
The near Real-Time RIC can gather the radio and system Key Performance Indicators (KPI) from E2 Nodes and can utilize the metrics for closed-loop control.
The KPI includes but not limited following; UE measurement, E2 Node Measurement, E2 Node Load-related measurement.
The KPIMON xApp provides the first step towards the successful closed-loop control, through the monitoring E2 Node's performance metrics, and writing it into Redis DB.
With the near real-time reporting KPIs, the near Real-Time RIC can immediately control E2 Nodes.

