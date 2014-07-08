// system states 
#define DEMO        0  // cycle through the lights
#define STAGING     1  // light top bar, blink second bar, others off
                       // turn on red, green, yellows for breaking beam of finish, trap, start, prestage
#define LANE1STAGED 2  // lane 1 staged for 3 seconds, stop it's blinking
#define LANE2STAGED 3  // lane 2 staged for 3 seconds, stop it's blinking
#define BOTHSTAGED  4  // both racers staged for 3 seconds, no blinking
#define RUNNING     5  // watching for the first racer to cross the finish line
#define LANE1WON    6  // Lane 1 crossed finish line first
#define LANE2WON    7  // Lane 2 crossed finish line first
#define LANE1FAULT  8  // Lane 1 crossed the start line before green
#define LANE2FAULT  9  // Lane 2 crossed the start line before green
#define BOTHFAULT   10 // Both lanes crossed the start line before green
#define LANE1DISQ   11 // 5 minutes have elapsed and lane 1 is DQ'd
#define LANE2DISQ   12 // 5 minutes have elapsed and lane 2 is DQ'd
#define TIED        13 // Both racers crossed finish line at the same moment as far as we can tell

#define lane1StagingEye    A11  // Arduino analog input pin A11 = Lane 1 staging photo-eye
#define lane1StartingEye   A10  // Arduino analog input pin A10 = Lane 1 starting line photo-eye
#define lane1SpeedTrapEye  A9   // Arduino analog input pin A9  = Lane 1 speed trap photo-eye
#define lane1FinishLineEye A8   // Arduino analog input pin A8  = Lane 1 finish line photo-eye

#define lane2StagingEye    A15  // Arduino analog input pin A15 = Lane 2 staging photo-eye
#define lane2StartingEye   A14  // Arduino analog input pin A14 = Lane 2 starting line photo-eye
#define lane2SpeedTrapEye  A13  // Arduino analog input pin A13 = Lane 2 speed trap photo-eye
#define lane2FinishLineEye A12  // Arduino analog input pin A12 = Lane 2 finish line photo-eye

const int lane1PreStageLight      = 41;  // Arduino digital output on pin 41 = Lane 1 pre-stage yellow light pair
const int lane1StageLight         = 43;  // Arduino digital output on pin 43 = Lane 1 stage yellow light pair
const int lane1Count3Light        = 45;  // Arduino digital output on pin 45 = Lane 1 count 3 yellow light
const int lane1Count2Light        = 47;  // Arduino digital output on pin 47 = Lane 1 count 2 yellow light
const int lane1Count1Light        = 49;  // Arduino digital output on pin 49 = Lane 1 count 1 yellow light
const int lane1StartGreenLight    = 51;  // Arduino digital output on pin 51 = Lane 1 start  green light
const int lane1FalseStartRedLight = 53;  // Arduino digital output on pin 53 = Lane 1 false-start red light

const int lane2PreStageLight      = 23;  // Arduino digital output on pin 23 = Lane 2 pre-stage yellow light pair
const int lane2StageLight         = 25;  // Arduino digital output on pin 25 = Lane 2 stage yellow light pair
const int lane2Count3Light        = 27;  // Arduino digital output on pin 27 = Lane 2 count 3 yellow light
const int lane2Count2Light        = 29;  // Arduino digital output on pin 29 = Lane 2 count 2 yellow light
const int lane2Count1Light        = 31;  // Arduino digital output on pin 31 = Lane 2 count 1 yellow light
const int lane2StartGreenLight    = 33;  // Arduino digital output on pin 33 = Lane 2 start  green light
const int lane2FalseStartRedLight = 35;  // Arduino digital output on pin 35 = Lane 2 false-start red light

const int startButton = 12;              // Arduino digital input on pin 12 = Race Controller's start countdown button

int state = DEMO;        // master variable for the state machine
boolean lane1StageState = LOW;
boolean lane2StageState = LOW;
boolean blinkState = LOW;

boolean lane1Count2State = LOW;
boolean lane2Count2State = LOW;
boolean lane1Count1State = LOW;
boolean lane2Count1State = LOW;
boolean lane1StartGreenState = LOW;
boolean lane2StartGreenState = LOW;
boolean lane1FalseStartRedState = LOW;
boolean lane2FalseStartRedState = LOW;
boolean raceStarted = LOW;

