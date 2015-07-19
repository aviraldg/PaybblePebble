#include <pebble.h>
#include "qrcode.h"

char qrcode[10000];
unsigned int qr_width, qr_height;
  
static Window *s_window;
static Layer *s_qrcode;
static TextLayer *s_info;
static GFont s_res_gothic_18_bold;

#define SF (4)
#define SW (144)
#define SH (168)

void qr_UpdateLayer(Layer *layer, GContext *ctx) {
  int wo = (SW - qr_width*SF)/2;
  int ho = (SH - qr_height*SF)/2;
  for(unsigned int i=0; i<qr_height*SF; i++) {
    for(unsigned int j=0; j<qr_width*SF; j++) {
      if(qrcode[(i/SF)*qr_width + (j/SF)])
        graphics_draw_pixel(ctx, GPoint(wo + i, ho + j));
    }
  }
}

void qr_Init() {
  s_window = window_create();
  
  s_qrcode = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(s_qrcode, qr_UpdateLayer);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_qrcode);
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  
  s_info = text_layer_create(GRect(0, 0, 144, 24));
  text_layer_set_background_color(s_info, GColorBlack);
  text_layer_set_text_color(s_info, GColorWhite);
  text_layer_set_text(s_info, "INR 114 | 0.007 BTC");
  text_layer_set_text_alignment(s_info, GTextAlignmentCenter);
  text_layer_set_font(s_info, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_info);
}

void qr_Start() {
  window_stack_push(s_window, true);
}

void qr_Destroy() {
  window_destroy(s_window);
}