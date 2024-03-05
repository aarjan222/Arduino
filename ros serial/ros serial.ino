#include <ros2arduino.h>
#include <user_config.h>
// #include "my_package/SensorMessage.hpp"



ros2::NodeHandle nh;
my_package::msg::SensorMessage msg;

void sensorCallback(const my_package::msg::SensorMessage& received_msg) {
  // Process received sensor data
  float sensor_data = received_msg.sensor_data;
  // Your Arduino logic goes here
}

void setup() {
  nh.initNode();
  nh.subscribe("sensor_data", &sensorCallback);
}

void loop() {
  nh.spinOnce();
  delay(100);
}