boolean count3State = LOW;
boolean count2State = LOW;
boolean count1State = LOW;

unsigned long lastTime = 0;
unsigned long currentMillis = 0;
unsigned long lastTimeLane1Unstaged = 0;
unsigned long lastTimeLane2Unstaged = 0;
unsigned long lastTimeStaged = 0;
unsigned long timeCountdownStarted = 0;

boolean valLane1Staged;
boolean valLane2Staged;
boolean valLane1Started;
boolean valLane2Started;
boolean valLane1Trapped;
boolean valLane2Trapped;
boolean valLane1Finished;
boolean valLane2Finished;

boolean valLane1Faulted;
boolean valLane2Faulted;
boolean valLane1Fault;
boolean valLane2Fault;

boolean valLane1Cleared = LOW;
boolean valLane2Cleared = LOW;

boolean valPressedStartButton;

long stageTime1 = 0;
long stageTime2 = 0;
long stopTime1 = 0;
long stopTime2 = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pins as outputs.
  pinMode(lane1PreStageLight,      OUTPUT);
  pinMode(lane1StageLight,         OUTPUT);
  pinMode(lane1Count3Light,        OUTPUT);
  pinMode(lane1Count2Light,        OUTPUT);
  pinMode(lane1Count1Light,        OUTPUT);
  pinMode(lane1StartGreenLight,    OUTPUT);
  pinMode(lane1FalseStartRedLight, OUTPUT);
  
  pinMode(lane2PreStageLight,      OUTPUT);
  pinMode(lane2StageLight,         OUTPUT);
  pinMode(lane2Count3Light,        OUTPUT);
  pinMode(lane2Count2Light,        OUTPUT);
  pinMode(lane2Count1Light,        OUTPUT);
  pinMode(lane2StartGreenLight,    OUTPUT);
  pinMode(lane2FalseStartRedLight, OUTPUT);
  
  pinMode(     lane1StagingEye,    INPUT);   // set pin to input
  digitalWrite(lane1StagingEye,    HIGH);    // turn on pullup resistors
  pinMode(     lane1StartingEye,   INPUT);   // set pin to input
  digitalWrite(lane1StartingEye,   HIGH);    // turn on pullup resistors
  pinMode(     lane1SpeedTrapEye,  INPUT);   // set pin to input lane1SpeedTrapEye
  digitalWrite(lane1SpeedTrapEye,  HIGH);    // turn on pullup resistors
  pinMode(     lane1FinishLineEye, INPUT);   // set pin to input
  digitalWrite(lane1FinishLineEye, HIGH);    // turn on pullup resistors
  
  pinMode(     lane2StagingEye,    INPUT);   // set pin to input
  digitalWrite(lane2StagingEye,    HIGH);    // turn on pullup resistors
  pinMode(     lane2StartingEye,   INPUT);   // set pin to input
  digitalWrite(lane2StartingEye,   HIGH);    // turn on pullup resistors
  pinMode(     lane2SpeedTrapEye,  INPUT);   // set pin to input
  digitalWrite(lane2SpeedTrapEye,  HIGH);    // turn on pullup resistors
  pinMode(     lane2FinishLineEye, INPUT);   // set pin to input
  digitalWrite(lane2FinishLineEye, HIGH);    // turn on pullup resistors
  
  
  
  LightsOut();
  
  Serial.begin(9600);
  // randomSeed(analogRead(0));  //TODO remove
}

