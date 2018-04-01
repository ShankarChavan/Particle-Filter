# Particle-Filter Kidnapped vehcile


## Overview
This project implements a Particle Filter applied to a Kidnapped car(robot) problem. 

## Project Introduction
Our robot has been kidnapped and transported to a new location! Luckily it has a map of this location, a (noisy) GPS estimate of its initial location, and lots of (noisy) sensor and control data.

In this project we needed to implement a 2 dimensional particle filter in C++. Our particle filter was given a map and some initial localization information (analogous to what a GPS would provide). At each time step our filter will also get observation and control data.

## Running the Code
This project involves the Term 2 Simulator which can be downloaded [here](https://github.com/udacity/self-driving-car-sim/releases)

This repository includes two files that can be used to set up and intall uWebSocketIO for either Linux or Mac systems. For windows you can use either Docker, VMware, or even Windows 10 Bash on Ubuntu to install uWebSocketIO.

Once the install for uWebSocketIO is complete, the main program can be built and ran by doing the following from the project top directory.

1. mkdir build
2. cd build
3. cmake ..
4. make
5. ./particle_filter

Alternatively some scripts have been included to streamline this process, these can be leveraged by executing the following in the top directory of the project:

1. ./clean.sh
2. ./build.sh
3. ./run.sh


# Implementing the Particle Filter
The directory structure of this repository is as follows:

```
root
|   build.sh
|   clean.sh
|   CMakeLists.txt
|   README.md
|   run.sh
|
|___data
|   |   
|   |   map_data.txt
|   
|   
|___src
    |   helper_functions.h
    |   main.cpp
    |   map.h
    |   particle_filter.cpp
    |   particle_filter.h
```


## Inputs to the Particle Filter
Inputs to the particle filter was in the `data` directory. 

#### The Map
`map_data.txt` includes the position of landmarks (in meters) on an arbitrary Cartesian coordinate system. Each row has three columns
1. x position
2. y position
3. landmark id

### All other data the simulator provides, such as observations and controls.

> * Map data provided by 3D Mapping Solutions GmbH.


## Output of the Project

At the start of the project

![Simulator Start](https://raw.github.com/ShankarChavan/Particle-Filter/master/img/start_particle.png)

Output while running the simulator and PF

![PF_running](https://raw.github.com/ShankarChavan/Particle-Filter/master/img/running_particle.png)

final output of the simulator and PF

![Successful criteria met](https://raw.github.com/ShankarChavan/Particle-Filter/master/img/particle_filter_proj.png)



