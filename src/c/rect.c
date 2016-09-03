#ifdef PBL_RECT  //RECTANGLE only code

#include <pebble.h>
#include "common.h"
#include "watchface.h"



extern GBitmap *fctx_buffer;

//------------------------------MY FCTX

#ifndef PBL_COLOR
void fctx_init_context_my(FContext* fctx, GContext* gctx) {

    GBitmap* frameBuffer = graphics_capture_frame_buffer(gctx);
    if (frameBuffer) {
        fctx->flag_bounds = gbitmap_get_bounds(frameBuffer);
        graphics_release_frame_buffer(gctx, frameBuffer);

        fctx->flag_buffer = fctx_buffer;
        CHECK(fctx->flag_buffer);

        fctx->gctx = gctx;
        fctx->subpixel_adjust = -FIXED_POINT_SCALE / 2;
        fctx->transform_pivot = FPointZero;
        fctx->transform_offset = FPointZero;
        fctx->transform_scale_from = FPointOne;
        fctx->transform_scale_to = FPointOne;
        fctx->transform_rotation = 0;
    }
}

#else
void fctx_init_context_my(FContext* fctx, GContext* gctx) {

    GBitmap* frameBuffer = graphics_capture_frame_buffer(gctx);
    if (frameBuffer) {
        fctx->flag_bounds = gbitmap_get_bounds(frameBuffer);
        graphics_release_frame_buffer(gctx, frameBuffer);
        fctx->gctx = gctx;
        fctx->flag_buffer = fctx_buffer;
        fctx->fill_color = GColorWhite;
        fctx->color_bias = 0;
        fctx->subpixel_adjust = -1;
        fctx->transform_pivot = FPointZero;
        fctx->transform_offset = FPointZero;
        fctx->transform_scale_from = FPointOne;
        fctx->transform_scale_to = FPointOne;
        fctx->transform_rotation = 0;
    }
}
#endif


//------------------------------MY FCTX



// graphics variables
FFont *ffont;


extern uint8_t TIME_DISPLAY, FLAG_TEMPERATURE_FORMAT, FLAG_SIDEBAR_LOCATION;
extern uint8_t FLAG_SHOW_ANALOG_SECONDS, FLAG_SECONDARY_INFO_2, FLAG_SECONDARY_INFO_5;

extern uint8_t global_battery_percent;
extern FPath *weather_icon, *bluetooth_icon;
extern int16_t temp_kelvin, temp_celcius, temp_fahrenheit;
extern char LOCATION_NAME[22];
extern uint8_t FLAG_HOURS_MINUTES_SEPARATOR, FLAG_LANGUAGE;

char SECONDARY_INFO[22];

#ifndef PBL_PLATFORM_APLITE
  extern uint16_t health_steps, health_step_goal, health_distance, health_time_active, health_calories_rest, health_calories_active;
  extern char ALT_TIMEZONE_NAME[4];
  extern int16_t ALT_TIMEZONE_OFFSET;
  extern uint8_t FLAG_GRAPHICAL_STEP_GOAL;
  extern uint8_t FLAG_SECONDARY_INFO_1, FLAG_SECONDARY_INFO_3, FLAG_SECONDARY_INFO_4, FLAG_SECONDARY_INFO_6;
  extern int32_t  PRIMARY_COLOR, SECONDARY_COLOR, BACK_COLOR, ICON_COLOR;
  extern char RANDOM_TEXT[22];
#endif 


