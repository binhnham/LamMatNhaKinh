#include <OneWire.h>
#include <DallasTemperature.h>

// Khai báo chân cảm biến và động cơ
#define IN1 8          // Chân IN1 của L298N
#define IN2 9          // Chân IN2 của L298N
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Ngưỡng nhiệt độ để bật động cơ
const float TEMP_THRESHOLD = 27.0;
bool manualControl = false;

void setup() {
  sensors.begin();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Đọc giá trị từ cảm biến nhiệt độ
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  Serial.println(temperature);

  if (Serial.available()) {
    String command = Serial.readString();
    command.trim(); // Loại bỏ khoảng trắng
     if (command == "ON") {
      digitalWrite(IN1, HIGH); // Bật động cơ
      digitalWrite(IN2, LOW);
      manualControl = true;    // Chuyển sang chế độ điều khiển thủ công
    } else if (command == "OFF") {
      digitalWrite(IN1, LOW);  // Tắt động cơ
      digitalWrite(IN2, LOW);
      manualControl = true;    // Chuyển sang chế độ điều khiển thủ công
    }
}
  if (!manualControl) {
    // Điều khiển động cơ
    if (temperature >= TEMP_THRESHOLD) {
      digitalWrite(IN1, HIGH); // Bật động cơ
      digitalWrite(IN2, LOW);
    } else {
      digitalWrite(IN1, LOW); // Tắt động cơ
      digitalWrite(IN2, LOW);
    }
  }
  delay(1000); // Chờ 1000ms trước khi đọc lại
}
