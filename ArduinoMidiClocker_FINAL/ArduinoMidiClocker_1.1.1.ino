#include "MIDIUSB.h"
#include <SPI.h>
#include <EEPROM.h>




//HARDWAR PIN NUMBERS:

#define CLOCK_1 1 //CLOCK LEDS AND OUTPUTS
#define CLOCK_2 0
#define CLOCK_3 2
#define CLOCK_4 3

#define RESET_1 4 //RESET LEDS
#define RESET_2 5
#define RESET_3 6
#define RESET_4 7

#define DELAY_1 A3 //POTIS
#define DELAY_2 A2
#define DELAY_3 A1
#define DELAY_4 A0



#define DELAY 8  // Button Pins
#define SUBDIV 9
#define RESET 10

#define DOUBLE_CLICK_TIME 500   // Max Time for Double Clicking in ms



#define DEL_RNG 50   // Define Delay Range in ms

#define SWG_RNG 50   // Define Swing Range in ms

#define DIV_RNG 7   // Define Clock Division Range: 0 = 2Pulse/1Sixteenth1 = 1 Pulse/Sixteenth, 2 = 1Pulse/Eights, 3 = 1 Pulse/1Quater, 4 = 1Pulse/2Quater, 5 = 1Pulse/1Bar, 6 = 1Pulse/2Bars, 7 = None

#define RES_RNG 7   // Define Reset Division Rannge: 0 = 2Pulse/1Sixteenth1 = 1 Pulse/Sixteenth, 2 = 1Pulse/Eights, 3 = 1 Pulse/1Quater, 4 = 1Pulse/2Quater, 5 = 1Pulse/1Bar, 6 = 1Pulse/2Bars, 7 = None

int SUBDIVISION[] = {255,255,255,255,240,216,192,168,144,120,96,72,60,54,48,42,30,27,24,21,18,15,12,9,8,7,6,5,4,3,3,3};  // Mapping Array for Fine Division

#define save_time_delay 30000       //  Save Times to Spare on EEPROM Cycles in ms
#define save_time_division 30000
#define save_time_reset 30000
#define save_time_random 30000
#define save_time_swing 30000

#define Pot_thr 5  // Deadband of Pot for Movement detection +/- this value from 1023
#define Buff 4

#define Pulse_Length 15    // Output Pulse Duration in ms (and for Pink LEDS)
#define Pulse_Length_Reset 10   //Pulse lenght for Reset Pulse and Green LED (Can adjust Green LED Brightness here




int D1 = EEPROM.read(4);     //Read Stored Values only once when powered up
int D2 = EEPROM.read(5);
int D3 = EEPROM.read(6);
int D4 = EEPROM.read(7);


int S1 = EEPROM.read(0);
int S2 = EEPROM.read(1);
int S3 = EEPROM.read(2);
int S4 = EEPROM.read(3);


int V1 = EEPROM.read(8);
int V2 = EEPROM.read(9);
int V3 = EEPROM.read(10);
int V4 = EEPROM.read(11);


int R1 = EEPROM.read(24);
int R2 = EEPROM.read(25);
int R3 = EEPROM.read(26);
int R4 = EEPROM.read(27);


int RND1 = EEPROM.read(12);
int RND2 = EEPROM.read(13);
int RND3 = EEPROM.read(14);
int RND4 = EEPROM.read(15);


int Pot1_Save = (EEPROM.read(16) << 8) + EEPROM.read(17);  //Pot Position Saves
int Pot2_Save = (EEPROM.read(18) << 8) + EEPROM.read(19);
int Pot3_Save = (EEPROM.read(20) << 8) + EEPROM.read(21);
int Pot4_Save = (EEPROM.read(22) << 8) + EEPROM.read(23);






// SETUP




void setup()
{
  pinMode(CLOCK_1, OUTPUT);  // Pin Configuration
  pinMode(CLOCK_2, OUTPUT);
  pinMode(CLOCK_3, OUTPUT);
  pinMode(CLOCK_4, OUTPUT);

  pinMode(RESET_1, OUTPUT);
  pinMode(RESET_2, OUTPUT);
  pinMode(RESET_3, OUTPUT);
  pinMode(RESET_4, OUTPUT);


  pinMode(DELAY_1, INPUT);
  pinMode(DELAY_2, INPUT);
  pinMode(DELAY_3, INPUT);
  pinMode(DELAY_4, INPUT);

  pinMode(DELAY, INPUT_PULLUP);  // Pullup Modes for Button Pins
  pinMode(SUBDIV, INPUT_PULLUP);
  pinMode(RESET, INPUT_PULLUP);


  digitalWrite(CLOCK_1, LOW); // Put Outputs Low 
  digitalWrite(CLOCK_2, LOW);
  digitalWrite(CLOCK_3, LOW);
  digitalWrite(CLOCK_4, LOW);

  digitalWrite(RESET_1, LOW);
  digitalWrite(RESET_2, LOW);
  digitalWrite(RESET_3, LOW);
  digitalWrite(RESET_4, LOW);


  SPI.begin();

  Serial.begin(115200);


}












//LOOP




