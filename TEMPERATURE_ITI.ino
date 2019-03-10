#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(49, 48, 26, 27, 28, 29);
// Data wire is conntec to the CUSTOMIZED Arduino board's digital pin 33
#define ONE_WIRE_BUS 33

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

float temp,faht,newvalue=0;
String receivedMsg="",mobNum,stored_MobNum="",senderNum,num[10];
int sensor_on=0,eeprom_store=0,i,hold[10],counter=0,id=1;
unsigned long last_time=0,update_time=60000; // update on cloud for every 1 minute(s)
String String_Id,String_Temp,String_Faht,String_Name="TempDevice";



void setup() {
  // put your setup code here, to run once:
  Serial.println("KernelSphere temperature Monitoring Device");
  lcd.begin(16,2); 
  Serial.begin(115200); 
  Serial2.begin(115200);
  sensors.begin();

   /*
   for(i=0;i<11;i++){
   EEPROM.write(i,0);
   }
  */

  
  lcd.clear();
  delay(50);
  lcd.setCursor(2,0); //column,row
  lcd.print("KernelSphere"); 
  lcd.setCursor(0,1); //column,row
  lcd.print("Temp Monitoring");  
  
  delay(5000);
  Serial2.println("AT\r");  
  delay(100);
  Serial2.println("AT+CMGF=1\r");  // set SMS mode to text
  delay(100);
  Serial2.println("AT+CNMI=2,2,0,0,0\r");  // set SMS mode to text(NEW MESSAGE INDICATIONS)
  delay(100);
  Serial2.println("AT+CMGD=1,4\r");
  delay(100);

  
  if(EEPROM.read(0)==1){
     Serial.println("In reserve Mode");
    
     Serial.println(EEPROM.read(1));
     Serial.println(EEPROM.read(2));
     Serial.println(EEPROM.read(3));
     Serial.println(EEPROM.read(4));
     Serial.println(EEPROM.read(5));
     Serial.println(EEPROM.read(6));
     Serial.println(EEPROM.read(7));
     Serial.println(EEPROM.read(8));
     Serial.println(EEPROM.read(9));
     Serial.println(EEPROM.read(10));     
 
        
     mobNum=String(EEPROM.read(1))+String(EEPROM.read(2))+String(EEPROM.read(3))+String(EEPROM.read(4))+String(EEPROM.read(5))+String(EEPROM.read(6))+String(EEPROM.read(7))+String(EEPROM.read(8))+String(EEPROM.read(9))+String(EEPROM.read(10));     
     
     Serial.println("@--------@---------@");
     Serial.println(mobNum);
     Serial.println("@--------@---------@");
     stored_MobNum=mobNum;
     eeprom_store=1;
     delay(100);
    }
  
}

