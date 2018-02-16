import processing.serial.*;

Serial myPort;
int h;
int bright;

void setup(){
  size(600, 600);
  colorMode(HSB, 255, 255, 255);
  
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
}

void draw(){
  color c = color(float(mouseX) / width * 255, float(mouseY) / height * 255, 255);
  background(float(mouseX) / width * 255, float(mouseY) / height * 255, 255);
  //print(unhex(hex(c, 6)));
  //print(", ");
  //print((floor(red(c)) << 16) + (floor(green(c)) << 8) + floor(blue(c)));
  //print(", ");
  //print((unhex(hex(c, 6)) >> 0) & 0xFF);
  //println();
  print(byte(floor(red(c))) & 0xFF);
  print(", ");
  print(byte(floor(green(c))) & 0xFF);
  print(", ");
  print(byte(floor(blue(c))) & 0xFF);
  println();
}

void mouseClicked(){
  color c = color(float(mouseX) / width * 255, float(mouseY) / height * 255, 255);
  int please = (floor(red(c)) << 16) + (floor(green(c)) << 8) + floor(blue(c));
  
  //constrain color size to a byte so we know that's
  //all we're sending
  
  byte[] colors = new byte[3];
  
  //byte r = byte(floor(red(c)));
  //byte g = byte(floor(green(c)));
  //byte b = byte(floor(blue(c)));
  
  colors[0] = byte(floor(red(c)));
  colors[1] = byte(floor(green(c)));
  colors[2] = byte(floor(blue(c)));
  
  //write bytes over serial, might need to use
  //char delimiters so we know the vals are in order
  //myPort.write(r);
  //myPort.write(g);
  //myPort.write(b);
  myPort.write(colors);
  
  //myPort.write(hex(c, 6) + '\0');
}