# CS177 Final Project

Simulates cars dropping students off at the front of the school before classes begin.

[About the Project](https://github.com/bypie5/CS177-Final-Project/blob/master/Project_Documentation.pdf)

### Authors: Brandon Yi

**Note: Uses [CSIM 19.0](http://www.mesquite.com/)**

## How To Use

There are two operating modes for this application: Regular drivers or Autonomous vehicles. Deciding which mode you'd like to use can be made before you compile the application.

### Regular Drivers

To compile the application in Regular drivers mode, perform the following steps:

```
git clone https://github.com/bypie5/CS177-Final-Project.git
cd CS177-Final-Project
make
```

You can now run the application in Regular drivers mode. 

### Autonomous Vehicles

To compile the application in Autonomous vehicles mode, perform the following steps:

```
git clone https://github.com/bypie5/CS177-Final-Project.git
cd CS177-Final-Project
make autopilot
```

## Execution

Now that the application has been compiled in one of the two modes, the application has been put into the `./bin` directory.

### Execution options

Run the application with the following options:

```
./bin/dropoff_sim <car lengths before dropoff zones> <# of dropoff zones> <car lengths after dropoff zones>
```

