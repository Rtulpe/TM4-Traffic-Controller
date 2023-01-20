import u3
import pandas as pd
import os
import matplotlib.pyplot as plt
import time as tm


# Testing for the Texas Instruments TivaTM TM4C1294NCPT
# micro-controller based traffic light controller
# with LabJack U3
#
# By Rustenis Tolpeznikas and Obed Oyandut
#

# Configure LabJack
path = 'C:/Users/oyand/OneDrive/Desktop/final_lab_data/'  # path to data (Note: This is not correct and must be
# changed on different device)
dframe = pd.DataFrame(columns=['time', 'FIO', 'EIO', 'CIO'])
d = u3.U3()
d.configAnalog()
# Clean up annoying "stuck bits"
d.getFeedback(u3.PortDirWrite(Direction=[0xff, 0xff, 0xff], WriteMask=[0xff, 0xff, 0xff]))
d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0xff, 0xff, 0xff]))

d.getFeedback(u3.PortDirWrite(Direction=[0x00, 0x00, 0x00], WriteMask=[0xff, 0xff, 0xff]))
d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))
# val = d.getFeedback(u3.PortStateRead())


# Collects 100 samples per run of all TM4s outputs
#
# name: Name of the results file
# but1: Main Pedestrian button
# but2: Side Pedestrian button
# but3: Car Sensor
def get_data(name, but1=False, but2=False, but3=False):
    initial_start_time = tm.time()  # initialize start time

    EIOList = []
    FIOList = []
    CIOList = []
    timeList = []

    if but1 is True:  # If Main Ped Button is pressed
        d.getFeedback(
            u3.PortStateWrite(State=[0x00, 0x00, 0x02], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 1
        tm.sleep(0.2)
        d.getFeedback(
            u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 1

    if but2 is True:  # If Side Ped Button is pressed
        d.getFeedback(
            u3.PortStateWrite(State=[0x00, 0x00, 0x04], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 2
        tm.sleep(0.2)
        d.getFeedback(
            u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 2

    if but3 is True:  # If Car sensor senses a car
        d.getFeedback(
            u3.PortStateWrite(State=[0x00, 0x00, 0x08], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 3
        tm.sleep(0.2)
        d.getFeedback(
            u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 3

    for i in range(100):
        val1 = d.getFeedback(u3.PortStateRead())
        time1 = tm.time()
        time2 = (time1 - initial_start_time)
        timeList.append(time2)
        FIOList.append(val1[0]['FIO'])
        EIOList.append(val1[0]['EIO'])
        CIOList.append(val1[0]['CIO'])
        tm.sleep(0.14)

    dframe['time'] = timeList
    dframe['FIO'] = FIOList
    dframe['EIO'] = EIOList
    dframe['CIO'] = CIOList

    # write to file
    try:
        dframe.to_csv(path + name + '.csv', mode='w')
    except:
        os.rmdir(path + name + '.csv')
        dframe.to_csv(path + name + '.csv', mode='w')
    print('End of test: ' + name)


# Records 12 test runs -> 3 modes * 4 button states (None, Main_Ped, Side_Ped, Car_Sensor)
def test_timing():
    d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x01], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 0
    tm.sleep(0.2)
    d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 0

    get_data("lab_4_data_standard")
    get_data("lab_4_data_standard_bu1", but1=True)
    get_data("lab_4_data_standard_bu2", but2=True)
    get_data("lab_4_data_standard_bu3", but3=True)

    d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x01], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 0
    tm.sleep(0.2)
    d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 0

    get_data("lab_4_data_rush")
    get_data("lab_4_data_rush_bu1", but1=True)
    get_data("lab_4_data_rush_bu2", but2=True)
    get_data("lab_4_data_rush_bu3", but3=True)

    d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x01], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 0
    tm.sleep(0.2)
    d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))  # output signals in cio 0

    get_data("lab_4_data_night")
    get_data("lab_4_data_night_bu1", but1=True)
    get_data("lab_4_data_night_bu2", but2=True)
    get_data("lab_4_data_night_bu3", but3=True)


def plot_test_results(file_name):
    test_file = pd.read_csv('data/'+file_name+'.csv')
    timing = test_file['time'].values.tolist()
    FIO = test_file['FIO'].values.tolist()
    EIO = test_file['EIO'].values.tolist()

    MR_car = []  # Main Road Car
    SR_car = []  # Side Road Car
    MR_ped = []  # Main Road Ped
    SR_ped = []  # Side Road Ped
    for i in range(len(EIO)):
        MR_car.append(EIO[i] & 0xE0)
        SR_car.append(EIO[i] & 0x07)
        MR_ped.append(FIO[i] & 0xC0)
        SR_ped.append(FIO[i] & 0x30)

    fig, axs = plt.subplots(2, 2)
    axs[0, 0].plot(timing, MR_car)
    axs[0, 0].set_title('Main Road Car')
    axs[0, 0].axhline(y=128, color='g', linestyle=':', label='Green')
    axs[0, 0].axhline(y=96, color='m', linestyle=':', label='Yellow-Red')
    axs[0, 0].axhline(y=64, color='y', linestyle=':', label='Yellow')
    axs[0, 0].axhline(y=32, color='r', linestyle=':', label='Red')
    axs[0, 1].plot(timing, SR_car)
    axs[0, 1].set_title('Side Road Car')
    axs[0, 1].axhline(y=4, color='g', linestyle=':', label='Green')
    axs[0, 1].axhline(y=3, color='m', linestyle=':', label='Yellow-Red')
    axs[0, 1].axhline(y=2, color='y', linestyle=':', label='Yellow')
    axs[0, 1].axhline(y=1, color='r', linestyle=':', label='Red')
    axs[1, 0].plot(timing, MR_ped)
    axs[1, 0].set_title('Main Road Ped')
    axs[1, 0].axhline(y=128, color='g', linestyle=':', label='Green')
    axs[1, 0].axhline(y=64, color='r', linestyle=':', label='Red')
    axs[1, 1].plot(timing, SR_ped)
    axs[1, 1].set_title('Side Road Ped')
    axs[1, 1].axhline(y=32, color='g', linestyle=':', label='Green')
    axs[1, 1].axhline(y=16, color='r', linestyle=':', label='Red')
    plt.savefig('testresults/'+file_name+'.png')

def save_test_results():
    plot_test_results("lab_4_data_standard")
    plot_test_results("lab_4_data_standard_bu1")
    plot_test_results("lab_4_data_standard_bu2")
    plot_test_results("lab_4_data_standard_bu3")

    plot_test_results("lab_4_data_rush")
    plot_test_results("lab_4_data_rush_bu1")
    plot_test_results("lab_4_data_rush_bu2")
    plot_test_results("lab_4_data_rush_bu3")

    plot_test_results("lab_4_data_night")
    plot_test_results("lab_4_data_night_bu1")
    plot_test_results("lab_4_data_night_bu2")
    plot_test_results("lab_4_data_night_bu3")


test_timing()
save_test_results()