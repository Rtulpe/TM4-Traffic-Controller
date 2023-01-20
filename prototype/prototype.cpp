#include <cstdio>
#include "main.h"
/*
 * Prototype for TIVA microcontroller based traffic light controller
 *
 * This is unused code, which was made to test the program out before
 * adapting it to a microcontroller
 *
 * Rustenis Tolpeznikas & Obed Oyandut
 */
Lights traffic;
Button_Flags flags;

int traffic_mode = 0;
int light_state = 0;
int ticks = 0; //Probably will use this for timing

void standard_mode(bool);
void night_mode();
void test_lights();
void clear_lights();

void set_main_lights(int);
void set_side_lights(int);
void set_night_lights(int);

int main() {
    while(1){
        char c = getchar(); //Replace with button handler

        switch (c) {
            case 'a':
                printf("Changing traffic_mode \n");
                clear_lights();
                traffic_mode++;
                light_state = traffic_mode== 1 ? 2 : 0;
                if (traffic_mode > 2) traffic_mode = 0;
                break;
            case 's':
                printf("Main pedestrian button \n");
                if(traffic_mode==1)break; // Rush hour main button is inactive
                if(traffic_mode==0 && (light_state!=0 && light_state<4))break;
                printf("Main Ped Flag \n");
                flags.main_ped_flag = true;
                break;
            case 'd':
                printf("Side pedestrian button \n");
                if(traffic_mode==0 && light_state>4) break;
                printf("Side Ped Flag \n");
                flags.side_ped_flag = true;
                break;
            case 'f':
                printf("Side car sensor \n");
                if (traffic_mode!=1) break;
                printf("Side car Flag \n");
                flags.car_sensor_flag = true;
                break;
            default:;
        }

        switch (traffic_mode) {
            case 0 :
                standard_mode(false);
                break;

            case 1 :
                standard_mode(true);
                break;

            case 2 :
                night_mode();
                break;

            default:
                printf("Unknown traffic_mode!!!");
        }
    }
}

void standard_mode(bool isRush){
    printf("%s",isRush ? "Rush" : "Standard");
    printf(", State: %d\n", light_state);

    if(!flags.in_motion){
        if (!isRush && flags.main_ped_flag){
            light_state = 0;
            flags.main_ped_flag = false;
            flags.in_motion = true;
        }

        if (!isRush && flags.side_ped_flag){
            light_state = 4;
            flags.side_ped_flag = false;
            flags.in_motion = true;
        }

        if (isRush && (flags.side_ped_flag || flags.car_sensor_flag))
        {
            light_state = 3;
            flags.side_ped_flag = false;
            flags.car_sensor_flag = false;
            flags.in_motion = true;
        }
    }

    switch (light_state) {
        case 0:
            set_main_lights(2);
            set_side_lights(2);
            light_state = 1;
            break;
        case 1:
            set_main_lights(3);
            set_side_lights(2);
            light_state = 2;
            break;
        case 2:
            set_main_lights(0);
            set_side_lights(2);
            light_state = isRush ? 2 : 3;

            flags.in_motion = false;

            break;
        case 3:
            set_main_lights(1);
            set_side_lights(2);
            light_state = 4;

            break;
        case 4:
            set_main_lights(2);
            set_side_lights(2);
            light_state = 5;

            break;
        case 5:
            set_main_lights(2);
            set_side_lights(3);
            light_state = 6;

            break;
        case 6:
            set_main_lights(2);
            set_side_lights(0);
            light_state = 7;

            flags.in_motion = false;

            break;
        case 7:
            set_main_lights(2);
            set_side_lights(1);
            light_state = 0;

            break;
        default:
            printf("Warning! Unknown light phase!");
    }
    test_lights();
}

void night_mode(){
    printf("Night \n");

    if (flags.side_ped_flag||flags.main_ped_flag){
        light_state = 2;
        flags.side_ped_flag = false;
        flags.main_ped_flag = false;
    }

    switch (light_state) {
        case 0:
            set_night_lights(0);
            light_state = 1;
            break;
        case 1:
            set_night_lights(1);
            light_state = 0;
            break;
        case 2:
            set_night_lights(2);
            light_state = 0;
            break;
        default:
            printf("Warning! Unknown light phase! \n");
    }
    test_lights();
}

/*
 * Software only function for testing lights
 */
void test_lights(){
    printf("Main: %d, %d, %d :: %d, %d\n",traffic.main_green,traffic.main_yellow,traffic.main_red,traffic.main_pass_green,traffic.main_pass_red);
    printf("Side: %d, %d, %d :: %d, %d\n",traffic.side_green,traffic.side_yellow,traffic.side_red,traffic.side_pass_green,traffic.side_pass_red);
}

/*
 * Exchange to GPIO outputs
 */
void clear_lights(){
    // Main lights
    traffic.main_green = false;
    traffic.main_yellow = false;
    traffic.main_red = false;
    traffic.main_pass_green = false;
    traffic.main_pass_red = false;
    // Side lights
    traffic.side_green = false;
    traffic.side_yellow = false;
    traffic.side_red = false;
    traffic.side_pass_green = false;
    traffic.side_pass_red = false;
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
            traffic.main_green = true;
            traffic.main_pass_green = true;
            break;
        case 1:
            traffic.main_yellow = true;
            traffic.main_pass_red = true;
            break;
        case 2:
            traffic.main_red = true;
            traffic.main_pass_red = true;
            break;
        case 3:
            traffic.main_red = true;
            traffic.main_yellow = true;
            traffic.main_pass_green = true;
            break;
        default:
            printf("Warning! Wrong main lights phase!");
    }
}

void set_side_lights(int phase){
    switch (phase) {
        case 0:
            traffic.side_green = true;
            traffic.side_pass_green = true;
            break;
        case 1:
            traffic.side_yellow = true;
            traffic.side_pass_red = true;
            break;
        case 2:
            traffic.side_red = true;
            traffic.side_pass_red = true;
            break;
        case 3:
            traffic.side_red = true;
            traffic.side_yellow = true;
            traffic.side_pass_green = true;
            break;
        default:
            printf("Warning! Wrong side lights phase!");
    }
}

void set_night_lights(int phase){
    switch (phase) {
        case 0:
            clear_lights();
            traffic.main_yellow = true;
            traffic.side_yellow = true;

            traffic.main_pass_red = true;
            traffic.side_pass_red = true;
            break;
        case 1:
            traffic.main_yellow = false;
            traffic.side_yellow = false;
            break;
        case 2:
            traffic.main_yellow = false;
            traffic.main_red = true;
            traffic.side_yellow = false;
            traffic.side_red = true;

            traffic.main_pass_red = false;
            traffic.main_pass_green = true;
            traffic.side_pass_red = false;
            traffic.side_pass_green = true;
            break;
        default:
            printf("Warning! Wrong night lights phase!");
    }
}
