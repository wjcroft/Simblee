/*The sketch accepts a Bluetooth Low Energy 4 connection from aniPhone and accepts commands from the iPhone to run upto 4standard servos.This sketch is suppose to work with the simbleeServo application.It receives two bytes from the iPhone.  The first byte containsthe servos to set (bit1 = servo a, bit2 = servo b, etc), andthe value is the number of degrees (0-180) to position the servotoo.*//* * Copyright (c) 2015 RF Digital Corp. All Rights Reserved. * * The source code contained in this file and all intellectual property embodied in * or covering the source code is the property of RF Digital Corp. or its licensors. * Your right to use this source code and intellectual property is non-transferable, * non-sub licensable, revocable, and subject to terms and conditions of the * SIMBLEE SOFTWARE LICENSE AGREEMENT. * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt * * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. * * This heading must NOT be removed from this file. */#include <Servo.h>#include <SimbleeBLE.h>Servo s1;Servo s2;Servo s3;Servo s4;void setup() {  s1.attach(2);  s2.attach(3);  s3.attach(4);  s4.attach(5);  SimbleeBLE.advertisementInterval = 675;  SimbleeBLE.advertisementData = "-servo";  SimbleeBLE.begin();}void loop() {  // Simblee_ULPDelay(INFINITE);}void SimbleeBLE_onReceive(char *data, int len){  int servo = data[0];  int degree = data[1];      if (bitRead(servo, 1))    s1.write(degree);  if (bitRead(servo, 2))    s2.write(degree);  if (bitRead(servo, 3))    s3.write(degree);  if (bitRead(servo, 4))    s4.write(degree);}