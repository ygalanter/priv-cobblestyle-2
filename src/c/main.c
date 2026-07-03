#include <pebble.h>
#include "main.h"
#include "pebble-events/pebble-events.h"


#ifndef PBL_PLATFORM_APLITE
  #include "pebble-simple-health/pebble-simple-health.h"
#endif

#include "watchface.h"
#include "common.h"

uint_least8_t TIME_DISPLAY, FLAG_TEMPERATURE_FORMAT, FLAG_WEATHER_INTERVAL, FLAG_LOCATION_SERVICE, FLAG_HOURS_MINUTES_SEPARATOR;
uint_least8_t global_battery_percent, FLAG_SECONDARY_INFO_2, FLAG_SECONDARY_INFO_5, FLAG_BLUETOOTH_ALERT, FLAG_BACKLIGHT_WHILE_CHARGING, FLAG_SHOW_ANALOG_SECONDS;
#ifdef PBL_RECT
  uint_least8_t FLAG_SIDEBAR_LOCATION;
#else
  uint_least8_t FLAG_SECONDARY_INFO_7, FLAG_SECONDARY_INFO_8, FLAG_SECONDARY_INFO_9, FLAG_SECONDARY_INFO_10;
#endif
int_least16_t temp_kelvin, temp_celcius, temp_fahrenheit;
int_least32_t COORDINATES_LATITUDE, COORDINATES_LONGITUDE;


#ifndef PBL_PLATFORM_APLITE
  uint_least8_t  FLAG_GRAPHICAL_STEP_GOAL, FLAG_SECONDARY_INFO_1, FLAG_SECONDARY_INFO_3, FLAG_SECONDARY_INFO_4, FLAG_SECONDARY_INFO_6, FLAG_LANGUAGE;
  int_least16_t ALT_TIMEZONE_OFFSET;
  uint_least16_t health_steps, health_step_goal, health_distance, health_time_active, health_calories_rest, health_calories_active, CUSTOM_STEP_GOAL;
  #if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
  uint_least32_t health_heart_rate;
  #endif
  int_least32_t  PRIMARY_COLOR, SECONDARY_COLOR, BACK_COLOR, ICON_COLOR;
  char RANDOM_TEXT[22];
  char ALT_TIMEZONE_NAME[4];
#endif

char LOCATION_NAME[22];

FPath *weather_icon, *bluetooth_icon;
extern FFont *ffont;
Window *s_window;
Layer *s_main_layer;
uint_least8_t  *buffer, *buffer_weather, *buffer_bluetooth;
GBitmap *fctx_buffer;

EventHandle my_time, my_battery, my_connection, my_appmesages;

bool inside_init = true;

// Compact persisted weather snapshot (replaces the old GenericWeatherInfo blob).
typedef struct {
  int_least16_t temp_celcius;
  uint_least8_t condition;
  uint_least8_t is_day;
  char name[22];
} WeatherData;

// even called when new health data ready
#ifndef PBL_PLATFORM_APLITE
void health_metrics_update(){

  health_steps =  health_get_metric_sum(HealthMetricStepCount);
  health_step_goal = CUSTOM_STEP_GOAL == 0? health_get_metric_goal(HealthMetricStepCount) : CUSTOM_STEP_GOAL;
  health_distance = health_get_metric_sum(HealthMetricWalkedDistanceMeters);
  health_time_active = health_get_metric_sum(HealthMetricActiveSeconds);
  health_calories_rest = health_get_metric_sum(HealthMetricRestingKCalories);
  health_calories_active = health_get_metric_sum(HealthMetricActiveKCalories);
  
  #if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
     health_heart_rate = health_service_peek_current_value(HealthMetricHeartRateBPM);
  #endif

}
#endif


