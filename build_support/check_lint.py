import os
import sys

files = os.listdir("../src/")

for filename in files:
   if (filename.endswith(".h") or filename.endswith(".cc")):
      os.system("python cpplint.py ../src/{}".format(filename))