void RunDemo() {
  //demo lights in lane 1, turn on in sequence
  digitalWrite(lane1PreStageLight,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1StageLight,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1Count3Light,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1Count2Light,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1Count1Light,HIGH);
  delay(500);
  digitalWrite(lane1StartGreenLight,HIGH);
  delay(500);
  digitalWrite(lane1FalseStartRedLight,HIGH);
  delay(500);

  //turn all off in lane 1
  digitalWrite(lane1PreStageLight,      LOW);
  digitalWrite(lane1StageLight,         LOW);
  digitalWrite(lane1Count3Light,        LOW);
  digitalWrite(lane1Count2Light,        LOW);
  digitalWrite(lane1Count1Light,        LOW);
  digitalWrite(lane1StartGreenLight,    LOW);
  digitalWrite(lane1FalseStartRedLight, LOW);
  delay(1000);
 
  //demo lights in lane 2, turn on in sequence
  digitalWrite(lane2PreStageLight,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane2StageLight,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane2Count3Light,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane2Count2Light,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane2Count1Light,HIGH);
  delay(500);
  digitalWrite(lane2StartGreenLight,HIGH);
  delay(500);
  digitalWrite(lane2FalseStartRedLight,HIGH);
  delay(500);

  //turn all off in lane 2
  digitalWrite(lane2PreStageLight,      LOW);
  digitalWrite(lane2StageLight,         LOW);
  digitalWrite(lane2Count3Light,        LOW);
  digitalWrite(lane2Count2Light,        LOW);
  digitalWrite(lane2Count1Light,        LOW);
  digitalWrite(lane2StartGreenLight,    LOW);
  digitalWrite(lane2FalseStartRedLight, LOW);
  delay(1000);

  //demo lights in both lanes together, turn on in sequence
  digitalWrite(lane1PreStageLight,HIGH);
  digitalWrite(lane2PreStageLight,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1StageLight,HIGH);
  digitalWrite(lane2StageLight,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1Count3Light,HIGH);
  digitalWrite(lane2Count3Light,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1Count2Light,HIGH);
  digitalWrite(lane2Count2Light,HIGH);
  delay(500);  // half second delay
  digitalWrite(lane1Count1Light,HIGH);
  digitalWrite(lane2Count1Light,HIGH);
  delay(500);
  digitalWrite(lane1StartGreenLight,HIGH);
  digitalWrite(lane2StartGreenLight,HIGH);
  delay(500);
  digitalWrite(lane1FalseStartRedLight,HIGH);
  digitalWrite(lane2FalseStartRedLight,HIGH);
  delay(1000);

  //turn all off in both lanes
  digitalWrite(lane1PreStageLight,      LOW);
  digitalWrite(lane1StageLight,         LOW);
  digitalWrite(lane1Count3Light,        LOW);
  digitalWrite(lane1Count2Light,        LOW);
  digitalWrite(lane1Count1Light,        LOW);
  digitalWrite(lane1StartGreenLight,    LOW);
  digitalWrite(lane1FalseStartRedLight, LOW);

  digitalWrite(lane2PreStageLight,      LOW);
  digitalWrite(lane2StageLight,         LOW);
  digitalWrite(lane2Count3Light,        LOW);
  digitalWrite(lane2Count2Light,        LOW);
  digitalWrite(lane2Count1Light,        LOW);
  digitalWrite(lane2StartGreenLight,    LOW);
  digitalWrite(lane2FalseStartRedLight, LOW);
  delay(1000);

  //demo lights turn all on at the same time
  digitalWrite(lane1PreStageLight,HIGH);
  digitalWrite(lane2PreStageLight,HIGH);
  digitalWrite(lane1StageLight,HIGH);
  digitalWrite(lane2StageLight,HIGH);
  digitalWrite(lane1Count3Light,HIGH);
  digitalWrite(lane2Count3Light,HIGH);
  digitalWrite(lane1Count2Light,HIGH);
  digitalWrite(lane2Count2Light,HIGH);
  digitalWrite(lane1Count1Light,HIGH);
  digitalWrite(lane2Count1Light,HIGH);
  digitalWrite(lane1StartGreenLight,HIGH);
  digitalWrite(lane2StartGreenLight,HIGH);
  digitalWrite(lane1FalseStartRedLight,HIGH);
  digitalWrite(lane2FalseStartRedLight,HIGH);
  delay(2000);

  //turn all off in both lanes
  LightsOut();
  delay(2000);
}

void LightsOut() {
  //reset lights
  digitalWrite(lane1PreStageLight,      LOW);
  digitalWrite(lane1StageLight,         LOW);
  digitalWrite(lane1Count3Light,        LOW);
  digitalWrite(lane1Count2Light,        LOW);
  digitalWrite(lane1Count1Light,        LOW);
  digitalWrite(lane1StartGreenLight,    LOW);
  digitalWrite(lane1FalseStartRedLight, LOW);
  
  digitalWrite(lane2PreStageLight,      LOW);
  digitalWrite(lane2StageLight,         LOW);
  digitalWrite(lane2Count3Light,        LOW);
  digitalWrite(lane2Count2Light,        LOW);
  digitalWrite(lane2Count1Light,        LOW);
  digitalWrite(lane2StartGreenLight,    LOW);
  digitalWrite(lane2FalseStartRedLight, LOW);
}

