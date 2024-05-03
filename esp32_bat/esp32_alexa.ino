//////////////// Library Files //////////////////
#include "Secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
// #include <BLEDevice.h>
/////////////////////////////////////////////////

///////////// BLE Initialization ////////////////
// String knownBLEAddresses[] = { "d6:36:01:21:04:ed", "ca:68:2b:92:d6:a9" };
// int num_ble_devices = sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0]);
// int RSSI_THRESHOLD = -80;
// int scanTime = 1;
// int ble_count = 0;
// BLEScan* pBLEScan;
// int lowest_rssi = 0;
// bool found = 0;
// int i = 0, j = 0;
// int rssi = 0;
// int total = 0;
/////////////////////////////////////////////////


///////////// Task Declarations /////////////////
// #define BLE_TASK_DELAY 100
// //#define BLE_TASK_DELAY 500------> for slave4 only
// // #define WIFI_TASK_DELAY 10000
// TaskHandle_t task_ble_handle = NULL;
// // TaskHandle_t task_wifi_handle = NULL;
/////////////////////////////////////////////////

////////////// WiFi Credentials /////////////////
#define EAP_IDENTITY "yashg@iisc.ac.in"
#define EAP_PASSWORD "NeedForSpeed87654321"
#define EAP_USERNAME "yashg@iisc.ac.in"
#define ssid "iiscwlan"
#define WIFI_TIMEOUT_MS 60000
#define MQTT_TIMEOUT_MS 10000
/////////////////////////////////////////////////

//////////// Topic Configurations ///////////////
#define device_name "slave1"
#define AWS_IOT_SUBSCRIBE_TOPIC "slave1/appliance"
#define AWS_IOT_PUBLISH_TOPIC "bat/result"
#define AWS_IOT_PUBLISH_RSSI "slave1/rssi"
/////////////////////////////////////////////////

//////////// Hardware Configurations ////////////
#define DRIVE 23
#define NOSHOT 22
#define PULL 5
/////////////////////////////////////////////////

//////////// Switch use Indicator ///////////////
bool switch_used = 0;
/////////////////////////////////////////////////

///////////// MQTT Client Initialize ////////////
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
/////////////////////////////////////////////////

void publishMessage(char* topic_name, int state) {
  StaticJsonDocument<200> doc;
  doc["result"] = state;
  // doc["appliance_state"] = state;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // print to client

  client.publish(topic_name, jsonBuffer);
}
void publishRSSI(char* topic_name, int rssi) {
  StaticJsonDocument<200> doc;
  doc["device_id"] = device_name;
  doc["rssi"] = rssi;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);  // print to client

  client.publish(topic_name, jsonBuffer);
}
void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("[MQTT]\t\t\tTopic Received: ");
  Serial.println(topic);

  if (strstr(topic, AWS_IOT_SUBSCRIBE_TOPIC)) {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);
    String Relay1 = doc["status"];
    Serial.print("[MQTT]\t\t\tString Status received: ");
    Serial.println(Relay1);
    int r1 = Relay1.toInt();
    Serial.print("[MQTT]\t\t\tStatus received: ");
    Serial.println(r1);
    if (r1 == 1) {
      // digitalWrite(APPLIANCE, LOW);
      Serial.println("[MQTT]\t\t\tAPPLIANCE is ON");
    } else if (r1 == 0) {
      // digitalWrite(APPLIANCE, HIGH);
      Serial.println("[MQTT]\t\t\tAPPLIANCE is OFF");
    }
  }
}

// void IRAM_ATTR switchChangeISR() {
//   if (!digitalRead(SWITCH)) {
//     digitalWrite(APPLIANCE, LOW);
//   } else {
//     digitalWrite(APPLIANCE, HIGH);
//   }
//   switch_used = 1;
// }

