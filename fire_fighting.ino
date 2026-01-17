#include <Adafruit_MLX90614.h>
#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#define M_1 2
#define M_2 3
#define M_3 4
#define M_4 5
#define relay 6
#define IR1 9
#define IR2 10
#define IR3 11
#define buzzer 13

int a, b, c, d;
int pos = 0;
unsigned long previousMillis = 0;
const long tempInterval = 1000;

SoftwareSerial SIM800L(7, 8);  // RX, TX

bool fireAlertSent = false;
unsigned long lastFireTime = 0;
const unsigned long fireResetDelay = 15000; // 15 sec cooldown

void setup() {
  Serial.begin(9600);
  SIM800L.begin(9600);

  // Sensor startup
  if (!mlx.begin()) {
    Serial.println("MLX90614 not found!");
    // Don't hang, just skip usage
  }

  pinMode(M_1, OUTPUT); pinMode(M_2, OUTPUT);
  pinMode(M_3, OUTPUT); pinMode(M_4, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(IR1, INPUT_PULLUP);
  pinMode(IR2, INPUT_PULLUP);
  pinMode(IR3, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  myservo.attach(12);
  myservo.write(90);

  digitalWrite(relay, LOW);
  digitalWrite(buzzer, LOW);

  Serial.println("System Ready.");
}

void loop() {
  a = digitalRead(IR1);
  b = digitalRead(IR2);
  c = digitalRead(IR3);

  temp();  
  fire();
}

void fire() {
  if (a == 0) {
    left();
  } else if (b == 0) {
    right();
  } else if (a == 1 && b == 1 && c == 1) {
    Stop();
  }

  if (c == 0 && d > 33) {
    Stop();
    digitalWrite(relay, HIGH);

    if (!fireAlertSent || (millis() - lastFireTime > fireResetDelay)) {
      fireAlertSent = true;
      lastFireTime = millis();
      servoNonBlocking();
      delay(2000);
      fireDetected();
    }

  } else if (c == 0) {
    back();
  } else {
    Stop();
    digitalWrite(relay, LOW);
    myservo.write(90);

    if (millis() - lastFireTime > fireResetDelay) {
      fireAlertSent = false;
    }
  }
}

void temp() {
  if (millis() - previousMillis >= tempInterval) {
    previousMillis = millis();

    float objTemp = mlx.readObjectTempC();
    if (isnan(objTemp)) {
      Serial.println("Sensor Error!");
      return;
    }

    d = (int)objTemp;
    Serial.print("Temperature: ");
    Serial.print(d);
    Serial.println(" °C");
  }
}

void forward() {
  digitalWrite(M_1, HIGH);
  digitalWrite(M_2, LOW);
  digitalWrite(M_3, HIGH);
  digitalWrite(M_4, LOW);
}

void back() {
  digitalWrite(M_1, LOW);
  digitalWrite(M_2, HIGH);
  digitalWrite(M_3, LOW);
  digitalWrite(M_4, HIGH);
}

void left() {
  digitalWrite(M_1, HIGH);
  digitalWrite(M_2, LOW);
  digitalWrite(M_3, LOW);  
  digitalWrite(M_4, HIGH);
}

void right() {
  digitalWrite(M_1, LOW);  
  digitalWrite(M_2, HIGH);
  digitalWrite(M_3, HIGH);
  digitalWrite(M_4, LOW);
}

void Stop() {
  digitalWrite(M_1, LOW);
  digitalWrite(M_2, LOW);
  digitalWrite(M_3, LOW);
  digitalWrite(M_4, LOW);
}

void fireDetected() {
  Serial.println("🔥 Fire Detected!");
  digitalWrite(buzzer, HIGH);
  sendSMS("🔥 Fire detected! Take action.");
  delay(1000);  // Short tone
  digitalWrite(buzzer, LOW);
  digitalWrite(relay, LOW);
  myservo.write(90);
}

void sendSMS(String message) {
  Serial.println("Sending SMS...");
  SIM800L.println("AT+CMGF=1");
  delay(1000);

  SIM800L.println("AT+CMGS=\"+91123456789\"");
  delay(1000);
  SIM800L.print(message);
  SIM800L.write(26);  // Ctrl+Z
  delay(3000);        // Allow send
  Serial.println("SMS Sent.");
}

// Non-blocking servo movement (optional, can expand further if needed)
void servoNonBlocking() {
  for (int i = 40; i <= 140; i += 5) {
    myservo.write(i);
    delay(15);
  }
  for (int i = 140; i >= 40; i -= 5) {
    myservo.write(i);
    delay(15);
  }
}