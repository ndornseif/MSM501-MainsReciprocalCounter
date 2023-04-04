//MSM501-MainsReciprocalCounter Firmware version 1.6

//Pin definitions
//Counter output connections starting at Y0.
const byte CBUS[8] = {9, 8, 7, 6, 5, 4, 3, 2};

const byte CounterClear = 14; //Pin connected to CCLR# on both counters.
const byte CounterRegClock = 16; //Pin connected to RCLK on both counters.
const byte GateTime = 15; //Pin used to open the main gate and pass the signal and reference to the counters

//The SBUS is used to select which counter, as well as what byte of said counter will be read.
//SBUS2 selects the counter to read. 0=SignalCounter; 1=ReferenceCounter
//SBUS0 and SBUS1 are used to select one of the four bytes stored in each counter.
//0=CounterA lower byte, 1=CounterA upper byte, 2=CounterB lower byte, 3=CounterB upper byte.
//Pulling SBUSE low puts the outputs of both counters in HiZ mode. Leaves the CBUS floating. Pull high to read.
const byte SBUS0 = 10;
const byte SBUS1 = 11;
const byte SBUS2 = 12;
const byte SBUSE = 13;

//Pin connected to the gateEnable signal out of the flipflop. Used to sense if main gate is open.
const byte ReadGateStatus = 17; 

//LED to indicate whatever. Connected to the positive terminal of the status LED.
const byte StatusLED = 18;

//Pin connected to the conditioned input signal. Currently unused.
const byte ConditionedSignal = 19;

//The frequency of the reference clock in uHz.
const float refClockFrequency = 1e6 * 1e6; //1MHz multiplied by 1e6 to convert from Hz to uHz.

//Default time for a single measurment in ms.
const unsigned long defaultGateTime = 60000;

//Resets Reference and Signal Counters.
void resetCounters(){
  digitalWrite(CounterClear,LOW);
  digitalWrite(CounterClear,HIGH);
}

//Latches the Reference and Signal Counter states in their internal registers.
void latchCounters(){
  digitalWrite(CounterRegClock,HIGH);
  digitalWrite(CounterRegClock,LOW);
}

//Read in the value represented on the CBUS.
byte readCBUS(){
  byte returnvalue = 0;
  for(byte i = 0; i < 8; i++){
    bitWrite(returnvalue,i,digitalRead(CBUS[i]));
  }
  return returnvalue;
}

//Read counter in. counterIndex=0 -> Signal Counter; counterIndex=1 -> ReferenceCounter
unsigned long readCounter(bool counterIndex){
  unsigned long returnvalue = 0;
  //Set bit for SBUS enable high and byte counter to three (selecting upper byte of counter B).
  byte SBUSState = 0b1011;
  //Select the counter ro read from.
  bitWrite(SBUSState, 2, counterIndex);
  //Extract all four bytes of the counter one by one
  for(byte i = 0; i < 4;i++){
    setSBUS(SBUSState-i);
    returnvalue = returnvalue << 8;
    returnvalue += readCBUS();
  }
  return returnvalue;
}

//Sets the SBUS up for a read using the four least significant bits of index for SBUS0,SBUS1,SBUS2,SBUSE.
void setSBUS(byte index){
  digitalWrite(SBUS0, bitRead(index, 0));
  digitalWrite(SBUS1, bitRead(index, 1));
  digitalWrite(SBUS2, bitRead(index, 2));
  digitalWrite(SBUSE, bitRead(index, 3));
}

//Measures the Frequency coming in averaged over gateTime (in ms) returns value in uHz.
unsigned long performMeasurement(unsigned long gateTime){
  resetCounters();
  digitalWrite(GateTime,HIGH);
  delay(gateTime);
  digitalWrite(GateTime,LOW);
  delay(500);
  latchCounters();
  float signalCounterValue = readCounter(0);
  float referenceCounterValue = readCounter(1);
  //All Frequency values are given in uHz.
  unsigned long measuredFrequency = round((signalCounterValue * refClockFrequency) / referenceCounterValue);
  return measuredFrequency;
}
void setup() {
  Serial.begin(115200);
  
  for(byte i = 0; i < 8; i++){
    pinMode(CBUS[i], INPUT);
  }

  pinMode(ReadGateStatus,INPUT);
  pinMode(ConditionedSignal,INPUT);

  pinMode(StatusLED,OUTPUT);
  pinMode(CounterClear,OUTPUT);
  pinMode(CounterRegClock,OUTPUT);
  pinMode(GateTime,OUTPUT);
  
  pinMode(SBUS0,OUTPUT);
  pinMode(SBUS1,OUTPUT);
  pinMode(SBUS2,OUTPUT);
  pinMode(SBUSE,OUTPUT);

  digitalWrite(SBUSE,HIGH); //Disable CBUS.
  digitalWrite(CounterClear,HIGH); //Dont clear counter.
  digitalWrite(CounterRegClock,LOW); //Dont latch the counter state.
  digitalWrite(GateTime,LOW); //Leave main gate closed.
  digitalWrite(StatusLED,LOW); //Status LED off.

  resetCounters();
}

void loop() {
  delay(100);
  Serial.println(performMeasurement(defaultGateTime));
}
