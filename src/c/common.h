#pragma once
#include <pebble.h>

#include <pebble-fctx/fctx.h>
#include <pebble-fctx/ffont.h>
#include <pebble-fctx/fpath.h>

#ifdef PBL_PLATFORM_APLITE
  #define FONT_BUFFER_SIZE 4648
#else
  #include <pebble-localize/pebble-localize.h>
  #define FONT_BUFFER_SIZE 10564
#endif


#define WEATHER_ICON_BUFFER_SIZE 672
#define BLUETOOTH_ICON_BUFFER_SIZE 196

#define TEMPERATURE_KELVIN  0
#define TEMPERATURE_CELCIUS 1
#define TEMPERATURE_FAHRENHEIT 2


#define SECONDARY_INFO_LOCATION 0
#define SECONDARY_INFO_AMPM 1
#define SECONDARY_INFO_SECONDS 2
#define SECONDARY_INFO_RANDOM_TEXT 3
#define SECONDARY_INFO_STEP_COUNT 4
#define SECONDARY_INFO_DISTANCE_METERS 5
#define SECONDARY_INFO_DISTANCE_KM 6
#define SECONDARY_INFO_DISTANCE_MILES 7
#define SECONDARY_INFO_SECOND_TIMEZONE 8
#define SECONDARY_INFO_NOTHING 9
#define SECONDARY_INFO_LOCAL_TIME 10
#define SECONDARY_INFO_WEEK_NUMBER 11
#define SECONDARY_INFO_FULL_DAY 14
#define SECONDARY_INFO_TIME_ACTIVE 16
#define SECONDARY_INFO_CALORIES_REST 17
#define SECONDARY_INFO_CALORIES_ACTIVE 18
#if PBL_API_EXISTS(health_service_set_heart_rate_sample_period)
  #define SECONDARY_INFO_HEART_RATE 19
#endif 

#ifdef PBL_ROUND 
  #define SECONDARY_INFO_DAY_MONTH 12
  #define SECONDARY_INFO_WEATHER 13
  #define SECONDARY_INFO_12H12H 15
#else
  #define SIDEBAR_LOCATION_RIGHT 0
  #define SIDEBAR_LOCATION_LEFT 1
#endif

#define LOCATION_SERVICE_AUTOMATIC 0
#define LOCATION_SERVICE_MANUAL 1

#define ANALOG_TIME 0
#define DIGITAL_TIME 1
#define BIG_TIME 2

#ifdef PBL_ROUND 
  #define INFO_POSITION_TOP_LEFT 1
  #define INFO_POSITION_INSIDE_CIRCLE_TOP 2
  #define INFO_POSITION_TOP_RIGHT 3
  #define INFO_POSITION_BOTTOM_LEFT 4
  #define INFO_POSITION_INSIDE_CIRCLE_BOTTOM 5
  #define INFO_POSITION_BOTTOM_RIGHT 6
  #define INFO_POSITION_BOTTOM 7
  #define INFO_POSITION_TOP 8
  #define INFO_POSITION_INSIDE_CIRCLE_LEFT 9
  #define INFO_POSITION_INSIDE_CIRCLE_RIGHT 10
#endif


#define BLUETOOTH_ALERT_SILENT 1
#define BLUETOOTH_ALERT_WEAK 2
#define BLUETOOTH_ALERT_NORMAL 3
#define BLUETOOTH_ALERT_STRONG 4
#define BLUETOOTH_ALERT_DOUBLE 5


// loading FFont using predefined internal buffer
FFont* ffont_load_from_resource(uint32_t resource_id);

// Draws antialiased text with given font, size, position and allignment
void fctx_draw_text(FContext *fctx, char * text, FFont* ffont, int16_t size, int16_t x, int16_t y, GTextAlignment text_align, FTextAnchor text_anchor);

FPath* fpath_create_from_resource_with_buffer(uint32_t resource_id, uint8_t *buffer);

//converts text to uppercase (with unicode fix)
void utf_decode_to_upper(char *sPtr);

//************************ Localizing stuff *********************

#ifndef PBL_PLATFORM_APLITE

#define LANGUAGE_SYSTEM 0
#define LANGUAGE_NORWEGIAN 1
#define LANGUAGE_HUNGARIAN 2
#define LANGUAGE_SWEDISH 3
#define LANGUAGE_CATALAN 4
#define LANGUAGE_DUTCH 5

void translate_full_day_name(uint8_t day_no, char *translation);
#ifdef PBL_RECT
void translate_short_day_name(uint8_t day_no, char *translation);
#endif
void translate_short_month_name(uint8_t month_no, char *translation);
void set_language(uint8_t language_id);

                                    
#endif