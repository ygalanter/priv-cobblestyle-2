#ifndef PBL_RECT  // ROUND only code

#include <pebble.h>
#include "common.h"
#include "watchface.h"

extern GBitmap *fctx_buffer;

//------------------------------MY FCTX
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
//------------------------------MY FCTX

//**************** RADIAL TEXT STUFF



uint_least32_t string_width_radial(
    FContext* fctx,
    const char* text,
    FFont* font,
    int_least16_t font_size,
    FPoint center,
    uint_least16_t radius,
    bool clockwise)
{
      fixed_t s;
      fixed_t t;
      fixed_t arc;
      FGlyph* glyph;
      const char* p;
   fctx_set_text_size(fctx, font, font_size);
    s = radius * 6588397 / TRIG_MAX_ANGLE;
    t = s * fctx->transform_scale_from.x / fctx->transform_scale_to.x;
    arc = 0;
    for (p = text; *p; ++p) {
        if (*p == 1) 
            {glyph = ffont_glyph_info(font, ((uint_least16_t)(*p) << 8) | *(p + 1)); p++;}
         else
            {glyph = ffont_glyph_info(font, *p);}
        if (glyph && p!=text) {
            arc += glyph->horiz_adv_x * (clockwise? 1 : -1);
        }
    }
    return arc * TRIG_MAX_ANGLE / t;
}




void draw_string_radial(
    FContext* fctx,
    const char* text,
    FFont* font,
    int_least16_t font_size,
    FPoint center,
    uint_least16_t radius,
    uint_least32_t angle,
    bool clockwise)
  
  
{
 

  fixed_t s;
  fixed_t t;
  fixed_t arc;
  fixed_t r;
  fixed_t rotation; 
  FGlyph* glyph;
  const char* p;
  //correcting angle for half string width
    angle -= string_width_radial(fctx,text,font,font_size, center, radius, clockwise)/2;
  
    fctx_set_text_size(fctx, font, font_size);
    fctx_set_offset(fctx, center);
    fctx_set_rotation(fctx, 0);

    s = radius * 6588397 / TRIG_MAX_ANGLE;
    t = s * fctx->transform_scale_from.x / fctx->transform_scale_to.x;
    r = INT_TO_FIXED(radius) * fctx->transform_scale_from.x / fctx->transform_scale_to.x;
    arc = 0;
    rotation = 0; // YG moved here so final angle can be returned


    for (p = text; *p; ++p) {
     
         if (*p == 1) 
            {glyph = ffont_glyph_info(font, ((uint_least16_t)(*p) << 8) | *(p + 1)); p++;}
         else
            {glyph = ffont_glyph_info(font, *p);}
      
        if (glyph) {

            if (p != text) {
                arc += glyph->horiz_adv_x / 2;
            }
            rotation = angle + arc * TRIG_MAX_ANGLE / t * (clockwise? 1 : -1);
            fctx_set_rotation(fctx, rotation);
            arc += glyph->horiz_adv_x / 2;

            FPoint advance;
            advance.x = glyph->horiz_adv_x / -2;
            advance.y = clockwise? r : -(r + font->ascent);;

            void* path_data = ffont_glyph_outline(font, glyph);
            fctx_draw_commands(fctx, advance, path_data, glyph->path_data_length);
        }
    }
  
 
}
//**************** RADIAL TEXT STUFF


// graphics variables
FFont *ffont;

extern uint_least8_t TIME_DISPLAY, FLAG_TEMPERATURE_FORMAT, FLAG_SIDEBAR_LOCATION, FLAG_SECONDARY_INFO_1, FLAG_SECONDARY_INFO_3, FLAG_SECONDARY_INFO_4, FLAG_SECONDARY_INFO_6, FLAG_SECONDARY_INFO_7, FLAG_SECONDARY_INFO_8, FLAG_SECONDARY_INFO_9, FLAG_SECONDARY_INFO_10;
extern uint_least8_t FLAG_SHOW_ANALOG_SECONDS, FLAG_SECONDARY_INFO_2, FLAG_SECONDARY_INFO_5, FLAG_GRAPHICAL_STEP_GOAL,global_battery_percent,FLAG_HOURS_MINUTES_SEPARATOR, FLAG_LANGUAGE;
extern uint_least16_t health_steps, health_step_goal, health_distance, health_time_active, health_calories_rest, health_calories_active;
extern int_least16_t temp_kelvin, temp_celcius, temp_fahrenheit;
extern int_least16_t ALT_TIMEZONE_OFFSET;
extern int_least32_t  PRIMARY_COLOR, SECONDARY_COLOR, BACK_COLOR, ICON_COLOR;
extern FPath *weather_icon, *bluetooth_icon;
extern char LOCATION_NAME[22], ALT_TIMEZONE_NAME[4], RANDOM_TEXT[22];


