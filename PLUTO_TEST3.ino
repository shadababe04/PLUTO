

#include <SoftwareSerial.h>
// defining communicationion pins for Software Serial
# define GSM_RX 3 // Connect TX of GSM module
# define GSM_TX 2 // Connect RX of GSM module


// Defining interfacing pins for Relay board
# define RELAY 4
# define MSG_1 5
# define MSG_2 6
# define MSG_3 7
//# define MSG_4 8

SoftwareSerial gsm_board(2,3);

const static String saveInSimCmd1="AT+CPBW=1";//write in phone book
const static String saveInSimCmd2="\",129,\"caller\"\r";//+'\r';

boolean call_flag=0, relay_flag=0;
int msg_flag=0;
int i=0,j=0,x=-1,y=-1,z=0,temp=0,count=0;
char n[3];
void gsm_initilaize();// used to inilitize the gsm and chk all its parameters
void relay(); // used to control relay outputs.
String a;
static String callerToRemind="1223445";
//char network_name[10] = " "; // need to reserve a length, otherwise strange results
//char c;

  String MSID1="";

void setup() {
  // put your setup code here, to run once:
  gsm_board.begin(9600);
  Serial.begin(9600);
  pinMode(RELAY,OUTPUT);
  pinMode(MSG_1,OUTPUT);
  pinMode(MSG_2,OUTPUT);
  pinMode(MSG_3,OUTPUT);
//  pinMode(MSG_4,OUTPUT);
  digitalWrite(RELAY,HIGH);
  digitalWrite(MSG_1,HIGH);
  digitalWrite(MSG_2,HIGH);
  digitalWrite(MSG_3,HIGH);
  //digitalWrite(MSG_4,HIGH);
  
  gsm_initilaize();

  delay(2000);
 // Serial.print("shadab");
//  gsm_board.println("ATD9810821638;");
 // gsm_board.println("ATD"+callerToRemind+";\r");
 // delay(10000);
//  Serial.print("rahul");
//  //delay(3000);
//  gsm_board.println("ATH");
 //gsm_board.print("AT+CPBR=1\r");
 //simFlush();
 gsm_board.print("AT+CPBR=1\r");
 //simFlush();
   
   
}
//////////////////////////////setup ends/////////////////////////////

/////////////////////loop begins///////////////////////////
void loop() {

  String gsm_data; // to hold incomming communication from GSM module

  String MSID;
   
  while(gsm_board.available()) 
  {
    char c=gsm_board.read();
    gsm_data+=c;
    delay(10);
  }  //read serial data and store it to gsm_data STRING instance;

//gsm_board.print("AT+CPBR=1\r");

//Serial.println(gsm_data);

 // Serial.print("Call");
  
  if(!call_flag) // if call is not in connected, checking for ring
  {
    //Serial.print("Call");
    // gsm_data.println("ATS0=1"); // automatically answer call after 1 ring
    gsm_board.print("AT+CPBF=\"caller\"");
    int a=gsm_board.read();
    Serial.print(a);
    x=gsm_data.indexOf("RING");
    if(x>-1)
    {
      y=gsm_data.startsWith("+CLIP:\r\n");
      //Serial.print(y);
      if(y==0)
      {
        
         //Serial.print(gsm_data);
        
       // Serial.print(MSID);
        
//+CLIP: "+919810821638",145,"",,"",0
//0123456789012345678901234567890
//        Serial.print("Mobile Numer ");
        callerToRemind=gsm_data.substring(21, 31);

        Serial.print(callerToRemind);
        delay(500);
        gsm_board.print(saveInSimCmd1+","+"\""+callerToRemind+saveInSimCmd2);
        delay(500);
        
      }

      
      delay(5000);
      gsm_board.println("ATA");
      call_flag=1;
    
  }
  }// ring test over, call flag high if sim rings
  if(call_flag) // if call is connected
  {
    if(!msg_flag)
    {
      //MSG_Pluto
    
    digitalWrite(MSG_1,LOW);
    //delay(500);
    //Serial.print("MSG1"); 
    delay(1500); 
    digitalWrite(MSG_1,HIGH);
    delay(1500);
    //Serial.print("MSG2"); 
    msg_flag=1;
    //Serial.print(msg_flag);
    //delay(10000);
    }
    x=gsm_data.indexOf("DTMF"); //checkinh dtmf and storing approprietly
    if(x>-1)
    {

      n[j]=gsm_data[x+5];
      Serial.println(n[j]);
      relay_flag=1;
    }
 
    x=gsm_data.indexOf("NO CARRIER"); // Checking whether call is still connected or not
    if(x>-1)
    {
      gsm_board.println("ATH");
      call_flag=0;
      msg_flag=0;
      j=0;
    }
  }
  
  if(relay_flag) // If a call was resently disconnected, changing relay states accordingly
  {
    Serial.print("relay"); 
    relay();
  }
  
}

