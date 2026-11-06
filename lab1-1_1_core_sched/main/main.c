/****************************************************************************
 * Copyright (C) 2020 by Fabrice Muller                                     *
 *                                                                          *
 * This file is useful for ESP32 Design course.                             *
 *                                                                          *
 ****************************************************************************/

/**
 * @file lab1-1_main.c
 * @author Fabrice Muller
 * @date 9 Sept. 2026
 * @brief File containing the lab1-1 of Part 3.
 *
 * @see https://github.com/fmuller-pns/esp32-vscode-project-template
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "esp_log.h"
#include "esp_task_wdt.h"

/* FreeRTOS.org includes. */
#include "freertos/FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_freertos_hooks.h"

#include "my_helper_fct.h"

/* Default stack size for tasks */
static const uint32_t STACK_SIZE = 4000;

/* Task Priority */
static const uint32_t T1_PRIO = 5;
static const uint32_t T2_PRIO = 6;

/* Used as a loop counter to create a very crude delay. */
#define mainDELAY_LOOP_COUNT		(0x283000)

/* The task function. */
void vTaskFunction(void *pvParameters);

void app_main(void) {
	
	DISPLAY_CYAN("Start of app_main task, priority = %d",uxTaskPriorityGet(NULL));

	vTaskSuspendAll();

	/* Create one of the two tasks. */
	xTaskCreate(vTaskFunction,	/* Pointer to the function that implements the task. */
		"Task 1",				/* Text name for the task.  This is to facilitate debugging only. */
		STACK_SIZE,				/* Stack depth - most small microcontrollers will use much less stack than this. */
		(void *)COLOR_PURPLE,	/* Pass the color to be printed in as the task parameter. */
		T1_PRIO,				/* This task will run at priority 1. */
		NULL);					/* We are not using the task handle. */

	xTaskCreate(vTaskFunction, "Task 2", STACK_SIZE, (void *)COLOR_CYAN, T2_PRIO, NULL);

	xTaskResumeAll();

	DISPLAY_CYAN("==== Exit APP_MAIN ====");
	
	/* to ensure its exit is clean */
	vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

void vTaskFunction(void *pvParameters) {
    /* Name of the calling task */
	const char *pcTaskName = pcTaskGetName(NULL);	

	/* Color passed at task creation (empty string = default terminal color) */
	const char *pcColor = (pvParameters != NULL) ? (const char *)pvParameters : "";

	volatile uint32_t ul;

	DISPLAYC(pcColor, "Start of %s task, priority = %d", pcTaskName, uxTaskPriorityGet(NULL));

	/* As per most tasks, this task is implemented in an infinite loop. */
	for (;; ) {
		DISPLAYC(pcColor, "Running %s", pcTaskName);

		/* Delay for simulating a computation */
		for (ul = 0; ul < mainDELAY_LOOP_COUNT; ul++){		
		}
		vTaskDelay(300 / portTICK_PERIOD_MS);
	}
}