GPoint get_point_from_angle(int_least32_t degree_angle, uint_least8_t length, GPoint offset) {
  
   return (GPoint){
      .x = (int_least16_t)(sin_lookup(DEG_TO_TRIGANGLE(degree_angle)) * (int_least32_t)(length) / TRIG_MAX_RATIO) + offset.x,
      .y = (int_least16_t)(-cos_lookup(DEG_TO_TRIGANGLE(degree_angle)) * (int_least32_t)(length) / TRIG_MAX_RATIO) + offset.y
   };
  
   
  
}

void draw_angled_line(GContext *ctx, int_least32_t degree_angle, uint_least8_t length, GPoint center){
   GPoint hand_endpoint = get_point_from_angle(degree_angle, length, center);
   GPoint hand_beginpoint = get_point_from_angle(degree_angle, length * 87/100, center);
   graphics_draw_line(ctx, hand_beginpoint, hand_endpoint);
}

void draw_line_with_color_and_width(GContext *ctx, GColor color, uint_least8_t width, GPoint start_point, GPoint end_point){
   graphics_context_set_stroke_color(ctx, color);
   graphics_context_set_stroke_width(ctx, width);
   graphics_draw_line(ctx, start_point, end_point);
}

void draw_angled_icon(FContext *fctx, FPath *icon, uint_least8_t scale_x, uint_least8_t scale_y, uint_least8_t offset_x, uint_least8_t offset_y, int_least16_t angle) {
   fctx_set_scale(fctx, FPoint(100,100), FPoint(scale_x, scale_y));
   fctx_set_rotation(fctx, DEG_TO_TRIGANGLE(angle));
   fctx_set_offset(fctx, FPointI(offset_x,  offset_y));
   fctx_draw_commands(fctx, FPointZero, icon->data, icon->size);
}


