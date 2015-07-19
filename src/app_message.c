#include <pebble.h>
#include <string.h>
#include "qrcode.h"

extern char qrcode[10000];
extern unsigned int qr_width, qr_height;
  
static Window *s_window;	

int heading = 0;
static char buf[100];
static char buf2[100];
static TextLayer *s_time_layer;
RotBitmapLayer *rbl;
static TextLayer *s_name;
static TextLayer *s_time;
static GBitmap *s_res_logo;
static ActionBarLayer *s_actionbar;
static TextLayer *s_orderid;
static GFont s_res_gothic_18_bold;
	
// Key values for AppMessage Dictionary
enum {
	STATUS_KEY = 0,	
	MESSAGE_KEY = 1,
  WALLETCODE_KEY = 2,
  WIDTH_KEY = 3,
  HEIGHT_KEY = 4,
  INRBTCRATE_KEY = 5
};

// Write message to buffer & send
void send_message(void){
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, STATUS_KEY, 0x1);
	
	dict_write_end(iter);
  	app_message_outbox_send();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
	tuple = dict_find(received, STATUS_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %d", (int)tuple->value->uint32); 
	}
	
	tuple = dict_find(received, MESSAGE_KEY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
	}
  
  tuple = dict_find(received, WIDTH_KEY);
  if(tuple) {
    qr_width = (unsigned int)tuple->value->uint32;
  }
  
  tuple = dict_find(received, HEIGHT_KEY);
  if(tuple) {
    qr_height = (unsigned int)tuple->value->uint32;
  }
  
  tuple = dict_find(received, INRBTCRATE_KEY);
  if(tuple) {
    qr_height = (unsigned int)tuple->value->uint32;
  }
  
  tuple = dict_find(received, 11); // customer name
  if(tuple) {
    text_layer_set_text(s_name, tuple->value->cstring);
  }
  
  tuple = dict_find(received, 12); // amount
  if(tuple) {
    
  }
  
  tuple = dict_find(received, 13); // distance
  if(tuple) {
    snprintf(buf, 100, "%u m away", (unsigned int)tuple->value->uint32);
    text_layer_set_text(s_time, buf);
  }
  
  tuple = dict_find(received, 14); // heading
  if(tuple) {
    heading = (unsigned int)tuple->value->uint32;
  }
  
  tuple = dict_find(received, WALLETCODE_KEY);
  if(tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Size: %d %d\n", tuple->length, tuple->type);
    memcpy(qrcode, tuple->value->data, tuple->length);
//     for(int i=0; i<tuple->length; i++) {
//       qrcode[i] = tuple->value->data[i];
//     }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Size: %d %d\n", tuple->length, tuple->type);
    //qr_Start();
  }
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

static GBitmap *arrow;
static void load_resources() {
  arrow = gbitmap_create_with_resource(RESOURCE_ID_ARROW);
}
static void destroy_resources() {
  gbitmap_destroy(arrow);
}
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  qr_Start();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}


static void compass_heading_handler(CompassHeadingData chd) {
  int h = (chd.true_heading);
  rot_bitmap_layer_set_angle(rbl, h + (heading*TRIG_MAX_ANGLE/360));
//   rot_bitmap_layer_increment_angle(rbl, 1);
//   snprintf(buf, 100, "%d", h);
//   text_layer_set_text(s_time_layer, buf);
}
static void main_window_load(Window *window) {
// Create time TextLayer
//   s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
//   text_layer_set_background_color(s_time_layer, GColorClear);
//   text_layer_set_text_color(s_time_layer, GColorBlack);
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_logo = gbitmap_create_with_resource(RESOURCE_ID_BILL);
  // s_actionbar
  s_actionbar = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbar, s_window);
  action_bar_layer_set_background_color(s_actionbar, GColorClear);
  action_bar_layer_set_icon(s_actionbar, BUTTON_ID_SELECT, s_res_logo);
  action_bar_layer_set_click_config_provider(s_actionbar, click_config_provider);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbar);
  rbl = rot_bitmap_layer_create(arrow);
  GRect gr = layer_get_frame((Layer *)rbl);
  gr.origin.x = 35;
  gr.origin.y = 25;
  layer_set_frame((Layer *)rbl, gr);
  s_time = text_layer_create(GRect(0, 110, 144, 30));
  text_layer_set_text(s_time, "20 m away");
  text_layer_set_text_alignment(s_time, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), (Layer *)s_time);
  s_name = text_layer_create(GRect(0, 130, 144, 30));
  text_layer_set_background_color(s_name, GColorBlack);
  text_layer_set_text_color(s_name, GColorWhite);
  text_layer_set_text(s_name, "Md. Nafees");
  text_layer_set_text_alignment(s_name, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), (Layer *)s_name);
//   int h = 1337;
//   snprintf(buf, 100, "%d", h);
//   text_layer_set_text(s_time_layer, buf);
  
  
  // s_orderid
  s_orderid = text_layer_create(GRect(0, 0, 144, 24));
  text_layer_set_background_color(s_orderid, GColorBlack);
  text_layer_set_text_color(s_orderid, GColorWhite);
  text_layer_set_text(s_orderid, "Order #1337");
  text_layer_set_text_alignment(s_orderid, GTextAlignmentCenter);
  text_layer_set_font(s_orderid, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_orderid);
  
  //rot_bitmap_layer_create()

  // Improve the layout to be more like a watchface
//   text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
//   text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), (Layer *)(rbl));
}

static void main_window_unload(Window *window) {
  rot_bitmap_layer_destroy(rbl);
}

void init(void) {
  qr_Init();
  load_resources();
	s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
	window_stack_push(s_window, true);
  
  compass_service_subscribe(compass_heading_handler);
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	send_message();
}

void deinit(void) {
  qr_Destroy();
  destroy_resources();
	app_message_deregister_callbacks();
	window_destroy(s_window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}