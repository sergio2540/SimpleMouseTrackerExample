FRAME = 1050
INTERVAL=300
PROJ= test
VIDEO= video.mp4

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
