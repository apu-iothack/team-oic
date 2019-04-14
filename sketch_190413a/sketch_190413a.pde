import controlP5.*;   //import ControlP5 library
import processing.serial.*;

Serial port;

ControlP5 cp5;  //create ControlP5 object
PFont font;

void setup(){  //same as arduino program
  size(300,450);  //window size, (width,height)
  
  printArray(Serial.list());  //print all available serial ports
  
  port = new Serial(this, "COM3", 9600);  //have connected arduino to com3, it would be different in linux and mac os
  
  //lets add button to empty window
  
  cp5 = new ControlP5(this);
  font = createFont("times new roman bold", 20);  //custom fonts for button and title
  
  cp5.addButton("on")  //"on" is the name of button
    .setPosition(100,50)  //x and y coordinates of upper left corner of button
    .setSize(100, 70)   //(width, height)
    .setFont(font)
   ;
   
   cp5.addButton("off")     //"on" is the name of button
    .setPosition(100,150)   //x and y coordinates of upper left corner of button
    .setSize(100, 70)   //(width, height)
    .setFont(font)
   ;
}

void draw(){  //same as loop in arduino
  background(200,0,200);   //background color of window (r,g,b) or (0 to 255)
  
  //lets give title to window
  textFont(font);
  text("Power", 100, 30);  //("text", x coordinate, y coordinate)
}

//add functions to our buttons
//so when we press any button, it sends particular char over serial port

void on(){
  port.write('o');
}

void off(){
  port.write('f');
}
