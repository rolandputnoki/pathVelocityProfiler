//
// Created by osboxes on 02/04/17.
//

#ifndef PATH_VELOCITY_PROFILER_CONF_H
#define PATH_VELOCITY_PROFILER_CONF_H

#pragma once

#define EPS             0.0001
#define TIME_STEP       0.05
#define DIST_STEP       0.05
#define RADIUS_SAMPLE   0.1

//#define TANG_ACCEL_MAX      16.0		//Minimális pályairányú gyorsulás
//#define TANG_ACCEL_MIN      -18.0		//Minimális pályairányú gyorsulás
//#define NORMAL_ACCEL_MAX    30.0		//Maximális centripetális gyorsulás
#define DRAG_COEF           0.0021		//Tapadási tényező
#define MAX_VELOCITY        12          //Maximális sebessége a robotnak


#define TANG_ACCEL_MAX      1
#define NORMAL_ACCEL_MAX    1
#define TANG_ACCEL_MIN      1


#endif //PATH_VELOCITY_PROFILER_CONF_H