void Staging() {
  //digitalWrite(lane1PreStageLight,      HIGH);
  //digitalWrite(lane2PreStageLight,      HIGH);
  
  currentMillis = millis();
  if (currentMillis - lastTime >= 2000)
  {
    lastTime = currentMillis;
    lane1StageState = !lane1StageState;
    lane2StageState = !lane2StageState;
    if (LANE1STAGED == state)
      digitalWrite(lane1PreStageLight, HIGH);
    else
      digitalWrite(lane1PreStageLight, lane1StageState);
    if (LANE2STAGED == state)
      digitalWrite(lane2PreStageLight, HIGH);
    else
      digitalWrite(lane2PreStageLight, lane2StageState);
      
    Serial.print("state: ");
    Serial.println(state);
    Serial.print("lastTimeLane1Unstaged: ");
    Serial.println(lastTimeLane1Unstaged);
  }

  valLane1Staged = !digitalRead(lane1StagingEye);     // read the input pin
  valLane2Staged = !digitalRead(lane2StagingEye);     // read the input pin
  valLane1Started = !digitalRead(lane1StartingEye);   // read the input pin
  valLane2Started = !digitalRead(lane2StartingEye);   // read the input pin
  valLane1Trapped = !digitalRead(lane1SpeedTrapEye );    // read the input pin
  valLane2Trapped = !digitalRead(lane2SpeedTrapEye);    // read the input pin
  valLane1Finished = !digitalRead(lane1FinishLineEye );  // read the input pin
  valLane2Finished = !digitalRead(lane2FinishLineEye);  // read the input pin

  if (!valLane1Staged || valLane1Started) lastTimeLane1Unstaged = millis();
  if (!valLane2Staged || valLane2Started) lastTimeLane2Unstaged = millis();
  if ((!valLane1Staged || valLane1Started) && BOTHSTAGED == state) state = LANE2STAGED;
  if ((!valLane2Staged || valLane2Started) && BOTHSTAGED == state) state = LANE1STAGED;
  if ((!valLane1Staged || valLane1Started) && LANE1STAGED == state) state = STAGING;
  if ((!valLane2Staged || valLane2Started) && LANE2STAGED == state) state = STAGING;
  if (millis() > lastTimeLane1Unstaged + 3000) state = LANE1STAGED;
  if (millis() > lastTimeLane2Unstaged + 3000) state = LANE2STAGED;
  if ((millis() > lastTimeLane1Unstaged + 3000) && (millis() > lastTimeLane2Unstaged + 3000)) state = BOTHSTAGED;
  digitalWrite(lane1StageLight,         valLane1Staged);
  digitalWrite(lane2StageLight,         valLane2Staged);
  digitalWrite(lane1Count1Light,        valLane1Started);
  digitalWrite(lane2Count1Light,        valLane2Started);
  digitalWrite(lane1StartGreenLight,    valLane1Trapped);
  digitalWrite(lane2StartGreenLight,    valLane2Trapped);
  digitalWrite(lane1FalseStartRedLight, valLane1Finished);
  digitalWrite(lane2FalseStartRedLight, valLane2Finished);
//*/
}

