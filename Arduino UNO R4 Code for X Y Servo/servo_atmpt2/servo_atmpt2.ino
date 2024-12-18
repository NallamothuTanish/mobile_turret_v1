#include<Servo.h>

Servo x, y;
int relayPin = 8; 
int width = 640, height = 480; 
int xpos = 90, ypos = 90;  
int centerTolerance = 45; 

void setup() {
  Serial.begin(9600);
  x.attach(9);
  y.attach(10);
  pinMode(relayPin, OUTPUT); 
  digitalWrite(relayPin, LOW); 
  x.write(xpos);
  y.write(ypos);
}

const int angle = 1;   

void loop() {
  if (Serial.available() > 0) {
    int x_mid, y_mid;
    if (Serial.read() == 'X') {
      x_mid = Serial.parseInt();  // Read center x-coordinate
      if (Serial.read() == 'Y') {
        y_mid = Serial.parseInt(); // Read center y-coordinate
      }
    }

    // Adjust servo positions if the face is outside the squared center region    
    if (x_mid > width / 2 + centerTolerance) xpos += angle;
    if (x_mid < width / 2 - centerTolerance) xpos -= angle;
    
    
    if (y_mid > height / 2 + centerTolerance) ypos -= angle; // Inverted
    if (y_mid < height / 2 - centerTolerance) ypos += angle;

    
    xpos = constrain(xpos, 0, 180);
    ypos = constrain(ypos, 35, 160);

    x.write(xpos);
    y.write(ypos);

    // Activate the relay (fire the gun) if the face is centered within tolerance
    if (abs(x_mid - width / 2) <= centerTolerance && abs(y_mid - height / 2) <= centerTolerance) {
      digitalWrite(relayPin, HIGH); // Activate relay to "fire"
      delay(100); // Relay active time, simulating a "fire" duration
      digitalWrite(relayPin, LOW); // Deactivate relay, ending the "fire" action
    }
    digitalWrite(relayPin, LOW);
  }
  
  digitalWrite(relayPin, LOW);
}

