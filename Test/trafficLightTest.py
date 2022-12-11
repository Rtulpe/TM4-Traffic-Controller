# This application test the performance of an embedded traffic light application
# For Embedded system test and verification lab03
# Obed Oyandut, Rustenis Tolpeznikas

import os
import pandas as pd
import numpy as np

# Test matrix for the evaluation of traffic light operation.
cols_name = ['main_red', 'main_red_duration', 'main_green', 'main_green_duration', 'main_yellow', 'main_yellow_duration', \
            'sec_red', 'sec_red_duration', 'sec_green', 'sec_green_duration', 'sec_yellow', 'sec_yellow_duration'] 
arr = np.zeros((6, 12), dtype=np.int8)
df = pd.DataFrame(arr, columns=cols_name)
dic = {'Test_Cases':['stand_mode', 'stand_mode_button_pressed', 'rushhour_mode', 'rushhour_mode_button_pressed', 'night_mode', 'night_mode_button_pressed']}
df1 = pd.DataFrame(dic)
df2 = pd.concat([df1, df], axis=1)
print(df2)