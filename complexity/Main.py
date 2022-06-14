import Readfile
import os
import numpy as np
import sys
import Halstead as hal
TypeArray = np.array(([[0,".py"], [1,".c"], [2, ".h"]]))

if ( len(sys.argv) == 1 or sys.argv[1] == "-l" ):
    Loc_Arr = Readfile.get_LOC_In_All_Subirectories(Readfile.Getsubfolders())
    for i in Loc_Arr:
        print("File:\033[92m", i[0], "\033[0m\n\tLines of Code:\033[33m", i[1], "\033[0m")

else:
    Loc_Arr = hal.get_n1_n2(Readfile.Getsubfolders())
    for i in Loc_Arr:
        print("File:\033[92m", i[0], "\033[0m\n\tn1:\033[33m", i[1], "\033[0m\n\tn2:\033[33m", i[2], "\033[0m")
