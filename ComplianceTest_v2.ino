//=============================================================\\
//DESN1000 - Rescue Robot
//Compliance Testing
//Written by Aryaman Sakthivel (z5455785)
// ============================================================\\


//=============================================================\\
//Ultrasonic Sensors\\
//=============================================================\\

int Rtrig = 7;
int Recho = 4;
int Ltrig = 12;
int Lecho = 8;

int RightSensor ()
{
  int distance;
  int RtimeMicro;

  digitalWrite(Rtrig,LOW);
  delayMicroseconds(2);
  digitalWrite(Rtrig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Rtrig,LOW);

  RtimeMicro = pulseIn(Recho,HIGH);
  distance = RtimeMicro /29 /2;
  return (distance);

}

int LeftSensor ()
{
  int distance;
  int LtimeMicro;
  digitalWrite(Ltrig,LOW);
  delayMicroseconds(2);
  digitalWrite(Ltrig,HIGH);
  delayMicroseconds(10);
  digitalWrite(Ltrig,LOW);

  LtimeMicro = pulseIn(Lecho,HIGH);
  distance = LtimeMicro /29 /2;
  return (distance);
  
}

//=============================================================\\
//Ultrasonic Sensors\\
//=============================================================\\

int RmotorLogicPin1 = 10;
int RmotorLogicPin2 = 11;
int LmotorLogicPin1 = 6;
int LmotorLogicPin2 = 5;
int pwmDutyCycle = 255;

char CurrentDir = '0';
static int motordata[4];

void initializing() 
{
  //Innitialise the digital output pins
  pinMode(RmotorLogicPin1, OUTPUT);
  pinMode(RmotorLogicPin2, OUTPUT);
  pinMode(LmotorLogicPin1, OUTPUT);
  pinMode(LmotorLogicPin2, OUTPUT);
  
  //Printing the motor configuration for wiring
  Serial.println("Initializing ...");
  Serial.print("Right Motor Pin 1 = ");
  Serial.println(RmotorLogicPin1);
  Serial.print("Right Motor Pin 2 = ");
  Serial.println(RmotorLogicPin2);
  Serial.print("Left Motor Pin 1 = ");
  Serial.println(LmotorLogicPin1);
  Serial.print("Left Motor Pin 2 = ");
  Serial.println(LmotorLogicPin2);
  Serial.println("Initialization Complete");
  Serial.println();
  Serial.println("Commands: ");

}

//Function: Recieve Command Byte
char recieveCommand()
{
  char command;
  //Check if there is anything in the buffer
  if (Serial.available()>0)
  {
    //Reads the first byte of the Serual Monitor and stores it 
    command = Serial.read();
  }
  return command;
}

//Exit Subroutine
void exit()
{
	Serial.println();
  Serial.println("Exiting..");
  	while(true);
}

//Subroutine: Update Direction
int updateDirection (char command)
{
  //Switch Case for Incoming Byte
  switch (command)
  {
    case 'F':
    CurrentDir = 'F';
    motordata[0]=0;
    motordata[1]=pwmDutyCycle;
    motordata[2]=0;
    motordata[3]=pwmDutyCycle;
    break;

    case 'B':
    CurrentDir = 'B';
    motordata[0]=pwmDutyCycle;
    motordata[1]=0;
    motordata[2]=pwmDutyCycle;
    motordata[3]=0;
    break;

    case 'R':
    CurrentDir = 'R';
    motordata[0]=pwmDutyCycle - pwmDutyCycle*0.2;
    motordata[1]=0;
    motordata[2]=0;
    motordata[3]=pwmDutyCycle - pwmDutyCycle*0.2;
    break;

    case 'L':
    CurrentDir = 'L';
    motordata[0]=0;
    motordata[1]=pwmDutyCycle - pwmDutyCycle *0.2;
    motordata[2]=pwmDutyCycle - pwmDutyCycle *0.2;
    motordata[3]=0;
    break;

    case '0':
    CurrentDir = '0';
    Serial.println("Stopping");
    motordata[0]=0;
    motordata[1]=0;
    motordata[2]=0;
    motordata[3]=0;
    break;
    
    case 'X':
    //Calling the exit subroutine
    exit();
    break;
  }
}

//Subroutine: Update Speed 
void updateSpeed(char command)
{
  int speed = atoi(&command); //converts variable type from Chr to Int 
      
  //Check if Motor Speed is between 1-5
  if (speed >= 1 and speed <= 5)
  {
    pwmDutyCycle = pwmDutyCycle * speed/10;
    Serial.print("Speed : ");
    Serial.print(pwmDutyCycle); //Sets the new Speed of the Motors
    Serial.print("%");
  }

  else if (speed == 9 && pwmDutyCycle<=229)
  {
    pwmDutyCycle = pwmDutyCycle + (pwmDutyCycle * 0.10);
    Serial.println();
    Serial.println("Speed Increased by 10%"); //Sets the new Speed of the Motors
  }

  else if (speed == 8 && pwmDutyCycle>=26)
  {
    pwmDutyCycle = pwmDutyCycle - (pwmDutyCycle * 0.10);
    Serial.println();
    Serial.println("Speed Decreased by 10%"); //Sets the new Speed of the Motors
  }

  else
  {
    Serial.println("Invalid Duty Cycle...");
  }

  updateDirection(CurrentDir);
  setMotorPins(motordata);


}

//Subroutine: Set Motor Pins
void setMotorPins(int motordata[])
{
  //Writing the data from motordata to the Motor Pins
  analogWrite (RmotorLogicPin1, motordata[0]);
  analogWrite (RmotorLogicPin2, motordata[1]);
  analogWrite (LmotorLogicPin1, motordata[2]);
  analogWrite (LmotorLogicPin2, motordata[3]);
}


//========================================================\\
//Main\\
//========================================================\\

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  Serial.println("Rescue Robot Team - 06");
  Serial.println();

  pinMode(Rtrig,OUTPUT);
  pinMode(Recho,INPUT);
  pinMode(Ltrig,OUTPUT);
  pinMode(Lecho,INPUT);

  initializing(); 
}

void loop() {
  int Rdist;
  int Ldist;

  char command = '0';
  char prev_command;

  prev_command = command;


  command = recieveCommand();

  if (isDigit(command)>0 && command != '0')
  {
    //Call subroutine to update speed
    Serial.println();
    updateSpeed(command);
  }
  
  else
  {
    //Call function to update the direction
    updateDirection(command);
    //Call subroutine to set the motor pins
    setMotorPins(motordata);
  }


  Rdist = RightSensor();
  Ldist = LeftSensor();

  Serial.println();
  Serial.print("\tRight Distance: ");
  Serial.print(Rdist);
  Serial.print("\tLeft Distance: ");
  Serial.print(Ldist);

  delay(100);
}
