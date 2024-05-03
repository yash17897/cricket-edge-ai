//////////////// Library Files //////////////////
#include "Secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
/////////////////////////////////////////////////


////////////// WiFi Credentials /////////////////
#define EAP_IDENTITY "yashg@iisc.ac.in"
#define EAP_PASSWORD "xxxxxxxxxxxxxxxxxxxxxx"
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

void allInitialization() {

  pinMode(DRIVE, INPUT);
  pinMode(NOSHOT, INPUT);
  pinMode(PULL, INPUT);


  WiFi.mode(WIFI_STA);

  Serial.print(F("[WiFi]\t\t\tConnecting to network: "));
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect from WiFi to set new WiFi connection

  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);  // without CERTIFICATE, RADIUS server EXCEPTION "for old devices" required


  Serial.println("[WiFi]\t\t\tWiFi is connected!");
  while (WiFi.status() != WL_CONNECTED) {
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
}

void setup() {
  Serial.begin(115200);
  allInitialization();
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
}
