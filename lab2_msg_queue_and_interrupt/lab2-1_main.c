#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "esp_log.h"

/* FreeRTOS.org includes. */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "my_helper_fct.h"

static const char* TAG = "MsgQ_MAIN";

/* The task functions declaration */


/* Message Queue declaration */


void app_main(void) {

	/* Create Message Queue */
	
	if (xMsgQ == NULL) {
		DISPLAYEC(TAG, COLOR_RED, "MsgQ cannot be created.");
		return;
	}

	/* Stop Scheduler */
	vTaskSuspendAll();

	/* Create 3 tasks. */


	/* Continue Scheduler */
	xTaskResumeAll();	

	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters) {

}

void vTask2(void *pvParameters){

}

void vTask3(void *pvParameters) {

}

