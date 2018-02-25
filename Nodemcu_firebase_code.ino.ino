#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
// Set these to run example.
#define FIREBASE_HOST "iotfirebaseproject-63fd6.firebaseio.com"
#define FIREBASE_AUTH "pF8ReKUvYUsxV0v8gBMMatwkJpfSgrI3hzPlPIIo"
#define WIFI_SSID "SLT_4GLTE"
#define WIFI_PASSWORD "71465260"
 bool motor_state = false;
 bool bubbler_state = false;
float grams ;
 int foodLevel=0;
void drop (){
    Serial.println("Droping");
    digitalWrite(BUILTIN_LED,HIGH);
    delay(2000);
    digitalWrite(BUILTIN_LED,LOW);
    delay(2000);
}
void motor_state_preocess(bool mot_state){
  if (mot_state==true){
                    grams =Firebase.getFloat("Nodemcu/grams"); 
                   int drop_time =grams/1.5;
                   int conform =0;
                 for (int u=0;u <drop_time;u++){
                        drop ();
                        conform++;
                 }
                 if (conform==drop_time){
                        Firebase.setBool("Nodemcu/state", false);
                        Firebase.setFloat("Nodemcu/grams", 0);
                        if (Firebase.failed()) {
                        Serial.print("setting /number failed:");
                        Serial.println(Firebase.error()); return; }
                        delay (100);
                 }
          }
  }

 void bubbler_control(bool bub_state){
        if (bub_state==true){
              Serial.println("Bubbler Working");
              digitalWrite(D1,HIGH);
          }else{
              digitalWrite(D1,LOW);
            }
  }
void  update_the_food_level(){
        Serial.println("Update the Food Level");
        foodLevel=map(analogRead(A0),0,1024,0,100);
        Firebase.setInt("food_level/current_level",foodLevel);  
  }
void setup() {
        Serial.begin(9600);
        pinMode(BUILTIN_LED, OUTPUT);
        pinMode(A0, INPUT);
        pinMode(D1, OUTPUT);
        // connect to wifi.
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
        Serial.print("connecting");
        while (WiFi.status() != WL_CONNECTED) {
              Serial.print(".");
               delay(500); 
         }
         Serial.println();
         Serial.print("connected: "); 
         Serial.println(WiFi.localIP()); 
         Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); }
void loop() {
        // set value //
       
        motor_state = Firebase.getBool("Nodemcu/state");
        bubbler_state =Firebase.getBool("bubbler");
        Serial.println(bubbler_state);
        // handle error
          if (Firebase.failed()) {
          Serial.print("setting /number failed:");
          Serial.println(Firebase.error()); return; }
          delay (100);
          update_the_food_level();
          motor_state_preocess(motor_state);
          bubbler_control(bubbler_state);
        delay(1000); 
 }