void draw_graphics(Layer *layer, GContext *ctx, struct tm *global_date_time) {
   
  GColor color_primary = GColorFromHEX(PRIMARY_COLOR); 
  GColor color_secondary = GColorFromHEX(SECONDARY_COLOR); 
  GColor color_back = GColorFromHEX(BACK_COLOR); 
  GColor color_icon = GColorFromHEX(ICON_COLOR); 
  
  FContext fctx;
  GRect bounds = layer_get_bounds(layer);
  GPoint center = grect_center_point(&bounds);
  GRect Circ;
  
  // drawing outer circle
  Circ = GRect(bounds.size.w * 15 / 100, bounds.size.h  * 15 / 100, bounds.size.w  * 70/ 100, bounds.size.h  * 70 / 100);
  graphics_context_set_stroke_width(ctx, 4);
  if (FLAG_GRAPHICAL_STEP_GOAL == 0) { // if we're not showing graphical step goal - just display full circle
    graphics_context_set_stroke_color(ctx, color_primary);
    graphics_draw_arc(ctx, Circ, GOvalScaleModeFitCircle, 0, TRIG_MAX_ANGLE);
    
  } else if (health_steps >= health_step_goal) { // goal achieved
    graphics_context_set_stroke_color(ctx, color_secondary);
    graphics_draw_arc(ctx, Circ, GOvalScaleModeFitCircle, 0, TRIG_MAX_ANGLE);
    
  } else { //otherwise spilt between goal & steps made
    uint_least16_t health_goal_percent = TRIG_MAX_ANGLE * health_steps / health_step_goal;
    
     // second part of circle
    graphics_context_set_stroke_color(ctx, color_primary);
    graphics_draw_arc(ctx, Circ, GOvalScaleModeFitCircle, health_goal_percent, TRIG_MAX_ANGLE);
    
    // first part of circle
    graphics_context_set_stroke_color(ctx, color_secondary);
    graphics_draw_arc(ctx, Circ, GOvalScaleModeFitCircle, 0, health_goal_percent);
    
  }
  
  // drawing separators 

    GPoint hand_endpoint;	
	
    graphics_context_set_stroke_color(ctx, color_primary);
    graphics_context_set_stroke_width(ctx, 1);
  
    uint_least8_t max_hand_length = bounds.size.w * 93 /100 /2;
  
    draw_angled_line(ctx, 0 + 43, max_hand_length, center);
    draw_angled_line(ctx, 90 - 10, max_hand_length, center);
    draw_angled_line(ctx, 90 + 10, max_hand_length, center);
    draw_angled_line(ctx, 180 - 43, max_hand_length, center);
    draw_angled_line(ctx, 180 + 43, max_hand_length, center);
    draw_angled_line(ctx, 270 - 10, max_hand_length, center);
    draw_angled_line(ctx, 270 + 10, max_hand_length, center);
    draw_angled_line(ctx, 360 - 43, max_hand_length, center);
    
  
  //***************** ANALOG CLOCK *******************//
  if (TIME_DISPLAY == ANALOG_TIME) {
    
    max_hand_length = bounds.size.w * 56 /100 /2;
    
    // ******************* hour hand
    hand_endpoint = get_point_from_angle( (360 * (((global_date_time->tm_hour % 12) * 6) + (global_date_time->tm_min / 10))) / (12 * 6)  , max_hand_length * 70 /100, center);
  
    
    //color under hand
    draw_line_with_color_and_width(ctx, color_back, 8, center, hand_endpoint);
    draw_line_with_color_and_width(ctx, color_icon, 6, center, hand_endpoint);
     
    // ******************** minute hand
    hand_endpoint = get_point_from_angle(360 * global_date_time->tm_min / 60  , max_hand_length, center);
    
    //color under hand
    draw_line_with_color_and_width(ctx, color_back, 4, center, hand_endpoint);
    draw_line_with_color_and_width(ctx, color_icon, 2, center, hand_endpoint);
    
    
   
    // ***************** second hand
    if (FLAG_SHOW_ANALOG_SECONDS == 1) {  
      
      hand_endpoint = get_point_from_angle(360 * global_date_time->tm_sec / 60  , max_hand_length, center);
      
      draw_line_with_color_and_width(ctx, color_secondary, 1, center, hand_endpoint);
     
    } 
    
    // first circle in the middle
    graphics_context_set_fill_color(ctx, color_icon);
    graphics_fill_circle(ctx, center, 5);
    
    // second circle in the middle
    graphics_context_set_fill_color(ctx, color_primary);  
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
    fctx_set_offset(&fctx, FPointI(bounds.size.w * 5/100, center.y * 93/100));
    fctx_set_scale(&fctx, FPoint(100,100), FPoint(86,86)); //slightly reducing icon size
    fctx_draw_commands(&fctx, FPointZero, bluetooth_icon->data, bluetooth_icon->size);
  }

  
  fctx_end_fill(&fctx);
}



