Python code to run the raspberry pi camera


To set up: 

1. Update Python3 to 3.7.1 - Follow steps from: https://raspberrypi.stackexchange.com/questions/59381/how-do-i-update-my-rpi3-to-python-3-6

   - Download Python-3.7.1.tgz from here: https://www.python.org/downloads/source/
   - Unzip and run ./configure 					(2 minutes ish)
   - make -j4  (-j4 uses all 4 rpi cores to run faster) 	(15 minutes ish)
   - sudo make install 						(2 minutes ish)

(Not sure if you need the most recent python3 version, but i like having an updated python version)

2. Enable Pi camera support

   - Interfacing Options -> Camera -> Yes

3. Run the script

   - python3 takePic.py