void draw_graphics(Layer *layer, GContext *ctx, struct tm *global_date_time) {
  
  GColor color_primary; 
  GColor color_secondary;
  GColor color_back;
  GColor color_icon;
  
  #ifndef PBL_PLATFORM_APLITE 
    color_primary =  GColorFromHEX(PRIMARY_COLOR); 
    color_secondary = GColorFromHEX(SECONDARY_COLOR); 
    color_back = GColorFromHEX(BACK_COLOR); 
    color_icon = GColorFromHEX(ICON_COLOR); 
  #else
    color_primary =  GColorWhite; 
    color_secondary = GColorWhite; 
    color_back = GColorBlack; 
    color_icon = GColorWhite; 
  #endif
  
  FContext fctx;
  
  uint8_t x1, x2;
  
  
  GRect bounds = layer_get_bounds(layer);
  GRect u_bounds =  layer_get_unobstructed_bounds(layer);
  
  // main vertical separator line
  if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT)
      x1 = bounds.size.w * 75 /100;
  else
      x1 = bounds.size.w * 25 /100 - 4;
 
  #ifndef PBL_PLATFORM_APLITE
    if (FLAG_GRAPHICAL_STEP_GOAL == 0) { // if we're not showing graphical step goal - just display full line
      graphics_context_set_fill_color(ctx, color_primary);
      graphics_fill_rect(ctx, GRect(x1, bounds.origin.y, 4, bounds.size.h), 0, GCornerNone);
    } else if (health_steps >=  health_step_goal) { // goal achieved
      graphics_context_set_fill_color(ctx, color_secondary);
      graphics_fill_rect(ctx, GRect(x1, bounds.origin.y, 4, bounds.size.h), 0, GCornerNone);
    }  else { //otherwise spilt between goal & steps made
      uint16_t health_goal_percent = bounds.size.h * health_steps / health_step_goal;
      graphics_context_set_fill_color(ctx, color_primary);
      graphics_fill_rect(ctx, GRect(x1, bounds.origin.y, 4, bounds.size.h - health_goal_percent), 0, GCornerNone); // still to go
      graphics_context_set_fill_color(ctx, color_secondary);
      graphics_fill_rect(ctx, GRect(x1, bounds.size.h - health_goal_percent, 4, health_goal_percent), 0, GCornerNone); // already stepped
    }
  #else
   graphics_context_set_fill_color(ctx, color_primary);
   graphics_fill_rect(ctx, GRect(x1, bounds.origin.y, 4, bounds.size.h), 0, GCornerNone);
  #endif
   
  
  // right side horizontal separator lines
  graphics_context_set_stroke_color(ctx, color_primary);
 
  if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT) 
     x1 = bounds.size.w * 78 /100 + 4;
  else 
     x1 = bounds.origin.x + 4;
  x2 = x1 + bounds.size.w * 16 /100;
  graphics_draw_line(ctx, GPoint(x1, bounds.size.h * 27/100), GPoint(x2, bounds.size.h * 27/100));
  graphics_draw_line(ctx, GPoint(x1, bounds.size.h * 55/100), GPoint(x2, bounds.size.h * 55/100));
  graphics_draw_line(ctx, GPoint(x1, bounds.size.h * 84/100), GPoint(x2, bounds.size.h * 84/100));
  
  // battery
  uint8_t battery_y = bounds.size.h * 59/100;
  uint8_t battery_w = bounds.size.w * 9/100;
  uint8_t battery_h = u_bounds.size.h * 12/100;
  if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT) 
      x1 = bounds.size.w * 85/100;
  else
      x1 = bounds.size.w * 15/100 - battery_w;
  
  graphics_context_set_fill_color(ctx, color_icon);
  graphics_fill_rect(ctx, GRect(x1, battery_y , battery_w, battery_h), 1, GCornersAll);
  
  graphics_context_set_fill_color(ctx, color_back);
  graphics_fill_rect(ctx, GRect(x1 + 2, battery_y + 2, battery_w - 4, battery_h - 4), 0, GCornerNone);  
    
  graphics_context_set_fill_color(ctx, color_icon);
  graphics_fill_rect(ctx, GRect(x1 + 3, battery_y + 3 + (battery_h - 6) * (100-global_battery_percent)/100, battery_w - 6, (battery_h - 6)  - (battery_h - 6) * (100-global_battery_percent)/100 ), 0, GCornerNone);    
  
 
  //***************** ANALOG CLOCK *******************//
  if (TIME_DISPLAY == ANALOG_TIME) {
    
    GPoint center;
    if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT)
      center.x =  u_bounds.size.w * 75 /100 /2;
    else
      center.x =  bounds.size.w * 25 /100 + u_bounds.size.w * 75 /100 /2; 
    
    center.y = u_bounds.size.h / 2;
    
    uint8_t max_hand_length = u_bounds.size.w * 75 /100 /2;
    
    // ******************* hour hand
    int32_t angle = (TRIG_MAX_ANGLE * (((global_date_time->tm_hour % 12) * 6) + (global_date_time->tm_min / 10))) / (12 * 6);
  
    GPoint hand_endpoint = {
      .x = (int16_t)(sin_lookup(angle) * (int32_t)(max_hand_length * 70/100) / TRIG_MAX_RATIO) + center.x,
      .y = (int16_t)(-cos_lookup(angle) * (int32_t)(max_hand_length * 70/100) / TRIG_MAX_RATIO) + center.y,
    };
    
    //color under hand
    graphics_context_set_stroke_color(ctx, color_back);
    graphics_context_set_stroke_width(ctx, 8);
    graphics_draw_line(ctx, center, hand_endpoint);
    
    graphics_context_set_stroke_color(ctx, color_icon);
    graphics_context_set_stroke_width(ctx, 6);
    graphics_draw_line(ctx, center, hand_endpoint);
     
    // ******************** minute hand
    angle = TRIG_MAX_ANGLE * global_date_time->tm_min / 60;

    
    hand_endpoint.x = (int16_t)(sin_lookup(angle) * (int32_t)max_hand_length / TRIG_MAX_RATIO) + center.x;
    hand_endpoint.y = (int16_t)(-cos_lookup(angle) * (int32_t)max_hand_length / TRIG_MAX_RATIO) + center.y;
    
    //color under hand
    graphics_context_set_stroke_color(ctx, color_back);
    graphics_context_set_stroke_width(ctx, 4);
    graphics_draw_line(ctx, center, hand_endpoint);
    
    graphics_context_set_stroke_color(ctx, color_icon);
    graphics_context_set_stroke_width(ctx, 2);
    graphics_draw_line(ctx, center, hand_endpoint);
    
    graphics_context_set_stroke_width(ctx, 1); //resetting for second hand and inner circle
   
    // ***************** second hand
    if (FLAG_SHOW_ANALOG_SECONDS == 1) {  
      angle = TRIG_MAX_ANGLE * global_date_time->tm_sec / 60;
      
      hand_endpoint.x = (int16_t)(sin_lookup(angle) * (int32_t)max_hand_length / TRIG_MAX_RATIO) + center.x;
      hand_endpoint.y = (int16_t)(-cos_lookup(angle) * (int32_t)max_hand_length / TRIG_MAX_RATIO) + center.y;
      
      graphics_context_set_stroke_color(ctx, color_secondary);  
      graphics_draw_line(ctx, center, hand_endpoint);
    } 
    
    // first circle in the middle
    graphics_context_set_fill_color(ctx, color_icon);
    graphics_fill_circle(ctx, center, 5);
    
    // second circle in the middle
    #ifndef PBL_PLATFORM_APLITE
      graphics_context_set_fill_color(ctx, color_primary);  
    #else
      graphics_context_set_fill_color(ctx, GColorBlack); 
    #endif
    graphics_fill_circle(ctx, center, 4);
    
  }  
  
  
  
  
   // initializing FCTX library
  //fctx_init_context(&fctx, ctx);
  fctx_init_context_my(&fctx, ctx);
  fctx_set_color_bias(&fctx, 0);
 
  fctx_begin_fill(&fctx);
  fctx_set_fill_color(&fctx, color_icon); 
  

  // bluetooth
  if (bluetooth_icon){
    if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT) 
        x1 = bounds.size.w * 85/100;
    else
        x1 = bounds.size.w * 13/200;
    fctx_set_offset(&fctx, FPointI(x1, bounds.size.h * 88/100));
    fctx_draw_commands(&fctx, FPointZero, bluetooth_icon->data, bluetooth_icon->size);
  }
  
 
  //weather icon
  if (weather_icon){
    if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT) 
        x1 = bounds.size.w * 165/200;
    else
        x1 = bounds.size.w * 4/100;
    fctx_set_scale(&fctx, FPoint(100,100), FPoint(87,87)); //slightly reducing icon size
    fctx_set_offset(&fctx, FPointI(x1, bounds.size.h * (temp_kelvin != -1280? 59: 70) / 200));
    fctx_draw_commands(&fctx, FPointZero, weather_icon->data, weather_icon->size);
  }
  
  fctx_end_fill(&fctx);

}

