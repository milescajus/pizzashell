import io
import os
import pandas as pd
from sklearn.feature_extraction.text import CountVectorizer
import shutil
import pathlib
import numpy as np
import Readfile
# halstead complexity_for_file
#
# Parameter	Meaning
# n1	Number of unique operators + - = * / %  ** // =
# n2	Number of unique operands
# N1	Number of total occurrence of operators
# N2	Number of total occurrence of operands
#
# Metric	Meaning	Mathematical Representation
# n	Vocabulary	n1 + n2
# N	Size	N1 + N2
# V	Volume	Length * Log2 Vocabulary
# D	Difficulty	(n1/2) * (N1/n2)
# E	Efforts	Difficulty * Volume
# B	Errors	Volume / 3000
# T	Testing time	Time = Efforts / S, where S=18 seconds.

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

# Number of unique operand
# def get_n2(progarray):# Number of total occurrence of operators
# def get_N1(progarray):
# # Number of total occurrence of operands
# def get_N2(progarray):
