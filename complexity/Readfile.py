import io
import os
import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer
import shutil
import pathlib
import numpy as np
def Getsubfolders(_my_path_ = os.getcwd()):
    #returns ARRAY of all subfolders and files, default to current working director
    path = pathlib.Path(_my_path_)
    #return array of paths
    PathArray = []
    for i in path.rglob("*"):
        PathArray.append(i)
    return PathArray


def get_LOC_In_All_Subirectories(progarray):
    #List of str for checked extensions and a placeholder for sorting for file extensions
    Py = np.array([[0,".py"]])
    C = np.array([[1,".c"]])
    Cheader = np.array([[2, ".h"]])
    Count_code = np.array([0,0,0])
    #Name of file, and lines of Count_code
    Count = []
    #for array of programs check line of code for each type of suffic


    for i in progarray:

        if i.is_file():
            Open=False
            Suffixnum = Switch_suffix(i.suffix)
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
            # open file and count lines if it is one we care about
            if (Open == True):

                Currentfile = i.read_text()
                Lines = len([l.strip() for l in Currentfile.splitlines()])
                Count.append([i.name,Lines])
                Open = False
    return Count
def Switch_suffix(suffix):
    #switch statement on suffix to number
    if(suffix == '.py'):
        return  1
    elif(suffix == ".c"):
        return 2
    elif(suffix == ".h"):
        return 3

    if(suffix == 1):
        return '.py'
    elif(suffix == 2):
        return '.c'
    elif(suffix == 3):
        return '.h'
    return "Suffix not in language"
