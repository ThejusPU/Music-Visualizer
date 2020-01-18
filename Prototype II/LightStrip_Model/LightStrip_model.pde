/*  FHT_128_channel_analyser.pde

    an open-source display for spectrum analyser
    Copyright (C) 2013  Jürgen Rimmelspacher

    For use with "processing": http://processing.org/

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

final int X_OFFSET  =  40;                     // x-distance to left upper corner of window
final int Y_OFFSET  =  60;                     // y-distance to left upper corner of window
final int BOT_DIST  =  80;                     // distance to bottom line of window
final int COL_WIDTH =   4;                     // column widt
final int Y_DIST    =  64;                     // distance horizontal lines
final int X_DIST    =   5;                     // distance vertical lines
final int X_MAX     = (128+1)*X_DIST+1;        // x-axis lenght
final int Y_MAX     = 256;                     // y-axis lenght
final int X_WINDOW  = X_MAX + 2*X_OFFSET;      // window width
final int Y_WINDOW  = Y_MAX+BOT_DIST+Y_OFFSET; // window height
final int X_ENUM    = 10;
PFont fontA;
color graphColor = color(25, 25, 250);
PFont fontGraph;
import processing.serial.*;
Serial port;
int[] inBuffer = new int[128];

final int FHT_N_2 = 128;
final int numLights = 150;
LightStrip ls;

void draw_grid()                               // draw grid an title
{ 
  int count=0;

  background(200);
  stroke(0);
  for (int x=0+X_DIST; x<=X_MAX; x+=X_DIST)    // vertical lines
  {
    if ( X_ENUM == count || 0 == count)
    { 
      line (x+X_OFFSET, Y_OFFSET, x+X_OFFSET, Y_MAX+Y_OFFSET+10);
      count=0;
    }
    else
    {
      line (x+X_OFFSET, Y_OFFSET, x+X_OFFSET, Y_MAX+Y_OFFSET);
    }    
    count++;
  }
  for (int y=0; y<=Y_MAX; y+=Y_DIST)           // horizontal lines 
  {
    line (X_OFFSET, y+Y_OFFSET, X_MAX+X_OFFSET, y+Y_OFFSET);
    textFont(fontA, 16);
    text( (Y_MAX-y), 7, y+Y_OFFSET+6);
  }
  textFont(fontA, 32);
  fill(graphColor); 
  text("128-Channel Spectrum Analyser", 215, 40);
  textFont(fontA, 16);
  text("magnitude", 7, 20);  
  text("(8bit-value)", 7, 40);  
  text("--> channel (number i)", X_OFFSET, Y_WINDOW-Y_OFFSET/2);
  text(" frequency   f ( i ) = i * SAMPLE_RATE / FHT_N ", 350, Y_WINDOW-Y_OFFSET/2 );
} 

void serialEvent(Serial p)                      // ISR triggerd by "port.buffer(129);"
{ 
  if ( 255 == port.read() )                     //  1 start-byte               
  {
    inBuffer = int( port.readBytes() );         // 128 data-byte
  }
}

void settings() {
  size(X_WINDOW, Y_WINDOW);                      // size of window
}

void setup() 
{ 
  noStroke();
  fontGraph = loadFont("ArialUnicodeMS-48.vlw");
  textFont(fontGraph, 12);
  println(Serial.list());                        // show available COM-ports
  port = new Serial(this, "COM4", 115200);
  port.buffer(129);                              // 1 start-byte + 128 data-bytes 
  fontA = loadFont("ArialUnicodeMS-48.vlw");
  textFont(fontA, 16);
  
  ls = new LightStrip(numLights);
  PApplet.runSketch(new String[] {ls.getClass().getSimpleName()}, ls);
}

void draw() 
{ 
  int count=0;
  
  draw_grid();

  for (int i=0; i<FHT_N_2; i++)
  { 
    fill(graphColor);
    rect(i*X_DIST+X_OFFSET+X_DIST-COL_WIDTH/2, height-BOT_DIST, COL_WIDTH, -inBuffer[i]);
    if ( X_ENUM == count || 0 == count)
    { 
      text(i, (i+1)*X_DIST+X_OFFSET-COL_WIDTH/2, height-BOT_DIST+25);
      count=0;
    }
    count++;
  }
  
}

class LightStrip extends PApplet {
  int numL;
  final int WINDOW_HEIGHT = 100;
  final int LED_WIDTH = 10;
  int WINDOW_LENGTH;
  float[] LED;
  color[] cLED;
  LightStrip(int numLs) {
    super();
    
    numL = numLs;
    LED = new float[numL];
    cLED = new color[numL];
    int temp;
    for (int x = 0; x < numL; x++) {
      if(x < numL/3) {
        temp = 255 * x * 3 / numL;
        cLED[x] = color(temp,0,255-temp);
      }
      else if(x < 2 * numL / 3) {
        temp = 255 * (x - (numL / 3)) * 3 / numL;
        cLED[x] = color(255-temp,temp,0);
      }
      else {
        temp = 255 * (x - (2 * numL / 3)) * 3 / numL;
        cLED[x] = color(0,255-temp,temp);
      }
    }
    WINDOW_LENGTH = LED_WIDTH * numL;
  }
  
  void settings() {
    size(WINDOW_LENGTH, WINDOW_HEIGHT);
  }
  
  void setup() {
    noStroke();      
    background(255); 
  }
  
  void draw() {
    
    frequency2strip();
    
  }
  
  void frequency2strip() {
    //updateLED_Octaves(inBuffer);
    updateLED_inRange(inBuffer, 4, 90);
    float max = findMax(128);
    
    float val;
    for (int x = 0; x < numL; x++) {
      val = LED[x] / max;
      if (val > 0.35) {
        fill(red(cLED[x]) * val, green(cLED[x]) * val, blue(cLED[x]) * val);
        rect(x * LED_WIDTH, 0, (x + 1) * LED_WIDTH, WINDOW_HEIGHT);
      }
      else {
        fill(0);
        rect(x * LED_WIDTH, 0, (x + 1) * LED_WIDTH, WINDOW_HEIGHT);
      }
    }
  }
  
  void updateLED_Octaves(int[] l) {
    int currLED = 0;
    int runSum = 0;
    int count = 0;
    for (int x = 2; x < l.length; x++) {
      int intLED = 36 * (int)log(x) - 25;
      
      if (intLED < 0) {
        intLED = 0;
      }
      runSum += l[x] * l[x];
      count++;
      if (intLED >= currLED + 1) {
        for (int y = currLED; y < intLED; y++) {
          LED[y] = sqrt(runSum / count);
        }
        currLED = intLED;
        runSum = 0;
        count = 0;
      }
    }
    
    for (int y = currLED; y < LED.length; y++) {
      LED[y] = sqrt(runSum / count);
    }
  }
  
  /* scaled linearly across frequency indeces low - high
   * low > 0 & high < 127
   */
  void updateLED_inRange(int[] l, float low, float high) {
    float m = numL / (high - low);
    float b = m * low;
    int currFreq = (int) low;
    float prevPerc;
    float currPerc;
    for (int x = 0; x < numL; x++) {
      prevPerc = currFreq - ((x + b) / m);
      currPerc = ((x + b + 1) / m) - currFreq;
      LED[x] = l[currFreq - 1] * prevPerc + l[currFreq] * currPerc;
      if ((int)((x + b + 2) / m) != currFreq) {
        currFreq++;
      }
    }
  }
  
  float findMax(int maxPoss) {
    float max = maxPoss;
    for (int x = 3; x < LED.length; x++) {
      if (LED[x] > max) {
        max = LED[x];
      }
    }
    return max;
  }
}