void loop()
{
  static unsigned long clock_timer_1 = 0, clock_timeout_1 = 0; // Clock Timers
  static unsigned long clock_timer_2 = 0, clock_timeout_2 = 0;
  static unsigned long clock_timer_3 = 0, clock_timeout_3 = 0;
  static unsigned long clock_timer_4 = 0, clock_timeout_4 = 0;

  static unsigned long reset_timer_1 = 0, reset_timeout_1 = 0; // Reset Timers
  static unsigned long reset_timer_2 = 0, reset_timeout_2 = 0;
  static unsigned long reset_timer_3 = 0, reset_timeout_3 = 0;
  static unsigned long reset_timer_4 = 0, reset_timeout_4 = 0;

  static unsigned long master_timer = 0, master_timeout = 0;


  static unsigned long save_timer_delay_1 = 0;  // Timer for delaying Saving Time
  static unsigned long save_timer_delay_2 = 0;
  static unsigned long save_timer_delay_3 = 0;
  static unsigned long save_timer_delay_4 = 0;

  static unsigned long save_timer_division_1 = 0;
  static unsigned long save_timer_division_2 = 0;
  static unsigned long save_timer_division_3 = 0;
  static unsigned long save_timer_division_4 = 0;


  static unsigned long save_timer_division_5 = 0;
  static unsigned long save_timer_division_6 = 0;
  static unsigned long save_timer_division_7 = 0;
  static unsigned long save_timer_division_8 = 0;

  static unsigned long save_timer_reset_1 = 0;
  static unsigned long save_timer_reset_2 = 0;
  static unsigned long save_timer_reset_3 = 0;
  static unsigned long save_timer_reset_4 = 0;

  static unsigned long save_timer_random_1 = 0;
  static unsigned long save_timer_random_2 = 0;
  static unsigned long save_timer_random_3 = 0;
  static unsigned long save_timer_random_4 = 0;

  static unsigned long save_timer_swing_1 = 0;
  static unsigned long save_timer_swing_2 = 0;
  static unsigned long save_timer_swing_3 = 0;
  static unsigned long save_timer_swing_4 = 0;




  static unsigned int clock_count_1 = 0; //Clock Counters
  static unsigned int clock_count_2 = 0;
  static unsigned int clock_count_3 = 0;
  static unsigned int clock_count_4 = 0;

  static unsigned int reset_count_1 = 0; //Reset Counters
  static unsigned int reset_count_2 = 0;
  static unsigned int reset_count_3 = 0;
  static unsigned int reset_count_4 = 0;

  static unsigned int master_count = 0;

  static bool MCS_1 = false; // Master Clock States  (undelayed Clock State)
  static bool MCS_2 = false;
  static bool MCS_3 = false;
  static bool MCS_4 = false;

  static bool MCS_5 = false; // Master Reset States (undelayed Reset State)
  static bool MCS_6 = false;
  static bool MCS_7 = false;
  static bool MCS_8 = false;

  static bool MCS = false;


  static bool Check_1 = false;  // Check bools for Save time delays 4 for each Parameter
  static bool Check_2 = false;
  static bool Check_3 = false;
  static bool Check_4 = false;
  static bool Check_5 = false;
  static bool Check_6 = false;
  static bool Check_7 = false;
  static bool Check_8 = false;
  static bool Check_9 = false;
  static bool Check_10 = false;
  static bool Check_11 = false;
  static bool Check_12 = false;
  static bool Check_13 = false;
  static bool Check_14 = false;
  static bool Check_15 = false;
  static bool Check_16 = false;
  static bool Check_17 = false;
  static bool Check_18 = false;
  static bool Check_19 = false;
  static bool Check_20 = false;
  static bool Check_21 = false;
  static bool Check_22 = false;
  static bool Check_23 = false;
  static bool Check_24 = false;
  

  static bool Reset_Check = false;


  static unsigned int rnd1 = 100;
  static unsigned int rnd2 = 100;
  static unsigned int rnd3 = 100;
  static unsigned int rnd4 = 100;

  static bool rndcheck1 = false;
  static bool rndcheck2 = false;
  static bool rndcheck3 = false;
  static bool rndcheck4 = false;



  static bool swing_state = false;


  static unsigned int s1 = 0;
  static unsigned int s2 = 0;
  static unsigned int s3 = 0;
  static unsigned int s4 = 0;
  
  


 
  




  
  bool Delay_Mode = false;   // Modes Detection (Shift keys)
  bool Division_Mode = false;
  bool Subdivision_Mode = false;
  bool Reset_Division_Mode = false;
  bool Random_Mode = false;
  bool Reset_Parameter_Mode = false;
  bool Swing_Mode = false;




  static unsigned long Switch_Timer_d = 0;  //Timers and Latches for Click counters  (Double Clicks)
  static unsigned long Switch_Timer_s = 0;
  static unsigned long Switch_Timer_r = 0;

  static unsigned int Switch_Counter_d = 0;
  static unsigned int Switch_Counter_s = 0; 
  static unsigned int Switch_Counter_r = 0;

  static bool latch_d = false;
  static bool latch_s = false;
  static bool latch_r = false;



  bool d = !digitalRead(DELAY);  //Switches Read In
  bool s = !digitalRead(SUBDIV);
  bool r = !digitalRead(RESET);







 if (master_count > 5) {
    
    s1 = S1;
    s2 = S2;
    s3 = S3;
    s4 = S4;
    
  }

  

 else if (master_count < 6)  {  //Set Swin delay for each channel if its an odd number of 16th steps
    
    s1 = 0;
    s2 = 0;
    s3 = 0;
    s4 = 0;
    
}





  if ((d == true) && (latch_d == false)) {  //Count Times Button Pressed
    Switch_Counter_d++;
    Switch_Timer_d = millis();
    latch_d = true;
  }
  else if (d == false) {
    latch_d = false;
  }

  

  if ((s == true) && (latch_s == false)) {
    Switch_Counter_s++;
    Switch_Timer_s = millis();
    latch_s = true;
  }
  else if (s == false) {
    latch_s = false;
  }

  

  if ((r == true) && (latch_r == false)) {
    Switch_Counter_r++;
    Switch_Timer_r = millis();
    latch_r = true;
  }
  else if (r == false) {
    latch_r = false;
  }





  if ((millis() >= Switch_Timer_d + DOUBLE_CLICK_TIME) && (d == false)) {  //Reset Count when no Button Pressed
    Switch_Counter_d = 0;
  }

  if ((millis() >= Switch_Timer_s + DOUBLE_CLICK_TIME) && (s == false)) {
    Switch_Counter_s = 0;
  }

  if ((millis() >= Switch_Timer_r + DOUBLE_CLICK_TIME) && (r == false)) {
    Switch_Counter_r = 0;
  }


  

  
  if ((d == true) && (Switch_Counter_d == 2)) {   // Mode Activation and Deactivation
    Delay_Mode = true;
  }
  else {
    Delay_Mode = false;
  }


  if ((r == true) && (Switch_Counter_r == 1)) {
    Reset_Division_Mode = true;
  }
  else {
    Reset_Division_Mode = false;
  }


  if ((s == true) && (Switch_Counter_s == 1)) {
    Subdivision_Mode = true;
  }
  else {
    Subdivision_Mode = false;
  }


  if ((r == true) && (Switch_Counter_r == 2)) {
    Reset_Parameter_Mode = true;
  }
  else {
    Reset_Parameter_Mode = false;
  }


  if ((s == true) && (Switch_Counter_s == 2)) {
    Random_Mode = true;
  }
  else {
    Random_Mode = false;
  }


  if ((d == true) && (Switch_Counter_d == 1)) {
    Swing_Mode = true;
  }
  else {
    Swing_Mode = false;
  }



  if ((d == false) && (r == false) && (s == false)) {
    Division_Mode = true;
  }
  else {
    Division_Mode = false;
  }





  if (Delay_Mode == true) {


    if ((analogRead(DELAY_1) <= Pot1_Save - Pot_thr) || (analogRead(DELAY_1) >= Pot1_Save + Pot_thr))  {

      D1 = int(map(analogRead(DELAY_1), 0+Buff, 1023-Buff, 0, DEL_RNG));  // Delay Wert Auslesen der Potentiometer
      save_timer_delay_1 = millis();
      Check_1 = true; 
      Pot1_Save = analogRead(DELAY_1);
     
    }


    if ((analogRead(DELAY_2) <= Pot2_Save - Pot_thr) || (analogRead(DELAY_2) >= Pot2_Save + Pot_thr))  {

      D2 = int(map(analogRead(DELAY_2), 0+Buff, 1023-Buff, 0, DEL_RNG));
      save_timer_delay_2 = millis();
      Check_2 = true;
      Pot2_Save = analogRead(DELAY_2);

    }


    if ((analogRead(DELAY_3) <= Pot3_Save - Pot_thr) || (analogRead(DELAY_3) >= Pot3_Save + Pot_thr))  {

      D3 = int(map(analogRead(DELAY_3), 0+Buff, 1023-Buff, 0, DEL_RNG));
      save_timer_delay_3 = millis();
      Check_3 = true;
      Pot3_Save = analogRead(DELAY_3);
      
    }


    if ((analogRead(DELAY_4) <= Pot4_Save - Pot_thr) || (analogRead(DELAY_4) >= Pot4_Save + Pot_thr))  {

      D4 = int(map(analogRead(DELAY_4), 0+Buff, 1023-Buff, 0, DEL_RNG));
      save_timer_delay_4 = millis();
      Check_4 = true;
      Pot4_Save = analogRead(DELAY_4);
      
    }
  }



    
    
  if (Swing_Mode == true) {


    if ((analogRead(DELAY_1) <= Pot1_Save - Pot_thr) || (analogRead(DELAY_1) >= Pot1_Save + Pot_thr))  {

      S1 = int(map(analogRead(DELAY_1), 0+Buff, 1023-Buff, 0, SWG_RNG));  // Delay Wert Auslesen der Potentiometer
      save_timer_swing_1 = millis();
      Check_21 = true; 
      Pot1_Save = analogRead(DELAY_1);
     
    }


    if ((analogRead(DELAY_2) <= Pot2_Save - Pot_thr) || (analogRead(DELAY_2) >= Pot2_Save + Pot_thr))  {

      S2 = int(map(analogRead(DELAY_2), 0+Buff, 1023-Buff, 0, SWG_RNG));
      save_timer_swing_2 = millis();
      Check_22 = true;
      Pot2_Save = analogRead(DELAY_2);

    }


    if ((analogRead(DELAY_3) <= Pot3_Save - Pot_thr) || (analogRead(DELAY_3) >= Pot3_Save + Pot_thr))  {

      S3 = int(map(analogRead(DELAY_3), 0+Buff, 1023-Buff, 0, SWG_RNG));
      save_timer_swing_3 = millis();
      Check_23 = true;
      Pot3_Save = analogRead(DELAY_3);
      
    }


    if ((analogRead(DELAY_4) <= Pot4_Save - Pot_thr) || (analogRead(DELAY_4) >= Pot4_Save + Pot_thr))  {

      S4 = int(map(analogRead(DELAY_4), 0+Buff, 1023-Buff, 0, SWG_RNG));
      save_timer_swing_4 = millis();
      Check_24 = true;
      Pot4_Save = analogRead(DELAY_4);
      
    }
  }







  else if (Reset_Division_Mode == true)  {

    if ((analogRead(DELAY_1) <= Pot1_Save - Pot_thr) || (analogRead(DELAY_1) >= Pot1_Save + Pot_thr))  {

      R1 = 3 * round(pow(2, int(map(analogRead(DELAY_1), 0+Buff, 1023-Buff, RES_RNG, 0)))); // Reset Wert Auslesen der Potentiometer
      save_timer_reset_1 = millis();
      Check_13 = true;
      Pot1_Save = analogRead(DELAY_1);
      
    }

    if ((analogRead(DELAY_2) <= Pot2_Save - Pot_thr) || (analogRead(DELAY_2) >= Pot2_Save + Pot_thr))  {

      R2 = 3 * round(pow(2, int(map(analogRead(DELAY_2), 0+Buff, 1023-Buff, RES_RNG, 0))));
      save_timer_reset_2 = millis();
      Check_14 = true;
      Pot2_Save = analogRead(DELAY_2);
      
    }

    if ((analogRead(DELAY_3) <= Pot3_Save - Pot_thr) || (analogRead(DELAY_3) >= Pot3_Save + Pot_thr))  {

      R3 = 3 * round(pow(2, int(map(analogRead(DELAY_3), 0+Buff, 1023-Buff, RES_RNG, 0))));
      save_timer_reset_3 = millis();
      Check_15 = true;
      Pot3_Save = analogRead(DELAY_3);
     
    }

    if ((analogRead(DELAY_4) <= Pot4_Save - Pot_thr) || (analogRead(DELAY_4) >= Pot4_Save + Pot_thr)) {

      R4 = 3 * round(pow(2, int(map(analogRead(DELAY_4), 0+Buff, 1023-Buff, RES_RNG, 0))));
      save_timer_reset_4 = millis();
      Check_16 = true;
      Pot4_Save = analogRead(DELAY_4);
     
    }

  }








  else if (Division_Mode == true)  {

    if ((analogRead(DELAY_1) <= Pot1_Save - Pot_thr) || (analogRead(DELAY_1) >= Pot1_Save + Pot_thr))  {

      V1 = 3 * round(pow(2, int(map(analogRead(DELAY_1), 0+Buff, 1023-Buff, DIV_RNG, 0)))); // Division Wert Auslesen der Potentiometer
      save_timer_division_1 = millis();
      Check_5 = true;
      Pot1_Save = analogRead(DELAY_1);
      
    }

    if ((analogRead(DELAY_2) <= Pot2_Save - Pot_thr) || (analogRead(DELAY_2) >= Pot2_Save + Pot_thr))  {

      V2 = 3 * round(pow(2, int(map(analogRead(DELAY_2), 0+Buff, 1023-Buff, DIV_RNG, 0))));
      save_timer_division_2 = millis();
      Check_6 = true;
      Pot2_Save = analogRead(DELAY_2);

    }

    if ((analogRead(DELAY_3) <= Pot3_Save - Pot_thr) || (analogRead(DELAY_3) >= Pot3_Save + Pot_thr))  {

      V3 = 3 * round(pow(2, int(map(analogRead(DELAY_3), 0+Buff, 1023-Buff, DIV_RNG, 0))));
      save_timer_division_3 = millis();
      Check_7 = true;
      Pot3_Save = analogRead(DELAY_3);

    }

    if ((analogRead(DELAY_4) <= Pot4_Save - Pot_thr) || (analogRead(DELAY_4) >= Pot4_Save + Pot_thr))  {

      V4 = 3 * round(pow(2, int(map(analogRead(DELAY_4), 0+Buff, 1023-Buff, DIV_RNG, 0))));
      save_timer_division_4 = millis();
      Check_8 = true;
      Pot4_Save = analogRead(DELAY_4);

    }




  }





  else if (Subdivision_Mode == true) {

    if ((analogRead(DELAY_1) <= Pot1_Save - Pot_thr) || (analogRead(DELAY_1) >= Pot1_Save + Pot_thr))  {

      V1 = SUBDIVISION[round(map(analogRead(DELAY_1), 0+Buff, 1023-Buff, 0, 31))]; //  Fine Division Wert Auslesen der Potentiometer
      save_timer_division_5 = millis();
      Check_9 = true;
      Pot1_Save = analogRead(DELAY_1);


    }

    if ((analogRead(DELAY_2) <= Pot2_Save - Pot_thr) || (analogRead(DELAY_2) >= Pot2_Save + Pot_thr))  {

      V2 = SUBDIVISION[round(map(analogRead(DELAY_2), 0+Buff, 1023-Buff, 0, 311))];
      save_timer_division_6 = millis();
      Check_10 = true;
      Pot2_Save = analogRead(DELAY_2);

    }

    if ((analogRead(DELAY_3) <= Pot3_Save - Pot_thr) || (analogRead(DELAY_3) >= Pot3_Save + Pot_thr))  {

      V3 = SUBDIVISION[round(map(analogRead(DELAY_3), 0+Buff, 1023-Buff, 0, 31))];
      save_timer_division_7 = millis();
      Check_11 = true;
      Pot3_Save = analogRead(DELAY_3);

    }

    if ((analogRead(DELAY_4) <= Pot4_Save - Pot_thr) || (analogRead(DELAY_4) >= Pot4_Save + Pot_thr))  {

      V4 = SUBDIVISION[round(map(analogRead(DELAY_4), 0+Buff, 1023-Buff, 0, 31))];
      save_timer_division_8 = millis();
      Check_12 = true;
      Pot4_Save = analogRead(DELAY_4);

    }

  }



  else if (Random_Mode == true) {

    if ((analogRead(DELAY_1) <= Pot1_Save - Pot_thr) || (analogRead(DELAY_1) >= Pot1_Save + Pot_thr))  {

      RND1 = map(analogRead(DELAY_1),0+Buff, 1023-Buff,1,100); //  Fine Division Wert Auslesen der Potentiometer
      save_timer_random_1 = millis();
      Check_17 = true;
      Pot1_Save = analogRead(DELAY_1);


    }

    if ((analogRead(DELAY_2) <= Pot2_Save - Pot_thr) || (analogRead(DELAY_2) >= Pot2_Save + Pot_thr))  {

      RND2 = map(analogRead(DELAY_2),0+Buff, 1023-Buff,1,100);
      save_timer_random_2 = millis();
      Check_18 = true;
      Pot2_Save = analogRead(DELAY_2);

    }

    if ((analogRead(DELAY_3) <= Pot3_Save - Pot_thr) || (analogRead(DELAY_3) >= Pot3_Save + Pot_thr))  {

      RND3 = map(analogRead(DELAY_3),0+Buff, 1023-Buff,1,100);
      save_timer_random_3 = millis();
      Check_19 = true;
      Pot3_Save = analogRead(DELAY_3);

    }

    if ((analogRead(DELAY_4) <= Pot4_Save - Pot_thr) || (analogRead(DELAY_4) >= Pot4_Save + Pot_thr))  {

      RND4 = map(analogRead(DELAY_4),0+Buff, 1023-Buff,1,100);
      save_timer_random_4 = millis();
      Check_20 = true;
      Pot4_Save = analogRead(DELAY_4);

    }

  }



  
  else if ((Reset_Parameter_Mode == true) && (Reset_Check == false)) {   //Reset All Parameters Mode (except Delays)
    Reset_Check = true;
    
    V1 = 24;   // Default Parameters to Reset to when Reset_Parameter_Mode was enabled
    V2 = 24;
    V3 = 24;
    V4 = 24;

    R1 = 96;
    R2 = 96;
    R3 = 96;
    R4 = 96;

    S1 = 0;
    S2 = 0;
    S3 = 0;
    S4 = 0;

    RND1 = 100;
    RND2 = 100;
    RND3 = 100;
    RND4 = 100;

    Pot1_Save = analogRead(DELAY_1);
    Pot2_Save = analogRead(DELAY_2);
    Pot3_Save = analogRead(DELAY_3);
    Pot4_Save = analogRead(DELAY_4);
    
    clock_count_1 = 0;
    clock_count_2 = 0;
    clock_count_3 = 0;
    clock_count_4 = 0;

    reset_count_1 = 0;
    reset_count_2 = 0;
    reset_count_3 = 0;
    reset_count_4 = 0;


    save_timer_delay_1 = millis(); 
    save_timer_delay_2 = millis();
    save_timer_delay_3 = millis();
    save_timer_delay_4 = millis();

    save_timer_division_1 = millis();
    save_timer_division_2 = millis();
    save_timer_division_3 = millis();
    save_timer_division_4 = millis();

    save_timer_reset_1 = millis();
    save_timer_reset_2 = millis();
    save_timer_reset_3 = millis();
    save_timer_reset_4 = millis();

    save_timer_random_1 = millis();
    save_timer_random_2 = millis();
    save_timer_random_3 = millis();
    save_timer_random_4 = millis();

    save_timer_swing_1 = millis();
    save_timer_swing_2 = millis();
    save_timer_swing_3 = millis();
    save_timer_swing_4 = millis();


    Check_1 = true;
    Check_2 = true;
    Check_3 = true;
    Check_4 = true;
    Check_5 = true;
    Check_6 = true;
    Check_7 = true;
    Check_8 = true;
    Check_13 = true;
    Check_14 = true;
    Check_15 = true;
    Check_16 = true;
    Check_17 = true;
    Check_18 = true;
    Check_19 = true;
    Check_20 = true;
    Check_21 = true;
    Check_22 = true;
    Check_23 = true;
    Check_24 = true;
    
    


   }

  
   
   if (Reset_Parameter_Mode == false) {
    Reset_Check = false;
   }





  if ((millis() - save_timer_delay_1 > save_time_delay) && (Check_1 == true))  {    //Save when Delay has changed and Save Time Has Expired
    EEPROM.write(4, D1);
    writetoEEPROM(16, Pot1_Save);

    Check_1 = false;
  }

  if ((millis() - save_timer_delay_2 > save_time_delay) && (Check_2 == true))  {
    EEPROM.write(5, D2);
    writetoEEPROM(18, Pot2_Save);

    Check_2 = false;
  }

  if ((millis() - save_timer_delay_3 > save_time_delay) && (Check_3 == true))  {
    EEPROM.write(6, D3);
    writetoEEPROM(20, Pot3_Save);

    Check_3 = false;
  }

  if ((millis() - save_timer_delay_4 > save_time_delay) && (Check_4 == true))  {
    EEPROM.write(7, D4);
    writetoEEPROM(22, Pot4_Save);

    Check_4 = false;
  }







  if ((millis() - save_timer_reset_1 > save_time_reset) && (Check_13 == true))  {    //Save when Reset has changed and Save Time Has Expired
    EEPROM.write(24, constrain(R1,0,255));
    writetoEEPROM(16, Pot1_Save);

    Check_13 = false;
  }

  if ((millis() - save_timer_reset_2 > save_time_reset) && (Check_14 == true))  {
    EEPROM.write(25, constrain(R2,0,255));
    writetoEEPROM(18, Pot2_Save);

    Check_14 = false;
  }

  if ((millis() - save_timer_reset_3 > save_time_reset) && (Check_15 == true))  {
    EEPROM.write(26, constrain(R3,0,255));
    writetoEEPROM(20, Pot3_Save);

    Check_15 = false;
  }

  if ((millis() - save_timer_reset_4 > save_time_reset) && (Check_16 == true))  {
    EEPROM.write(27, constrain(R4,0,255));
    writetoEEPROM(22, Pot4_Save);

    Check_16 = false;
  }









  if ((millis() - save_timer_division_1 > save_time_division) && (Check_5 == true))  {     //Save when Division has changed and Save Time Has Expired
    EEPROM.write(8, constrain(V1,0,255));
    writetoEEPROM(16, Pot1_Save);

    Check_5 = false;
  }

  if ((millis() - save_timer_division_2 > save_time_division) && (Check_6 == true))  {
    EEPROM.write(9, constrain(V2,0,255));
    writetoEEPROM(18, Pot2_Save);

    Check_6 = false;
  }

  if ((millis() - save_timer_division_3 > save_time_division) && (Check_7 == true))  {
    EEPROM.write(10, constrain(V3,0,255));
    writetoEEPROM(20, Pot3_Save);

    Check_7 = false;
  }

  if ((millis() - save_timer_division_4 > save_time_division) && (Check_8 == true))  {
    EEPROM.write(11, constrain(V4,0,255));
    writetoEEPROM(22, Pot4_Save);

    Check_8 = false;
  }




  if ((millis() - save_timer_division_5 > save_time_division) && (Check_9 == true))  {    //Save when Fine Division has changed and Save Time Has Expired
    EEPROM.write(8, constrain(V1,0,255));
    writetoEEPROM(16, Pot1_Save);

    Check_9 = false;
  }

  if ((millis() - save_timer_division_6 > save_time_division) && (Check_10 == true))  {
    EEPROM.write(9, constrain(V2,0,255));
    writetoEEPROM(18, Pot2_Save);

    Check_10 = false;
  }

  if ((millis() - save_timer_division_7 > save_time_division) && (Check_11 == true))  {
    EEPROM.write(10, constrain(V3,0,255));
    writetoEEPROM(20, Pot3_Save);
  
    Check_11 = false;
  }

  if ((millis() - save_timer_division_8 > save_time_division) && (Check_12 == true))  {
    EEPROM.write(11, constrain(V4,0,255));
    writetoEEPROM(22, Pot4_Save);
    
    Check_12 = false;
  }






   if ((millis() - save_timer_random_1 > save_time_random) && (Check_17 == true))  {    //Save when Random has changed and Save Time Has Expired
    EEPROM.write(12, RND1);
    writetoEEPROM(16, Pot1_Save);

    Check_17 = false;
  }

  if ((millis() - save_timer_random_2 > save_time_random) && (Check_18 == true))  {
    EEPROM.write(13, RND2);
    writetoEEPROM(18, Pot2_Save);

    Check_18 = false;
  }

  if ((millis() - save_timer_random_3 > save_time_random) && (Check_19 == true))  {
    EEPROM.write(14, RND3);
    writetoEEPROM(20, Pot3_Save);
  
    Check_19 = false;
  }

  if ((millis() - save_timer_random_4 > save_time_random) && (Check_20 == true))  {
    EEPROM.write(15, RND4);
    writetoEEPROM(22, Pot4_Save);
    
    Check_20 = false;
  }





   if ((millis() - save_timer_swing_1 > save_time_swing) && (Check_21 == true))  {    //Save when Swing has changed and Save Time Has Expired
    EEPROM.write(0, S1);
    writetoEEPROM(16, Pot1_Save);

    Check_21 = false;
  }
  
  if ((millis() - save_timer_swing_2 > save_time_swing) && (Check_22 == true))  {  
    EEPROM.write(1, S2);
    writetoEEPROM(18, Pot2_Save);

    Check_22 = false;
  }
  
  if ((millis() - save_timer_swing_3 > save_time_swing) && (Check_23 == true))  {    
    EEPROM.write(2, S3);
    writetoEEPROM(20, Pot3_Save);

    Check_23 = false;
  }

  if ((millis() - save_timer_swing_4 > save_time_swing) && (Check_24 == true))  {    
    EEPROM.write(3, S4);
    writetoEEPROM(22, Pot4_Save);

    Check_24 = false;
  }





  







  if ((clock_timer_1 > 0) && (millis() - clock_timer_1 > Pulse_Length + D1 + s1 )) {   // Set Master Clock States low after Pulse_Lenghts Time
    MCS_1 = false;
    clock_timer_1 = 0;
  }

  if ((clock_timer_2 > 0) && (millis() - clock_timer_2 > Pulse_Length + D2 + s2 )) {
    MCS_2 = false;
    clock_timer_2 = 0;
  }

  if ((clock_timer_3 > 0) && (millis() - clock_timer_3 > Pulse_Length + D3 + s3 )) {
    MCS_3 = false;
    clock_timer_3 = 0;
  }

  if ((clock_timer_4 > 0) && (millis() - clock_timer_4 > Pulse_Length + D4 + s4 )) {
    MCS_4 = false;
    clock_timer_4 = 0;
  }





  if ((reset_timer_1 > 0) && (millis() - reset_timer_1 > Pulse_Length_Reset + D1 + s1)) {   // Set Master Reset States low after Pulse_Lenghts Time
    MCS_5 = false;
    reset_timer_1 = 0;
  }

  if ((reset_timer_2 > 0) && (millis() - reset_timer_2 > Pulse_Length_Reset + D2 + s2)) {
    MCS_6 = false;
    reset_timer_2 = 0;
  }

  if ((reset_timer_3 > 0) && (millis() - reset_timer_3 > Pulse_Length_Reset + D3 + s3)) {
    MCS_7 = false;
    reset_timer_3 = 0;
  }

  if ((reset_timer_4 > 0) && (millis() - reset_timer_4 > Pulse_Length_Reset + D4 + s4)) {
    MCS_8 = false;
    reset_timer_4 = 0;
  }




  if ((master_timer > 0) && (millis() - master_timer > Pulse_Length + s4)) {
    MCS = false;
    master_timer = 0;
  }








  if (((MCS_1 == true) && (millis() - clock_timer_1 >= D1 + s1)) && (rndcheck1 == false)) { //Set Clockpins HIGH or LOW with Delays
    rndcheck1 = true;
    rnd1 = random(1,100);
    
    if (rnd1 <= RND1) {
       digitalWrite(CLOCK_1, HIGH);
    }
    
  }
  else if (MCS_1 == false)  {
    digitalWrite(CLOCK_1, LOW);
    rndcheck1 = false;
  }



  if (((MCS_2 == true) && (millis() - clock_timer_2 >= D2 + s2)) && (rndcheck2 == false)) {
    rndcheck2 = true;
    rnd2 = random(1,100);
    
    if (rnd2 <= RND2) {
       digitalWrite(CLOCK_2, HIGH);
    }
    
  }
  else if (MCS_2 == false) {
    digitalWrite(CLOCK_2, LOW);
    rndcheck2 = false;
  }



  if (((MCS_3 == true) && (millis() - clock_timer_3 >= D3 + s3)) && (rndcheck3 == false)) {
    rndcheck3 = true;
    rnd3 = random(1,100);
    
    if (rnd3 <= RND3) {
       digitalWrite(CLOCK_3, HIGH);
    }
    
  }
  else if (MCS_3 == false) {
    digitalWrite(CLOCK_3, LOW);
    rndcheck3 = false;
  }



  if (((MCS_4 == true) && (millis() - clock_timer_4 >= D4 + s4)) && (rndcheck4 == false)) {
    rndcheck4 = true;
    rnd4 = random(1,100);
   
    if (rnd4 <= RND4) {
       digitalWrite(CLOCK_4, HIGH);
    }
    
  }
  else if (MCS_4 == false) {
    digitalWrite(CLOCK_4, LOW);
    rndcheck4 = false;
  }








  if ((MCS_5 == true) && (millis() - reset_timer_1 >= D1 + s1)) {  //Set Reset Pins High or Low (Only used for LED but could be used to send reset triggers out through jacks!!!)
    digitalWrite(RESET_1, HIGH);
  }
  else {
    digitalWrite(RESET_1, LOW);
  }



  if ((MCS_6 == true) && (millis() - reset_timer_2 >= D2 + s2)) {
    digitalWrite(RESET_2, HIGH);
  }
  else {
    digitalWrite(RESET_2, LOW);
  }



  if ((MCS_7 == true) && (millis() - reset_timer_3 >= D3 + s3)) {
    digitalWrite(RESET_3, HIGH);
  }
  else {
    digitalWrite(RESET_3, LOW);
  }



  if ((MCS_8 == true) && (millis() - reset_timer_4 >= D4 + s4)) {
    digitalWrite(RESET_4, HIGH);
  }
  else {
    digitalWrite(RESET_4, LOW);
  }










  //MIDI EVENT


  midiEventPacket_t rx = MidiUSB.read();  // listen for new MIDI messages
  switch (rx.byte1) {

    case 0xF8:      //Clock



      if (millis() > clock_timeout_1 + 300) clock_count_1 = 0; // Prevents Clock from starting in between quarter notes after clock is restarted!
      clock_timeout_1 = millis();

      if (millis() > clock_timeout_2 + 300) clock_count_2 = 0;
      clock_timeout_2 = millis();

      if (millis() > clock_timeout_3 + 300) clock_count_3 = 0;
      clock_timeout_3 = millis();

      if (millis() > clock_timeout_4 + 300) clock_count_4 = 0;
      clock_timeout_4 = millis();


      if (millis() > reset_timeout_1 + 300) reset_count_1 = 0; // Prevents Reset from starting in between quarter notes after clock is restarted!
      reset_timeout_1 = millis();

      if (millis() > reset_timeout_2 + 300) reset_count_2 = 0;
      reset_timeout_2 = millis();

      if (millis() > reset_timeout_3 + 300) reset_count_3 = 0;
      reset_timeout_3 = millis();

      if (millis() > reset_timeout_4 + 300) reset_count_4 = 0;
      reset_timeout_4 = millis();

  

      if (millis() > master_timeout + 300) master_count = 0;
      master_timeout = millis();








      if (clock_count_1 == 0) {   //Clock Counter gets pushed +1 and Master Clock State gets enabled
        MCS_1 = true;
        clock_timer_1 = millis();
      }
      clock_count_1++;

      if (clock_count_2 == 0) {
        MCS_2 = true;
        clock_timer_2 = millis();
      }
      clock_count_2++;

      if (clock_count_3 == 0) {
        MCS_3 = true;
        clock_timer_3 = millis();
      }
      clock_count_3++;

      if (clock_count_4 == 0) {
        MCS_4 = true;
        clock_timer_4 = millis();
      }
      clock_count_4++;



      if (reset_count_1 == 0) {   //Clock Counter gets pushed +1 and Master Reset State gets enabled
        MCS_5 = true;
        reset_timer_1 = millis();
      }
      reset_count_1++;

      if (reset_count_2 == 0) {
        MCS_6 = true;
        reset_timer_2 = millis();
      }
      reset_count_2++;

      if (reset_count_3 == 0) {
        MCS_7 = true;
        reset_timer_3 = millis();
      }
      reset_count_3++;

      if (reset_count_4 == 0) {
        MCS_8 = true;
        reset_timer_4 = millis();
      }
      reset_count_4++;



      if (master_count == 0) {
        MCS = true;
        master_timer = millis();
      }
      master_count++;









      if ((clock_count_1 >= V1) && (V1 <= 254)) {  // Reset Clock Counter through Divisions:  MIDI timing clock sends 24 pulses per quarter note.  Send pulse only once every 12 pulses
        clock_count_1 = 0;
      }

      if ((clock_count_2 >= V2) && (V2 <= 254)) {
        clock_count_2 = 0;
      }

      if ((clock_count_3 >= V3) && (V3 <= 254)) {
        clock_count_3 = 0;
      }

      if ((clock_count_4 >= V4) && (V4 <= 254)) {
        clock_count_4 = 0;
      }




      if ((reset_count_1 >= R1) && (R1 <= 254)) {  // Reset Clock Counter through Divisions
        reset_count_1 = 0; 
        clock_count_1 = 0;
      }

      if ((reset_count_2 >= R2) && (R2 <= 254)) {
        reset_count_2 = 0;
        clock_count_2 = 0;
      }

      if ((reset_count_3 >= R3) && (R3 <= 254)) {
        reset_count_3 = 0;
        clock_count_3 = 0;
      }

      if ((reset_count_4 >= R4) && (R4 <= 254)) {
        reset_count_4 = 0;
        clock_count_4 = 0;
      }




      if (master_count >= 12) {
        master_count = 0; 
      }







      break;
  }


}



//Functions


void writetoEEPROM(int address, int number)  //Write Int to EEPROM
{ 
  EEPROM.write(address, number >> 8);
  EEPROM.write(address + 1, number & 0xFF);
}
