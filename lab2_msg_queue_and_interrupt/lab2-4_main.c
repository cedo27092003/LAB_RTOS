#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "esp_log.h"
#include "driver/gpio.h"

/* FreeRTOS.org includes. */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "my_helper_fct.h"

static const char* TAG = "MsgQ_MAIN";

// Push button for interrupt
static const gpio_num_t PIN_PUSH_BUTTON = 15;

// Led
static const gpio_num_t PIN_LED = 2;

/* Stack size for all tasks */
const uint32_t TASK_STACK_SIZE = 4096;

/* The task functions and interrupt */


/* Message Queue declaration : xMsgQ */


/* Count ISR interrupt global variable */ 


void app_main(void) {

	// Config GPIO for Button
	gpio_config_t config_in;
	config_in.intr_type = GPIO_INTR_NEGEDGE;	// falling edge interrupt
	config_in.mode = GPIO_MODE_INPUT;			// Input mode (push button)
	config_in.pull_down_en = false;
	config_in.pull_up_en = true;						// Pull-up resistor
	config_in.pin_bit_mask = (1ULL<<PIN_PUSH_BUTTON);	// Pin number
	gpio_config(&config_in);

	// Config GPIO for LED
	gpio_config_t config_out;
	config_out.intr_type = GPIO_INTR_DISABLE;
	config_out.mode = GPIO_MODE_OUTPUT;
	config_out.pin_bit_mask = (1ULL<<PIN_LED);
	gpio_config(&config_out);

	/* Create Message Queue : xMsgQ */


	/* Create task, priority 2 : vCounterTask*/
					

	/* Install ISR */


	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}

void vCounterTask(void *pvParameters) {
	uint32_t uxCount = 0;
	for (;; ) {

		// Wait for queue message (xMsgQ) with timout 5 sec. otherwise DISPLAY a message to push it

    	if ( ??? ) {

			// Increment counter
			uxCount++;
    		
			// DISPLAYC macro (COLOR_PURPLE) "Button pushed" and the isrCount variable


			// Get the number of items (messages) in the queue
			

			// DISPLAYC macro (COLOR_RED) number of items (messages) IF greater than 1
		

			// Waiting for push button signal is 1 again (test every 20 ms using vTaskDelay)


			// DISPLAY (COLOR_PURPLE) "Button released"
			DISPLAYC(COLOR_PURPLE, "Button released");

			// Turn on/off the LED
			gpio_set_level(PIN_LED, (uxCount % 2));

			// Waiting for push button signal is 0 again (test every 20 ms using vTaskDelay)

		}
		else {
			// Time out
			DISPLAYC(COLOR_YELLOW, "Please, push the button!");
		}
		
	}
}


static void IRAM_ATTR Push_button_isr_handler(void *args) {

	// Get the pin number of the push button from the argument
    gpio_num_t pinButtonNumber = (gpio_num_t)args;
    
	// Increment isrCount (global variable)

	// Send message from ISR to the queue (arg: pinButtonNumber)

}

