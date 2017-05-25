/*
 File: simple_scale.ino
 Part of the 'SimpleScale' 3D printable scale

 The MIT License (MIT)

 Copyright (c) 2017 Damian Schneider

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <Q2HX711.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>  //linienabstand: 16
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>


#define LM_SIZE 50 //moving average size
Adafruit_SSD1306 display(0); //do not use reset pin

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const byte hx711_data_pin = A1;
const byte hx711_clock_pin = A2;

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);
const double scaling = 0.0004344; //calibration factor. To adjust it:   scaling_new = current_scaling * actual_weight/displayed_weight
long runningAverage(long M) {

  static long LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return sum / count;
}




long initialmeasurement;
void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT),
  digitalWrite(13, LOW); //GND for display
 digitalWrite(2, HIGH); //pullup on tare pin

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Wire.setClock(400000);  // use fast speed
  display.clearDisplay();
  display.display();  //
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  display.setCursor(29, 15);
  display.setFont(&FreeSansBold12pt7b);
  display.println("simple");
  display.setCursor(36, 54);
  display.print("scale");
  display.display();

  //fill the moving average filter with 10 measurements
  for (int i = 0; i < 10; i++)
  {
    //take 10 measurements
    initialmeasurement = hx711.read();
    for (int j = 0; j < (LM_SIZE / 10); j++)
    {
      runningAverage(initialmeasurement);
    }
  }
}
long reading, lastreading;

void loop() {
  //lowpass = (double)0.005 * hx711.read() + (double)0.995 * lowpass;
  //Serial.println((long)(lowpass));
  lastreading = reading;
  reading = hx711.read();

  if (lastreading - reading > 700 || lastreading - reading < -700 ) //change is bigger than 0.3g on 1kg sensor
  {
    //fill the averaging buffer with the current value
    for (int i = 0; i < LM_SIZE; i++)
    {
      runningAverage(reading);
    }
  }
  float outputweight = (runningAverage(reading) - initialmeasurement) * scaling;
  Serial.println(outputweight, 2 );
  //Serial.print("\t");
  //Serial.println((reading - initialmeasurement)*scaling, 1);


  display.clearDisplay();
  displayweight(outputweight);
  displayBattery();
  
  display.display();

if(digitalRead(2) == 0) //tare
{
  initialmeasurement = runningAverage(hx711.read());
}
  readBatteryVoltage();
}
