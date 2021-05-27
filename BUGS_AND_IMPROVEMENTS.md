#Bugs and improvements

This document lists the known bugs and ideas for improvements.

* SensorData parsing throws uncontrolled exception when parse fails
* Lack of logging
* named argument list would be easier to use instead of environment variables to get sensor file paths
* Template specialization would result a more developer-friendly error message if the developer tries to put a non-Timestamped type into TimestampedTimingSensor\<T>
* Current implementation does not allow multiple sensor update callbacks on a single sensor (YAGNI)
* GeographicLib makes the build time very long. (Could we somehow specify what to build from that?)
* Different timing of the sensors might lead misfunctioning (e.g. if SpeedOfSound's update rate would be under 50Hz)

