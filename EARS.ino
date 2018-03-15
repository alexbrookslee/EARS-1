
size_t numOut = 10;
double C = .12; // meters per microsecond
int micAdd[] = {0,1,2};// origin, x, y
int motorAdd[] = {3,4,5,6,7,8,9,10,11,12};
double mics[] = {0,0,0};
double sides[] = {0,0,0}; // Alpha(x), Beta(y), P(direct vector)


double *outArray; 
int rangeSize = 36;

void setup() {
   Serial.begin(9600);
    for(int i = 0; i <= 9; i++){
        pinMode(motorAdd[i], OUTPUT);
    }
    
    for(int i = 0; i <= 2; i++){
        pinMode(micAdd[i], INPUT);
    }
   
    attachInterrupt(digitalPinToInterrupt(micAdd[0]),measureOne , RISING);
    attachInterrupt(digitalPinToInterrupt(micAdd[1]),measureTwo , RISING);
    attachInterrupt(digitalPinToInterrupt(micAdd[2]),measureThree , RISING);
    outArray = (double*) calloc(numOut, sizeof(double));
    
     pinMode(LED_BUILTIN, OUTPUT);
     digitalWrite(LED_BUILTIN, HIGH); 
}

void loop() {

  double AA = findAA();
  
  fillArray(AA);
  runMotors();
   
  for(int i = 0; i < (int)numOut; i++){ 
      outArray[i] = 0; 
  }
  
  

}

void measureOne(){ // records mic_1's time stamp
    if(mics[0] == 0){
        mics[0]= micros();
    }

}

void measureTwo(){ // records mic_2's time stamp
    if(mics[1] == 0){
      mics[1]= micros();
    }

}

void measureThree(){ // records mic_3's time stamp
    if(mics[2] == 0){
      mics[2]= micros();

    }

}



double findAA(){ // Approach Angle 
    sides[0] = 10*C * abs(mics[1]- mics[0]);
    sides[1] =10* C * abs(mics[2]-mics[0]);
    
    sides[2] = abs((2*(sides[1] + sides[0]))/(1 - pow(sides[0],2) - pow(sides[1],2) + 1));
    double sigma = acos(((1 - pow(sides[0],2)) /((2* sides[2]) - sides[0])) - sides[0]);
     
     return 180 * (sigma /3.14);   
}

void fillArray(double angle){ // 0 - 360 around the head. 
    double temp = angle;
        
    if(sides[0] < sides[1] && temp > 90){
        temp = 360 - temp ;
  }
  temp = (int)temp % 360;
  int pos = temp / rangeSize;
  outArray[pos] = 1;
    
}
void runMotors(){
    // turn on motors based on flags
    Serial.println("Motors On");
    for(int i = 0; i <=9; i++){
        if(outArray[i] == 1){
            digitalWrite(motorAdd[i], HIGH);
            Serial.println(motorAdd[i]);
         }
    }   
    delay(2000);
    //turn off all motors
    for(int i = 0; i <=9; i++){
       if(outArray[i] == 1){
           digitalWrite(motorAdd[i], LOW);
       }
    }

}