void BothStaged() {
  currentMillis = millis();
  if (currentMillis - lastTime >= 1000)
  {
    lastTime = currentMillis;
      
    Serial.print("state: ");
    Serial.println(state);
    Serial.print("currentMillis: ");
    Serial.println(currentMillis);
    Serial.print("lastTimeLane1Unstaged: ");
    Serial.println(lastTimeLane1Unstaged);
  }

  digitalWrite(lane1PreStageLight,      HIGH);
  digitalWrite(lane2PreStageLight,      HIGH);
  digitalWrite(lane1StageLight,         HIGH);
  digitalWrite(lane2StageLight,         HIGH);
  digitalWrite(lane1Count2Light,        LOW);
  digitalWrite(lane2Count2Light,        LOW);
  digitalWrite(lane1Count1Light,        LOW);
  digitalWrite(lane2Count1Light,        LOW);
  digitalWrite(lane1StartGreenLight,    LOW);
  digitalWrite(lane2StartGreenLight,    LOW);
  digitalWrite(lane1FalseStartRedLight, LOW);
  digitalWrite(lane2FalseStartRedLight, LOW);

  valLane1Staged = !digitalRead(lane1StagingEye);     // read the input pin
  valLane2Staged = !digitalRead(lane2StagingEye);     // read the input pin
  valLane1Started = !digitalRead(lane1StartingEye);   // read the input pin
  valLane2Started = !digitalRead(lane2StartingEye);   // read the input pin
  valLane1Trapped = !digitalRead(lane1SpeedTrapEye );    // read the input pin
  valLane2Trapped = !digitalRead(lane2SpeedTrapEye);    // read the input pin
  valLane1Finished = !digitalRead(lane1FinishLineEye );  // read the input pin
  valLane2Finished = !digitalRead(lane2FinishLineEye);  // read the input pin

  if (!valLane1Staged && BOTHSTAGED == state) state = LANE2STAGED;
  if (!valLane2Staged && BOTHSTAGED == state) state = LANE1STAGED;
  if (!valLane1Staged && LANE1STAGED == state) state = STAGING;
  if (!valLane2Staged && LANE2STAGED == state) state = STAGING;
  
  currentMillis = millis();
  valPressedStartButton = !digitalRead(startButton);  // read the input pin
  // TODO wait for race controller to press the READY button
  // for now, just count off 13 seconds with the racers staged behind the start line, 10 sec after the lights stopped blinking
  if ((currentMillis > lastTimeLane1Unstaged + 13000) && (currentMillis > lastTimeLane2Unstaged + 13000))
  
  /////////////////////////// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  /////////////////////////// 13000 = 13 seconds
  
  //if (valPressedStartButton)
  {
    state = RUNNING;
    timeCountdownStarted = currentMillis;
    valLane1Faulted = LOW;
    valLane2Faulted = LOW;
    raceStarted = LOW;
    count3State = LOW;
    count2State = LOW;
    count1State = LOW;
    Serial.println("Entering Running state.");
  }
//*/
}

