#ifndef MY_HELPER_FCT_H
#define MY_HELPER_FCT_H

#include <stdio.h>
#include <unistd.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

/* Core constants for xTaskCreatePinnedToCore() function */
static const uint32_t CORE_0 = 0;
static const uint32_t CORE_1 = 1;
static const uint32_t PRO_CPU = 0;
static const uint32_t APP_CPU = 1;

/* ------------------------------------------------------------------------- */
/* ANSI terminal colors - standard (8 colors, supported everywhere)          */
/* ------------------------------------------------------------------------- */
#define COLOR_BLACK         "\033[0;30m"
#define COLOR_RED           "\033[0;31m"
#define COLOR_GREEN         "\033[0;32m"
#define COLOR_YELLOW        "\033[0;33m"
#define COLOR_BLUE          "\033[0;34m"
#define COLOR_MAGENTA       "\033[0;35m"
#define COLOR_CYAN          "\033[0;36m"
#define COLOR_LIGHT_GRAY    "\033[0;37m"

/* ------------------------------------------------------------------------- */
/* ANSI terminal colors - bright (codes 90..97)                              */
/* ------------------------------------------------------------------------- */
#define COLOR_GRAY          "\033[0;90m"
#define COLOR_LIGHT_RED     "\033[0;91m"
#define COLOR_LIGHT_GREEN   "\033[0;92m"
#define COLOR_LIGHT_YELLOW  "\033[0;93m"
#define COLOR_LIGHT_BLUE    "\033[0;94m"
#define COLOR_LIGHT_MAGENTA "\033[0;95m"
#define COLOR_LIGHT_CYAN    "\033[0;96m"
#define COLOR_WHITE         "\033[0;97m"

/* ------------------------------------------------------------------------- */
/* ANSI terminal colors - 256-color palette (modern terminals)               */
/* ------------------------------------------------------------------------- */
#define COLOR_BROWN         "\033[38;5;130m"
#define COLOR_DARK_BROWN    "\033[38;5;94m"
#define COLOR_ORANGE        "\033[38;5;208m"
#define COLOR_DARK_ORANGE   "\033[38;5;166m"
#define COLOR_PINK          "\033[38;5;205m"
#define COLOR_PURPLE        "\033[38;5;93m"
#define COLOR_VIOLET        "\033[38;5;177m"
#define COLOR_GOLD          "\033[38;5;220m"
#define COLOR_OLIVE         "\033[38;5;100m"
#define COLOR_LIME          "\033[38;5;118m"
#define COLOR_DARK_GREEN    "\033[38;5;22m"
#define COLOR_TEAL          "\033[38;5;30m"
#define COLOR_NAVY          "\033[38;5;18m"
#define COLOR_SKY_BLUE      "\033[38;5;117m"
#define COLOR_DARK_GRAY     "\033[38;5;240m"
#define COLOR_SILVER        "\033[38;5;250m"

/* Any color from the 256-color palette, N between 0 and 255 */
#define COLOR_256(N)        "\033[38;5;" #N "m"

/* Any RGB color (true color, 24 bits), each component between 0 and 255 */
#define COLOR_RGB(R, G, B)  "\033[38;2;" #R ";" #G ";" #B "m"

/* ------------------------------------------------------------------------- */
/* Bold variants                                                             */
/* ------------------------------------------------------------------------- */
#define COLOR_B_RED         "\033[1;31m"
#define COLOR_B_GREEN       "\033[1;32m"
#define COLOR_B_YELLOW      "\033[1;33m"
#define COLOR_B_BLUE        "\033[1;34m"

/* ------------------------------------------------------------------------- */
/* Background colors (can be combined with a text color)                     */
/* ------------------------------------------------------------------------- */
#define BG_RED              "\033[41m"
#define BG_GREEN            "\033[42m"
#define BG_YELLOW           "\033[43m"
#define BG_BLUE             "\033[44m"
#define BG_GRAY             "\033[100m"

#define COLOR_RESET         "\033[0m"


/**
 * @brief Macro to display the buffer with arguments. msg is a formatted string as printf() function.
 * 
 */
