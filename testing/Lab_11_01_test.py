
import u3
import pandas as pd
import os
import matplotlib.pyplot as plt
import time as tm


def getData(name, but1 = False, but2 = False, but3 = False):
    initial_start_time = tm.time() # initialize start time

    EIOList = []
    FIOList = []
    CIOList = []
    timeList = []

    time2 = 0

    if but1 is True:
        d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x02], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0
        tm.sleep(0.2)
        d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0

    if but2 is True:
        d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x04], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0
        tm.sleep(0.2)
        d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0

    if but3 is True:
        d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x08], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0
        tm.sleep(0.2)
        d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0



    ## while time2 < 
    for i in range(100):
        val1 = d.getFeedback(u3.PortStateRead())
        time1 = tm.time()
        time2 = (time1 - initial_start_time)
        timeList.append(time2)
        FIOList.append(val1[0]['FIO'])
        EIOList.append(val1[0]['EIO'])
        CIOList.append(val1[0]['CIO'])
        tm.sleep(0.14)

    dframe['time']=timeList
    dframe['FIO']=FIOList
    dframe['EIO']=EIOList
    dframe['CIO']=CIOList


    # wriite to file
    try:
        dframe.to_csv(path + name + '.csv', mode='w')
    except:
        os.rmdir(path + name + '.csv')
        dframe.to_csv(path + name + '.csv', mode='w')
    print('End of test')


initial_start_time = 0
path = 'C:/Users/oyand/OneDrive/Desktop/final_lab_data/' # path to data
name = 'lab_4_data_rushTest'
dframe = pd.DataFrame(columns=['time', 'FIO', 'EIO', 'CIO'])



d = u3.U3()
d.configAnalog()
d.getFeedback(u3.PortDirWrite(Direction=[0x00, 0x00, 0x00], WriteMask=[0xff, 0xff, 0xff]))
d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff]))
val = d.getFeedback(u3.PortStateRead())

d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x01], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0
tm.sleep(0.2)
d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0

getData("lab_4_data_standard")
getData("lab_4_data_standard_bu1", but1=True)
getData("lab_4_data_standard_bu2", but2=True)
getData("lab_4_data_standard_bu3", but3=True)

d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x01], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0
tm.sleep(0.2)
d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0

getData("lab_4_data_rush")
getData("lab_4_data_rush_bu1", but1=True)
getData("lab_4_data_rush_bu2", but2=True)
getData("lab_4_data_rush_bu3", but3=True)

d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x01], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0
tm.sleep(0.2)
d.getFeedback(u3.PortStateWrite(State=[0x00, 0x00, 0x00], WriteMask=[0x00, 0x00, 0xff])) # output signals in cio 0

getData("lab_4_data_night")
getData("lab_4_data_night_bu1", but1=True)
getData("lab_4_data_night_bu2", but2=True)
getData("lab_4_data_night_bu3", but3=True)