// loading weather icon depending on conditions
void set_weather_icon(uint_least8_t condition_code, bool is_day) {
  uint_least32_t resource_id;

    switch (condition_code){
      case WEATHER_COND_CLEAR_SKY:
        resource_id = is_day? RESOURCE_ID_WEATHER_ICON_CLEAR_SKY_DAY : RESOURCE_ID_WEATHER_ICON_CLEAR_SKY_NIGHT;
        break;
      case WEATHER_COND_FEW_CLOUDS:
        resource_id = is_day? RESOURCE_ID_WEATHER_ICON_FEW_CLOUDS_DAY : RESOURCE_ID_WEATHER_ICON_FEW_CLOUDS_NIGHT;
        break;
      case WEATHER_COND_SCATTERED_CLOUDS:
        resource_id = RESOURCE_ID_WEATHER_ICON_SCATTERED_CLOUDS;
        break;
      case WEATHER_COND_BROKEN_CLOUDS:
        resource_id = RESOURCE_ID_WEATHER_ICON_BROKEN_CLOUDS;
        break;
      case WEATHER_COND_SHOWER_RAIN:
        resource_id = RESOURCE_ID_WEATHER_ICON_SHOWER_RAIN;
        break;
      case WEATHER_COND_RAIN:
        resource_id = RESOURCE_ID_WEATHER_ICON_RAIN;
        break;
      case WEATHER_COND_THUNDERSTORM:
        resource_id = RESOURCE_ID_WEATHER_ICON_THUNDERSTORM;
        break;
      case WEATHER_COND_SNOW:
        resource_id = RESOURCE_ID_WEATHER_ICON_SNOW;
        break;
      case WEATHER_COND_MIST:
        resource_id = RESOURCE_ID_WEATHER_ICON_MIST;
        break;
      case WEATHER_COND_UNKNOWN:
      default:
        resource_id = RESOURCE_ID_WEATHER_ICON_UNKNOWN;
        break;
    }

    weather_icon = fpath_create_from_resource_with_buffer(resource_id, buffer_weather);

}

// maps an Open-Meteo WMO weather code to our WEATHER_COND_* condition
static uint_least8_t wmo_to_condition(int wmo) {
  switch (wmo) {
    case 0:  return WEATHER_COND_CLEAR_SKY;
    case 1:
    case 2:  return WEATHER_COND_FEW_CLOUDS;
    case 3:  return WEATHER_COND_BROKEN_CLOUDS;
    case 45:
    case 48: return WEATHER_COND_MIST;
    case 51: case 53: case 55:
    case 56: case 57: return WEATHER_COND_SHOWER_RAIN;
    case 61: case 63: case 65:
    case 66: case 67:
    case 80: case 81: case 82: return WEATHER_COND_RAIN;
    case 71: case 73: case 75: case 77:
    case 85: case 86: return WEATHER_COND_SNOW;
    case 95: case 96: case 99: return WEATHER_COND_THUNDERSTORM;
    default: return WEATHER_COND_UNKNOWN;
  }
}

// stores decoded weather into local variables (temp in Celsius; F & K derived)
void copy_weather(int_least16_t temp_c, uint_least8_t condition, bool is_day, const char *name) {
  temp_celcius = temp_c;
  temp_fahrenheit = temp_c * 9 / 5 + 32;
  temp_kelvin = temp_c + 273;

  //YG 2916-07-31: To avoid overflow of LOCATION variable and crash
  if (strlen(name) < 20) {
    strcpy(LOCATION_NAME, name);
  } else {
    strncpy(LOCATION_NAME, name, 20);
    LOCATION_NAME[20] = 0;
  }

  set_weather_icon(condition, is_day);
}

// displays a status icon for a non-available weather result
static void set_weather_status_icon(uint_least8_t status) {
  uint_least32_t resource_id;
  switch (status) {
    case WEATHER_STATUS_NOT_YET_FETCHED:
      resource_id = RESOURCE_ID_WEATHER_ICON_NOT_YET_FETCHED;
      break;
    case WEATHER_STATUS_FAILED:
      resource_id = RESOURCE_ID_WEATHER_ICON_FAILED;
      break;
    default: // WEATHER_STATUS_LOCATION_UNAVAILABLE
      resource_id = RESOURCE_ID_WEATHER_ICON_LOCATION_UNAVAILABLE;
      break;
  }
  weather_icon = fpath_create_from_resource_with_buffer(resource_id, buffer_weather);
}





// battery state handling procedure
void battery_handler(BatteryChargeState state) {
  
  global_battery_percent = state.charge_percent;
  
  light_enable(state.is_charging && FLAG_BACKLIGHT_WHILE_CHARGING == 1);
  
  layer_mark_dirty(s_main_layer);
  
}

