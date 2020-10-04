#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int t1 = 0;
int t2 = 0;
int t3 = 0;

xTaskHandle xT;

void vTestTask1(void *vParameter) {
      // portTickType xLastWakeTime;
	// unsigned portBASE_TYPE uxPriority =  uxTaskPriorityGet( NULL );

   // xLastWakeTime = xTaskGetTickCount();

   while (1) {
    // for (long int i = 0; i < 1000000; ++i);
    write(1, "1\n", 2);
    t1++;
    taskYIELD();
    // vTaskPrioritySet( xT, uxPriority + 1 );
	// vTaskDelay(500 /  portTICK_PERIOD_MS);
	// vTaskDelayUntil( &xLastWakeTime, ( 100 / portTICK_RATE_MS ) );
   }
   vTaskDelete(NULL);
}

void vTestTask2(void *vParameter) {
       // portTickType xLastWakeTime;

   // xLastWakeTime = xTaskGetTickCount();

    while (1) {
    // for (long int i = 0; i < 10000000; ++i) {
        // if(i % 1000000 == 0) {
          t2++;  
          write(1, "2\n", 2);
          taskYIELD();
        // }
    // }
    // vTaskDelay(500 /  portTICK_PERIOD_MS);
          // vTaskDelayUntil( &xLastWakeTime, ( 100 / portTICK_RATE_MS ) );
    }
    vTaskDelete(NULL);
}

void vTestTask3(void *vParameter) {
   // portTickType xLastWakeTime;

   // xLastWakeTime = xTaskGetTickCount();

   while (1) {
    t3++;
    write(1, "3\n", 2);
    taskYIELD();
	// vTaskDelay(500 /  portTICK_PERIOD_MS);
	// vTaskDelayUntil( &xLastWakeTime, ( 100 / portTICK_RATE_MS ) );
   }
   vTaskDelete(NULL);
}

void vTestTask4(void *vParameter) {
   // portTickType xLastWakeTime;

   // xLastWakeTime = xTaskGetTickCount();

   while (1) {
    printf("t1 = %d t2 = %d t3 = %d\n",t1, t2, t3);
    taskYIELD();
    // write(1, "3\n", 2);
	// vTaskDelayUntil( &xLastWakeTime, ( 500 / portTICK_RATE_MS ) );
   }
   vTaskDelete(NULL);
}

void app_main(void) {
    xTaskCreate(vTestTask1, "vTestTask1", 4096, NULL, 2, NULL);
    xTaskCreate(vTestTask2, "vTestTask2", 4096, NULL, 2, NULL);
    // xTaskCreate(vTestTask1, "vTestTask1", 4096, NULL, 1, NULL);
    xTaskCreate(vTestTask3, "vTestTask3", 4096, NULL, 2, NULL);
    xTaskCreate(vTestTask4, "vTestTask4", 4096, NULL, 2, NULL);
    // vTaskStartScheduler();
}