#define DISPLAY(msg, ...) printf("%lu:%d>\t"msg"\r\n", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);


/**
 * @brief Macro to display the buffer with arguments in a given color.
 *        color can be a COLOR_xxx constant or a const char* variable.
 *        Example: DISPLAYC(COLOR_CYAN, "Value = %d", val);
 */
#define DISPLAYC(color, msg, ...) printf("%s%lu:%d>\t" msg COLOR_RESET "\r\n", (color), xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

/* Shortcuts for the most common colors */
#define DISPLAY_RED(msg, ...)     DISPLAYC(COLOR_RED,     msg, ##__VA_ARGS__)
#define DISPLAY_GREEN(msg, ...)   DISPLAYC(COLOR_GREEN,   msg, ##__VA_ARGS__)
#define DISPLAY_BLUE(msg, ...)    DISPLAYC(COLOR_BLUE,    msg, ##__VA_ARGS__)
#define DISPLAY_YELLOW(msg, ...)  DISPLAYC(COLOR_YELLOW,  msg, ##__VA_ARGS__)
#define DISPLAY_CYAN(msg, ...)    DISPLAYC(COLOR_CYAN,    msg, ##__VA_ARGS__)
#define DISPLAY_MAGENTA(msg, ...) DISPLAYC(COLOR_MAGENTA, msg, ##__VA_ARGS__)


/**
 * @brief Macro to display error/warning/info for the buffer with arguments. msg is a formatted string as printf() function.
 *        Colors (E = red, W = yellow, I = green) are added by ESP-IDF itself
 *        when CONFIG_LOG_COLORS is enabled in menuconfig.
 */
#define DISPLAYE(tag, msg, ...) ESP_LOGE(tag, "%lu:%d>\t"msg"", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

#define DISPLAYW(tag, msg, ...) ESP_LOGW(tag, "%lu:%d>\t"msg"", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

#define DISPLAYI(tag, msg, ...) ESP_LOGI(tag, "%lu:%d>\t"msg"", xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);


/**
 * @brief Same as DISPLAYE/W/I but the message part is displayed in a custom color.
 *        color can be a COLOR_xxx constant or a const char* variable.
 *        The "E (1234) TAG:" prefix keeps the ESP-IDF color, the message uses 'color'.
 *        Example: DISPLAYIC(TAG, COLOR_BLUE, "Connected to %s", ssid);
 */
#define DISPLAYEC(tag, color, msg, ...) ESP_LOGE(tag, "%s%lu:%d>\t" msg COLOR_RESET, (color), xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

#define DISPLAYWC(tag, color, msg, ...) ESP_LOGW(tag, "%s%lu:%d>\t" msg COLOR_RESET, (color), xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);

#define DISPLAYIC(tag, color, msg, ...) ESP_LOGI(tag, "%s%lu:%d>\t" msg COLOR_RESET, (color), xTaskGetTickCount(), xPortGetCoreID(), ##__VA_ARGS__);


/* Consume CPU cycles. Time parameter is for the default frequency */
#if CONFIG_IDF_TARGET_ESP32S3
//ESP32-S3
#define COEFF_CPU_CYCLES 5.85
#else 
// ESP32
#define COEFF_CPU_CYCLES 5.02
#endif

#define COMPUTE_IN_TIME_US(time_us) {register uint32_t ulComputeCnt_ = (uint32_t)((time_us) * COEFF_CPU_CYCLES * 100 / 22); while (ulComputeCnt_-- > 0) {asm(" nop");}}

#define COMPUTE_IN_TIME_MS(time_ms) {register uint32_t ulComputeCnt_ = (uint32_t)((time_ms) * 1000 * COEFF_CPU_CYCLES * 100 / 22); while (ulComputeCnt_-- > 0) {asm(" nop");}}

#define COMPUTE_IN_TICK(tick) {register uint32_t ulComputeCnt_ = (uint32_t)(((tick) * 1000 / configTICK_RATE_HZ) * 1000 * COEFF_CPU_CYCLES * 100 / 22); while (ulComputeCnt_-- > 0) {asm(" nop");}}

#endif