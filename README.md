# cricket-edge-ai
On-premise intelligence for Cricket Bat and Ball

Bat- Shot Detection:

1. Set up the circuit as in the circuit diagram attached. 
2. Place the whole circuit on the lower part of the bat, right behind the Sweetspot.
3. Switch on the circuit.
4. Using data cable put the code using Arduino IDE, which has been dowloaded from GitHub.
5. Select the boards correct boards while uploading the code.
6. Code under bat folder is for the Arduino Nano Board attached to the Bat.
7. Code under esp32_bat folder is for the ESP32 board attached to the Bat.
8. Make the necessary changes in the SSID, Username, Password definitions as per your credentials.
9. In Secrets.h file change the AWS IoT endpoint according your AWS account.
10. Login to your AWS account and go to AWS IoT Core service.
11. Open MQTT Test Client, and subscribe to bat/result topic.
12. Swing the bat and check your shot.

Ball:
1. Connect the 9V cell to the Arduino Nano board.
2. Code under ball folder  is for the Arduino Nano Board attached to the Ball.
3. Code under esp32_ball folder is for the ESP32 board attached to the Ball.
4. Make the necessary changes in the SSID, Username, Password definitions as per your credentials.
5. In Secrets.h file change the AWS IoT endpoint according your AWS account.
6. Login to your AWS account and go to AWS IoT Core service.
7. Open MQTT Test Client, and subscribe to ball/result topic.
8. Throw the ball and check your shot.


Ball Tracker:
1. Connect the 5V cell to the ESP32 CAM board.
3. Code under esp32cam folder is for the ESP32 Cam board attached to the Ball.
4. Make the necessary changes in the SSID, Username, Password definitions as per your credentials.
5. In Secrets.h file change the AWS IoT endpoint according your AWS account.
6. Login to your AWS account and go to AWS IoT Core service.
7. Open MQTT Test Client, and subscribe to camera/ip topic.
8. Open the IP inyour browser and access the Ball traker.
