import os.path as path
import sys


# this is a script that tests the ability to import a script from one directory
# up. the __file__ isn't used in C++ so it didn't work. perhaps this could be
# tested with another method
path = path.abspath(path.join(__file__ ,'../..'))
sys.path.insert(0, path)

import plotter
