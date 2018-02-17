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

  //print(byte(floor(red(c))) & 0xFF);
  //print(", ");
  //print(byte(floor(green(c))) & 0xFF);
  //print(", ");
  //print(byte(floor(blue(c))) & 0xFF);
  //println();
}

void mouseMoved(){
  sendPositionData();
}

void sendColorData(){
  color c = color(float(mouseX) / width * 255, float(mouseY) / height * 255, 255);
  
  //constrain color size to a byte so we know that's
  //all we're sending
  byte[] colors = new byte[3];
  colors[0] = byte(floor(red(c)));
  colors[1] = byte(floor(green(c)));
  colors[2] = byte(floor(blue(c)));
  
  //write byte array over serial
  myPort.write(colors);
}

void sendPositionData(){
  int positionInt = floor(float(mouseX)/width * 90);
  println(byte(positionInt));
  
  myPort.write(byte(positionInt));
}