// asks the phone (pkjs) to fetch fresh weather, passing the location settings
// (mode + manual coordinates, degrees x100000) so JS can query Open-Meteo/nominatim
void get_weather() {
  DictionaryIterator *iter;
  if (app_message_outbox_begin(&iter) != APP_MSG_OK) return;
  dict_write_uint8(iter, MESSAGE_KEY_WEATHER_REQUEST, 1);
  dict_write_uint8(iter, MESSAGE_KEY_LOCATION_SERVICE, FLAG_LOCATION_SERVICE);
  dict_write_int32(iter, MESSAGE_KEY_LOCATION_LATITUDE, COORDINATES_LATITUDE);
  dict_write_int32(iter, MESSAGE_KEY_LOCATION_LONGITUDE, COORDINATES_LONGITUDE);
  app_message_outbox_send();
}


//connection state handling procedure
void connection_handler(bool state) {
  
  
   if (state) {
     bluetooth_icon = fpath_create_from_resource_with_buffer(RESOURCE_ID_BLUETOOTH_ICON_CONNECTED, buffer_bluetooth);
   } else {
     bluetooth_icon = fpath_create_from_resource_with_buffer(RESOURCE_ID_BLUETOOTH_ICON_DISCONNECTED, buffer_bluetooth);
   }
  
 layer_mark_dirty(s_main_layer);
  
 if (inside_init) return; //if it's initial call - don't buzz
  
 switch (FLAG_BLUETOOTH_ALERT){
    case BLUETOOTH_ALERT_WEAK:
      vibes_enqueue_custom_pattern(VIBE_PATTERN_WEAK);
      break;
    case BLUETOOTH_ALERT_NORMAL:
      vibes_enqueue_custom_pattern(VIBE_PATTERN_NORMAL);
      break;
    case BLUETOOTH_ALERT_STRONG:
    vibes_enqueue_custom_pattern(VIBE_PATTERN_STRONG);
      break;
    case BLUETOOTH_ALERT_DOUBLE:
      vibes_enqueue_custom_pattern(VIBE_PATTERN_DOUBLE);
      break;    
  }
  
}


// time tick handling procedure
void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
   
  // only getting weather at givinen intervals
  if (!(tick_time->tm_min % FLAG_WEATHER_INTERVAL) && (tick_time->tm_sec == 0)){
     get_weather();
  }
  
  // timer ticks every second 
  if (tick_time->tm_sec == 0 || FLAG_SHOW_ANALOG_SECONDS == 1
        #ifndef PBL_PLATFORM_APLITE
          // OR flags to display seconds is set
          || FLAG_SECONDARY_INFO_2 == SECONDARY_INFO_SECONDS|| FLAG_SECONDARY_INFO_1 == SECONDARY_INFO_SECONDS||  FLAG_SECONDARY_INFO_3 == SECONDARY_INFO_SECONDS
          || FLAG_SECONDARY_INFO_4 == SECONDARY_INFO_SECONDS || FLAG_SECONDARY_INFO_5 == SECONDARY_INFO_SECONDS|| FLAG_SECONDARY_INFO_6 == SECONDARY_INFO_SECONDS
           #ifdef PBL_ROUND
          || FLAG_SECONDARY_INFO_9 == SECONDARY_INFO_SECONDS|| FLAG_SECONDARY_INFO_10 == SECONDARY_INFO_SECONDS
          #endif
        #endif
     ){
    
      // and marking layer dirty to display it
      layer_mark_dirty(s_main_layer);
  }


}


// true when any active element needs a per-second redraw (second hand or a
// "Seconds" widget). Used to avoid the battery cost of a per-second tick.
static bool seconds_needed() {
  if (FLAG_SHOW_ANALOG_SECONDS == 1) return true;
  #ifndef PBL_PLATFORM_APLITE
    if (FLAG_SECONDARY_INFO_1 == SECONDARY_INFO_SECONDS || FLAG_SECONDARY_INFO_2 == SECONDARY_INFO_SECONDS
     || FLAG_SECONDARY_INFO_3 == SECONDARY_INFO_SECONDS || FLAG_SECONDARY_INFO_4 == SECONDARY_INFO_SECONDS
     || FLAG_SECONDARY_INFO_5 == SECONDARY_INFO_SECONDS || FLAG_SECONDARY_INFO_6 == SECONDARY_INFO_SECONDS
     #ifdef PBL_ROUND
      || FLAG_SECONDARY_INFO_9 == SECONDARY_INFO_SECONDS || FLAG_SECONDARY_INFO_10 == SECONDARY_INFO_SECONDS
     #endif
    ) return true;
  #endif
  return false;
}

