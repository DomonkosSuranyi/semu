# norbit
The software builds up from the following modules,
all of them might be organized to a separated library:

### MockSensor
The file-feeded mocked sensor and all its parts

### Emulator
The logic of the emulation and the unterface it requires

### SonarDetect
This is the signal-processing module.  
It receives the sensor signals, collects them into an organized data-structure,  
and evaluates and exports them when possible.

## Build
```
mkdir build && cd build
cmake ..
make
```

It will fetch and build GeographicLib.  
It's fetch would take a while depending on the hosting website's responsiveness.  

Find the executable at `build/src/norbit`.

## Usage
The program requires the three sensor's (sonar, GNSS/INS, speed-of-sound) input file path as environment variable.  
These are the following:
```
SPEED_OF_SOUND_PATH
GNSS_PATH
SONAR_PATH
```

So run the program like this (assume that you are in the build directory):
```
SONAR_PATH="/path/to/sonar_short.txt" SPEED_OF_SOUND_PATH="/path/to/speed_of_sound.txt" GNSS_PATH="/path/to/gnss.txt" src/norbit
```

Find the output file next to the path of the execution, named `output.txt`

