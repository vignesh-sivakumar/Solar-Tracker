#include<EEPROM.h>
#include<TinyGPS++.h>
#include<SoftwareSerial.h>

#define dir1 8              //chinna motor
#define stp1 9
#define en1 10

#define dir2 5              //periya motor
#define stp2 6
#define en2 7
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(dir1, OUTPUT);
  pinMode(stp1, OUTPUT);
  pinMode(en1,INPUT);
  digitalWrite(en1, HIGH);

  pinMode(dir2, OUTPUT);
  pinMode(stp2, OUTPUT);
  pinMode(en2,INPUT);
  digitalWrite(en2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  double lati = 12.97244100;
  double longi = 80.12604500;
  double dd = 23.00000000;
  double mm = 12.00000000;
  double yy = 2019.00000000;
  double hr=15.00000000,mins=50.00000000,sec=10.00000000;
  
  double prev_alt,prev_azimuth,rot_alt,rot_azimuth;
  double alt      = calculateAngles(lati,longi,dd,mm,yy,hr,mins,sec,"azimuth");
  double azimuth  = calculateAngles(lati,longi,dd,mm,yy,hr,mins,sec,"altitude");

  if(alt>=130)
  {  EEPROM.put(201,0.00000000);
     EEPROM.put(301,0.00000000);
     delay(3600000);
  }
  else
  {
    prev_alt = EEPROM.get(201,prev_alt);
    prev_azimuth = EEPROM.get(301,prev_azimuth);

    rot_alt = alt-prev_alt;
    rot_azimuth = azimuth-prev_azimuth;
    
    //Serial.println(alt);
    //Serial.println(azimuth);
  
    step1(rot_azimuth);
    delay(1000);
    step2(rot_alt);
    delay(900000);
  }
}

double dayspassed(int dd,int mm,int yy) {
  double numOfDaysPassed=0.00000000;
  switch(1){
    case 1:
      if(mm==1)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=31;
      }
    case 2:
    if(mm==2)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        if(yy%4==0 && yy%100==0 && yy%400==0)
         numOfDaysPassed+=29;
        else
         numOfDaysPassed+=28; 
      }
    case 3:
    if(mm==3)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=31;
      }
    case 4:
    if(mm==4)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=30;
      }
    case 5:
    if(mm==5)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=31;
      }
    case 6:
    if(mm==6)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=30;
      }
    case 7:
    if(mm==7)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=31;
      }
    case 8:
    if(mm==8)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=31;
      }
    case 9:
    if(mm==9)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=30;
      }
    case 10:
    if(mm==10)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=31;
      }
    case 11:
    if(mm==11)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=30;
      }
    case 12:
    if(mm==12)
      {
        numOfDaysPassed+=dd;
        break;
      }
      else
      {
        numOfDaysPassed+=31;
      }
    
    
  }
  return(numOfDaysPassed);
}

double calculateAngles(double lati,double longi,double dd,double mm,double yy,double hr,double mins,double sec,char* selection)
{
  double D2R=0.01745330;
  double azimuth = 0.00000000;
  double hg = 5.00000000;
  double mg = 30.00000000;
  
  double numOfDaysPassed=dayspassed(dd,mm,yy);
  double localTimeMinutes = (hr*60.00000000)+mins+(sec/60.00000000);
  
  double gmtgap = hg+(mg/60.00000000);
  double lstm = 15.00000000*gmtgap;
  
  double tc1 = 4.00000000*(longi-lstm);
  double B = (360.00000000/365.00000000)*(numOfDaysPassed-81.00000000);
  double eotcorr=(9.87000000*sin(2.00000000*B*D2R)) - (7.53000000*cos(B*D2R)) - (1.50000000*sin(B*D2R));
  double tc=tc1+eotcorr;

  double localSolarTime=(localTimeMinutes+tc)/60.00000000;
  double hourangle=15.00*(localSolarTime-12.00000000);
  double decangle=23.45000000*sin((360.00000000/365.00000000)*D2R*(285.00000000+numOfDaysPassed));

  double alt = 1.00000000/D2R*(asin(sin(decangle*D2R)*sin(lati*D2R)+cos(decangle*D2R)*cos(lati*D2R)*cos(hourangle*D2R)));
  
  if(localSolarTime<12.00000000 || hourangle < 0.00000000)
  {
   azimuth = 1.00000000/D2R * (acos((sin(decangle*D2R)*cos(lati*D2R)-cos(decangle*D2R)*sin(lati*D2R)*cos(hourangle*D2R))/cos(alt*D2R)));
  }
  else{
    azimuth = 360.00000000 - (1.00/D2R * (acos((sin(decangle*D2R)*cos(lati*D2R)-cos(decangle*D2R)*sin(lati*D2R)*cos(hourangle*D2R))/cos(alt*D2R))));
  }
/*
  Serial.println(localTimeMinutes);
  Serial.println(gmtgap);
  Serial.println(numOfDaysPassed);
  Serial.println(lstm);
  Serial.println(tc1);
  Serial.println(B);
  Serial.println(eotcorr);
  Serial.println(tc);
  Serial.println(localSolarTime);
  Serial.println(hourangle);
  Serial.println(decangle);
  Serial.println();
*/
  if(selection=="altitude")
    return(alt);
  else
    return(azimuth);
}

void step1(double azimuth)
{
  for (int i = 0; i < 400; i++)
  { 
    
      MoveClk1();
   
  }
  delay(2000);
  for (int i = 0; i < 400; i++)
  { 
    
      MoveAntiClk1();
   
  }
  
  digitalWrite(en1,LOW);

}

void MoveAntiClk1()
{
  digitalWrite(dir1, 0);
  
  digitalWrite(stp1, HIGH);
  delay(1);
  digitalWrite(stp1, LOW);
  delay(1);

}

void MoveClk1()
{

  digitalWrite(dir1, 1);
  
  digitalWrite(stp1, HIGH);
  delay(1);
  digitalWrite(stp1, LOW);
  delay(1);

}

void step2(double alt)
{
  for (int i = 0; i < 50; i++)
  { 
    
      MoveClk2();
   
  }
  
  delay(1000);
  for (int i = 0; i < 50; i++)
  { 
    
      MoveAntiClk2();
  }
  
  digitalWrite(en2,LOW);

}

void MoveAntiClk2()
{
  digitalWrite(dir2, 1);
  
  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

}


void MoveClk2()
{

  digitalWrite(dir2, 0);
  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

  
  digitalWrite(stp2, HIGH);
  delay(1);
  digitalWrite(stp2, LOW);
  delay(1);

}
