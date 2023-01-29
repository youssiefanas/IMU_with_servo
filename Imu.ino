#include <Servo.h>
#include "Wire.h"
#include <MPU6050_light.h>
#include <ros.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>

Servo myservo;

ros::NodeHandle nh;
sensor_msgs::Imu Imu_data;
ros::Publisher my_imu("my_imu",&Imu_data);

void servomsg(const std_msgs::Int16& dgr){
myservo.write(dgr.data);

}

/*
void servomsg(const std_msgs::String& srvmsg){
  if (strcmp(srvmsg.data, "r")==0){
   myservo.write(0);
  }
  else if (strcmp(srvmsg.data, "l")==0){
    myservo.write(150);
  }
  else if(strcmp(srvmsg.data, "s")==0){
    myservo.write(90);
  }
}
*/
ros::Subscriber<std_msgs::Int16> servo_dirc("servo_direction", &servomsg);
unsigned long timer = 0;
MPU6050 mpu(Wire);

void setup() {
   Serial.begin(57600);
  myservo.attach(3);
    Wire.begin();
    byte status = mpu.begin();
    while(status!=0){ }
    delay(100);
    mpu.calcOffsets();
    nh.initNode();
    nh.advertise(my_imu);
    nh.subscribe(servo_dirc);

}

void loop() {
    mpu.update();
  
  if((millis()-timer)>10){ // print data every 100ms
 
  Imu_data.orientation.x=mpu.getAngleX();
  Imu_data.orientation.y=mpu.getAngleY();
  Imu_data.orientation.z=mpu.getAngleZ();
  Imu_data.orientation.w=1;
Serial.println(mpu.getAngleX());
Serial.println(mpu.getAngleY());
Serial.println(mpu.getAngleZ());
  //Serial.println(mpu.getAngleX());
  timer = millis();  

  my_imu.publish(&Imu_data);
  nh.spinOnce();
  delay(10);
  }
  // put your main code here, to run repeatedly:

}
