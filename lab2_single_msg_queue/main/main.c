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
static const uint32_t T1_PRIO = 2;
static const uint32_t T2_PRIO = 2; 
static const uint32_t T3_PRIO = 3;

static const uint32_t STACK_SIZE = 4000;

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
	xTaskCreatePinnedToCore(vTask1, "Task 1", STACK_SIZE, (void *)COLOR_PURPLE, T1_PRIO, NULL, CORE_0);
	xTaskCreatePinnedToCore(vTask2, "Task 2", STACK_SIZE, (void*)COLOR_B_GREEN, T2_PRIO, NULL, CORE_0);
	xTaskCreatePinnedToCore(vTask3, "Task 3", STACK_SIZE, (void*)COLOR_ORANGE, T3_PRIO, NULL, CORE_0);
	/* Continue Scheduler */
	xTaskResumeAll();	

	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

void vTask1(void *pvParameters) {
	const char *pcTaskName = pcTaskGetName(NULL);
	const char  *pcColor = (pvParameters !=NULL) ? (const char *)pvParameters : "";

	TickType_t xLastWakeTime = xTaskGetTickCount();
	uint32_t value = 0;

	DISPLAYC(pcColor, "Start of %s task, priority = %u", pcTaskName, uxTaskPriorityGet(NULL));


	for ( ; ; )
	{	
		value++;
		if(xQueueSend(xMsgQ, &value, 0) == pdPASS) {
			DISPLAYC(pcColor, "%s added amessage to MsgQ : %lu ", pcTaskName, value);
		} else {
			DISPLAYC(COLOR_RED, " Failed to post message to MsgQ ");
		}
		COMPUTE_IN_TIME_MS(40)

		//Attente de 500 ms pour la période 
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
	}
	

}

void vTask2(void *pvParameters){
	const char *pcTaskName = pcTaskGetName(NULL);
	const char *pcColor = (pvParameters != NULL) ? (const char *)pvParameters : "";
	uint32_t value; 

	for( ; ; ){							     
		if (xQueueReceive(xMsgQ, &value, portMAX_DELAY) == pdPASS)
		{
			DISPLAYC(pcColor, "%s , mess = %lu", pcTaskName, value);
			COMPUTE_IN_TIME_MS(30);
		}	
	}

}

void vTask3(void *pvParameters) {
	const char *pcTaskName = pcTaskGetName(NULL);
	const char *pcColor = (pvParameters != NULL) ? (const char *)pvParameters : "";

	for ( ; ; )
	{
		DISPLAYC(pcColor, "%s waiting ...", pcTaskName);
		vTaskDelay(pdMS_TO_TICKS(100));
		DISPLAYC(pcColor, "%s Computation", pcTaskName );
		COMPUTE_IN_TIME_MS(20);

	}
	

}
