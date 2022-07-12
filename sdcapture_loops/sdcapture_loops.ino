#include <SD.h>

File myFile;
const int chipSelect = BUILTIN_SDCARD;
volatile int state = 0;


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
//GSP should be the same as SPS

#define totalgck 195
#define totaldclk 298



int numgck = 0;
int numdclk = 0;
//volatile int numclk = 0;
byte filestream[100000];
int currentindex = 0;



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
  pinMode(R0, INPUT);
  pinMode(R1, INPUT);
  pinMode(R2, INPUT);
  pinMode(R3, INPUT);
  pinMode(R4, INPUT);
  pinMode(R5, INPUT);
  for(int i=0;i>100000;i++){
    filestream[i] = 0;
  }
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.print("Started");
  SD.begin(chipSelect);
  myFile = SD.open("tost.txt", FILE_WRITE);
  Serial.print("Started");
  
  
}

int frame(){
  while(digitalRead(GCK)){
    //wait until next gck cycle
  }
  while(!digitalRead(GCK)){
    //wait the gap
  }
  Serial.println("Actually started");
  //First row has started now
  int index=0;
  numdclk=0;
  while(true){
    while(digitalRead(GCK)&&(index<1000)){
      //Serial.println("GCK time");
      while(digitalRead(DCLK)){
        //one pixel data
      }
      numdclk++;
      while(!digitalRead(DCLK)){
        //another pixel data
      }
      numdclk++;
      index++;
    }
    Serial.println("End GCK");
    //GCK cycle has ended
    numgck++;
    Serial.print(numdclk);
    if(numgck>=totalgck){
      return 1;//exit the function
    }
    while(digitalRead(GCK)){}
    while(!digitalRead(GCK)){}
  }
  
}

void loop(){
  Serial.println("Started");
  while(digitalRead(GSP)){
    //wait
  }
  Serial.println("Started");
  frame();
  Serial.println("Writing files");
  for(int i=0;i<=currentindex;i++){
    myFile.print(filestream[i]);
  }
  myFile.close();
  Serial.println("File Closed");
  while(1){
    
  }
}