void draw_secondary_info(FContext *fctx, uint8_t font_size, uint8_t x, uint8_t y, uint8_t secondary_info_type, struct tm *my_time, GColor color){
  
   #ifndef PBL_PLATFORM_APLITE
   char format[] = "%A:%B";
   time_t nowg;
   struct tm *tg;
  
    // building format 12h/24h
         if (clock_is_24h_style()) {
            strcpy(format, "%H:%M"); // e.g "14:46"
         } else {
            strcpy(format, "%l:%M %P"); // e.g " 2:46 PM" -- with leading space
         }   
     
         format[2] = FLAG_HOURS_MINUTES_SEPARATOR;
   #endif
  
   switch(secondary_info_type){
       case SECONDARY_INFO_LOCATION:
         strcpy(SECONDARY_INFO, LOCATION_NAME);  
         break;
       case SECONDARY_INFO_RANDOM_TEXT:
         #ifndef PBL_PLATFORM_APLITE
           strcpy(SECONDARY_INFO, RANDOM_TEXT);  
         #else
           strcpy(SECONDARY_INFO, "COBBLESTYLE 2");
         #endif
         break;
     
       #ifndef PBL_PLATFORM_APLITE
       case SECONDARY_INFO_AMPM:
         strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%p", my_time);
         break;
       case SECONDARY_INFO_SECONDS:
         strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%S", my_time);
         break;
      
       case SECONDARY_INFO_TIME_ACTIVE:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "TA  %d%c%d", health_time_active / 3600, FLAG_HOURS_MINUTES_SEPARATOR, health_time_active % 3600 / 60);
         break;
       case SECONDARY_INFO_CALORIES_REST:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "CR  %d", health_calories_rest);
         break;
       case SECONDARY_INFO_CALORIES_ACTIVE:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "CA  %d",health_calories_active);
         break; 
     
       case SECONDARY_INFO_STEP_COUNT:
         if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "ST  %d", health_steps);
         else snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%s  %d", _("ST"), health_steps);
         break;
       case SECONDARY_INFO_DISTANCE_METERS:
         if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "M  %d", health_distance);
         else snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%s  %d", _("M"), health_distance);        
         break;
       case SECONDARY_INFO_DISTANCE_KM:
         if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "KM  %d.%d", health_distance / 1000, health_distance % 1000 / 100);
         else snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%s  %d.%d", _("KM"), health_distance / 1000, health_distance % 1000 / 100);
         break;
       case SECONDARY_INFO_DISTANCE_MILES:
         if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "MI  %d.%d", health_distance / 1609, health_distance * 1000 / 1609 % 1000 / 100);
         else snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%s  %d.%d", _("MI"), health_distance / 1609, health_distance * 1000 / 1609 % 1000 / 100);
         utf_decode_to_upper(SECONDARY_INFO);
         break;
       case SECONDARY_INFO_NOTHING:
         strcpy(SECONDARY_INFO, "\0");
         break;
       case SECONDARY_INFO_SECOND_TIMEZONE:
         nowg = time(NULL) + ALT_TIMEZONE_OFFSET * 60;
         tg = gmtime(&nowg); 
     
         strcpy(SECONDARY_INFO, ALT_TIMEZONE_NAME); //prepending with timezone name
         SECONDARY_INFO[3]=' '; SECONDARY_INFO[4]=' ';
         
       
         strftime(&SECONDARY_INFO[5], sizeof(SECONDARY_INFO), format, tg); //adding timezone time
     
         break;
      case SECONDARY_INFO_LOCAL_TIME:
        
          strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), format, my_time);
           
          // if time begin with space - eliminate it for even centering
          if (SECONDARY_INFO[0] == ' ') {
              memcpy(&SECONDARY_INFO[0], &SECONDARY_INFO[1], strlen(SECONDARY_INFO));
          }
         break;
     case SECONDARY_INFO_WEEK_NUMBER:
          strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "WK  %W", my_time);
          if (FLAG_LANGUAGE != LANGUAGE_SYSTEM) strncpy(&SECONDARY_INFO[0], _("WK"), 2);
         break;
    case SECONDARY_INFO_FULL_DAY:
         if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%A", my_time);
         else translate_full_day_name(my_time->tm_wday, SECONDARY_INFO);
         break;
         #endif
     
   }
     utf_decode_to_upper(SECONDARY_INFO);
  
     fctx_begin_fill(fctx);
     fctx_set_fill_color(fctx, color);  
  
     uint8_t str_len = strlen(SECONDARY_INFO);
     if (str_len > 14) {
       font_size = font_size * 14 / str_len;
     }
  
     fctx_draw_text(fctx, SECONDARY_INFO, ffont, font_size, x, y, GTextAlignmentCenter, FTextAnchorMiddle);
     fctx_end_fill(fctx);
}