void draw_secondary_info(FContext *fctx, uint_least8_t position, uint_least8_t secondary_info_type, GRect bounds, struct tm *my_time, GColor color){
   char format[] = "%l:%M %P";
   time_t nowg;
   struct tm *tg;
   GPoint center = grect_center_point(&bounds);
   char SECONDARY_INFO[22];
       
         // building format 12h/24h
         if (clock_is_24h_style()) {
             strcpy(format, "%H:%M"); // e.g "14:46"
          } else {
             strcpy(format, "%l:%M %P"); // e.g " 2:46" -- with leading space
         }  
                
         format[2] = FLAG_HOURS_MINUTES_SEPARATOR;
  
   switch(secondary_info_type){
       case SECONDARY_INFO_LOCATION:
         strcpy(SECONDARY_INFO, LOCATION_NAME);  
         utf_decode_to_upper(SECONDARY_INFO);
         break;
       case SECONDARY_INFO_RANDOM_TEXT:
           strcpy(SECONDARY_INFO, RANDOM_TEXT);  
           utf_decode_to_upper(SECONDARY_INFO);
         break;
       case SECONDARY_INFO_AMPM:
         strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%p", my_time);
         break;
       case SECONDARY_INFO_SECONDS:
         strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%S", my_time);
         break;
     
       case SECONDARY_INFO_TIME_ACTIVE:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "TA  %d%c%d", health_time_active / 3600, FLAG_HOURS_MINUTES_SEPARATOR, health_time_active % 3600 /60);
         break;
       case SECONDARY_INFO_CALORIES_REST:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "CR  %d", health_calories_rest);
         break;
       case SECONDARY_INFO_CALORIES_ACTIVE:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "CA  %d",health_calories_active);
         break;    
     
       case SECONDARY_INFO_STEP_COUNT:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%d", health_steps);
         break;
       case SECONDARY_INFO_DISTANCE_METERS:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "M  %d", health_distance);
         break;
       case SECONDARY_INFO_DISTANCE_KM:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "KM  %d.%d", health_distance / 1000, health_distance % 1000 / 100);
         break;
       case SECONDARY_INFO_DISTANCE_MILES:
         snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), "MI  %d.%d", health_distance / 1609, health_distance * 1000 / 1609 % 1000 / 100);
         break;
       case SECONDARY_INFO_NOTHING:
         SECONDARY_INFO[0] = 0;
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
         break;
      case SECONDARY_INFO_DAY_MONTH:
         strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%d  %b", my_time);
         if (FLAG_LANGUAGE != LANGUAGE_SYSTEM) translate_short_month_name(my_time->tm_mon, &SECONDARY_INFO[4]);
         utf_decode_to_upper(SECONDARY_INFO);
         SECONDARY_INFO[7] = 0; // so it's no more than 7 chars eg. 06  JUL
        
         break;
      case SECONDARY_INFO_WEATHER:
         if (weather_icon && temp_kelvin != -1280) {
          switch (FLAG_TEMPERATURE_FORMAT) {
           case TEMPERATURE_KELVIN:
             snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), " %d\u00B0",temp_kelvin);
             break;
           case TEMPERATURE_CELCIUS:
             snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), " %d\u00B0",temp_celcius);
             break;
           case TEMPERATURE_FAHRENHEIT:
             snprintf(SECONDARY_INFO, sizeof(SECONDARY_INFO), " %d\u00B0",temp_fahrenheit);
             break;
         }
          utf_decode_to_upper(SECONDARY_INFO); //correcting degree sign
        } else {
          strcpy(SECONDARY_INFO,"...");
        }
       break;
     case SECONDARY_INFO_FULL_DAY:
       if (FLAG_LANGUAGE == LANGUAGE_SYSTEM) strftime(SECONDARY_INFO, sizeof(SECONDARY_INFO), "%A", my_time);
       else translate_full_day_name(my_time->tm_wday, SECONDARY_INFO);
       utf_decode_to_upper(SECONDARY_INFO);
       break;
     case SECONDARY_INFO_12H12H:
       strcpy(SECONDARY_INFO, clock_is_24h_style()? "24H" : "12H");
       break;
   } 
 
    fctx_begin_fill(fctx);
    fctx_set_fill_color(fctx, color);  
  
     uint_least8_t str_len = strlen(SECONDARY_INFO), font_size;
  
     switch (position) {
        case INFO_POSITION_TOP:
           font_size = bounds.size.h * 11/100; if (str_len > 10) {font_size = font_size * 9 / str_len;}
           draw_string_radial(fctx, SECONDARY_INFO, ffont,  font_size, FPointI(center.x, center.y), bounds.size.h * 39 / 100, TRIG_MAX_ANGLE, true);
           break;
        case INFO_POSITION_INSIDE_CIRCLE_TOP:
           font_size = bounds.size.h * 10/100; if (str_len > 10) {font_size = font_size * 10 / str_len;}
       
            if (secondary_info_type == SECONDARY_INFO_WEATHER && weather_icon) {
               fctx_draw_text(fctx, SECONDARY_INFO, ffont, font_size, bounds.size.w * 50 /100, bounds.size.h * 32/100, GTextAlignmentRight, FTextAnchorMiddle);  
               draw_angled_icon(fctx, weather_icon, 86, 86, bounds.size.w * 52/100, bounds.size.h * 27/100, 0);
            } else {
               fctx_draw_text(fctx, SECONDARY_INFO, ffont, font_size, bounds.size.w / 2, bounds.size.h * 32/100, GTextAlignmentCenter, FTextAnchorMiddle);   
            }  
           
           break;
        case INFO_POSITION_INSIDE_CIRCLE_BOTTOM:
           font_size = bounds.size.h * 10/100; if (str_len > 10) {font_size = font_size * 10 / str_len;}
           if (secondary_info_type == SECONDARY_INFO_WEATHER && weather_icon) {
               fctx_draw_text(fctx, SECONDARY_INFO, ffont, font_size, bounds.size.w * 50 /100, bounds.size.h * 69/100, GTextAlignmentRight, FTextAnchorMiddle);  
               draw_angled_icon(fctx, weather_icon, 86, 86, bounds.size.w * 52/100, bounds.size.h * 64/100, 0);
            } else {
               fctx_draw_text(fctx, SECONDARY_INFO, ffont, font_size, bounds.size.w / 2, bounds.size.h * 69/100, GTextAlignmentCenter, FTextAnchorMiddle);
           }
           break;
        case INFO_POSITION_BOTTOM:
           font_size = bounds.size.h * 11/100; if (str_len > 10) {font_size = font_size * 9 / str_len;}
           draw_string_radial(fctx, SECONDARY_INFO, ffont,  font_size, FPointI(center.x, center.y), bounds.size.h * 39 / 100 , 0, false);
           break;
        case INFO_POSITION_TOP_LEFT:
           if (secondary_info_type == SECONDARY_INFO_WEATHER && weather_icon) {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(270 + 19), true);
               draw_angled_icon(fctx, weather_icon, 74, 74, bounds.size.w * 19/200, bounds.size.h * 53/200, -49);
           } else {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(270 + 29), true);
           }
           break;
        case INFO_POSITION_TOP_RIGHT:
           if (secondary_info_type == SECONDARY_INFO_WEATHER && weather_icon) {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(90 - 36), true);
               draw_angled_icon(fctx, weather_icon, 74, 74, bounds.size.w * 185/200, bounds.size.h * 29/100, 62);
           } else {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(90 - 28), true);
               
           }
           break;
        case INFO_POSITION_BOTTOM_LEFT:
            if (secondary_info_type == SECONDARY_INFO_WEATHER && weather_icon) {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(90 - 20), false);
               draw_angled_icon(fctx, weather_icon, 74, 74, bounds.size.w * 18/100, bounds.size.h * 69/100, 62);
           } else {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(90 - 29), false);
           }
           break;
        case INFO_POSITION_BOTTOM_RIGHT:
       
             if (secondary_info_type == SECONDARY_INFO_WEATHER && weather_icon) {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(270 + 38), false);
               draw_angled_icon(fctx, weather_icon, 74, 74, bounds.size.w * 84/100, bounds.size.h * 67/100, -62);
           } else {
               font_size = bounds.size.h * 7/100;
               draw_string_radial(fctx, SECONDARY_INFO, ffont, font_size, FPointI(center.x, center.y), bounds.size.h * 40 / 100 , DEG_TO_TRIGANGLE(270 + 29), false);
           }
           break;
       case INFO_POSITION_INSIDE_CIRCLE_LEFT:
           font_size = bounds.size.h * 9/100; 
           fctx_draw_text(fctx, SECONDARY_INFO, ffont, font_size, bounds.size.w * 53 / 200, bounds.size.h / 2, GTextAlignmentCenter, FTextAnchorMiddle);
           break;
       case INFO_POSITION_INSIDE_CIRCLE_RIGHT:
           font_size = bounds.size.h * 9/100; 
           fctx_draw_text(fctx, SECONDARY_INFO, ffont, font_size, bounds.size.w * 151 / 200, bounds.size.h / 2, GTextAlignmentCenter, FTextAnchorMiddle);
           break;
          
        
     }
  
     fctx_end_fill(fctx);
  
}