void loop() {
  // put your main code here, to run repeatedly:

  
  sensors.requestTemperatures(); 
  temp=sensors.getTempCByIndex(0);
  faht=sensors.getTempFByIndex(0);

   String_Id     = String(id); 
   String_Temp  = String(temp);  
   String_Faht   =String(faht);
 
  
  lcd.clear();
  delay(50);
  lcd.setCursor(4,0);
  lcd.print("Device:");
  lcd.print(id);
  delay(2000);
  
  lcd.clear();
  delay(50);  
  lcd.setCursor(1,0); //column,row
  lcd.print("Deg :");  
  lcd.print(temp);
  lcd.print("'C");
  
  lcd.setCursor(1,1); //column,row
  lcd.print("Faht:"); 
  lcd.print(faht);
  lcd.print("'F");
  delay(2000);
  
  
 
 if(eeprom_store==0)
 {
  
  Serial.println("Please provide a recipient Mobile number");
  
  //**************************** Receive message for new number ****************************************//  
  int a = Serial2.available();    //Always displays Serial status
  Serial.print("Serial value = ");
  Serial.print(a);
  Serial.println();
  delay(1000);
 }
  

if(Serial2.available()>0)
  {
  ///////////////When SMS is sent from Recipient number////////////////////////////////////////////////////
  if (Serial2.find("\n+CMT: "))
  {
   receivedMsg="";
   Serial.println("MSG RECEIVED");
   receivedMsg = readSMS();
   Serial.println(receivedMsg);
   senderNum=(receivedMsg.substring(4,14));
   
   if(receivedMsg.substring(43,44)=="C" || receivedMsg.substring(43,44)=="c")
       {
         if(receivedMsg.substring(44,45)=="M" || receivedMsg.substring(44,45)=="m")
         {
          if(EEPROM.read(0)==1)
          {
           clearmem();
          }
          else
          {
           freemem();
          }
         }
       }

   if(receivedMsg.substring(43,44)=="C" || receivedMsg.substring(43,44)=="c" )
       {
         if(receivedMsg.substring(44,45)=="N" || receivedMsg.substring(44,45)=="n") 
          { 
              mobNum=receivedMsg.substring(45,55); //Use command CN944XXXXXXX to change for other recipient number 
              Serial.print("Recipient number = ");
              Serial.print(mobNum);
              Serial.println();
       
      
       if(mobNum!=stored_MobNum)
       { 
        EEPROM.write(0,1);  
        num[0]=mobNum.substring(0,1);
        hold[0]=num[0].toInt();
        Serial.println(hold[0]);
  
        num[1]=mobNum.substring(1,2);
        hold[1]=num[1].toInt();
        Serial.println(hold[1]);
  
        num[2]=mobNum.substring(2,3);
        hold[2]=num[2].toInt();
        Serial.println(hold[2]);
  
        num[3]=mobNum.substring(3,4);
        hold[3]=num[3].toInt();
        Serial.println(hold[3]);
        
        num[4]=mobNum.substring(4,5);
        hold[4]=num[4].toInt();  
        Serial.println(hold[4]);

        num[5]=mobNum.substring(5,6);
        hold[5]=num[5].toInt();  
        Serial.println(hold[5]);
        
        num[6]=mobNum.substring(6,7);
        hold[6]=num[6].toInt();  
        Serial.println(hold[6]); 

        num[7]=mobNum.substring(7,8);
        hold[7]=num[7].toInt();  
        Serial.println(hold[7]);

        num[8]=mobNum.substring(8,9);
        hold[8]=num[8].toInt();  
        Serial.println(hold[8]);

        num[9]=mobNum.substring(9,10);
        hold[9]=num[9].toInt();  
        Serial.println(hold[9]);
        
         
         EEPROM.write(1,hold[0]); //store the recipient number in EEPROM
         EEPROM.write(2,hold[1]);
         EEPROM.write(3,hold[2]);
         EEPROM.write(4,hold[3]);
         EEPROM.write(5,hold[4]);
         EEPROM.write(6,hold[5]);
         EEPROM.write(7,hold[6]);
         EEPROM.write(8,hold[7]);
         EEPROM.write(9,hold[8]);
         EEPROM.write(10,hold[9]);
         
         
         
         Serial.println("Number stored");
         //sensor_on=1;
         eeprom_store=1;
         newNum();  //Change to new recipient number function call
         stored_MobNum=mobNum;
        }
        
        else
         {
          errorMsg();    
          } 
          }
       } 

    }
  } 

if(eeprom_store == 1)
{
  if(millis()-last_time> update_time)  //Send data to cloud for every 10 minutes
  {
   datapost();//Upendar
   delay(2000);
   lcd.clear();
   lcd.setCursor(3,0);
   lcd.print("Dataposted");
   lcd.setCursor(4,1);
   lcd.print("to cloud");
   delay(2000);
   
   sendSMS(); // Send message to the SIM stored on EEPROM 
   //websiteUrl();
  Serial.print("Celsius temperature: ");  
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print("-->Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));

  last_time = millis();
  }
 
 }
}


//************************************Read incoming message*******************************************//
String readSMS()
{
   while (Serial2.available())
    {
      char c = Serial2.read();
      receivedMsg+=c;
      delayMicroseconds(70);
     }
    Serial2.flush();
    return receivedMsg;
}

//***************************Function to clear EEPROM **************************//

void clearmem(){
 for(i=0;i<11;i++)
 {
   EEPROM.write(i,0);
   }
   Serial.println("EEPROM memory cleared successfully on device " + String(id) + "!");
   mobNum="";
   memClear();
   eeprom_store=0;
  }
 
//*********************************Free memory function(Nothing to clear display message)**************************************//
void freemem(){
     Serial2.println("AT\r");// Check for GSM modem working
     delay(100);
     Serial2.println("AT+CMGF=1");    //To send SMS in Text Mode
     delay(100);
     Serial2.println("AT+CMGS=\"" + senderNum + "\"");//using Variable
     //Serial2.println("AT+CMGS=\"6361066623\"\r"); //using phone number 
     delay(3000);

     Serial2.println("EEPROM already cleared on device " + String(id) + "!"); //the content of the message
     delay(200);
     Serial2.print("\r");
     Serial2.println((char)26); //the stopping character Ctrl+Z
     Serial2.flush();
     delay(100);   
    }

//****************************EEPROM memory clear message************************//
void memClear()
{
     Serial2.println("AT\r");// Check for GSM modem working
     delay(100);
     Serial2.println("AT+CMGF=1");    //To send SMS in Text Mode
     delay(100);
     Serial2.println("AT+CMGS=\"" + senderNum + "\"");//using Variable
     //Serial2.println("AT+CMGS=\"6361066623\"\r"); //using phone number 
     delay(3000);

     Serial2.println("Memory cleared on device " + String(id) + "!"); //the content of the message
     delay(200);
     Serial2.print("\r");
     Serial2.println((char)26); //the stopping character Ctrl+Z
     Serial2.flush();
     delay(100);   
}

//*********************************** Message to changed recipient number ***************************//
 void newNum()
{    
     delay(200);
     Serial2.println("AT\r");// Check for GSM modem working
     delay(100);
     Serial2.println("AT+CMGF=1");    //To send SMS in Text Mode
     delay(100);
     
     Serial2.println("AT+CMGS=\"" + mobNum + "\"");//using Variable
     //Serial2.println("AT+CMGS=\"6361066623\"\r"); //using phone number 
     delay(3000);

     Serial2.println("Number changed Successfully on device " + String(id) + "!");//the content of the message
     delay(200);
     Serial2.print("\r");
     Serial2.println((char)26); //the stopping character Ctrl+Z
     Serial2.flush();
     delay(100);   
   
 }

//****************************************** message to recipient number *********************************************************/
void sendSMS()
{
    
     Serial2.println("AT\r");// Check for GSM modem working
     delay(100);
     Serial2.println("AT+CMGF=1");    //To send SMS in Text Mode
     delay(100);
     
     Serial2.println("AT+CMGS=\"" + mobNum + "\"");//using Variable
     //Serial2.println("AT+CMGS=\"6361066623\"\r"); //using phone number 
     delay(3000);
    
     Serial2.println("Degree  = " +  String(temp) + " 'C" + "\n" + "Fahrent = " + String(faht) + " 'F" + "\n" + "Temperature data from device " +String(id) + "\n" + "Check following URL for cloud data:-http://bihariti.kernelsphere.com/api/temperature");//the content of the message
     delay(200);
     Serial2.print("\r");
     Serial2.println((char)26); //the stopping character Ctrl+Z
     Serial2.flush();
     delay(100);   
   
 }

//****************************Error Message function*****************************//
void errorMsg()
{
     Serial2.println("AT\r");// Check for GSM modem working
     delay(100);
     Serial2.println("AT+CMGF=1");    //To send SMS in Text Mode
     delay(100);
     Serial2.println("AT+CMGS=\"" + senderNum + "\"");//using Variable
     //Serial2.println("AT+CMGS=\"6361066623\"\r"); //using phone number 
     delay(3000);

     Serial2.println("Mobile number already exists on device "+String(id)+",provide unique number!");//the content of the message
     delay(200);
     Serial2.print("\r");
     Serial2.println((char)26); //the stopping character Ctrl+Z
     Serial2.flush();
     delay(100);   
}

//***********************Cloud communication********************************************//

void datapost()
{
 Serial2.println("AT+CGATT?");
  delay(100);
  toSerial();

  // bearer settings
  Serial2.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  toSerial();

  // bearer settings
  Serial2.println("AT+SAPBR=3,1,\"APN\",\"epc.tmobile.com\"");
  delay(2000);
  toSerial();


// bearer settings
  Serial2.println("AT+SAPBR=0,1");
  delay(2000);
  toSerial();
  
  // bearer settings
  Serial2.println("AT+SAPBR=1,1");
  delay(2000);
  toSerial();

  // initialize http service
   Serial2.println("AT+HTTPINIT");
   delay(2000); 
   toSerial();

// String mywebapi = "AT+HTTPPARA=\"URL\",\"http://jusco.kernelsphere.com/api/SmartEnergy/SaveEnergy?v="+String_voltage+"&c="+String_current+"&pf="+String_powerfactor+"&f="+String_Frequency+"&deviceid="+String_Id+"&devicename="+String_Name+"\"";
   // String mywebapi = "AT+HTTPPARA=\"URL\",\"https://bihariti.kernelsphere.com/api/SmartEnergy/SaveEnergy?v=220.00&c=0.08&pf=-0.05&f=50.18&deviceid=1&devicename=aa\"";
   //   String mywebapi = "AT+HTTPPARA=\"URL\",\"http://bihariti.kernelsphere.com/api/SmartEnergy/SaveEnergy?v=220.00&c=0.08&pf=-0.05&f=50.18&deviceid=1&devicename=aa\"";
      String mywebapi = "AT+HTTPPARA=\"URL\",\"http://bihariti.kernelsphere.com/api/SmartTemperature/SaveTemperature?temp="+String_Temp+"&faht="+String_Faht+"&deviceid="+String_Id+"&devicename="+String_Name+"\"";

   Serial2.println(mywebapi);
   delay(2000);
   toSerial();

   // set http action type 0 = GET, 1 = POST, 2 = HEAD
   Serial2.println("AT+HTTPACTION=0");
   delay(2000);
   toSerial();

   // read server response
   Serial2.println("AT+HTTPREAD"); 
   delay(2000);
   toSerial();

   Serial2.println("");
   Serial2.println("AT+HTTPTERM");
   toSerial();
   delay(3000);

   Serial2.println("");
   delay(1000);   
}

void toSerial()
{
  while(Serial2.available()!=0)
  {
    Serial.write(Serial2.read());
  }
}

//***********************************Message to send URL of cloud*****************************************//
void websiteUrl(){
     Serial2.println("AT\r");// Check for GSM modem working
     delay(100);
     Serial2.println("AT+CMGF=1");    //To send SMS in Text Mode
     delay(100);
     Serial2.println("AT+CMGS=\"" + mobNum + "\"");//using Variable
     //Serial2.println("AT+CMGS=\"6361066623\"\r"); //using phone number 
     delay(3000);

     Serial2.println("Check URL for data:-https://bihariti.kernelsphere.com/api/temperature");//the content of the message
     delay(200); 
     Serial2.print("\r");
     Serial2.println((char)26); //the stopping character Ctrl+Z
     Serial2.flush();
     delay(100);   
    }