void allInitialization() {

  pinMode(DRIVE, INPUT);
  pinMode(NOSHOT, INPUT);
  pinMode(PULL, INPUT);
  // digitalWrite(APPLIANCE, HIGH);
  // attachInterrupt(SWITCH, switchChangeISR, CHANGE);


  WiFi.mode(WIFI_STA);
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print(F("[WiFi]\t\t\tConnecting to network: "));
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect from WiFi to set new WiFi connection

  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);  // without CERTIFICATE, RADIUS server EXCEPTION "for old devices" required

  // Serial.println("[WiFi]\t\t\tConnecting to Wi-Fi");
  Serial.println("[WiFi]\t\t\tWiFi is connected!");
  // Serial.print("[WiFi]\t\t\tIP address set: ");
  // Serial.println(WiFi.localIP());  //print LAN IP
  while (WiFi.status() != WL_CONNECTED) {
    // delay(500);
    // Serial.println("[WiFi]\t\t\t.");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);

  // Create a message handler
  client.setCallback(messageHandler);

  Serial.println("[AWS Conn]\t\tConnecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    // Serial.println("[AWS Conn]\t\t.");
    // delay(100);
  }

  if (!client.connected()) {
    Serial.println("[AWS Conn]\t\tAWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);


  Serial.println("[AWS Conn]\t\tAWS IoT Connected!");


  // Serial.println("[BLE]\t\t\tBLE initializing");
  // BLEDevice::init("");
  // pBLEScan = BLEDevice::getScan();  //create new scan
  // pBLEScan->setActiveScan(true);    //active scan uses more power, but get results faster
  // pBLEScan->setInterval(50);        // set Scan interval
  // pBLEScan->setWindow(49);
  // Serial.println("[BLE]\t\t\tBLE initialized");
}


// void bleHandler(void* parameters) {
//   // vTaskeDelay(2000 / portTICK_PERIOD_MS);
//   for (;;) {
//     lowest_rssi = 0;
//     found = 0;
//     BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
//     for (i = 0; i < foundDevices.getCount(); i++) {
//       BLEAdvertisedDevice device = foundDevices.getDevice(i);
//       rssi = device.getRSSI();
//       if (rssi > RSSI_THRESHOLD) {
//         for (j = 0; j < num_ble_devices; j++) {
//           if (strcmp(device.getAddress().toString().c_str(), knownBLEAddresses[j].c_str()) == 0) {
//             Serial.print("[BLE]\t\t\tDevice Address: ");
//             Serial.print(device.getAddress().toString().c_str());
//             Serial.print(" | RSSI: ");
//             Serial.println(rssi);
//             found = 1;
//             if (lowest_rssi > rssi)
//               lowest_rssi = rssi;
//           }
//         }
//       }
//     }
//     if (found == 0) {
//       total++;
//     } else {
//       total = 0;
//     }
//     pBLEScan->clearResults();
//     vTaskDelay(BLE_TASK_DELAY / portTICK_PERIOD_MS);
//     // continue;
//   }
// }


void setup() {
  Serial.begin(115200);
  allInitialization();
  // xTaskCreate(
  //   bleHandler,
  //   "BLE Handler",
  //   5000,
  //   NULL,
  //   2,
  //   &task_ble_handle);
}


void loop() {
  // delay(100);
  client.loop();
  if (!digitalRead(DRIVE)) {
    Serial.println("[MODEL]\t\t\tDrive");
    publishMessage(AWS_IOT_PUBLISH_TOPIC, 1);
    while(!digitalRead(DRIVE)){}
  }
  if (!digitalRead(NOSHOT)) {
    Serial.println("[MODEL]\t\t\tNo Shot");
    publishMessage(AWS_IOT_PUBLISH_TOPIC, 0);
    while(!digitalRead(NOSHOT)){}
  }
  if (!digitalRead(PULL)) {
    Serial.println("[MODEL]\t\t\tPull Shot");
    publishMessage(AWS_IOT_PUBLISH_TOPIC, 2);
    while(!digitalRead(PULL)){}
  }
  // if (lowest_rssi !=0) {
  //   if (total < 10) {
  //     // digitalWrite(APPLIANCE, LOW);
  //     publishRSSI(AWS_IOT_PUBLISH_RSSI, lowest_rssi);
  //   } else {
  //     // digitalWrite(APPLIANCE, HIGH);
  //     publishRSSI(AWS_IOT_PUBLISH_RSSI, 100);
  //     total = 10;
  //   }
  // }
  // // if (lowest_rssi < RSSI_THRESHOLD)
  // //   publishRSSI(AWS_IOT_PUBLISH_RSSI, lowest_rssi);
  // // else
  // //   publishRSSI(AWS_IOT_PUBLISH_RSSI, 100);
  // // delay(100);
  // if (switch_used) {
  //   Serial.print("[SWITCH]\t\tAppliance is switched ");
  //   if (!digitalRead(APPLIANCE)) {
  //     Serial.println("On");
  //     publishMessage(AWS_IOT_PUBLISH_TOPIC, 1);
  //   } else {
  //     Serial.println("Off");
  //     publishMessage(AWS_IOT_PUBLISH_TOPIC, 0);
  //   }
  //   switch_used = 0;
  // }
}
