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
                # print("i is: \n",i)
                # print("Count shape",Count.shape)
                # Currentfilepath = i.parent + "/"+ i.name +"/"+ Suffixnum
                # Currentfile = Currentfilepath.read_text()
                Currentfile = i.read_text()
                # use bloom filter?
                Lines = len([l.strip() for l in Currentfile.splitlines()])
                # print(" array Shape",np.array([i.name,Lines]).shape)
                # print("Program name:", i. name)
                # print("\n",Lines)
                Count.append([i.name,Lines])
                # print(" array Shape",np.array([i.name,Lines]).shape)
                # print("Count shape",Count.shape)
                
                Open = False
            # for i in Count_code:
            #     print("Count_code:",i)
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
#     folder = r"<MAIN FOLDER>"
#     subfolders = [f.path for f in os.scandir(folder) if f.is_dir()]
#         for sub in subfolders:
#             for f in os.listdir(sub):
#                 src = os.path.join(sub, f):
#
#             # create document term matrix
#             df_dtm = pd.DataFrame(cv_matrix.toarray(), index=df['TEMP'].values, columns=cv.get_feature_names())
#             view raw
#
#             # initialize
#             df = pd.DataFrame({'TEMP': ['OpenMP', 'MPi', 'Fortran'], 'text':texts})
#             cv = CountVectorizer()
#             cv_matrix = cv.fit_transform(df['text'])
#
#
# # stream = io.open("file.txt", "r")
# buffered_reader = io.BufferedReader(stream)
# # set the stream to byte 250
# buffered_reader.seek(250)
# # read up to byte 750 (500 bytes from position 250)
# data = buffered_reader.read(500)
#
#
# from sklearn.feature_extraction.text import CountVectorizer
