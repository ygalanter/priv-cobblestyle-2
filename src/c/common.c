
#include <pebble.h>
#include "common.h"


extern uint8_t *buffer;

// creates ffont with predefined buffer
static FFont* ffont_create_from_resource_with_buffer(uint32_t resource_id, uint8_t* buffer){
    ResHandle rh = resource_get_handle(resource_id);
    size_t rs = resource_size(rh);
    if (buffer) {
        resource_load(rh, buffer, rs);
        return (FFont*)buffer;
    }
    return NULL;    
}

// allocates buffer and loads ffont into it
FFont* ffont_load_from_resource(uint32_t resource_id) {
  if (!buffer) { // if buffer isn't created yet - create it
    buffer = malloc(FONT_BUFFER_SIZE);
  }
  FFont* ffont =  ffont_create_from_resource_with_buffer(resource_id, buffer);  
  
  return ffont;
  
}


// dowing own text drawing without UTF8 decoding - for cases where string is *already* decoded
static void fctx_draw_string_no_decoding(FContext* fctx, const char* text, FFont* font, GTextAlignment alignment, FTextAnchor anchor) {

    FPoint advance = {0, 0};
    const char* p;
    FGlyph* glyph;
    fixed_t width = 0;

    if (alignment != GTextAlignmentLeft) {
        
        for (p = text; *p; ++p) {
               if (*p == 1)  // if first byte is one - it's a 2-byte character outside Latin range - decode it below
                  {glyph = ffont_glyph_info(font, ((uint16_t)(*p) << 8) | *(p + 1)); p++;}
                else
                  {glyph = ffont_glyph_info(font, *p);}
          
                if (glyph) {
                    width += glyph->horiz_adv_x;
                }
        }
        if (alignment == GTextAlignmentRight) {
            advance.x = -width;
        } else /* alignment == GTextAlignmentCenter */ {
            advance.x = -width / 2;
        }
    }

    if (anchor == FTextAnchorBaseline) {
        advance.y = 0;
    } else if (anchor == FTextAnchorMiddle) {
        advance.y = -font->ascent / 2;
    } else if (anchor == FTextAnchorTop) {
        advance.y = -font->ascent;
    } else /* anchor == FTextAnchorBottom */ {
        advance.y = -font->descent;
    }

    for (p = text; *p; ++p) {
             FGlyph* glyph;
                if (*p == 1)  // if first byte is one - it's a 2-byte character outside Latin range - decode it below
                  {glyph = ffont_glyph_info(font, ((uint16_t)(*p) << 8) | *(p + 1)); p++;}
                else
                  {glyph = ffont_glyph_info(font, *p);}
      
            if (glyph) {
                void* path_data = ffont_glyph_outline(font, glyph);
                fctx_draw_commands(fctx, advance, path_data, glyph->path_data_length);
                advance.x += glyph->horiz_adv_x;
            }
    }
}


// Draws antialiased text with given font, size, position and allignment
void fctx_draw_text(FContext *fctx, char * text, FFont* ffont, int16_t size, int16_t x, int16_t y, GTextAlignment text_align, FTextAnchor text_anchor) {
  fctx_set_offset(fctx, FPointI(x,y));
  fctx_set_text_size(fctx, ffont, size);
  fctx_draw_string_no_decoding(fctx, text, ffont, text_align, text_anchor);
}


FPath* fpath_create_from_resource_with_buffer(uint32_t resource_id, uint8_t *buffer) {
    ResHandle res_handle = resource_get_handle(resource_id);
    size_t res_size = resource_size(res_handle);
    FPath* fpath = (FPath *)buffer;
    if (fpath) {
        fpath->size = (int16_t)res_size;
        resource_load(res_handle, fpath->data, res_size);
        return fpath;
    }
    return NULL;
}


