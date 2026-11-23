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
static const int STACK_SIZE = 4096;
static const int T1_PRIO = 2; 
TaskHandle_t vtask1Handle = NULL;

/* The task functions declaration */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters); 

/* Message Queue declaration */
QueueHandle_t xMsgQ;

void app_main(void) {

	/* Create Message Queue */
	xMsgQ = xQueueCreate(5, sizeof(uint32_t));

	if (xMsgQ == NULL) {
		DISPLAYEC(TAG, COLOR_RED, "MsgQ cannot be created.");
		return;
	}

	/* Stop Scheduler */
	vTaskSuspendAll();

	/* Create 3 tasks. */
	xTaskCreatePinnedToCore(vTask1, "vTask1", STACK_SIZE, (void *)COLOR_PURPLE, T1_PRIO, &vtask1Handle, CORE_0);

	/* Continue Scheduler */
	xTaskResumeAll();	

	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters) {
	const char *pcTaskName = pcTaskGetName(NULL);
	const char *pcColor = (pvParameters!=NULL) ? pvParameters : "";
	TickType_t lastWake = xTaskGetTickCount();
	uint32_t value=0;
	
	for( ; ; ) {
		value++;
		uint32_t result = xQueueSend(xMsgQ, &value, 0);
		if(result) {
			DISPLAYC(pcColor, " %s , added message to MsgQ : %lu", pcTaskName, value);
		} else {
			DISPLAYC(COLOR_RED, " Failed to post message");
		}
		COMPUTE_IN_TIME_MS(40);
		vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(500));
	}

}

void vTask2(void *pvParameters){
	const char *pcTaskName = 
}

void vTask3(void *pvParameters) {

}
