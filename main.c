/**
 * Embedded software for the Texas Instruments TivaTM TM4C1294NCPT
 * micro-controller based traffic light controller
 *
 * By Rustenis Tolpeznikas and Obed Oyandut
 */

#include "inc/tm4c1294ncpdt.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/*******************************************************************************************/

/*******************************************************************************************/
bool main_ped_flag = false; // button pressed by main road pedestrians
bool side_ped_flag = false; // button pressed by side road pedestrians
bool car_sensor_flag = false; // side road car sensor
bool in_motion = false; // used to avoid flag abusers

int traffic_mode = -1;
int light_state = 0;
int ticks = 0; // Counting timeouts
int next_tick = 1; // Sets time out

void button_handler();
void standard_mode(bool);
void night_mode();
void clear_lights();

void set_main_lights(int);
void set_side_lights(int);
void set_night_lights(int);
/*******************************************************************************************/
// Configuration
/*******************************************************************************************/

/**
 * Enables Ports K, L and M
 * K is used for Road Lights
 * L is used for Pedestrian Lights
 * M is used for Button Inputs
 */
void configure_led()
{
    SYSCTL_RCGCGPIO_R = 0x00000E00; // switch on clock for Port K, L, M

    while((SYSCTL_PRGPIO_R & 0x00000E00) != 0x00000E00);

    GPIO_PORTK_DEN_R = 0xE7; //Main Road: G7 Y6 R5, Side Road: G2 Y1 R0
    GPIO_PORTK_DIR_R = 0xE7;

    GPIO_PORTL_DEN_R = 0x33; //Main Ped: G5, R4, Side Ped: G1, R1
    GPIO_PORTL_DIR_R = 0x33;

    GPIO_PORTM_DEN_R = 0x0F; //enable pin M1
    GPIO_PORTM_DIR_R &= ~0x0F; //enable pin M1
    GPIO_PORTM_DATA_R = 0x0F; //clear stray data

    // GPIO Interrupt / Button
    GPIO_PORTM_IS_R |= 0x0F; //level-sensitive Car 3, Side 2, Main 1, Mode 0
    GPIO_PORTM_IBE_R &= ~0x0F; //single-edge
    GPIO_PORTM_IEV_R |= 0x0F; //rising edge
    GPIO_PORTM_ICR_R |= 0x0F; //clears ICR
    GPIO_PORTM_IM_R |= 0x0F; // Enable pin interrupts
}

/**
 * Sets up the timer of the microcontroller
 * It is set to "tick" every 0.25 of second
 */
void configureTimer1A()
{
    SYSCTL_RCGCTIMER_R = 0x02; //Timer A enable
    while((SYSCTL_RCGCTIMER_R & 0x02) != 0x02);

    TIMER1_CTL_R = 0x0; // Stop Timer
    TIMER1_CFG_R = 0x4; // 16 bit
    TIMER1_TAMR_R = 0x12; // Periodic Upwards
    TIMER1_TAPR_R = 61; // Prescale
    TIMER1_TAILR_R = 64515; // Preload
    TIMER1_CTL_R = 0x1; // Starts
}

/**
 * 1. Enables the input/out pins
 * 2. Configures and switches on the timer
 * 3. Performs actions only every 0.25 of a second, else time is burned
 * 4. Checks if any of the buttons were pressed
 * 5. Handles LED pattern changes. Pattern only changes after specific amount of time
 */

int main() {
    configure_led(); //1. Pin config
    configureTimer1A();//2. Timer config
    while(1){
        if(TIMER1_RIS_R == 0x1) //3. check timeout
        {
            TIMER1_ICR_R |= 0x00000001; // clear timeout

            button_handler(); //4. checks button presses

            //5. Code below handles the LED pattern changes
            // Check is made, so pattern would only change
            // at certain time
            if(next_tick <= ticks){
                ticks = 0;
                switch (traffic_mode) {
                    case -1 : // Used for testing purposes, in order to synchronize with LabJack
                        break;

                    case 0 : // Standard LED pattern changes
                        standard_mode(false);
                        break;

                    case 1 : // Rush Hour, Main road LED always stays green, unless prompted
                        standard_mode(true);
                        break;

                    case 2 : // Night mode, road lights blink yellow, unless prompted by pedestrian
                        night_mode();
                        break;

                    default:
                        printf("Unknown traffic_mode!!!");
                }
            }
            ticks++;
        }
    }
    return -1; //Shall not be reached. If it is -> Error
}

/**
 * Responsible for handling the button input
 */
