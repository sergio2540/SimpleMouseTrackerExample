FRAME = 1050
INTERVAL=300
PROJ= test
VIDEO= photo/mouse.jpg

#Directory with c code
C_DIR= ./c

#Directory with js code, data.json (data/), 
#script to generate program.js (pre/)
#and script for post processing (post/)
CROWDPROCESS_DIR= ./crowdprocess

#C compiler
#CC=path/to/c_compiler
#Example: CC=cc
CC = cc

#Flags to C compiler
CFLAGS=-O1

#C sources
SOURCES=laser.c 

#Executable name
EXEC=laser

LIBJS= ../crowdprocess/libs/libcv.a ../crowdprocess/libs/libcvaux.a ../crowdprocess/libs/libhighgui.a ../crowdprocess/libs/libcxcore.a ../crowdprocess/libs/libml.a

#Emscripten C compiler
#EMCC=path/to/emscripten/emcc
#Example:
#EMCC= /home/sergio/emscripten/emcc 
EMCC=~/emscripten/emcc

#Flags for emscripten C compiler
#-O<optimization level>
#See: https://github.com/kripken/emscripten/wiki/Optimizing-Code
EMCCFLAGS=-O1

#Various compiling-to-JS parameters.
#See https://github.com/kripken/emscripten/blob/master/src/settings.js
SETTINGS= -s BUILD_AS_WORKER=1 -s ASMJS=1 -s INVOKE_RUN=0

DATA= ./data/data.json

install:
	sudo apt-get install python2.7
	sudo apt-get install python-numpy
	sudo apt-get install python-opencv

step1:
	python SimpleMouseTracker/create_project.py $(PROJ) $(VIDEO)
step2:
	python SimpleMouseTracker/set_mask.py $(PROJ)
step3:
	python SimpleMouseTracker/get_data.py -s $(PROJ)
step4:
	python SimpleMouseTracker/get_data.py -j $(FRAME) -d $(INTERVAL) $(PROJ)

cp:
	mkdir -p $(CROWDPROCESS_DIR)/build
	mkdir -p $(CROWDPROCESS_DIR)/data
	mkdir -p $(CROWDPROCESS_DIR)/pre/build
	cd $(C_DIR) && \
	$(EMCC) $(EMCCFLAGS) $(SOURCES) ../$(CROWDPROCESS_DIR)/$(LIBJS) $(SETTINGS) -o ../$(CROWDPROCESS_DIR)/pre/build/$(EXEC).js
	cd $(CROWDPROCESS_DIR)/pre/ &&\
	cat ./data/data.json | gencpd --compress ./lib/LZString > ../$(DATA) && \
	cat ./view/view.json | gencpp --template ./template/template.mustache > ../build/$(EXEC).js

run-editor:
	reagenzglas -p $(CROWDPROCESS_DIR)/build/$(EXEC).js

