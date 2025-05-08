#include <Arduino.h>
#include <MQUnifiedsensor.h>

#define placa "STM32"
#define Voltage_Resolution 3.3
#define pin PA0
#define type "MQ-135"
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  

MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

unsigned long lastDataTime = 0;
const int samplingInterval = 1000;

void setup() {
  Serial.begin(115200);

  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setA(102.2); MQ135.setB(-2.473); // Cấu hình phương trình để tính nồng độ NH4

  /*
    Hồi quy hàm mũ:
    Khí      | a      | b
    CO       | 605.18 | -3.937  
    Cồn      | 77.255 | -3.18 
    CO2      | 110.47 | -2.862
    Toluen   | 44.947 | -3.445
    NH4      | 102.2  | -2.473
    Aceton   | 34.668 | -3.369
  */
  
  MQ135.init(); 
  
  Serial.print("Đang hiệu chuẩn, vui lòng chờ.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update();
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  Hoàn tất!");
  
  if(isinf(calcR0)) {Serial.println("Cảnh báo: Lỗi kết nối, R0 là vô hạn (Phát hiện mạch hở), vui lòng kiểm tra dây và nguồn cấp"); while(1);}
  if(calcR0 == 0){Serial.println("Cảnh báo: Lỗi kết nối, R0 bằng 0 (Chân analog bị nối tắt xuống đất), vui lòng kiểm tra dây và nguồn cấp"); while(1);}
 
  MQ135.serialDebug(true);
}

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastDataTime >= samplingInterval) {
      lastDataTime = currentTime;
    MQ135.update();
    MQ135.readSensor();
    MQ135.serialDebug();
    }
}