//////////////////////////////loop ends/////////////////////////////

/////////////////////gsm inilitize begins///////////////////////////

void gsm_initilaize()
{
 String MSD2="";
  
  boolean gsm_Ready=1;
  Serial.println("initializing GSM module");
  while(gsm_Ready>0)
  {
   gsm_board.println("AT");
   Serial.println("AT"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("OK")>0)
       gsm_Ready=0;
   }
   delay(2000);
  }
  Serial.println("AT READY");
  // GSM MODULE REPLIED 'OK' TO 'AT' INPUT, INDICAING THE MODULE IS OK AND FUNCTIONING
  
  boolean ntw_Ready=1;
  Serial.println("finding network");
  while(ntw_Ready>0)
  {
   gsm_board.println("AT+CPIN?");
   Serial.println("AT+CPIN?"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("+CPIN: READY")>0)
       ntw_Ready=0;
   }
   delay(2000);
  }
  Serial.println("NTW READY");

  

// GSM MODULE REPLIED '+CPIN:READY' TO 'AT+CPIN?' INPUT, INDICAING THE NETWORK IS OK AND FUNCTIONING
  
  boolean DTMF_Ready=1;
  Serial.println("turning DTMF ON");
  while(DTMF_Ready>0)
  {
   gsm_board.println("AT+DDET=1");
   Serial.println("AT+DDET=1"); 
   while(gsm_board.available())
   {
     if(gsm_board.find("OK")>0)
       DTMF_Ready=0;
   }
   delay(2000);
  }
  Serial.println("DTMF READY");// GSM MODULE REPLIED '+OK' TO 'AT+DDET=1?' INPUT, INDICAING THE DTMF IS ON;
 MSD2=gsm_board.print("AT+CPBR=1\r");
 simFlush();
 //Serial.print(MSD2);
  
}

//////////////////////////////gsm inilitization ends/////////////////////////////

/////////////////////relay begins///////////////////////////

void relay()
{
      Serial.print("RLYIN \n"); 
      if(n[0]=='1')
      {
       Serial.print("RLY1\n"); 
       digitalWrite(RELAY,HIGH); //relay 1 on
        //digitalWrite(RELAY,HIGH);//DEBUG
        Serial.println("RELAY 1 ON\n");
        digitalWrite(MSG_2,LOW);
        delay(500);
        digitalWrite(MSG_2,HIGH);

        delay(2000);
      }
       else if(n[0]=='2')
      {
        Serial.print("RLY2\n"); 
        digitalWrite(RELAY,LOW);// relay 1 off
        Serial.println("RELAY 1 OFF");
        digitalWrite(MSG_3,LOW);
        delay(500);
        digitalWrite(MSG_3,HIGH);

        delay(2000);
      }
//      else if(n[0]=='3')
//      {//MSG_Pluto
//        digitalWrite(MSG_4,LOW);
//        delay(500);
//        digitalWrite(MSG_4,HIGH);
//
//        delay(3000);
//        Serial.println("MSG Repeat");
//      }
//    
    
    relay_flag=0;
    Serial.print("RLYout"); 
}

/////////////////////relay ends///////////////////////////

void simFlush() {
while (gsm_board.available()) gsm_board.read();
}


