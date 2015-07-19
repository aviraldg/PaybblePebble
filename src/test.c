#include <pebble.h>
#include "test.h"
#include "qrcode.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_logo;
static GFont s_res_gothic_18_bold;
static ActionBarLayer *s_actionbar;
static TextLayer *s_orderid;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_logo = gbitmap_create_with_resource(RESOURCE_ID_LOGO);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_actionbar
  s_actionbar = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbar, s_window);
  action_bar_layer_set_background_color(s_actionbar, GColorClear);
  action_bar_layer_set_icon(s_actionbar, BUTTON_ID_SELECT, s_res_logo);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbar);
  
  // s_orderid
  s_orderid = text_layer_create(GRect(0, 0, 144, 20));
  text_layer_set_background_color(s_orderid, GColorBlack);
  text_layer_set_text_color(s_orderid, GColorWhite);
  text_layer_set_text(s_orderid, "Order #1337");
  text_layer_set_text_alignment(s_orderid, GTextAlignmentCenter);
  text_layer_set_font(s_orderid, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_orderid);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(s_actionbar);
  text_layer_destroy(s_orderid);
  gbitmap_destroy(s_res_logo);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
  qr_Destroy();
}

void show_test(void) {
  initialise_ui();
  qr_Init();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_test(void) {
  window_stack_remove(s_window, true);
}
