# Tiva TM4C1294NCPT Traffic Light Controller Software Requirements

## Functional Requirements

#### RE1: The software should produce 10 binary(on-off) outputs:
* 1-3:  Main road: 1 Green, 2 Yellow, 3 Red;
* 4-6:  Side road: 4 Green, 5 Yellow, 6 Red;
* 7-8:  Main Pedestrian 7 Green, 8 Red;
* 9-10: Side Pedestrian 9 Green, 10 Red)

#### RE2: The software should take 4 binary inputs:
1. Main Pedestrian Button;
2. Side Pedestrian Button;
3. Side Road Sensor;
4. Mode Switch;

#### RE3: By pushing Mode Switch, it should be possible to loop through operation modes:
-> Standard(Default) -> Rush -> Night ->

3 Operation modes have to be implemented:

### Standard mode:

#### RE4: During standard mode, there are 4 combinations of single road lights, which loop in a cycle:

1. Green - Green lights up
2. Green to Red - Yellow lights up
3. Red - Red lights up 
4. Red to Green - Yellow and Red lights up


#### RE5: During Standard mode, there are 8 states (combinations taken from RE4):
1. 0 Main 3, Side 3 (0.25 sec)
2. 1 Main 4, Side 3 (0.25 sec)
3. 2 Main 1, Side 3 (5 sec)
4. 3 Main 2, Side 3 (0.5 sec)
5. 4 Main 3, Side 3 (0.25 sec)
6. 5 Main 3, Side 4 (0.25 sec)
7. 6 Main 3, Side 1 (3 sec)
8. 7 Main 3, Side 2 (0.5 sec)

*Note: 0.25 and 0.5 were chosen as safety margin is 1 sec.*

#### RE6: During Standard mode, Pedestrian light modes depends on the mode of the road they cross (Main Pedestrian - Main Road):
1. Red -    RE4 modes 2, 3
2. Green -  RE4 modes 1, 4

#### RE7: During all modes pedestrian button should only work when the corresponding light is red.

#### RE8: Pressing the button will do the following:
*For Main Road: Go to RE5 Phase 1,*
Adjust Phase 2 (1 sec), Phase 3 (3 sec), continue normal

*For Side Road: Go to RE5 Phase 5,*
Adjust Phase 6 (1 sec), Phase 7 (3 sec), continue normal

### Rush Hour mode:

#### RE9: During Rush mode, lights are always in RE5 phase 3, unless interrupted.

#### RE10: During Rush mode, Main pedestrian button is disabled.

#### RE11: During Rush mode, button press by Side Road Sensor or Side Pedestrian will loop through RE5 phases 1 time and return to RE5 phase 3.

### Night mode:

#### RE12: During Night mode, there are 3 light combinations:
1. All road lights light up yellow
2. All road lights off
3. All road lights red
    
#### RE13: During Night mode, there are 2 passive states:
1. RE12 1 for 1 sec.
2. RE12 2 for 1 sec.

#### RE14: Pressing any pedestrian button will do the following:
1. Turn to RE12 3 for 6 seconds.
2. Return to RE13 behavior.

## Non-Functional Requirements

#### NRE1: The switching speed must match customer's specifications for all 3 modes and should not deviate more than 1/10th of second.

#### NRE2: For development purposes, 3 modes should be linked to microcontroller's LEDs:
* LED 1: Standard mode
* LED 2: Rush mode
* LED 3: Night mode