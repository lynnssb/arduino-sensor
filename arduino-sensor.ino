/*
 Name:    arduino_sensor.ino
 Created: 5/18/2018 3:46:35 PM
 Author:  lynn
*/
#include <OneWire.h>
#include <dht11.h>
 
/*************************************************
 *  DS18B20 防水温度传感器 连接（Digital）10号引脚
*************************************************/
//
OneWire ds(10);
 
/*************************************************
 *        DHT11 空气温度湿度传感器
*************************************************/
dht11 DHT11;
#define DHT11PIN 2 //2号引脚（Digital）
struct Dht11Data {
  //空气温度
  float temperature;
  //空气湿度
  float humidity;
};
 
/*************************************************
*          触摸传感器
************************************************/
//A5
 
/*************************************************
*          人体红外传感器
************************************************/
#define HWPIN 4
 
 
/*************************************************
 *           距离传感器
 ************************************************/
//#define PIN_TRIG 6
//#define PIN_ECHO 7
 
 
//获取光照传感器数据(单位：勒克斯：Lux/Lx)
float getLightSensor();
//获取空气温湿度传感器数据(单位： 温度：℃ 摄氏度 / 湿度: %)
Dht11Data getDht11Sensor();
//获取土壤湿度传感器数据(单位: %)
float getMoistureSensor();
//获取DS18B20温度防水传感器数据（单位： ℃ 摄氏度）
float getDS18B20Sensor();
//获取触摸传感器数据
int getTouchData();
//获取人体红外传感器数据
int getHwSensor();
 
// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(A5, INPUT);
  pinMode(HWPIN, INPUT);
  Serial.begin(9600);
}
 
// the loop function runs over and over again until power down or reset
void loop() {
//  int _touch = getTouchData();
//  if (_touch < 100) {
//    Display();
//  }
  //Serial.println(_touch);
 
  Display();
  delay(1000);
 
//  int _hw = getHwSensor();
//  Serial.println(_hw);
//  delay(100);
}
 
void Display() {
  float _ds18b20Val = getDS18B20Sensor();
  Serial.print("(DS18B20防水)温度：");
  Serial.print(_ds18b20Val);
  Serial.println(" ℃");
 
  Dht11Data _dht11Data = getDht11Sensor();
  Serial.print("(DHT11)空气温度：");
  Serial.print(_dht11Data.temperature, 2);
  Serial.print(" ℃   ");
  Serial.print("(DHT11)空气湿度：");
  Serial.print(_dht11Data.humidity, 2);
  Serial.println(" %");
 
  float _lightVal = getLightSensor();
  Serial.print("光照度：");
  Serial.print(_lightVal, 2);
  Serial.println(" Lux(勒克斯)");
 
  float _moistureVal = getMoistureSensor();
  Serial.print("土壤湿度：");
  Serial.print(_moistureVal);
  Serial.println(" %");
  Serial.println("==========================================");
 
  //delay(3000);
}
 
 
float getLightSensor() 
{
  return analogRead(A2);
}
 
Dht11Data getDht11Sensor()
{
  Dht11Data _dht;
  int chk = DHT11.read(DHT11PIN);
  switch (chk) {
  case DHTLIB_OK:
    break;
  case DHTLIB_ERROR_CHECKSUM:
    //Serial.println("DHT11 CHECKSUM Error!");
    break;
  case DHTLIB_ERROR_TIMEOUT:
    Serial.println("DHT11 TimeOut Error!");
    break;
  default:
    Serial.println("DHT11 Unknown Error!");
  }
  _dht.temperature = (float)DHT11.temperature;
  _dht.humidity = (float)DHT11.humidity;
  return _dht;
}
 
float getMoistureSensor()
{
  return analogRead(A0);
}
 
float getDS18B20Sensor() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
 
  if (!ds.search(addr)) {
    ds.reset_search();
    delay(250);
    return;
  }
 
  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return;
  }
 
  switch (addr[0]) {
  case 0x10:
    type_s = 1;
    break;
  case 0x28:
    type_s = 0;
    break;
  case 0x22:
    type_s = 0;
    break;
  default:
    return;
  }
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  delay(1000);
 
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);
 
  for (i = 0; i < 9; i++) 
  {
    data[i] = ds.read();
  }
 
  unsigned int raw = (data[1] << 8) | data[0];
  if (type_s) 
  {
    raw = raw << 3;
    if (data[7] == 0x10) 
    {
      raw = (raw & 0xFF0) + 12 - data[6];
    }
  }
  else 
  {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00)
    {
      raw = raw << 3;
    }
    else if (cfg == 0x20)
    {
      raw = raw << 2;
    }
    else if(cfg == 0x40)
    {
      raw = raw << 1;
    }
  }
  celsius = (float)raw / 16.0;
  //fahrenheit = celsius * 1.8 + 32.0;
  return celsius;
}
 
int getTouchData()
{
  return analogRead(A5);
}
 
int getHwSensor()
{
  return digitalRead(HWPIN);
}