// subscribes to SECOND_UNIT only while seconds are actually displayed, otherwise
// MINUTE_UNIT -- saving battery. Safe to call repeatedly (e.g. after settings change).
static TimeUnits s_tick_units = MINUTE_UNIT;
static void update_tick_subscription() {
  TimeUnits needed = seconds_needed() ? SECOND_UNIT : MINUTE_UNIT;
  if (my_time && needed == s_tick_units) return;
  if (my_time) events_tick_timer_service_unsubscribe(my_time);
  s_tick_units = needed;
  my_time = events_tick_timer_service_subscribe(s_tick_units, tick_handler);
}


#if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
// Enables 1Hz heart-rate sampling only while a "Heart Rate" widget is actually
// shown; otherwise turns the HRM sensor off (period 0) to save battery.
static bool heart_rate_needed() {
  return FLAG_SECONDARY_INFO_1 == SECONDARY_INFO_HEART_RATE || FLAG_SECONDARY_INFO_2 == SECONDARY_INFO_HEART_RATE
      || FLAG_SECONDARY_INFO_3 == SECONDARY_INFO_HEART_RATE || FLAG_SECONDARY_INFO_4 == SECONDARY_INFO_HEART_RATE
      || FLAG_SECONDARY_INFO_5 == SECONDARY_INFO_HEART_RATE || FLAG_SECONDARY_INFO_6 == SECONDARY_INFO_HEART_RATE;
}
static void update_heart_rate_sampling() {
  health_service_set_heart_rate_sample_period(heart_rate_needed() ? 1 : 0);
}
#endif


bool need_update;

