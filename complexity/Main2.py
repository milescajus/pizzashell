import Readfile
import os
import numpy as np
import sys
import Halstead as hal
n = len(sys.argv)
if( n == 1):
    TypeArray =  np.array(([[0,".py"],[1,".c"],[2, ".h"]]))
    # print (TypeArray.shape)
    # print (TypeArray.size)

    Loc_Arr = hal.get_n1_n2(Readfile.Getsubfolders())
    for i in Loc_Arr:
        print("The current file is:",i[0],"n1 is:",i[1],"n2 is:",i[2],"\n")
    #
    # print (Loc_Arr.shape)
    # print (Loc_Arr.size)
    # for i in Loc_Arr:
    #    print("type: \n  name: \n ,LOC \n" TypeArray[2,i],Loc_Arr[1,i],Loc_Arr[2,i])

# else:
#     #if str(sys.argv).includes("l"):
#     for i in range(1, n):
#         if (sys.argv[i] == "h"):
#             Loc_Arr = hal.get_n1_n2(Readfile.Getsubfolders())
#             for i in Loc_Arr:
#                 print("The current file is:",i[0],"n1 is:",i[1],"n2 is:",i[2],"\n")
#         if (sys.argv[i] == "l"):
#             Loc_Arr = Readfile.get_LOC_In_All_Subirectories(Readfile.Getsubfolders())
#             for i in Loc_Arr:
#                 print ("The current file is:",i[0]," Lines of Code:",i[1],"\n")
