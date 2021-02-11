#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 256 // Default 128

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

#define led_green 4
#define led_red 5
#define buzzer 6
#define trig 9
#define echo 8

long lecture_echo;
long cm;

// dapatkan auth key dari blynk emlalui email
char auth[] = "VkSKV8tGf7bw94JmRLTFROPy6USrlIwj";

// gunakan nama hotspot hp yang ada aplikasi blynk
char ssid[] = "Redmi";
char pass[] = "12345678";

// kalau espnya belum disetting pake 115000
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
}

void setup() {

  Serial.begin(9600);
  
  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, wifi, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, wifi, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(trig, LOW);
  led_off();
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  lecture_echo = pulseIn(echo, HIGH);
  cm = lecture_echo / 57;

  if(cm <= 10){
    led_off();
    digitalWrite(led_red, HIGH);
    warning(1000);
    Blynk.notify("Bahaya!! Level air tinggi, indikasi banjir!");
  } else {
    led_off();
    digitalWrite(led_green, HIGH);
  }
  delay(500);
}

void led_off() {
  digitalWrite(led_green, LOW);
  digitalWrite(led_red, LOW);
}

void warning(long freq) {
  tone(buzzer, freq);
  delay(500);
  noTone(buzzer);
  delay(5);
}
