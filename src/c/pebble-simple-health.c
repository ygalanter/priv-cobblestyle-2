#include <pebble.h>
#include <pebble-events/pebble-events.h>
#include "pebble-simple-health.h"

//external callback to raise
health_callback *s_callback_proc;

// to determine if health services are available
static bool s_health_available;
static EventHandle s_health_event;

bool health_is_available() {
  return s_health_available;
}


// getting requested metric goal/average
int health_get_metric_goal(HealthMetric metric) {
  
  const time_t start = time_start_of_today();
  const time_t end = start + SECONDS_PER_DAY;
  
  HealthServiceAccessibilityMask mask = health_service_metric_averaged_accessible(metric, start, end, HealthServiceTimeScopeDaily);
  
  if(mask == HealthServiceAccessibilityMaskAvailable) {
    return (int)health_service_sum_averaged(metric, start, end, HealthServiceTimeScopeDaily);
  } else {
    return 0;
  }

}


// getting requested metric value
int health_get_metric_sum(HealthMetric metric) {
  
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, time_start_of_today(), time(NULL));
  
  if(mask == HealthServiceAccessibilityMaskAvailable) {
    return (int)health_service_sum_today(metric);
  } else {
    return 0;
  }
}

// raising event in main code to retreive health data
static void health_handler(HealthEventType event, void *context) {
   s_callback_proc();
}


// initializing health services
void health_init(health_callback *callback_proc) {
  #ifdef PBL_HEALTH
    s_health_event = events_health_service_events_subscribe(health_handler, NULL);
    if (s_health_event)  {
       s_callback_proc = callback_proc;
       s_health_available = true;
    } else {
       s_health_available = false;
    }
  #else
    s_health_available = false;
  #endif
}

// unsubscribing from health services
void health_deinit(){
  #ifdef PBL_HEALTH
    events_health_service_events_unsubscribe(s_health_event);
  #endif
}


