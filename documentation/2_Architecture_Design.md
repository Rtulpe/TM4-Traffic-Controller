# Tiva TM4C1294NCPT Traffic Light Controller Architecture Document

This document describes all the components of the software and how they relate to one another.

## Components

The components consist of Functions and Global variables, which the functions will use

### Functions

#### Main Function

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

Used: Main Function

Uses: None

#### Timer Configurator

Used: Main Function

Uses: None

#### Button Handler

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

Used: Main Function

Uses:
1. Night Mode Handler
2. Main Pedestrian Flag
3. Side Pedestrian Flag
4. Light State
5. Next Tick

#### Main Lights handler

Used: Standard and Rush mode handler

Uses: Light State

#### Side Lights handler

Used: Standard and Rush mode handler

Uses: Light State

#### Night Lights handler

Used: Night Mode Handler

Uses: Light State

### Global variables
Global variables cannot use other components
#### Main Pedestrian Flag

Used:
1. Button Handler
2. Standard and Rush Mode Handler
3. Night Mode Handler

#### Side Pedestrian Flag

Used:
1. Button Handler
2. Standard and Rush Mode Handler
3. Night Mode Handler

#### Car Sensor Flag

Used:
1. Button Handler
2. Standard and Rush Mode Handler

#### Motion Flag

Used:
1. Button Handler
2. Standard and Rush Mode Handler

#### Traffic Mode

Used:
1. Main Function
2. Button Handler

#### Light State

Used:
1. Button Handler
2. Standard and Rush Mode Handler
3. Night Mode Handler
4. Main Lights Handler
5. Side Lights Handler
6. Night Lights Handler

#### Ticks

Used:
1. Main Function
2. Button Handler

#### Next Tick

Used:
1. Main Function
2. Button Handler
3. Standard and Rush Mode Handler
4. Night Mode Handler