void button_handler(){
    // Mode button
    // Changes traffic mode: Standard -> Rush - > Night -> Standard
    if(GPIO_PORTM_RIS_R == 0x1){
        printf("Mode Button Pressed!");
        GPIO_PORTM_ICR_R |= 0x1;
        next_tick = 1;

        printf("Changing traffic_mode \n");
        clear_lights();
        traffic_mode++;
        light_state = traffic_mode== 1 ? 2 : 0;
        if (traffic_mode > 2) traffic_mode = 0;
    }

    // Main pedestrian button
    if(GPIO_PORTM_RIS_R == 0x2){
        printf("Main Button Pressed!");
        GPIO_PORTM_ICR_R |= 0x2;

        if(!(traffic_mode==1) && !(traffic_mode==0 && (light_state!=0 && light_state<4)))main_ped_flag = true;
    }
    // Side button
    if(GPIO_PORTM_RIS_R == 0x4){
        printf("Side Button Pressed!");
        GPIO_PORTM_ICR_R |= 0x4;

        if(!(traffic_mode==0 && light_state>4)) side_ped_flag = true;
    }
    // Car button
    if(GPIO_PORTM_RIS_R == 0x8){
        printf("Car Button Pressed!");
        GPIO_PORTM_ICR_R |= 0x8;

        if (traffic_mode==1) car_sensor_flag = true;
    }
}

void standard_mode(bool isRush){
    printf("%s",isRush ? "Rush" : "Standard");
    printf(", State: %d\n", light_state);

    if(!in_motion){
        if (!isRush && main_ped_flag){
            light_state = 0;
            main_ped_flag = false;
            in_motion = true;
        }

        if (!isRush && side_ped_flag){
            light_state = 4;
            side_ped_flag = false;
            in_motion = true;
        }

        if (isRush && (side_ped_flag || car_sensor_flag))
        {
            light_state = 3;
            side_ped_flag = false;
            car_sensor_flag = false;
            in_motion = true;
        }
    }

    switch (light_state) {
        case 0:
            set_main_lights(2);
            set_side_lights(2);
            light_state = 1;
            next_tick = 4;
            break;
        case 1:
            set_main_lights(3);
            set_side_lights(2);
            light_state = 2;
            next_tick = 4;
            break;
        case 2:
            set_main_lights(0);
            set_side_lights(2);
            light_state = isRush ? 2 : 3;
            next_tick = 20;

            in_motion = false;

            break;
        case 3:
            set_main_lights(1);
            set_side_lights(2);
            light_state = 4;
            next_tick = 4;

            break;
        case 4:
            set_main_lights(2);
            set_side_lights(2);
            light_state = 5;
            next_tick = 4;

            break;
        case 5:
            set_main_lights(2);
            set_side_lights(3);
            light_state = 6;
            next_tick = 4;

            break;
        case 6:
            set_main_lights(2);
            set_side_lights(0);
            light_state = 7;
            next_tick = 12;

            in_motion = false;

            break;
        case 7:
            set_main_lights(2);
            set_side_lights(1);
            light_state = 0;
            next_tick = 4;

            break;
        default:
            printf("Warning! Unknown light phase!");
    }
}

void night_mode(){
    printf("Night \n");

    if (side_ped_flag||main_ped_flag){
        light_state = 2;
        side_ped_flag = false;
        main_ped_flag = false;
    }

    switch (light_state) {
        case 0:
            set_night_lights(0);
            light_state = 1;
            next_tick = 4;
            break;
        case 1:
            set_night_lights(1);
            light_state = 0;
            next_tick = 4;
            break;
        case 2:
            set_night_lights(2);
            light_state = 0;
            next_tick = 24;
            break;
        default:
            printf("Warning! Unknown light phase! \n");
    }
}

/*
 * Exchange to GPIO outputs
 */
void clear_lights(){
    // Road lights
    GPIO_PORTK_DATA_R = 0x00;
    // Pedestrian lights
    GPIO_PORTL_DATA_R = 0x00;
}

/*
 * 4 states:
 * 0: Green road, Green pedestrian
 * 1: Yellow road, Red pedestrian
 * 2: Red road, Red pedestrian
 * 3: Red and Yellow road, Green pedestrian
 *
 * To be changed to GPIO outputs
 */
void set_main_lights(int phase){
    clear_lights();
    switch (phase) {
        case 0:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x80);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x20);
            break;
        case 1:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x40);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x10);
            break;
        case 2:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x20);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x10);
            break;
        case 3:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x60);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x10);
            break;
        default:
            printf("Warning! Wrong main lights phase!");
    }
}

void set_side_lights(int phase){
    switch (phase) {
        case 0:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x04);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x02);
            break;
        case 1:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x02);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x01);
            break;
        case 2:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x01);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x01);
            break;
        case 3:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x03);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x02);
            break;
        default:
            printf("Warning! Wrong side lights phase!");
    }
}

void set_night_lights(int phase){
    clear_lights();
    switch (phase) {
        case 0:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x42);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x11);
            break;
        case 1:
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x11);
            break;
        case 2:
            GPIO_PORTK_DATA_R = (GPIO_PORTK_DATA_R | 0x21);
            GPIO_PORTL_DATA_R = (GPIO_PORTL_DATA_R | 0x22);
            break;
        default:
            printf("Warning! Wrong night lights phase!");
    }
}