int_least32_t get_int_from_inbox(DictionaryIterator *iterator, uint_least32_t msg_key, int_least32_t original_value) {
  
   int_least32_t value = original_value;
   Tuple *t = dict_find(iterator, msg_key); 
  
   if(t) { 
         value = atol(t->value->cstring); 
         persist_write_int(msg_key, value);
         need_update = true;
    }
  
  return value;
  
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  
  
  Tuple *t;

  // ---- weather response from the phone (self-contained Open-Meteo / nominatim) ----
  t = dict_find(iterator, MESSAGE_KEY_WEATHER_STATUS);
  if (t) {
    uint_least8_t status = t->value->uint8;
    if (status == WEATHER_STATUS_AVAILABLE) {
      Tuple *t_temp = dict_find(iterator, MESSAGE_KEY_WEATHER_TEMP);
      Tuple *t_code = dict_find(iterator, MESSAGE_KEY_WEATHER_CODE);
      Tuple *t_day  = dict_find(iterator, MESSAGE_KEY_WEATHER_IS_DAY);
      Tuple *t_city = dict_find(iterator, MESSAGE_KEY_WEATHER_CITY);

      int_least16_t temp_c = t_temp ? t_temp->value->int32 : 0;
      uint_least8_t cond = wmo_to_condition(t_code ? t_code->value->int32 : -1);
      bool is_day = t_day ? (t_day->value->int32 != 0) : true;
      const char *city = t_city ? t_city->value->cstring : "";

      copy_weather(temp_c, cond, is_day, city); // store + build icon

      WeatherData wd = {.temp_celcius = temp_c, .condition = cond, .is_day = is_day ? 1 : 0};
      strncpy(wd.name, LOCATION_NAME, sizeof(wd.name)); wd.name[sizeof(wd.name) - 1] = 0;
      persist_write_data(MESSAGE_KEY_WEATHER_STORAGE, &wd, sizeof(wd)); // persist it
    } else {
      temp_kelvin = WEATHER_NO_DATA;
      set_weather_status_icon(status);
    }
    layer_mark_dirty(s_main_layer);
    return;
  }

  t = dict_find(iterator, MESSAGE_KEY_JSREADY);
  if(t) { // if JS ready - call weather
    get_weather();

  } else { // otherwise check the rest of the keys

      need_update = false;

      FLAG_LOCATION_SERVICE = get_int_from_inbox(iterator, MESSAGE_KEY_LOCATION_SERVICE, FLAG_LOCATION_SERVICE);
      COORDINATES_LATITUDE = get_int_from_inbox(iterator, MESSAGE_KEY_LOCATION_LATITUDE, COORDINATES_LATITUDE);
      COORDINATES_LONGITUDE = get_int_from_inbox(iterator, MESSAGE_KEY_LOCATION_LONGITUDE, COORDINATES_LONGITUDE);
      TIME_DISPLAY = get_int_from_inbox(iterator, MESSAGE_KEY_TIME_DISPLAY, TIME_DISPLAY);

      FLAG_TEMPERATURE_FORMAT = get_int_from_inbox(iterator, MESSAGE_KEY_TEMPERATURE_FORMAT, FLAG_TEMPERATURE_FORMAT);
      FLAG_WEATHER_INTERVAL = get_int_from_inbox(iterator, MESSAGE_KEY_WEATHER_INTERVAL, FLAG_WEATHER_INTERVAL); 
      #ifdef PBL_RECT
      FLAG_SIDEBAR_LOCATION = get_int_from_inbox(iterator, MESSAGE_KEY_SIDEBAR_LOCATION, FLAG_SIDEBAR_LOCATION);
      #endif
      FLAG_BLUETOOTH_ALERT = get_int_from_inbox(iterator, MESSAGE_KEY_BLUETOOTH_ALERT, FLAG_BLUETOOTH_ALERT);
    
    
      t = dict_find(iterator, MESSAGE_KEY_SHOW_ANALOG_SECONDS); 
          if(t) { // show second hand
             FLAG_SHOW_ANALOG_SECONDS = t->value->uint8;
             persist_write_int(MESSAGE_KEY_SHOW_ANALOG_SECONDS, FLAG_SHOW_ANALOG_SECONDS);
             need_update = true;
      }
    
      t = dict_find(iterator, MESSAGE_KEY_HOURS_MINUTES_SEPARATOR); 
          if(t) { // show second hand
             FLAG_HOURS_MINUTES_SEPARATOR = t->value->uint8;
             persist_write_int(MESSAGE_KEY_HOURS_MINUTES_SEPARATOR, FLAG_HOURS_MINUTES_SEPARATOR);
             need_update = true;
      }
    
      t = dict_find(iterator, MESSAGE_KEY_BACKLIGHT_WHILE_CHARGING); 
          if(t) { // show second hand
             FLAG_BACKLIGHT_WHILE_CHARGING = t->value->uint8;
             persist_write_int(MESSAGE_KEY_BACKLIGHT_WHILE_CHARGING, FLAG_BACKLIGHT_WHILE_CHARGING);
             need_update = true;
      }
    
    
      #ifndef PBL_PLATFORM_APLITE
          FLAG_LANGUAGE = get_int_from_inbox(iterator, MESSAGE_KEY_LANGUAGE, FLAG_LANGUAGE);
          FLAG_SECONDARY_INFO_1 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_1, FLAG_SECONDARY_INFO_1); 
          FLAG_SECONDARY_INFO_2 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_2, FLAG_SECONDARY_INFO_2); 
          FLAG_SECONDARY_INFO_3 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_3, FLAG_SECONDARY_INFO_3); 
          FLAG_SECONDARY_INFO_4 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_4, FLAG_SECONDARY_INFO_4); 
          FLAG_SECONDARY_INFO_5 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_5, FLAG_SECONDARY_INFO_5); 
          FLAG_SECONDARY_INFO_6 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_6, FLAG_SECONDARY_INFO_6); 
          #ifdef PBL_ROUND
          FLAG_SECONDARY_INFO_7 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_7, FLAG_SECONDARY_INFO_7); 
          FLAG_SECONDARY_INFO_8 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_8, FLAG_SECONDARY_INFO_8); 
          FLAG_SECONDARY_INFO_9 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_9, FLAG_SECONDARY_INFO_9); 
          FLAG_SECONDARY_INFO_10 = get_int_from_inbox(iterator, MESSAGE_KEY_SECONDARY_INFO_10, FLAG_SECONDARY_INFO_10); 
          #endif
          ALT_TIMEZONE_OFFSET = get_int_from_inbox(iterator, MESSAGE_KEY_ALT_TIMEZONE_OFFSET, ALT_TIMEZONE_OFFSET); 
          CUSTOM_STEP_GOAL = get_int_from_inbox(iterator, MESSAGE_KEY_CUSTOM_STEP_GOAL, CUSTOM_STEP_GOAL); 
        
          t = dict_find(iterator, MESSAGE_KEY_PRIMARY_COLOR); 
          if(t) { // show graphical step goal or not
             PRIMARY_COLOR = t->value->int32;
             persist_write_int(MESSAGE_KEY_PRIMARY_COLOR, PRIMARY_COLOR);
             need_update = true;
          }
        
           t = dict_find(iterator, MESSAGE_KEY_SECONDARY_COLOR); 
          if(t) { // show graphical step goal or not
             SECONDARY_COLOR = t->value->int32;
             persist_write_int(MESSAGE_KEY_SECONDARY_COLOR, SECONDARY_COLOR);
             need_update = true;
          }
    
          t = dict_find(iterator, MESSAGE_KEY_BACK_COLOR); 
          if(t) { // show graphical step goal or not
             BACK_COLOR = t->value->int32;
             persist_write_int(MESSAGE_KEY_BACK_COLOR, BACK_COLOR);
             need_update = true;
          }
        
           t = dict_find(iterator, MESSAGE_KEY_ICON_COLOR); 
          if(t) { // show graphical step goal or not
             ICON_COLOR = t->value->int32;
             persist_write_int(MESSAGE_KEY_ICON_COLOR, ICON_COLOR);
             need_update = true;
          }
        
        
         t = dict_find(iterator, MESSAGE_KEY_GRAPHICAL_STEP_GOAL); 
          if(t) { // show graphical step goal or not
             FLAG_GRAPHICAL_STEP_GOAL = t->value->uint8;
             persist_write_int(MESSAGE_KEY_GRAPHICAL_STEP_GOAL, FLAG_GRAPHICAL_STEP_GOAL);
             need_update = true;
          }
        
         
          t = dict_find(iterator, MESSAGE_KEY_RANDOM_TEXT); 
          if(t) { // Custom Text
             strcpy(RANDOM_TEXT, t->value->cstring);  
             persist_write_string(MESSAGE_KEY_RANDOM_TEXT, RANDOM_TEXT);
             need_update = true;
           }
         
        
          t = dict_find(iterator, MESSAGE_KEY_ALT_TIMEZONE_NAME); 
          if(t) { 
             strcpy(ALT_TIMEZONE_NAME, t->value->cstring);  
             persist_write_string(MESSAGE_KEY_ALT_TIMEZONE_NAME, ALT_TIMEZONE_NAME);
             need_update = true;
          }
        
    
    #endif
    
    
    if (need_update) {
      #ifndef PBL_PLATFORM_APLITE
        set_language(FLAG_LANGUAGE);
        health_metrics_update();
        window_set_background_color(s_window, GColorFromHEX(BACK_COLOR));
        #if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
        update_heart_rate_sampling(); // HR widget may have been toggled
        #endif
      #endif
      update_tick_subscription(); // seconds widget/hand may have been toggled
      layer_mark_dirty(s_main_layer);
      get_weather();
    }
     
  }
 
}


