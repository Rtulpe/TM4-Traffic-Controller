# Tiva TM4C1294NCPT Traffic Light Controller Testing Document
This document describes the testing procedure of the software. Refer to:
1. [Requirements](documentation/1_Engineering.md)
2. [Architecture and Design](documentation/2_Architecture_Design.md)

For software details.

## Wiring Table

* MR - Main Road
* SR - Side Road
* G - Green
* Y - Yellow
* R - Red

| LED or Button | uC pin | LabJack pin |
|---------------|--------|-------------|
| MR Car G      | K7     | EIO-7       |
| MR Car Y      | K6     | EIO-6       |
| MR Car R      | K5     | EIO-5       |
| SR Car G      | K2     | EIO-2       |
| SR Car Y      | K1     | EIO-1       |
| SR Car R      | K0     | EIO-0       |
| MR Ped G      | L5     | FIO-7       |
| MR Ped R      | L4     | FIO-6       |
| SR Ped G      | L1     | FIO-5       |
| SR Ped R      | L0     | FIO-4       |
| Mode Change   | M0     | CIO-0       |
| MR Ped Button | M1     | CIO-1       |
| SR Ped Button | M2     | CIO-2       |
| Car Sensor    | M3     | CIO-3       |

## Tests:

Button tests are done, by pressing the button before beginning the test.

* Button 1: Main Pedestrian button
* Button 2: Side Pedestrian button
* Button 3: Car Sensor

In Total There will be 12 Tests, 4 button press cases for each of 3 operating modes:

| Test Name         | Test Usage                                      | Requirement                    |
|-------------------|-------------------------------------------------|--------------------------------|
| Standard          | To check the operation of all LEDs.             | RE1, RE4, RE5, RE6             |
| Standard Button 1 | The button press should alter LED operation.    | RE2, RE7, RE8                  |
| Standard Button 2 | The button press should alter LED operation.    | RE2, RE7, RE8                  |
| Standard Button 3 | Button press should be ignored .                | RE2                            |
| Rush              | To check the operation of all LEDs.             | RE1, RE3, RE4, RE5, RE9        |
| Rush Button 1     | Button press should be ignored.                 | RE2, RE10                      |
| Rush Button 2     | Button press should loop the operation 1 cycle. | RE2, RE11                      |
| Rush Button 3     | Button press should loop the operation 1 cycle. | RE2, RE7, RE11                 |
| Night             | To check the operation of all LEDs.             | RE1, RE3, RE4, RE5, RE12, RE13 |
| Night Button 1    | Button press should interrupt the blinking.     | RE2, RE14                      |
| Night Button 2    | Button press should interrupt the blinking.     | RE2, RE14                      |
| Night Button 3    | Button press should be ignored.                 | RE2                            |



### Test Data:

The data of the tests can be found at:
[Test Data](/testing/data)

### Test Results:

The results of the tests can be found at:
[Test Results](/testing/testresults)

### Interpretation of the tests:

While almost everything has worked as intended, there is a problem with the main pedestrian button. Throughout all the tests directly involving this button, it has not shown any changes to the behaviour of the operation. This a bug in the software.

Test Status:

| Test Name         | Test Status |
|-------------------|-------------|
| Standard          | Passed      |
| Standard Button 1 | Failed      |
| Standard Button 2 | Passed      |
| Standard Button 3 | Passed      |
| Rush              | Passed      |
| Rush Button 1     | Passed      |
| Rush Button 2     | Passed      |
| Rush Button 3     | Passed      |
| Night             | Passed      |
| Night Button 1    | Failed      |
| Night Button 2    | Passed      |
| Night Button 3    | Passed      |