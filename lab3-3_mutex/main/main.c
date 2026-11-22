/****************************************************************************
 * Copyright (C) 2020 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file lab3-1_main.c
 * @author Fabrice Muller
 * @date 6 Oct. 2026
 * @brief File containing the lab3-1 of Part 3.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include "esp_log.h"

/* FreeRTOS.org includes. */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "my_helper_fct.h"

static const char *TAG = "SEM";

/* Application constants */
#define STACK_SIZE 4096
#define TABLE_SIZE 4300

/* Task Priority */
const uint32_t TIMER_TASK_PRIORITY = 5;
const uint32_t INC_TABLE_TASK_PRIORITY = 3;
const uint32_t DEC_TABLE_TASK_PRIORITY = 3;
const uint32_t INSPECTOR_TASK_PRIORITY = 4;

/* Communications */
//SemaphoreHandle_t xSemClk;
SemaphoreHandle_t xSemIncTab;
SemaphoreHandle_t xSemDecTab;
SemaphoreHandle_t xSemMutex;

/* Tasks */
void vTaskTimer(void *pvParameters);
void vTaskIncTable(void *pvParameters);
void vTaskDecTable(void *pvParameters);
void vTaskInspector(void *pvParameters);


/* constNumber for Inc Table */
const uint32_t constNumber = 10;

/* Datas */
int Table[TABLE_SIZE];

/* Main function */
void app_main(void) {

	/* Init Table */
	for(int i=0; i<TABLE_SIZE; i++){
		Table[i] = i;
	}

	/* Create semaphore */
	xSemIncTab = xSemaphoreCreateBinary();
	xSemDecTab = xSemaphoreCreateBinary();
	xSemMutex = xSemaphoreCreateMutex();


	if(xSemIncTab == NULL || xSemDecTab == NULL || xSemMutex == NULL) {
		DISPLAYEC(TAG, COLOR_RED, "Semaphore cannot be created");
		return;
	}

	/* Stop scheduler */
	vTaskSuspendAll();

	/* Create Tasks */
	xTaskCreatePinnedToCore(vTaskTimer, "vTaskTimer", STACK_SIZE, (void *)COLOR_PURPLE, TIMER_TASK_PRIORITY, NULL, CORE_0);
	xTaskCreatePinnedToCore(vTaskIncTable, "vTaskIncTable", STACK_SIZE, (void *)COLOR_OLIVE, INC_TABLE_TASK_PRIORITY, NULL, CORE_0);
	xTaskCreatePinnedToCore(vTaskDecTable, "vTaskDecTable", STACK_SIZE, (void *)COLOR_MAGENTA, DEC_TABLE_TASK_PRIORITY, NULL, CORE_0);
	xTaskCreatePinnedToCore(vTaskInspector, "vTaskInspector", STACK_SIZE, (void *)COLOR_ORANGE, INSPECTOR_TASK_PRIORITY, NULL, CORE_0);

	
	/* Continue scheduler */
	xTaskResumeAll();

	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

void vTaskTimer(void *pvParameters) {
	const char *pcTaskName = pcTaskGetName(NULL);
	const char *pcColor = (pvParameters!=NULL) ? (const char *)pvParameters : "";
	TickType_t xLastWakeTime = xTaskGetTickCount();

	for( ; ; ){
		

		COMPUTE_IN_TIME_MS(20);
		DISPLAYC(pcColor, "%s : give first Clk", pcTaskName);	
		xSemaphoreGive(xSemIncTab);

		DISPLAYC(pcColor, "%s : give second Clk", pcTaskName);
		xSemaphoreGive(xSemDecTab);
		
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
	}


}

void vTaskIncTable(void *pvParameters) {
	const char *pcTaskName = pcTaskGetName(NULL);
	const char *pcColor = (pvParameters!=NULL) ? (const char *)pvParameters : "";
	uint32_t ActivationNumber = 0, index;
	

	for ( ; ; )
	{
		xSemaphoreTake(xSemIncTab, portMAX_DELAY);

		if(ActivationNumber==0) {
			DISPLAYC(pcColor, "%s is starting computation", pcTaskName);
			xSemaphoreTake(xSemMutex, portMAX_DELAY);	
			for(index=0; index<TABLE_SIZE; index++){
				COMPUTE_IN_TIME_US(12);
				Table[index] = Table[index] + constNumber;
			}
			xSemaphoreGive(xSemMutex);
			ActivationNumber = 4;
			DISPLAYC(pcColor, "%s  is ending computation", pcTaskName);	

		} else {
			ActivationNumber -= 1;
			DISPLAYC(pcColor, "Task %s is decrementing NbActivations : %lu", pcTaskName, ActivationNumber);	

		}
	}
	
	
}

void vTaskDecTable(void *pvParameters) {
	const char *pcTaskName = pcTaskGetName(NULL);
	const char *pcColor = (pvParameters!=NULL) ? (const char *)pvParameters : "";
	uint32_t index;

	for( ; ;) {
		xSemaphoreTake(xSemDecTab, portMAX_DELAY);

		DISPLAYC(pcColor, "%s is starting computation", pcTaskName);
		xSemaphoreTake(xSemMutex, portMAX_DELAY);
		for(index=0; index<TABLE_SIZE; index++){
			COMPUTE_IN_TIME_US(12);
			Table[index] -= 1;
		}
		xSemaphoreGive(xSemMutex);
		DISPLAYC(pcColor, "%s is ending computation", pcTaskName);
		
	}

}

void vTaskInspector(void *pvParameters) {
	const char *pcTaskName = pcTaskGetName(NULL);
	const char *pcColor = (pvParameters!=NULL) ? (const char *)pvParameters : "";
	int reference, index, refPlusIndex;
	
	for( ; ;){
		DISPLAYC(pcColor, "%s is checking", pcTaskName);
		xSemaphoreTake(xSemMutex, portMAX_DELAY);
		reference=Table[0];
		for(index=1; index<TABLE_SIZE; index++){
			COMPUTE_IN_TIME_US(10);
			refPlusIndex = reference+index;
			if(Table[index]!=refPlusIndex){
				DISPLAYC(COLOR_RED, "Consistency error in the Table variable at index %d : value = %d, expected = %d", index, Table[index], refPlusIndex);
				exit(1);
			}
		}
		xSemaphoreGive(xSemMutex);
		DISPLAYC(pcColor, "Task Inspector ended its checking.");
		vTaskDelay(pdMS_TO_TICKS(20));
	}

}