void draw_data(Layer *layer, GContext *ctx, struct tm *global_date_time) {
  
   GColor color_icon = GColorFromHEX(ICON_COLOR);
   
    FContext fctx;
    char format[] = "%A:%B";
  
    //text-output buffer variables
    char s_time[] = "PAR 88:44 PM";
    char s_battery[] = "100";
    
    GRect bounds = layer_get_bounds(layer);
    GPoint center = grect_center_point(&bounds);
    
    // initializing FCTX library
    //fctx_init_context(&fctx, ctx);
    fctx_init_context_my(&fctx, ctx);
    fctx_set_color_bias(&fctx, 0);
    
    //*********** BEGIN WRITING BOLD TEXT OUPUT *******************************
    
    // loading bold font
    ffont = ffont_load_from_resource(RESOURCE_ID_ROBOTO_CONDENSED_BOLD_FFONT);
  
    //======================== Displaying battery text  ===========================================
    snprintf(s_battery, sizeof(s_battery), "%d", global_battery_percent);  
    fctx_begin_fill(&fctx);
    fctx_set_fill_color(&fctx, color_icon);  
    fctx_draw_text(&fctx, s_battery, ffont, bounds.size.h * (global_battery_percent == 100? 8 : 10) /100, bounds.size.w * 93 / 100, center.y, GTextAlignmentCenter, FTextAnchorMiddle);
    fctx_end_fill(&fctx);
  
    //======================== Secondary Info  ===========================================
     draw_secondary_info(&fctx, INFO_POSITION_TOP, FLAG_SECONDARY_INFO_8, bounds, global_date_time, color_icon);
     draw_secondary_info(&fctx, INFO_POSITION_TOP_LEFT, FLAG_SECONDARY_INFO_1, bounds, global_date_time, color_icon);
     draw_secondary_info(&fctx, INFO_POSITION_TOP_RIGHT, FLAG_SECONDARY_INFO_3, bounds, global_date_time, color_icon);
     draw_secondary_info(&fctx, INFO_POSITION_BOTTOM_LEFT, FLAG_SECONDARY_INFO_4, bounds, global_date_time, color_icon);
     draw_secondary_info(&fctx, INFO_POSITION_BOTTOM_RIGHT, FLAG_SECONDARY_INFO_6, bounds, global_date_time, color_icon);
     draw_secondary_info(&fctx, INFO_POSITION_BOTTOM, FLAG_SECONDARY_INFO_7, bounds, global_date_time, color_icon);
     fctx_set_rotation(&fctx, 0); // restoring angle after round texts
       
  
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
        fctx_draw_text(&fctx, s_time, ffont, bounds.size.h * 38/100, center.x, center.y * 116 / 100, GTextAlignmentCenter, FTextAnchorBottom);
        fctx_end_fill(&fctx);
      
         //======================== Secondary Info  ===========================================
        draw_secondary_info(&fctx, INFO_POSITION_INSIDE_CIRCLE_LEFT, FLAG_SECONDARY_INFO_9, bounds, global_date_time, color_icon);
        draw_secondary_info(&fctx, INFO_POSITION_INSIDE_CIRCLE_RIGHT, FLAG_SECONDARY_INFO_10, bounds, global_date_time, color_icon);
   
    } else { // of we're not displaying digital time full screen - show it completely
      
         //======================== Secondary Info  ===========================================
        draw_secondary_info(&fctx, INFO_POSITION_INSIDE_CIRCLE_TOP, FLAG_SECONDARY_INFO_2, bounds, global_date_time, color_icon);
        draw_secondary_info(&fctx, INFO_POSITION_INSIDE_CIRCLE_BOTTOM, FLAG_SECONDARY_INFO_5, bounds, global_date_time, color_icon);
      
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
            fctx_draw_text(&fctx, s_time, ffont, bounds.size.h * 24/100, center.x, center.y, GTextAlignmentCenter, FTextAnchorMiddle);
        }
        fctx_end_fill(&fctx);
        
     
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
      fctx_draw_text(&fctx, s_time, ffont, bounds.size.h * 38/100, center.x, center.y * 100/100, GTextAlignmentCenter, FTextAnchorTop);
      fctx_end_fill(&fctx);
      
    } 
  
}



#endif
