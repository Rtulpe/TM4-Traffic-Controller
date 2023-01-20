# Tiva TM4C1294NCPT Traffic Light Controller Architecture and Design Document

This document describes all the components of the software and how they relate to one another.

## Components

The components consist of Functions and Global variables, which the functions will use

### Functions

#### Main Function
Used for configuration, and overall program flow handling

Used: None

Uses:
1. Led Configurator
2. Timer Configurator
3. Button Handler
4. Standard and Rush mode handler
5. Night mode handler
6. Traffic mode
7. Ticks
8. Next Ticks

#### Led Configurator
Sets up the pins to output, makes button pins interrupt-aware

Used: Main Function

Uses: None

#### Timer Configurator
Sets up timer to tick every 0.25 of a second, repeatedly

Used: Main Function

Uses: None

#### Button Handler
Handles button presses

Used: Main Function

Uses: 
1. Main Pedestrian Flag
2. Side Pedestrian Flag
3. Car Sensor Flag
4. Traffic Mode
5. Light State
6. Ticks
7. Next Tick

#### Standard and Rush mode handler
Checks the flags, which are raised by button presses. Instructs lights handler to change lights for standard and rush modes.

Used: Main Function

Uses:
1. Main Lights handler
2. Side Lights handler
3. Main Pedestrian Flag
4. Side Pedestrian Flag
5. Car Sensor flag
6. Motion flag
7. Light State
8. Next Tick

#### Night mode handler
Checks the flags, which are raised by button presses. Instructs lights handler to change lights for night mode.

Used: Main Function

Uses:
1. Night Mode Handler
2. Main Pedestrian Flag
3. Side Pedestrian Flag
4. Light State
5. Next Tick

#### Main Lights handler
Changes Lights for the main car and pedestrian road.

Used: Standard and Rush mode handler

Uses: Light State

#### Side Lights handler
Changes Lights for the side car and pedestrian road.

Used: Standard and Rush mode handler

Uses: Light State

#### Night Lights handler
Changes all Lights, for night mode only.

Used: Night Mode Handler

Uses: Light State

### Global variables
Global variables cannot use other components
#### Main Pedestrian Flag
Raised by main road pedestrian button press.

Used:
1. Button Handler
2. Standard and Rush Mode Handler
3. Night Mode Handler

#### Side Pedestrian Flag
Raised by side road pedestrian button press.

Used:
1. Button Handler
2. Standard and Rush Mode Handler
3. Night Mode Handler

#### Car Sensor Flag
Raised by car sensor being active.

Used:
1. Button Handler
2. Standard and Rush Mode Handler

#### Motion Flag
Used as a "wait" for Pedestrian button presses, so pedestrians could not repeatedly press and abuse the buttons.

Used:
1. Button Handler
2. Standard and Rush Mode Handler

#### Traffic Mode
Indicates which traffic mode it is:
* -1 - Stopped mode
* 0  - Standard mode
* 1 - Rush mode
* 2 - Night mode

Used:
1. Main Function
2. Button Handler

#### Light State
Indicates which light state it is, as described in [Requirements](documentation/1_Engineering.md)  RE5

Used:
1. Button Handler
2. Standard and Rush Mode Handler
3. Night Mode Handler
4. Main Lights Handler
5. Side Lights Handler
6. Night Lights Handler

#### Ticks
Used to count current time (in 0.25 of second).

Used:
1. Main Function
2. Button Handler

#### Next Tick
Used to indicate when the next action (light change) should happen (in 0.25 of second).

Used:
1. Main Function
2. Button Handler
3. Standard and Rush Mode Handler
4. Night Mode Handler
