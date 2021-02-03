#include <ESP8266WiFi.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>

String serverName = "http://MY.SERVER.ADDRESS:PORT/spindel";

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication
const uint8_t scl = 2;
const uint8_t sda = 0;

// sensitivity scale factor respective to full scale setting provided in datasheet
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;
const uint8_t MPU6050_REGISTER_SLEEP  = 0x06;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

const char* ssid = "MY_SSID";
const char* password = "MY_WPA2_KEY";
 
//int ledPin = D4;
WiFiServer server(80);

void setup() {
  
  //Serial.begin(115200);
  Wire.begin(sda, scl);
  
  MPU6050_Init();

  delay(5000);

  WiFi.begin(ssid, password);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i < 100)  {
    delay(100);
    i++;
    //Serial.print("...Trying to connect...");
  }

  server.begin();

  double Ax, Ay, Az, T, tilt, yaw, pitch, roll;
 
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
 
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  T = (double)Temperature/340+36.53; //temperature formula

  tilt = (double)acos(Az / (sqrt(Ax * Ax + Ay * Ay + Az * Az))) * 180.0 / 3.14159265;
  
  yaw = (double)atan(Az / (sqrt(Ay * Ay + Ax * Ax))) * 180.0 / 3.14159265;
  pitch = (double)atan(Ax / (sqrt(Ay * Ay + Az * Az))) * 180.0 / 3.14159265;
  roll = (double)atan(Ay / (sqrt(Ax * Ax + Az * Az))) * 180.0 / 3.14159265;

  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    String serverPath = serverName + "?yaw=" + yaw + "&pitch=" + pitch + "&roll=" + roll + "&tilt=" + tilt + "&temp=" + T;
    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();
  }

  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 1 << MPU6050_REGISTER_SLEEP);
  ESP.deepSleep(30e6);

}


void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  //GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  //GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  //GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x00);
  //I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}


void loop() {
  // put your main code here, to run repeatedly:

}
