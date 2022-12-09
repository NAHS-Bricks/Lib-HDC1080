#include <nahs-Bricks-Lib-HDC1080.h>

HDC1080_SerialNumber sn;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(15);
  Serial.println(HDC1080.begin());
  HDC1080.getSN(sn);
  HDC1080.triggerRead();
}

void loop() {
  Serial.println(HDC1080.isConnected());
  Serial.println(HDC1080.snToString(sn));
  Serial.println(HDC1080.getT());
  Serial.println(HDC1080.getH());
  HDC1080.triggerRead();
  delay(1000);
}