void CountDownWatchForFinish() {
  valLane1Fault = !digitalRead(lane1StartingEye);   // read the input pin
  valLane2Fault = !digitalRead(lane2StartingEye);   // read the input pin

  if (!raceStarted && valLane1Fault)
  {
    valLane1Faulted = HIGH;
    digitalWrite(lane1PreStageLight,      LOW);
    digitalWrite(lane1StageLight,         LOW);
    digitalWrite(lane1Count3Light,        LOW);
    digitalWrite(lane1Count2Light,        LOW);
    digitalWrite(lane1Count1Light,        LOW);
    digitalWrite(lane1FalseStartRedLight, HIGH);
  }
  if (!raceStarted && valLane2Fault)
  { 
    valLane2Faulted = HIGH;
    digitalWrite(lane2PreStageLight,      LOW);
    digitalWrite(lane2StageLight,         LOW);
    digitalWrite(lane2Count3Light,        LOW);
    digitalWrite(lane2Count2Light,        LOW);
    digitalWrite(lane2Count1Light,        LOW);
    digitalWrite(lane2FalseStartRedLight, HIGH);
  }

  currentMillis = millis();
  if (!count3State)
  {
    digitalWrite(lane1Count3Light, HIGH);
    digitalWrite(lane2Count3Light, HIGH);
    count3State = HIGH;
  }
  if (!count2State && (currentMillis >= timeCountdownStarted + 500))
  {
    digitalWrite(lane1Count2Light, HIGH);
    digitalWrite(lane2Count2Light, HIGH);
    count2State = HIGH;
  }
  if (!count1State && (currentMillis >= timeCountdownStarted + 1000))
  {
    digitalWrite(lane1Count1Light, HIGH);
    digitalWrite(lane2Count1Light, HIGH);
    count1State = HIGH;
  }
  if (!raceStarted && (currentMillis >= timeCountdownStarted + 1500))
  {
    if (!valLane1Faulted)
      digitalWrite(lane1StartGreenLight, HIGH);
    if (!valLane2Faulted)
      digitalWrite(lane2StartGreenLight, HIGH);
    if (valLane1Faulted && valLane2Faulted) 
    {
      state = BOTHFAULT;
      lastTimeStaged = millis();
    }
    Serial.println("Countdown complete");
    raceStarted = HIGH;
  }
  
  valLane1Finished = !digitalRead(lane1FinishLineEye);  // read the input pin
  valLane2Finished = !digitalRead(lane2FinishLineEye);  // read the input pin
  
  if (valLane1Finished && valLane2Finished && !valLane1Faulted && !valLane2Faulted && raceStarted)
  {
    state = TIED;
    lastTimeStaged = millis();
    valLane1Cleared = LOW;
    valLane2Cleared = LOW;
  }
  else if (valLane1Finished && !valLane2Finished && !valLane1Faulted && !valLane2Faulted && raceStarted)
  {
    state = LANE1WON;
    digitalWrite(lane2PreStageLight,      LOW);
    digitalWrite(lane2StageLight,         LOW);
    digitalWrite(lane2Count3Light,        LOW);
    digitalWrite(lane2Count2Light,        LOW);
    digitalWrite(lane2Count1Light,        LOW);
    digitalWrite(lane2StartGreenLight,    LOW);
    digitalWrite(lane2FalseStartRedLight, LOW);
    lastTimeStaged = millis();
    valLane1Cleared = LOW;
    valLane2Cleared = LOW;
  }
  else if (!valLane1Finished && valLane2Finished && !valLane1Faulted && !valLane2Faulted && raceStarted)
  {
    state = LANE2WON;
    digitalWrite(lane1PreStageLight,      LOW);
    digitalWrite(lane1StageLight,         LOW);
    digitalWrite(lane1Count3Light,        LOW);
    digitalWrite(lane1Count2Light,        LOW);
    digitalWrite(lane1Count1Light,        LOW);
    digitalWrite(lane1StartGreenLight,    LOW);
    digitalWrite(lane1FalseStartRedLight, LOW);
    lastTimeStaged = millis();
    valLane1Cleared = LOW;
    valLane2Cleared = LOW;
  }
  else if (!valLane1Finished && valLane2Finished && valLane1Faulted && !valLane2Faulted && raceStarted)
  {
    state = LANE2WON;
    digitalWrite(lane1PreStageLight,      LOW);
    digitalWrite(lane1StageLight,         LOW);
    digitalWrite(lane1Count3Light,        LOW);
    digitalWrite(lane1Count2Light,        LOW);
    digitalWrite(lane1Count1Light,        LOW);
    digitalWrite(lane1StartGreenLight,    LOW);
    digitalWrite(lane1FalseStartRedLight, LOW);
    lastTimeStaged = millis();
    valLane1Cleared = LOW;
    valLane2Cleared = LOW;
  }
  else if (valLane1Finished && !valLane2Finished && !valLane1Faulted && valLane2Faulted && raceStarted)
  {
    state = LANE1WON;
    digitalWrite(lane2PreStageLight,      LOW);
    digitalWrite(lane2StageLight,         LOW);
    digitalWrite(lane2Count3Light,        LOW);
    digitalWrite(lane2Count2Light,        LOW);
    digitalWrite(lane2Count1Light,        LOW);
    digitalWrite(lane2StartGreenLight,    LOW);
    digitalWrite(lane2FalseStartRedLight, LOW);
    lastTimeStaged = millis();
    valLane1Cleared = LOW;
    valLane2Cleared = LOW;
  }
  else if (valLane1Finished && valLane2Finished)
  {
    state = TIED;
    lastTimeStaged = millis();
    valLane1Cleared = LOW;
    valLane2Cleared = LOW;
  }
  
  currentMillis = millis();
  if (currentMillis - lastTime >= 1000)
  {
    lastTime = currentMillis;
    Serial.print("state: ");
    Serial.println(state);
    Serial.print("lastTime: ");
    Serial.println(lastTime);
  }
}


