#! /usr/bin/python
import sys
import os
if(len(sys.argv) == 1):
        exit()
proName = sys.argv[1]

out = """

cmake_minimum_required(VERSION 2.8)
project( {0} )
find_package( OpenCV REQUIRED )
add_executable( {0} {0}.cpp )
target_link_libraries( {0} ${{OpenCV_LIBS}} )
""".format(proName)

with open('CMakeLists.txt','w') as f:
        f.write(out)
os.system('cmake .')
os.system('make')
