#include <SD.h>

File myFile;
const int chipSelect = BUILTIN_SDCARD;
volatile int state = 0;

#define VSYNC 0
#define HSYNC 1
#define DCLK 2//Pixel Clock
#define GCK 3//Essentially HSYNC
//SP__ are speaker pins
#define GSP 5//Essentially VSYNC

#define R0 23
#define R1 22
#define R2 21
#define R3 20
#define R4 19
#define R5 18

#define IMXRT_GPIO6_DIRECT  (*(volatile uint32_t *)0x42000000)

inline uint32_t fixbits()
{
  register uint32_t data  = IMXRT_GPIO6_DIRECT;  // 0B11111111111111110011000000000000
  register uint32_t data2 = data >> 12;
  asm volatile("bfi %0, %1, 14, 2" : "+r"(data) : "r"(data2));
  return (data >> 14);
}
//GSP should be the same as SPS



volatile int numgck = 0;
volatile int numclk = 0;
volatile byte filestream[100000];
volatile int currentindex = 0;

//use FASTRUN to make something go quickly
//Interrupt for GSP which enables GCK interrupt
//GCK interrupt enables DCLK Reading
//End of GCK interrupt checks if there were readings - if none then disables GCK interrupt

//GSP is normally HIGH, but when it drops to low is when it should interrupt
//GCK is normally LOW - changing to high should interrupt
//DCLK is normally LOW - changing to high is when pixels should be read?

//Looks like there are 195 GCK Cycles per frame (with a few useless ones)
//300 DCK cycles per frame

FASTRUN void gspinterrupt(){
  //enables GCK interrupt
  //Erases the frame array - ready to read a new frame  
  //Serial.println(numgck);
  //numgck=0;
  if (state==0){
    Serial.println("Started2");
    state=1;
    addfilestream(4);
  }else{
    Serial.println("Writing files");
    detachInterrupt(GSP);
    detachInterrupt(GCK);
    detachInterrupt(DCLK);
    Serial.println("Writing files");
    for(int i=0;i<=currentindex;i++){
      myFile.print(filestream[i]);
    }
    myFile.close();
    Serial.println("File Closed");
    while(1){
      
    }
  }
  
}
void gckrisinginterrupt(){
  //checks for DCLK readings and writes pixels and disables itself if needed
  //numgck++;
  //Serial.println(numclk);
  //numclk = 0;
  addfilestream(3);
  
  
}
FASTRUN void dclkinterrupt(){
  //numclk++;
  uint32_t rawValue = fixbits();
  byte red = ((rawValue & 0x000F0000) >> 14) + ((rawValue & 0x00003000) >> 12);
  if(red!=0){
  Serial.println(red);}
  addfilestream(red);

  //It was able to keep up with the screen before I added these digitalReads
  //digitalRead(R4);
  //digitalRead(R3);
  //digitalRead(R2);
  //digitalRead(R1);
  //digitalRead(R0);
  
}

FASTRUN void addfilestream(int toAdd){
  filestream[currentindex]=toAdd;
  currentindex++;
}

//Have just under 4 ms between frames
void setup() {
  // put your setup code here, to run once:
  pinMode(DCLK, INPUT);
  pinMode(GCK, INPUT);
  pinMode(GSP, INPUT);
  for(int i=0;i>100000;i++){
    filestream[i] = 6;
  }
  Serial.begin(115200);
  while (!Serial) {
  }
  SD.begin(chipSelect);
  myFile = SD.open("tost.txt", FILE_WRITE);
  attachInterrupt(GSP, gspinterrupt, FALLING);
  attachInterrupt(GCK, gckrisinginterrupt, RISING);
  attachInterrupt(DCLK, dclkinterrupt, CHANGE);
  
}

void loop(){

}