void WatchForStaging() {
  valLane1Staged = !digitalRead(lane1StagingEye);     // read the input pin
  valLane2Staged = !digitalRead(lane2StagingEye);     // read the input pin
  
  if (!valLane1Staged) valLane1Cleared = HIGH;
  if (!valLane2Staged) valLane2Cleared = HIGH;
  
  currentMillis = millis();
  if (currentMillis - lastTime >= 2000)
  {
/*  
    lastTime = currentMillis;
    blinkState = !blinkState;
    if (TIED == state)
    {
      digitalWrite(lane1Count3Light, blinkState);
      digitalWrite(lane1Count2Light, blinkState);
      digitalWrite(lane1Count1Light, blinkState);
      digitalWrite(lane2Count3Light, blinkState);
      digitalWrite(lane2Count2Light, blinkState);
      digitalWrite(lane2Count1Light, blinkState);
    }
    else if (LANE1WON == state)
    {
      digitalWrite(lane1Count3Light, blinkState);
      digitalWrite(lane1Count2Light, blinkState);
      digitalWrite(lane1Count1Light, blinkState);
    }
    else if (LANE2WON == state)
    {
      digitalWrite(lane2Count3Light, blinkState);
      digitalWrite(lane2Count2Light, blinkState);
      digitalWrite(lane2Count1Light, blinkState);
    }
*/
    Serial.print("state: ");
    Serial.println(state);
    Serial.print("lastTime: ");
    Serial.println(lastTime);
  }

  currentMillis = millis();
  if (valLane1Cleared && valLane1Cleared && (valLane1Staged || valLane2Staged))
  {
    if (currentMillis - lastTimeStaged >= 1000)
      lastTimeLane1Unstaged = millis();
      lastTimeLane2Unstaged = millis();
      LightsOut();
      state = STAGING;
  }
  else 
    lastTimeStaged = currentMillis;
}

/*
#define DEMO        0  // cycle through the lights
#define STAGING     1  // light top bar, blink second bar, others off
                       // turn on red, green, yellows for breaking beam of finish, trap, start, prestage
#define LANE1STAGED 2  // lane 1 staged for 3 seconds, stop it's blinking
#define LANE2STAGED 3  // lane 2 staged for 3 seconds, stop it's blinking
#define BOTHSTAGED  4  // both racers ready, no blinking
#define RUNNING     5  // watching for the first racer to cross the finish line
#define LANE1WON    6  // Lane 1 crossed finish line first
#define LANE2WON    7  // Lane 2 crossed finish line first
#define LANE1FAULT  8  // Lane 1 crossed the start line before green
#define LANE2FAULT  9  // Lane 2 crossed the start line before green
#define BOTHFAULT   10 // Both lanes crossed the start line before green
#define LANE1DISQ   11 // 5 minutes have elapsed and lane 1 is DQ'd
#define LANE2DISQ   12 // 5 minutes have elapsed and lane 2 is DQ'd
#define TIED        13 // Both racers crossed finish line at the same moment as far as we can tell

  Serial.print("valLane1Trapped: ");
  Serial.println(valLane1Trapped);

*/

// the loop routine runs over and over again forever:
void loop() {  

  if (currentMillis - lastTime >= 4000)
  {
    lastTime = currentMillis;
    Serial.print("state: ");
    Serial.println(state);
    Serial.print("lastTime: ");
    Serial.println(lastTime);
  }


  switch(state)
  {
  case DEMO:
    //RunDemo();
    state = STAGING;
    lastTime = millis();
    lane1StageState = LOW;
    lane2StageState = HIGH;
    lastTimeLane1Unstaged = millis();
    lastTimeLane2Unstaged = millis();
    break;
  case STAGING:
    Staging();
    break;
  case LANE1STAGED:
    Staging();
    break;
  case LANE2STAGED:
    Staging();
    break;
  case BOTHSTAGED:
    BothStaged();
    break;
  case RUNNING:
    CountDownWatchForFinish();
    break;
  case LANE1WON:
    WatchForStaging();
    break;
  case LANE2WON:
    WatchForStaging();
    break;
  case BOTHFAULT:
    WatchForStaging();
    break;
  case TIED:
    WatchForStaging();
    break;
     // TODO Add code to collect speed trap data
     // TODO Add code to detect winner and flash lights for winning lane

//    if(millis()-RunningTime > RUNNING_TIMEOUT)
//      {
//      Serial.println("Running Timeout");  
//      Stop();  
//      }
  default:
    Serial.print("Unknown State: ");
    Serial.println(state);
    LightsOut(); // Should never get here
    delay(5000);
    state = STAGING;
    break;  
  }
}

