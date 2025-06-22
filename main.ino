// Project Author: Dohan Dhananjaya
#include <SPI.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "ui.h"

TFT_eSPI tft = TFT_eSPI();

// LVGL display buffer - Updated for LVGL 9.x
static lv_color_t buf[240 * 10]; // Buffer for 10 lines

// Display flushing - Updated for LVGL 9.x
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    Serial.println("Flush called");
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)px_map, w * h, true);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

void setup(void) 
{
    Serial.begin(115200);
    
    // Initialize TFT
    tft.init();
    tft.setRotation(0); // Adjust rotation as needed
    tft.fillScreen(TFT_BLACK);

    // Initialize LVGL
    lv_init();

    // Initialize the display - Updated for LVGL 9.x
    lv_display_t *disp = lv_display_create(240, 240);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, buf, NULL, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Initialize UI (your SquareLine Studio generated UI)
    ui_init();
    
    Serial.println("LVGL UI Initialized");
}

void loop() 
{
    static unsigned long last_update = 0;
    static int counter = 0;
    lv_tick_inc(5); // Add LVGL tick increment
    lv_timer_handler(); // Handle LVGL tasks
    
    unsigned long now = millis();
    if (now - last_update >= 1000) {
        last_update = now;
        char buf[3];
        snprintf(buf, sizeof(buf), "%02d", counter);
        if (ui_BIGHH == NULL) {
            Serial.println("ui_BIGHH is NULL!");
        } else {
            _ui_label_set_property(ui_BIGHH, _UI_LABEL_PROPERTY_TEXT, buf);
            lv_obj_invalidate(lv_scr_act()); // Force full screen redraw
        }
        counter = (counter + 1) % 100;
        Serial.println(counter);
    }
    delay(5); // Small delay to prevent watchdog issues
}