static void main_update_proc(Layer *layer, GContext *ctx) {
  // getting current time
  time_t temp = time(NULL);
  struct tm *global_date_time = localtime(&temp);

  draw_data(layer, ctx, global_date_time); // drawing time, battery, health
  draw_graphics(layer, ctx, global_date_time); // drawing graphics
  
  

}

static void window_load(Window *window) {
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);


  // FCTX rasterization buffer, matched to each platform's framebuffer:
  //   1-bit for B&W (aplite/diorite/flint), circular 8-bit for round (chalk/gabbro),
  //   plain 8-bit for color rect (basalt/emery).
  #if defined(PBL_BW)
    fctx_buffer = gbitmap_create_blank(bounds.size, GBitmapFormat1Bit);
  #elif defined(PBL_ROUND)
    fctx_buffer = gbitmap_create_blank(bounds.size, GBitmapFormat8BitCircular);
  #else
    fctx_buffer = gbitmap_create_blank(bounds.size, GBitmapFormat8Bit);
  #endif
  
  //pre-allocating ram for fonts and icons
  buffer = malloc(FONT_BUFFER_SIZE);

  buffer_weather = malloc(WEATHER_ICON_BUFFER_SIZE + sizeof(FPath));
  buffer_bluetooth = malloc(BLUETOOTH_ICON_BUFFER_SIZE + sizeof(FPath));
  
   #ifndef PBL_PLATFORM_APLITE
  char *sys_locale = setlocale(LC_ALL, "");
  // we're not supporting chinese - or Korean yet
  if (strcmp("zh_CN", sys_locale) == 0 || strcmp("zh_TW", sys_locale) == 0 || strcmp("ko_KR", sys_locale) == 0) {
    setlocale(LC_ALL, "en_US");
  }
  #endif
  
  //create main layer
  s_main_layer = layer_create(bounds);
  layer_set_update_proc(s_main_layer, main_update_proc);
  layer_add_child(window_layer, s_main_layer);
  
  
}

static void window_unload(Window *window) {
  layer_destroy(s_main_layer);
}