void draw_data(Layer *layer, GContext *ctx, struct tm *global_date_time){
  
    GColor color_icon;
    #ifndef PBL_PLATFORM_APLITE
      GColor color_secondary;
      color_secondary = GColorFromHEX(SECONDARY_COLOR); 
      color_icon = GColorFromHEX(ICON_COLOR);
    #else
      color_icon = GColorWhite;
    #endif
   
  FContext fctx;
  char format[] = "%A:%B";

  //text-output buffer variables
  char s_time[] = "PAR 88:44 PM";
  char s_dow[] = "WED   "; 
  char s_day[] = "05";
  char s_month[] = "OCT   ";
  char s_battery[] = "100";
  char s_temp[6];
  
  uint8_t x1;
  
  GRect bounds = layer_get_bounds(layer);
  GRect u_bounds =  layer_get_unobstructed_bounds(layer);
  
  // initializing FCTX library
  //fctx_init_context(&fctx, ctx);
  fctx_init_context_my(&fctx, ctx);
  fctx_set_color_bias(&fctx, 0);
 
  fctx_begin_fill(&fctx);
  fctx_set_fill_color(&fctx, color_icon); 
  
  //*********** BEGIN WRITING BOLD TEXT OUPUT *******************************
  
  // loading bold font
  ffont = ffont_load_from_resource(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_FFONT);
  
  //======================== Displaying day of the month ==================================
  strftime(s_day, sizeof(s_day), "%d", global_date_time);
 
  if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT) 
        x1 = bounds.size.w * 89 / 100;
    else
        x1 = bounds.size.w * 11/100;
  fctx_draw_text(&fctx, s_day, ffont, bounds.size.h * 12/100, x1, bounds.size.h * 13/100, GTextAlignmentCenter, FTextAnchorMiddle);
    
    
  //======================== Displaying day of the week ==================================
  #ifdef PBL_PLATFORM_APLITE
  strftime(s_dow, sizeof(s_dow), "%a", global_date_time);
  #else
   if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) strftime(s_dow, sizeof(s_dow), "%a", global_date_time);
   else translate_short_day_name(global_date_time->tm_wday, s_dow);
  #endif
  utf_decode_to_upper(s_dow);
  fctx_draw_text(&fctx, s_dow, ffont, bounds.size.h * 8/100, x1, bounds.size.h * 4/100, GTextAlignmentCenter, FTextAnchorMiddle);
  
  //======================== Displaying month  ===========================================
  #ifdef PBL_PLATFORM_APLITE
  strftime(s_month, sizeof(s_month), "%b", global_date_time);
  #else
  if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) strftime(s_month, sizeof(s_month), "%b", global_date_time);
  else translate_short_month_name(global_date_time->tm_mon, s_month);
  #endif
  utf_decode_to_upper(s_month);
  fctx_draw_text(&fctx, s_month, ffont, bounds.size.h * 8/100, x1, bounds.size.h * 22/100, GTextAlignmentCenter, FTextAnchorMiddle);
  
  //======================== Displaying battery text  ===========================================
  snprintf(s_battery, sizeof(s_battery), "%d", global_battery_percent);  
  fctx_draw_text(&fctx, s_battery, ffont, bounds.size.h * 10/100, x1, bounds.size.h * 77/100, GTextAlignmentCenter, FTextAnchorMiddle);
  
  //======================== Displaying temperature  ===========================================
    if (weather_icon && temp_kelvin != -1280) {
      switch (FLAG_TEMPERATURE_FORMAT) {
       case TEMPERATURE_KELVIN:
         snprintf(s_temp, sizeof(s_temp), "%i\u00B0",temp_kelvin);
         break;
       case TEMPERATURE_CELCIUS:
         snprintf(s_temp, sizeof(s_temp), "%i\u00B0",temp_celcius);
         break;
       case TEMPERATURE_FAHRENHEIT:
         snprintf(s_temp, sizeof(s_temp), "%i\u00B0",temp_fahrenheit);
         break;
     }
    utf_decode_to_upper(s_temp); //decoding degree sign
    fctx_draw_text(&fctx, s_temp, ffont, bounds.size.h * 10/100, x1, bounds.size.h * 48/100, GTextAlignmentCenter, FTextAnchorMiddle);
 
   }
  
  if (FLAG_SIDEBAR_LOCATION == SIDEBAR_LOCATION_RIGHT) 
      x1 = (bounds.size.w * 75/100) / 2;
  else
      x1 = (bounds.size.w * 75/100) / 2 + bounds.size.w * 25/100; 
  
  
  //======================== Displaying Digital Time ==================================
  
  
  // if we're displaying digital time full screen - IN BOLD showing only hours
  if (TIME_DISPLAY == BIG_TIME) {
    
      // building format 12h/24h
      if (clock_is_24h_style()) {
             strcpy(format, "%H"); // e.g "14"
          } else {
             strcpy(format, "%I"); // e.g "02" -- with leading zero
      }  
      
      // outputting hours
      strftime(s_time, sizeof(s_time), format, global_date_time);
       
      // if time begin with space - eliminate it for even centering
      if (s_time[0] == ' ') {
          memcpy(&s_time[0], &s_time[1], strlen(s_time));
      }
         
      // displaying hours
      fctx_draw_text(&fctx, s_time, ffont, u_bounds.size.h * 60/100, x1, u_bounds.size.h * 2 / 100, GTextAlignmentCenter, FTextAnchorTop);
      fctx_end_fill(&fctx);
 
  } else { // of we're not displaying digital time full screen - show it completely
    
      if (TIME_DISPLAY != ANALOG_TIME) {
  
          // building format 12h/24h
          if (clock_is_24h_style()) {
                 strcpy(format, "%H:%M"); // e.g "14:46"
              } else {
                 strcpy(format, "%l:%M"); // e.g " 2:46" -- with leading space
          }  
          
          // outputting time
          format[2] = FLAG_HOURS_MINUTES_SEPARATOR;
          strftime(s_time, sizeof(s_time), format, global_date_time);
           
          // if time begin with space - eliminate it for even centering
          if (s_time[0] == ' ') {
              memcpy(&s_time[0], &s_time[1], strlen(s_time));
          }
             
          // displaying time
          fctx_draw_text(&fctx, s_time, ffont, u_bounds.size.h * 26/100, x1, u_bounds.size.h/2, GTextAlignmentCenter, FTextAnchorMiddle);
      }
      fctx_end_fill(&fctx);
      
    
      //======================== Secondary Info  ===========================================
      draw_secondary_info(&fctx, bounds.size.h * 10/100, x1, u_bounds.size.h * 19/100, FLAG_SECONDARY_INFO_2, global_date_time, color_icon);
      draw_secondary_info(&fctx, bounds.size.h * 10/100, x1, u_bounds.size.h * 83/100, FLAG_SECONDARY_INFO_5, global_date_time, color_icon);
   
  }
  
  //*********** END WRITING BOLD TEXT OUPUT *******************************
  
  
  //*********** BEGIN WRITING regular TEXT OUPUT *******************************
  
  //loading regular font
  ffont = ffont_load_from_resource(RESOURCE_ID_ROBOTO_CONDENSED_REGULAR_FFONT);
  
  
  // if we're displaying digital time full screen - in regular font showing only minutes
  if (TIME_DISPLAY == BIG_TIME) {
    
    // outputting minutes
    fctx_begin_fill(&fctx);
    strftime(s_time, sizeof(s_time), "%M", global_date_time);
    fctx_draw_text(&fctx, s_time, ffont, u_bounds.size.h * 60/100, x1, u_bounds.size.h * 52 / 100, GTextAlignmentCenter, FTextAnchorTop);
    fctx_end_fill(&fctx);
    
  } else { // if we're not in full screen - display secondary info as well
   
     //======================== Secondary Info  ===========================================
    #ifndef PBL_PLATFORM_APLITE
    draw_secondary_info(&fctx, bounds.size.h * 10/100, x1, u_bounds.size.h * 7/100, FLAG_SECONDARY_INFO_1, global_date_time, color_secondary);
    draw_secondary_info(&fctx, bounds.size.h * 10/100, x1, u_bounds.size.h * 31/100, FLAG_SECONDARY_INFO_3, global_date_time, color_secondary);
    draw_secondary_info(&fctx, bounds.size.h * 10/100, x1, u_bounds.size.h * 71/100, FLAG_SECONDARY_INFO_4, global_date_time, color_secondary);
    draw_secondary_info(&fctx, bounds.size.h * 10/100, x1, u_bounds.size.h * 95/100, FLAG_SECONDARY_INFO_6, global_date_time, color_secondary);
    #endif
   
  }  
  

} 


#endif