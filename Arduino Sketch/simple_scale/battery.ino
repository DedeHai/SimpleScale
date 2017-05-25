/*
 File: battery.ino
 Part of the 'SimpleScale' 3D printable scale

 The MIT License (MIT)

 Copyright (c) 2017 Damian Schneider

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


uint16_t readBatteryVoltage(void) //returns batteryvoltage in volts*100
{
  long batteryvoltage;
  analogReference(INTERNAL);
  batteryvoltage = ((long)analogRead(6) * 1200) / 1024; //1.1V reference voltage, 100k/10k voltage divider

  //Serial.print("Battery voltage = ");
  //Serial.println((float)batteryvoltage / 100.0);

  analogReference(DEFAULT);
  return (uint16_t)batteryvoltage;
}

void displayBattery(void)
{

  //draw a battery symbol with 10 lines inside. full is 4.1V, empty is 3.75V (circuit stops working at about 3.8V input due to voltage drop accross LDO regulator) only 50% of battery capacity is acutally used at 3.8V
  static float voltage = 380;

  voltage = (float)voltage*0.8 + (float)readBatteryVoltage()*0.2; //apply a simple averaging filter on the input voltage

  uint16_t numberoflines = map(voltage, 375, 405, 0, 10);
  if(numberoflines > 10) numberoflines = 10;

  // Serial.println(voltage);
   //   Serial.println(numberoflines);
  //display battery symbol
  display.drawRect(112, 0, 12, 7, 1); //battery rectangle, 22x10 pixels (20x8 inside)
  display.drawRect(124, 2, 2, 3, 1); //battery head
  for (int i = 0; i < numberoflines; i++)
  {
    display.drawFastVLine(113+i, 1, 5, 1);
  }
}

