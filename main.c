#include <string.h>
#include <stdlib.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/**
 * Brief:
 * This code shows how to configure a simple Push Button to perform Single/Double/Triple Tap as well as Short/Long Press
 *
 * GPIO status:
 * GPIO34   : input ( externally pulled up )
 *
 * Test:
 * Connect GPIO34 with Tactile Push Button and ground
 */

#define GPIO_INPUT 34

// Task to perform Single/Double/Triple and Short/Long Press
void button_task(void *pvParameters)
{

    long int time_current = 0;      //++ To get current ESP time ( in micro-seconds )
    long int time_released = 0;     //++ To get ESP time when button is released ( in micro-seconds )
    long int time_pressed = 0;      //++ To get ESP time when button is pressed ( in micro-seconds )
    long int current_time2 = 0;

    int FLAG = 0;
    int button_state = 0;           //++ To enter conditional loop only once for taps and short press
    int button_press_count = 0;     //++ To track number of taps on the push button
    int long_button_state = 0;      //++ To enter conditional loop only once for long press

    while (1)
    {
        int bits = gpio_get_level(GPIO_INPUT);      //++ Continuously read the  defined GPIO pin ( push button )

        if (bits == 1 && FLAG == 0)                 //++ Enter button released state only once
        {
            time_released = esp_timer_get_time();
            FLAG = 1;
            button_state = 1;
            long_button_state = 0;
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        else if (bits == 0 && FLAG == 1)            //++ Enter button pressed state only once
        {
            time_pressed = esp_timer_get_time();
            long_button_state = 1;
            FLAG = 0;
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
        time_current = esp_timer_get_time();        //++ Continuously get current ESP32 time 

        if ((time_released - time_pressed < 1000000) && button_state == 1)      //++ Condition to check for any taps
        {
            current_time2 = esp_timer_get_time();
            if (button_press_count == 0)                                        //++ Single Tap Condition
            {
                // printf("SINGLE TAP\n");
                button_press_count = 1;
            }
            else if (button_press_count == 1)                                   //++ Double Tap Condition
            {
                // printf("DOUBLE TAP\n");
                button_press_count = 2;
            }
            else if (button_press_count == 2)                                   //++ Triple Tap Condition
            {
                // printf("TRIPLE TAP\n");
                button_press_count = 3;
            }
            button_state = 0;
        }
        else if ((time_released - time_pressed >= 1000000) && (time_released - time_pressed <= 3000000) && button_state == 1)   //++ Condition to check for Short Press
        {
            printf("SHORT PRESS\n");
            button_state = 0;
        }
        else if ((time_current - time_pressed > 3000000) && long_button_state == 1)     //++ Condition to check for Long Press
        {
            printf("LONG PRESS\n");
            while(1)
            {
                bits = gpio_get_level(GPIO_INPUT);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                if(bits==1)                                                             //++ Reset all the conditions after Long Press
                {
                    long_button_state = 0;
                    button_state = 0;
                    FLAG = 1;
                    break;
                }
            }
        }

        if (time_current - current_time2 > 1000000 && button_press_count > 0)           //++ Condition to determine Tap Counts
        {
            printf("TAP COUNT : %d\n", button_press_count);
            button_press_count = 0;
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


void app_main() {
    gpio_reset_pin(GPIO_INPUT);

    /* Set the GPIO direction */
    gpio_set_direction(GPIO_INPUT, GPIO_MODE_INPUT);

    xTaskCreate(button_task, "button_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}