void handle_init() {
  
 
    
  // loading flags
  COORDINATES_LATITUDE = persist_exists(MESSAGE_KEY_LOCATION_LATITUDE) ? persist_read_int(MESSAGE_KEY_LOCATION_LATITUDE) : -1;
  COORDINATES_LONGITUDE = persist_exists(MESSAGE_KEY_LOCATION_LONGITUDE) ? persist_read_int(MESSAGE_KEY_LOCATION_LONGITUDE) : -1;

  
  TIME_DISPLAY = persist_exists(MESSAGE_KEY_TIME_DISPLAY) ? persist_read_int(MESSAGE_KEY_TIME_DISPLAY) : DIGITAL_TIME;
  FLAG_TEMPERATURE_FORMAT = persist_exists(MESSAGE_KEY_TEMPERATURE_FORMAT) ? persist_read_int(MESSAGE_KEY_TEMPERATURE_FORMAT) : TEMPERATURE_FAHRENHEIT;
  FLAG_WEATHER_INTERVAL = persist_exists(MESSAGE_KEY_WEATHER_INTERVAL) ? persist_read_int(MESSAGE_KEY_WEATHER_INTERVAL) : 60;
  #ifdef PBL_RECT
  FLAG_SIDEBAR_LOCATION = persist_exists(MESSAGE_KEY_SIDEBAR_LOCATION) ? persist_read_int(MESSAGE_KEY_SIDEBAR_LOCATION) : SIDEBAR_LOCATION_RIGHT;
  #endif
  FLAG_LOCATION_SERVICE = persist_exists(MESSAGE_KEY_LOCATION_SERVICE) ? persist_read_int(MESSAGE_KEY_LOCATION_SERVICE) : LOCATION_SERVICE_AUTOMATIC;
  FLAG_HOURS_MINUTES_SEPARATOR = persist_exists(MESSAGE_KEY_HOURS_MINUTES_SEPARATOR) ? persist_read_int(MESSAGE_KEY_HOURS_MINUTES_SEPARATOR) : ':';
  FLAG_BLUETOOTH_ALERT = persist_exists(MESSAGE_KEY_BLUETOOTH_ALERT) ? persist_read_int(MESSAGE_KEY_BLUETOOTH_ALERT) : BLUETOOTH_ALERT_WEAK;
 
  FLAG_SHOW_ANALOG_SECONDS = persist_exists(MESSAGE_KEY_SHOW_ANALOG_SECONDS) ? persist_read_int(MESSAGE_KEY_SHOW_ANALOG_SECONDS) : 0;
  FLAG_BACKLIGHT_WHILE_CHARGING = persist_exists(MESSAGE_KEY_BACKLIGHT_WHILE_CHARGING) ? persist_read_int(MESSAGE_KEY_BACKLIGHT_WHILE_CHARGING) : 0;
  
  #ifndef PBL_PLATFORM_APLITE
      FLAG_LANGUAGE = persist_exists(MESSAGE_KEY_LANGUAGE) ? persist_read_int(MESSAGE_KEY_LANGUAGE) : LANGUAGE_SYSTEM;
      FLAG_SECONDARY_INFO_1 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_1) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_1) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_2 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_2) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_2) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_3 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_3) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_3) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_4 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_4) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_4) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_5 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_5) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_5) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_6 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_6) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_6) : SECONDARY_INFO_NOTHING;
      #ifdef PBL_ROUND
      FLAG_SECONDARY_INFO_7 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_7) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_7) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_8 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_8) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_8) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_9 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_9) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_9) : SECONDARY_INFO_NOTHING;
      FLAG_SECONDARY_INFO_10 = persist_exists(MESSAGE_KEY_SECONDARY_INFO_10) ? persist_read_int(MESSAGE_KEY_SECONDARY_INFO_10) : SECONDARY_INFO_NOTHING;
      #endif
      FLAG_GRAPHICAL_STEP_GOAL = persist_exists(MESSAGE_KEY_GRAPHICAL_STEP_GOAL) ? persist_read_int(MESSAGE_KEY_GRAPHICAL_STEP_GOAL) : 0;
      ALT_TIMEZONE_OFFSET = persist_exists(MESSAGE_KEY_ALT_TIMEZONE_OFFSET) ? persist_read_int(MESSAGE_KEY_ALT_TIMEZONE_OFFSET) : 0;
      CUSTOM_STEP_GOAL = persist_exists(MESSAGE_KEY_CUSTOM_STEP_GOAL) ? persist_read_int(MESSAGE_KEY_CUSTOM_STEP_GOAL) : 0;
      PRIMARY_COLOR = persist_exists(MESSAGE_KEY_PRIMARY_COLOR) ? persist_read_int(MESSAGE_KEY_PRIMARY_COLOR) : 0xAAAA00;
      SECONDARY_COLOR = persist_exists(MESSAGE_KEY_SECONDARY_COLOR) ? persist_read_int(MESSAGE_KEY_SECONDARY_COLOR) : 0xFFFF00;
      BACK_COLOR = persist_exists(MESSAGE_KEY_BACK_COLOR) ? persist_read_int(MESSAGE_KEY_BACK_COLOR) : 0x000000;
      ICON_COLOR = persist_exists(MESSAGE_KEY_ICON_COLOR) ? persist_read_int(MESSAGE_KEY_ICON_COLOR) : 0xFFFFFF;
  
      if (persist_exists(MESSAGE_KEY_RANDOM_TEXT)) persist_read_string(MESSAGE_KEY_RANDOM_TEXT, RANDOM_TEXT, persist_get_size(MESSAGE_KEY_RANDOM_TEXT));
      if (persist_exists(MESSAGE_KEY_ALT_TIMEZONE_NAME)) persist_read_string(MESSAGE_KEY_ALT_TIMEZONE_NAME, ALT_TIMEZONE_NAME, persist_get_size(MESSAGE_KEY_ALT_TIMEZONE_NAME));

  #else
      FLAG_SECONDARY_INFO_2 = SECONDARY_INFO_RANDOM_TEXT;
      FLAG_SECONDARY_INFO_5 = SECONDARY_INFO_LOCATION;
  #endif
  
  
  s_window = window_create();
  #ifndef PBL_PLATFORM_APLITE
    //setting language
    set_language(FLAG_LANGUAGE);
    window_set_background_color(s_window, GColorFromHEX(BACK_COLOR));
  #else
    window_set_background_color(s_window, GColorBlack);
  #endif
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
    
  window_stack_push(s_window, true);
  
  // restoring saved weather data (or showing the "not yet fetched" icon)
  temp_kelvin = WEATHER_NO_DATA;
  if (persist_exists(MESSAGE_KEY_WEATHER_STORAGE)) {
    WeatherData wd;
    persist_read_data(MESSAGE_KEY_WEATHER_STORAGE, &wd, sizeof(wd));
    copy_weather(wd.temp_celcius, wd.condition, wd.is_day != 0, wd.name);
  } else {
    set_weather_status_icon(WEATHER_STATUS_NOT_YET_FETCHED);
  }

  update_tick_subscription();
  my_battery = events_battery_state_service_subscribe(battery_handler);
  my_connection = events_connection_service_subscribe((ConnectionHandlers){.pebble_app_connection_handler = connection_handler});
  
  
  //get initial battery value
  global_battery_percent = battery_state_service_peek().charge_percent;
  
  //get initial bluetooth
  inside_init = true;
  connection_handler(connection_service_peek_pebble_app_connection());
  inside_init = false;

  #ifndef PBL_PLATFORM_APLITE

    health_init(health_metrics_update);
    // only run the HRM sensor when a Heart Rate widget is actually displayed
    #if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
    update_heart_rate_sampling();
    #endif
    health_metrics_update();

    events_app_message_request_inbox_size(450);
  #else
    events_app_message_request_inbox_size(300);
  #endif
  events_app_message_request_outbox_size(128); // for our weather-request messages
  
  //initializng app messages
  my_appmesages = events_app_message_subscribe_handlers((EventAppMessageHandlers){.received = inbox_received_callback}, NULL);
  events_app_message_open();

}

void handle_deinit(void) {
  free(buffer_weather);
  free(buffer_bluetooth);
  if (buffer != NULL) {free(buffer); buffer = NULL;}
  gbitmap_destroy(fctx_buffer);
  
  events_tick_timer_service_unsubscribe(my_time);
  events_battery_state_service_unsubscribe(my_battery);
  events_connection_service_unsubscribe(my_connection);
  events_app_message_unsubscribe(my_appmesages);
  #ifndef PBL_PLATFORM_APLITE
  
    #if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
    health_service_set_heart_rate_sample_period(0);
    #endif
  
    health_deinit();
  #endif
  
  window_destroy(s_window);
}



int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
