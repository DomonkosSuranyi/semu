#Bugs and improvements

This document lists the known bugs and ideas for improvements.

##Emulator

##Mocksensor
* SensorData parsing throws uncontrolled exception when parse fails

##Sonardetect

##Other
* Lack of logging
* named argument list would be easier to use instead of environment variables to get sensor file paths
* There should be a `Timestamped` abstract class over `SonarData` which stores the timestamp. Thus the `TimestampedTimingSensor`'s template type could be ensured to have a `timestamp` field. (And all the timestamp-based timing could be generalized.)
* Current implementation does not allow multiple sensor update callbacks on a single sensor (YAGNI)

