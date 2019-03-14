// This file was automatically generated by the Scheduler_Factory.
// Before editing the file please consider generating it again.

#include "orbsvcs/Scheduler_Factory.h"


// There were no scheduling anomalies.


static ACE_Scheduler_Factory::POD_RT_Info infos[] = {
{"Dispatching_Task-250000.us",          1,          0,          0,          0,     250000, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          1,         58,          4,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{"Dispatching_Task-500000.us",          2,          0,          0,          0,     500000, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          1,         58,          5,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{"Dispatching_Task-1000000.us",          3,          0,          0,          0,    1000000, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          1,         58,          6,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{"Dispatching_Task-2000000.us",          4,          0,          0,          0,    2000000, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          1,         58,          7,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{"Dispatching_Task-10000000.us",          5,          0,          0,          0,   10000000, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          1,         58,          8,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{  "consumer_event_1",          6,      20000,      20000,      20000,          0, (RtecScheduler::Criticality_t) 4, (RtecScheduler::Importance_t) 0,      20000,          0,         59,          0,          0, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{  "consumer_event_2",          7,      10000,      10000,      10000,          0, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,      10000,          0,         58,          1,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{"(consumer_event_1#rep||consumer_event_2#rep)",          8,          0,          0,          0,          0, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          0,         58,          3,          1, (RtecScheduler::Info_Type_t) 2 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{"consumer_event_1#rep",          9,          0,          0,          0,          0, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          0,         58,          0,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{"consumer_event_2#rep",         10,          0,          0,          0,          0, (RtecScheduler::Criticality_t) 0, (RtecScheduler::Importance_t) 0,          0,          0,         58,          2,          1, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{  "supplier_event_1",         11,          0,          0,          0,     100000, (RtecScheduler::Criticality_t) 4, (RtecScheduler::Importance_t) 0,          0,          1,         59,          1,          0, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED },
{  "supplier_event_2",         12,          0,          0,          0,     200000, (RtecScheduler::Criticality_t) 4, (RtecScheduler::Importance_t) 0,          0,          1,         59,          2,          0, (RtecScheduler::Info_Type_t) 0 , (RtecScheduler::RT_Info_Enabled_Type_t) RtecScheduler::RT_INFO_ENABLED }
};

static int infos_size = sizeof(infos)/sizeof(infos[0]);


static ACE_Scheduler_Factory::POD_Config_Info configs[] = {
  {          0,         59, (RtecScheduler::Dispatching_Type_t) 2 },
  {          1,         58, (RtecScheduler::Dispatching_Type_t) 2 }
};

static int configs_size = sizeof(configs)/sizeof(configs[0]);


// This sets up Scheduler_Factory to use the runtime version.
int scheduler_factory_setup =
  ACE_Scheduler_Factory::use_runtime (configs_size, configs, infos_size, infos);

// EOF
