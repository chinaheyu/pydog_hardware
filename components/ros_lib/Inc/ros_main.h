#ifndef ROS_MAIN_H
#define ROS_MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
 
extern void ros_main(void *argument);
extern void gas_sensor_callback(double raw);
extern void anemometer_callback(double wind_direction, double wind_speed);
 
#ifdef __cplusplus
	}
#endif

#endif // ROS_MAIN_H