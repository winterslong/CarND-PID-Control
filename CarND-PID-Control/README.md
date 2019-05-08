# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

Fellow students have put together a guide to Windows set-up for the project [here](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/files/Kidnapped_Vehicle_Windows_Setup.pdf) if the environment you have set up for the Sensor Fusion projects does not work for this project. There's also an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3).

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Reflection

### Describe the effect each of the P, I, D components had in your implementation.

- The proportional portion of the controller (P-control) tries to steer the car toward the center line (against the cross-track error). If used along, the car overshoots the central line very easily and go out of the road very quickly. 

  In P-control, the coefficient Kp will directly affect the actual control effect. In a reasonable range, the larger the Kp, the better the control effect (the faster it returns to the reference line). However, when the position of itself is far away from the reference line and the Kp coefficient is large, the vehicle will overshoot and lose control.

- The differential portion helps to counteract the proportional trend to overshoot the center line by smoothing the approach to it. 

  In the PD control, we have two coefficients to adjust, intuitively speaking, increasing Kp coefficient will increase the tendency of car moving toward the reference line, and increasing Kd coefficient will increase the resistance of car moving toward the reference line, thus making the motion toward the reference line more smooth.

  The system with too large Kp coefficient and too small Kd coefficient is called underdamped. In this case, the unmanned car will oscillate along the reference line. On the contrary, if the Kp coefficient is too small and the Kd coefficient is too large, then we call it overdamped, which will make the unmanned car take a long time to correct its error. Proper selection of Kp and Kd parameters can enable the car to return to the reference line quickly while maintaining good motion on the reference line.

- The integral portion tries to eliminate a possible bias on the controlled system that could prevent the error to be eliminated. If used along, it makes the car to go in circles. In the case of the simulator, no bias is present. 

  The integral coefficient Ki also affects the stability of the whole control system. Too large Ki will make the control system "oscillate" and too small Ki will make the controlled vehicle in a steady state take a long time to return to the reference line after encountering disturbances, which in some cases will inevitably put the vehicle in a dangerous situation.

### Describe how the final hyperparameters were chosen.

The parameters were chosen manually by try and error. First, make sure the car can drive straight with zero as parameters. Then add the proportional and the car start going on following the road but it starts overshooting go out of it. Then add the differential to try to overcome the overshooting. The integral part only moved the car out of the road; so, it stayed as zero. After the car drove the track without going out of it, the parameters increased to minimize the average cross-track error on a single track lap. The final parameters where [Kp:0.25, Ki:0.0003, Kd:2.5]

## Simulation

### The vehicle have successfully driven  laps around the track.



