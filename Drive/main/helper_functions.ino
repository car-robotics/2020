#define WHEEL_DIAMETER_CM 9.6
#define TICKS_PER_ROTATION 500 //370
#define TICKS_PER_CM (TICKS_PER_ROTATION / (WHEEL_DIAMETER_CM * M_PI))

double distanceToTicks(double distance) {
  if(distance < 0){
    return -1.0 * distance * TICKS_PER_CM;
  }
  return distance * TICKS_PER_CM;
}

void deliver(int angle){ //decrease number to move up
  deliverer.write(angle);
  delay(200);
}

bool isValidDouble(String s) {
  bool hasUsedDigit = true;
  bool hasUsedDot = false;
  for (int i = 0; i < s.length(); i++) {
    char c = s.charAt(i);
    if (c == '-') {
      if (i > 0) {
        return false;
      }
    } else if (c == '.') {
      if (hasUsedDot) {
        return false;
      }
      hasUsedDot = true;
    } else if (!isDigit(c)) {
      return false;
    } else {
      hasUsedDigit = true;
    }
  }
  return hasUsedDigit;
}

double readDouble() {
  if (Serial.available()) {
    String s = Serial.readString();
    s = s.substring(0, s.length() - 1);
    if (isValidDouble(s)) {
      return s.toDouble();
    }
  }
  return -99;
}

double getHeading() {
  return getHeadingDiff(0);
}

double getHeadingDiff(double targetHeading) {
  sensors_event_t event;
  bno.getEvent(&event);
  
  double heading = event.orientation.x - headingOffset - targetHeading;
  
  while (heading > 180) {
    heading -= 360;
  }
  while (heading < -180) {
    heading += 360;
  }
  return heading;
}

double signum(double value) {
  if (value > 0) return 1;
  if (value < 0) return -1;
  return 0;
}

double clip(double value, double minVal, double maxVal) {
  return min(max(value, minVal), maxVal);
}

void setPower(double leftPower, double rightPower) //sets the speed of the servos
{
  leftPower *= 0.80;
  rightPower *= 1.15;
//  Serial.println("Setting powers left=" + String(leftPower) + ",\tright=" + String(rightPower));
  servoLeft.write(90 - leftPower * 90); 
  servoRight.write(90 + rightPower * 90);
}

void setPower(double power) {
  setPower(power, power);
}
