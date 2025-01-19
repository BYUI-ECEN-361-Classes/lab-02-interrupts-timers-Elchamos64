#include "main.h"
#include "stm32l4xx_it.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MultiFunctionShield.h"
extern TIM_HandleTypeDef htim3;  // Points to the timer structure   Timer3 is the Reaction Timer
extern void MX_TIM3_Init(void);	// To reset the timer
extern bool got_start_button;
extern bool got_stop_button;
extern bool got_fastest_button;
extern int best_reaction_time_in_millisec;

// Globals
#define upper_limit_millisec_to_wait  7000;  // Give the user up to 7 seconds to wait

int rand_millisec;
int last_reaction_time_in_millisec = 0;
bool started_doing_reaction_timers = false;

void show_a_random_number()
{
    if (!started_doing_reaction_timers)
    {
        rand_millisec = rand() % upper_limit_millisec_to_wait;
        MultiFunctionShield_Display(rand_millisec);
        HAL_Delay(2000);  // Delay for 2 seconds before the counter on the 7-Seg display
    }
}

void got_start()
{
    /* Here's the code to do when the Start Button is pushed
        When Start is pressed:
        1.) Display the Waiting "----"
        2.) Wait for a random number of milliseconds
        3.) Turn on all the 7-Seg lights (that's "GO")
        4.) Start the Reaction timer. (Hint: Use the same function you used to start the other timers)
    */
    started_doing_reaction_timers = true;
    Clear_LEDs();
    rand_millisec = rand() % upper_limit_millisec_to_wait;

    /**************** STUDENT TO FILL IN START HERE ********************/

    // Step 1: Display "----"
    Display_Waiting();

    // Step 2: Wait for a random amount of time
    HAL_Delay(rand_millisec);

    // Step 3: Turn on all 7-Seg lights (Display "GO")
    Display_All();

    // Step 4: Start the Reaction timer (timer3)
    HAL_TIM_Base_Start(&htim3);

    /**************** STUDENT TO FILL IN END HERE ********************/
}

void got_stop()
{
    /* Here's the code for the STOP button --
     * When pushed:
     1.) Stop the reaction timer
     2.) Read the value of the timer
     3.) Display the value
     */
    // 1.) Stop the reaction timer (timer3)
    HAL_TIM_Base_Stop(&htim3);

    /**************** STUDENT TO FILL IN START HERE ********************/

    // 2.) Read the value of the timer (in milliseconds)
    last_reaction_time_in_millisec = __HAL_TIM_GetCounter(&htim3) / 10;  // Divide by 10 to convert to milliseconds

    // 3.) Display the reaction time
    MultiFunctionShield_Display(last_reaction_time_in_millisec);

    /**************** STUDENT TO FILL IN END HERE ********************/

    // Keep the best time in a global variable
    if (last_reaction_time_in_millisec < best_reaction_time_in_millisec)
        best_reaction_time_in_millisec = last_reaction_time_in_millisec;

    // Show some stats
    printf("Random Delay was: %d\n\r", rand_millisec);
    printf("Reaction Time from Timer   : %d\n\r", last_reaction_time_in_millisec);

    // Just to keep things random -- reseed with the last reaction time
    srand((unsigned)last_reaction_time_in_millisec);
}

void got_fastest()
{
    got_fastest_button = false;
    MultiFunctionShield_Display(best_reaction_time_in_millisec);
}
