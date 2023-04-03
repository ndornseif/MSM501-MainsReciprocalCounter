//Make sure StartPin has a higher pin number that EndPin.
#define CBUSStartPin 9 //First bit of the counter outputs. Should be connected to Y0.
//The next lower seven pins are the remainig CBUS pins. E.g. if CBUSStartPin is 9, connect Y0 to pin 9 and Y7 to pin 2

#define CounterClear A0 //Pin connected to CCLR# on both counters.
#define CounterRegClock A2 //Pin connected to RCLK on both counters.
#define GateTime A1 //Pin used to open the main gate and pass the signal and reference to the counters

//The SBUS is used to select which counter, as well as what byte of said counter will be read.
//SBUS2 selects the counter to read. 0=SignalCounter; 1=ReferenceCounter
//SBUS0 and SBUS1 are used to select one of the four bytes stored in each counter.
//0=CounterA lower byte, 1=CounterA upper byte, 2=CounterB lower byte, 3=CounterB upper byte.
//Pulling SBUSE low puts the outputs of both counters in HiZ mode. Leaves the CBUS floating. Pull high to read.
#define SBUS0 10
#define SBUS1 11
#define SBUS2 12
#define SBUSE 13

#define ReadGateStatus A3 //The pin connected to the gateEnable signal out of the flipflop. Used to sense if main gate is open.

#define StatusLED A4 //Red LED to indicate whatever. Connected to the positive terminal of the status LED.

#define ConditionedSignal A5 //The pin connected to the conditioned input signal. Used for general debugging.

//The frequency of the reference clock in uHz.
const float refClockFrequency = 1000000000000;

//The time a single measurment is perfomed by default in ms.
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

byte readCBUS(){
  byte returnvalue = 0;
  for(byte i = 0; i < 8; i++){
    bitWrite(returnvalue,i,digitalRead(CBUSStartPin-i));
  }
  return returnvalue;
}

//Read counter in. counterIndex=0 -> Signal Counter; counterIndex=1 -> ReferenceCounter
unsigned long readCounter(bool counterIndex){
  unsigned long returnvalue = 0;
  byte SBUSState = 0b1011; //Set bit for SBUS enable high and byte counter (two LSBs) to three.
  bitWrite(SBUSState, 2, counterIndex);//Select the counter ro read from.
  for(byte i = 0; i < 4;i++){ //Extract all four bytes of the counter one by one
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
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GateTime,HIGH);
  delay(gateTime);
  digitalWrite(GateTime,LOW);
  delay(100);
  latchCounters();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  float signalCounterValue = readCounter(0);
  float referenceCounterValue = readCounter(1);
  //All Frequency values are given in uHz.
  unsigned long measuredFrequency = round((signalCounterValue * refClockFrequency) / referenceCounterValue);
  return measuredFrequency;
}
void setup() {
  Serial.begin(115200);
  //CBUS pins are numbered in decending order.
  for(byte i = 0; i < 8; i++){
    pinMode(CBUSStartPin-i, INPUT);
  }
  pinMode(ReadGateStatus,INPUT);
  pinMode(StatusLED,OUTPUT);
  pinMode(ConditionedSignal,INPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);
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
  analogWrite(StatusLED,0); //Status LED off.
  digitalWrite(LED_BUILTIN,LOW); //Arduino genral purpose LED off.

  resetCounters();
}

void loop() {
  delay(100);
  Serial.println(performMeasurement(defaultGateTime));
}