// This function decodees 2-bytes unicode chars and converts them to uppercase
void utf_decode_to_upper(char *sPtr) {  
    char *Result = sPtr;
  
    while(*sPtr != 0) {
        
         //******** decoding possible unicode char
         uint16_t decode_state = 0;   
         uint16_t  decoded_result = 0;
         do {  
            decode_utf8_byte(*sPtr, &decode_state, &decoded_result);
            if (decode_state>0 && decode_state<6 ) {++sPtr;}
        } while (decode_state !=0 && decode_state !=6 );
         //******************************************   
      
         // own implementation of toupper
      if (decode_state == 0) {
        
            if ( (decoded_result > 96 && decoded_result < 123) || (decoded_result > 223 && decoded_result < 256) )
               *Result = decoded_result - 32;
            else if (decoded_result > 256) // if it's a 2-byte character outside Latin range - encode it into 2 chars below 
                // (if it's a lowercase (odd code) also substracting 1 to make it capital)
                {*Result = (decoded_result - decoded_result % 2) >> 8; ++Result; *Result = (decoded_result - decoded_result % 2) & 0xFF; }
            else
                *Result = decoded_result;
       }
      
         ++Result;
         ++sPtr;
    }
   
    *Result = 0;
}

//************************ Localizing stuff *********************

#ifndef PBL_PLATFORM_APLITE

void translate_full_day_name(uint8_t day_no, char *translation) {
    switch (day_no){
      case 0: strcpy(translation, _("Sunday")); break;
      case 1: strcpy(translation, _("Monday")); break;
      case 2: strcpy(translation, _("Tuesday")); break;
      case 3: strcpy(translation, _("Wednesday")); break;
      case 4: strcpy(translation, _("Thursday")); break;
      case 5: strcpy(translation, _("Friday")); break;
      case 6: strcpy(translation, _("Saturday")); break;    
    }
}

#ifdef PBL_RECT
void translate_short_day_name(uint8_t day_no, char *translation) {
    switch (day_no){
      case 0: strcpy(translation, _("Sun")); break;
      case 1: strcpy(translation, _("Mon")); break;
      case 2: strcpy(translation, _("Tue")); break;
      case 3: strcpy(translation, _("Wed")); break;
      case 4: strcpy(translation, _("Thu")); break;
      case 5: strcpy(translation, _("Fri")); break;
      case 6: strcpy(translation, _("Sat")); break;    
    }
}
#endif


void translate_short_month_name(uint8_t month_no, char *translation) {
    switch (month_no){
      case 0: strcpy(translation, _("Jan")); break;
      case 1: strcpy(translation, _("Feb")); break;
      case 2: strcpy(translation, _("Mar")); break;
      case 3: strcpy(translation, _("Apr")); break;
      case 4: strcpy(translation, _("May")); break;
      case 5: strcpy(translation, _("Jun")); break;
      case 6: strcpy(translation, _("Jul")); break;    
      case 7: strcpy(translation, _("Aug")); break;
      case 8: strcpy(translation, _("Sep")); break;
      case 9: strcpy(translation, _("Oct")); break;
      case 10: strcpy(translation, _("Nov")); break;
      case 11: strcpy(translation, _("Dec")); break;
    
    }
}

void set_language(uint8_t language_id) {
    
    //localize_deinit();
  
    if (language_id != LANGUAGE_SYSTEM) {
      
       //setting localize cache for 8 strings
       localize_set_cache_size(8);
      
       uint32_t resource_id = 0;
          
        switch (language_id) {
          case LANGUAGE_NORWEGIAN:  resource_id = RESOURCE_ID_LOC_NORWEGIAN; break;
          case LANGUAGE_HUNGARIAN:  resource_id = RESOURCE_ID_LOC_HUNGARIAN; break;
          case LANGUAGE_SWEDISH:  resource_id = RESOURCE_ID_LOC_SWEDISH; break;
          case LANGUAGE_CATALAN:  resource_id = RESOURCE_ID_LOC_CATALAN; break;
          case LANGUAGE_DUTCH:  resource_id = RESOURCE_ID_LOC_DUTCH; break;
          
        }
      
        localize_init(resource_id);
    } 
  
}

                                    
#endif