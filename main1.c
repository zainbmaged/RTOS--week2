/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"
#include "semphr.h"

/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

TaskHandle_t String_100_Handler = NULL;
TaskHandle_t String_500_Handler = NULL;
SemaphoreHandle_t xSemaphore = NULL;

/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/

TickType_t start_timer;

signed  char s1[8] = "string1 ";//string to be sent in task1
unsigned short size1 =8;
signed  char s2[8] = "string2 ";//string to be sent in task2
unsigned short size2 =8;

/* Task1  send string1 to uart 10 times each 500ms */
void String_500( void * pvParameters )
{	
  int m ,n, l;//intial variables for loops

     for( ;; )
     {
			 
			  if( xSemaphore != NULL )
    {
        /* See if we can obtain the semaphore.  If the semaphore is not
        available wait 10 ticks to see if it becomes free. */
        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 0 ) == pdTRUE )
        {
            /* We were able to obtain the semaphore and can now access the
            shared resource. */
	        vTaskDelay(500);//delay for task cycle 500ms
		//loop to send string1 to uart 10 times
		for(n=0;n<10;n++){
		vSerialPutString(s1,size1);
		for(m=0;m<100000;m++){}//loop to represent heavy load 
		}
						
		 vTaskDelay(500);//delay for task cycle 500 ms
             /* We have finished accessing the shared resource.  Release the
            semaphore. */
             xSemaphoreGive( xSemaphore );
	    for(l=0;l<10000000;l++){}//wait to prevent same task from taking semaphore again
        }
        else
        {
            /* We could not obtain the semaphore and can therefore not access
            the shared resource safely. */
        }
    }
        
			
			 
     }

    
				
}
/* Task2  send string2 to uart 10 times each 100ms */
void String_100( void * pvParameters )
{	
  int i ,j ,k;//intial variables for loops

     for( ;; )
     {
			
			  if( xSemaphore != NULL )
    {
        /* See if we can obtain the semaphore.  If the semaphore is not
        available wait 10 ticks to see if it becomes free. */
        if( xSemaphoreTake( xSemaphore, ( TickType_t ) 1) == pdTRUE )
        {
            /* We were able to obtain the semaphore and can now access the
            shared resource. */
		vTaskDelay(100);//delay for task cycle 100ms
		//loop to send string1 to uart 10 times
		for(j=0;j<10;j++){
						
		vSerialPutString(s2,size2);
		for(i=0;i<100000;i++){}//loop to allow time for writing on uart 
						
		}
				
		vTaskDelay(100);//delay for task cycle 100ms
	    xSemaphoreGive( xSemaphore );/* We have finished accessing the shared resource.  Release the
            semaphore. */
	    for(k=0;k<10000000;k++){}//wait to prevent same task from taking semaphore again
            
           
        }
        else
        {
            /* We could not obtain the semaphore and can therefore not access
            the shared resource safely. */
        }
    }
       
			 
   }

    
				
}				
 



/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
  
	
    /* Create Tasks here */
	

			
	
										
  //send string1 to uart every 500ms
				xTaskCreate(
                    String_500,       /* Function that implements the task. */
                    "String_500",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &String_500_Handler );      /* Used to pass out the created task's handle. */		
										 //Toggle LED PIN2 each 0.5 second
	// send string2 to uart every 100ms 
				xTaskCreate(
                    String_100,       /* Function that implements the task. */
                    "String_100",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    2,/* Priority at which the task is created. */
                    &String_100_Handler );      /* Used to pass out the created task's handle. */		
										 //Toggle LED PIN2 each 0.5 second
			
   
				/* Create a mutex type semaphore. */
   xSemaphore = xSemaphoreCreateMutex();

   if( xSemaphore != NULL )
   {
       /* The semaphore was created successfully and
       can be used. */
   }			
										
     
			  
					

	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/
