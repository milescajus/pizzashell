import io
import os
import pathlib
import numpy as np
import Readfile

operator_set = set({"+", "-", "=", "*", "/", "%", "**","//","=","+=","-=","*=","/=","%=","//=","**=","%=","|=","^=",">>=","<<=","==","!=",">","<",">=","<=","and","or","not","is","is not", "in","not in","&","|","^","~","<<",">>"})

# Number of unique operators
def get_n1_n2(progarray):
    #List of str for checked extensions and a placeholder for sorting for file extensions
    Py = np.array([[0,".py"]])
    C = np.array([[1,".c"]])
    Cheader = np.array([[2, ".h"]])
    Count_code = np.array([0,0,0])
    #Name of file, and lines of Count_code
    Count = []
    #for array of programs check line of code for each type of suffic

    for i in progarray:
        myset = {}
        if i.is_file():
            name = i.name
            Open=False
            Suffixnum = Readfile.Switch_suffix(i.suffix)
            if  Suffixnum == 1:
                Count_code[0] += 1
                Open = True
            elif Suffixnum == 2:
                Count_code[1] += 1
                Open = True
            elif Suffixnum == 3:
                Count_code[2] += 1
                Open = True
            elif Suffixnum == 4:
                Count_code[3] += 1
                Open = True
            if (Open == True):
                Currentfile = i.read_text()
                n1 = set()
                n2 = set()
                Lines = [Currentfile.strip() for l in Currentfile.splitlines()]
                for i in Lines:
                    for entry in i.split(' '):
                        if entry not in operator_set:
                            n1.add(entry)
                        elif entry in operator_set:
                            n2.add(entry)
                Count.append([name,len(n1),len(n2)])
                Open = False
            # for i in Count_code:
            #     print("Count_code:",i)
    return Count
