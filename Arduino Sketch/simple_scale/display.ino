
/*
 File: display.ino
 Part of the 'SimpleScale' 3D printable scale

 The MIT License (MIT)

 Copyright (c) 2017 Damian Schneider

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

void displayweight(float weight_in_g) {


  char numberbuffer[6];
  display.setFont(&FreeSansBold18pt7b);


  if (weight_in_g > 1500 || weight_in_g < -1000)
  {
    display.setCursor(10, 30);
    display.setFont(&FreeSansBold18pt7b);
    display.print("OVER-");
    display.setCursor(10, 55);
    display.print("LOAD!");
  }
  else if (weight_in_g >= 1000)
  {
    display.setFont(&FreeSansBold12pt7b);
    dtostrf(weight_in_g, 6, 2, numberbuffer);
    display.setCursor(15, 40);
    display.print(numberbuffer);
    display.print('g');
  }
  else if (weight_in_g <= -100)
  {
    //adds a leading minus, needs more space so make the font smaller on bigger numbers
    display.setFont(&FreeSansBold12pt7b);
    dtostrf(weight_in_g, 6, 2, numberbuffer);
    display.setCursor(15, 40);
    display.print(numberbuffer);
    display.print('g');
  }
  
  else
  {
    dtostrf(weight_in_g, 6, 2, numberbuffer);
    display.setCursor(2, 45);
    display.print(numberbuffer);
    display.print('g');
  }

}

