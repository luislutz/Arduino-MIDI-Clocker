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




#define DELAY 8
#define SUBDIV 9
#define RESET 10

#define DEL_RNG 50   // Define Delay Range in ms

#define DIV_RNG 7   // Define Clock Division Range: 0 = 2Pulse/1Sixteenth1 = 1 Pulse/Sixteenth, 2 = 1Pulse/Eights, 3 = 1 Pulse/1Quater, 4 = 1Pulse/2Quater, 5 = 1Pulse/1Bar, 6 = 1Pulse/2Bars, 7 = None

#define RES_RNG 7   // Define Reset Division Rannge: 0 = 2Pulse/1Sixteenth1 = 1 Pulse/Sixteenth, 2 = 1Pulse/Eights, 3 = 1 Pulse/1Quater, 4 = 1Pulse/2Quater, 5 = 1Pulse/1Bar, 6 = 1Pulse/2Bars, 7 = None

int SUBDIVISION[] = {255,255,255,255,240,216,192,168,144,120,96,72,60,54,48,42,30,27,24,21,18,15,12,9,8,7,6,5,4,3,3,3};  // Mapping Array for Fine Division

#define save_time_delay 30000       //  Save timer to Spare on EEPROM Cycles 
#define save_time_division 30000
#define save_time_reset 30000

#define Pot_thr 63  // Resolution for pot movement detection (Range: ~5 - 255)

#define Pulse_Length 20    // Output Pulse Duration in ms





int D1 = EEPROM.read(4);     //Read Stored Values only once when powered up
int D2 = EEPROM.read(5);
int D3 = EEPROM.read(6);
int D4 = EEPROM.read(7);

int D1_Save = EEPROM.read(16);
int D2_Save = EEPROM.read(17);
int D3_Save = EEPROM.read(18);
int D4_Save = EEPROM.read(19);


int V1 = EEPROM.read(8);
int V2 = EEPROM.read(9);
int V3 = EEPROM.read(10);
int V4 = EEPROM.read(11);

int V1_Save = EEPROM.read(12);
int V2_Save = EEPROM.read(13);
int V3_Save = EEPROM.read(14);
int V4_Save = EEPROM.read(15);

int V1_Save_FINE = EEPROM.read(20);
int V2_Save_FINE = EEPROM.read(21);
int V3_Save_FINE = EEPROM.read(22);
int V4_Save_FINE = EEPROM.read(23);

int R1 = EEPROM.read(24);
int R2 = EEPROM.read(25);
int R3 = EEPROM.read(26);
int R4 = EEPROM.read(27);

int R1_Save = EEPROM.read(28);
int R2_Save = EEPROM.read(29);
int R3_Save = EEPROM.read(30);
int R4_Save = EEPROM.read(31);








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

  pinMode(DELAY, INPUT_PULLUP);
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




  static unsigned int clock_count_1 = 0; //Clock Counters
  static unsigned int clock_count_2 = 0;
  static unsigned int clock_count_3 = 0;
  static unsigned int clock_count_4 = 0;

  static unsigned int reset_count_1 = 0; //Reset Counters
  static unsigned int reset_count_2 = 0;
  static unsigned int reset_count_3 = 0;
  static unsigned int reset_count_4 = 0;

  static bool MCS_1 = false; // Master Clock States  (undelayed Clock State)
  static bool MCS_2 = false;
  static bool MCS_3 = false;
  static bool MCS_4 = false;

  static bool MCS_5 = false; // Master Reset States (undelayed Reset State)
  static bool MCS_6 = false;
  static bool MCS_7 = false;
  static bool MCS_8 = false;


  static bool Check_1 = false;  // Check bools for Save time delay
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







  bool D = !digitalRead(DELAY);  //Switches Read in
  bool S = !digitalRead(SUBDIV);
  bool R = !digitalRead(RESET);




  if (D == true) {


    if (D1_Save != map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr))  {

      D1 = int(map(analogRead(DELAY_1), 0, 1023, 0, DEL_RNG));  // Delay Wert Auslesen der Potentiometer
      save_timer_delay_1 = millis();
      Check_1 = true;
      V1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      V1_Save_FINE = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      R1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
    }


    if (D2_Save != map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr))  {

      D2 = int(map(analogRead(DELAY_2), 0, 1023, 0, DEL_RNG));
      save_timer_delay_2 = millis();
      Check_2 = true;
      V2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      V2_Save_FINE = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      R2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);

    }


    if (D3_Save != map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr))  {

      D3 = int(map(analogRead(DELAY_3), 0, 1023, 0, DEL_RNG));
      save_timer_delay_3 = millis();
      Check_3 = true;
      V3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      V3_Save_FINE = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      R3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
    }


    if (D4_Save != map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr))  {

      D4 = int(map(analogRead(DELAY_4), 0, 1023, 0, DEL_RNG));
      save_timer_delay_4 = millis();
      Check_4 = true;
      V4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      V4_Save_FINE = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      R4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
    }
  }







  else if (R == true)  {

    if (R1_Save != map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr))  {

      R1 = 3 * round(pow(2, int(map(analogRead(DELAY_1), 0, 1023, RES_RNG, 0)))); // Reset Wert Auslesen der Potentiometer
      save_timer_reset_1 = millis();
      Check_13 = true;
      V1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      V1_Save_FINE = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      D1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
    }

    if (R2_Save != map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr))  {

      R2 = 3 * round(pow(2, int(map(analogRead(DELAY_2), 0, 1023, RES_RNG, 0))));
      save_timer_reset_2 = millis();
      Check_14 = true;
      V2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      V2_Save_FINE = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      D2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
    }

    if (R3_Save != map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr))  {

      R3 = 3 * round(pow(2, int(map(analogRead(DELAY_3), 0, 1023, RES_RNG, 0))));
      save_timer_reset_3 = millis();
      Check_15 = true;
      V3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      V3_Save_FINE = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      D3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
    }

    if (R4_Save != map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr)) {

      R4 = 3 * round(pow(2, int(map(analogRead(DELAY_4), 0, 1023, RES_RNG, 0))));
      save_timer_reset_4 = millis();
      Check_16 = true;
      V4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      V4_Save_FINE = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      D4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
    }

  }








  else if (S == false)  {

    if (V1_Save != map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr))  {

      V1 = 3 * round(pow(2, int(map(analogRead(DELAY_1), 0, 1023, DIV_RNG, 0)))); // Division Wert Auslesen der Potentiometer
      save_timer_division_1 = millis();
      Check_5 = true;
      D1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      V1_Save_FINE = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      R1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
    }

    if (V2_Save != map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr))  {

      V2 = 3 * round(pow(2, int(map(analogRead(DELAY_2), 0, 1023, DIV_RNG, 0))));
      save_timer_division_2 = millis();
      Check_6 = true;
      D2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      V2_Save_FINE = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      R2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
    }

    if (V3_Save != map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr))  {

      V3 = 3 * round(pow(2, int(map(analogRead(DELAY_3), 0, 1023, DIV_RNG, 0))));
      save_timer_division_3 = millis();
      Check_7 = true;
      D3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      V3_Save_FINE = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      R3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
    }

    if (V4_Save != map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr))  {

      V4 = 3 * round(pow(2, int(map(analogRead(DELAY_4), 0, 1023, DIV_RNG, 0))));
      save_timer_division_4 = millis();
      Check_8 = true;
      D4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      V4_Save_FINE = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      R4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);

    }




  }





  else  {

    if (V1_Save_FINE != map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr))  {

      V1 = SUBDIVISION[round(analogRead(DELAY_1)/32)]; //  Fine Division Wert Auslesen der Potentiometer
      save_timer_division_5 = millis();
      Check_9 = true;
      D1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      V1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);
      R1_Save = map(analogRead(DELAY_1), 0, 1023, 0, Pot_thr);

    }

    if (V2_Save_FINE != map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr))  {

      V2 = SUBDIVISION[round(analogRead(DELAY_2)/32)];
      save_timer_division_6 = millis();
      Check_10 = true;
      D2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      V2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
      R2_Save = map(analogRead(DELAY_2), 0, 1023, 0, Pot_thr);
    }

    if (V3_Save_FINE != map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr))  {

      V3 = SUBDIVISION[round(analogRead(DELAY_3)/32)];
      save_timer_division_7 = millis();
      Check_11 = true;
      D3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      V3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
      R3_Save = map(analogRead(DELAY_3), 0, 1023, 0, Pot_thr);
    }

    if (V4_Save_FINE != map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr))  {

      V4 = SUBDIVISION[round(analogRead(DELAY_4)/32)];
      save_timer_division_8 = millis();
      Check_12 = true;
      D4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      V4_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
      R1_Save = map(analogRead(DELAY_4), 0, 1023, 0, Pot_thr);
    }




  }



  if ((millis() - save_timer_delay_1 > save_time_delay) && (Check_1 == true))  {    //Save when Delay has changed and Save Time Has Expired
    EEPROM.write(4, D1);
    EEPROM.write(12, V1_Save);
    EEPROM.write(20, V1_Save_FINE);
    EEPROM.write(28, R1_Save);
    Check_1 = false;
  }

  if ((millis() - save_timer_delay_2 > save_time_delay) && (Check_2 == true))  {
    EEPROM.write(5, D2);
    EEPROM.write(13, V2_Save);
    EEPROM.write(21, V2_Save_FINE);
    EEPROM.write(29, R2_Save);
    Check_2 = false;
  }

  if ((millis() - save_timer_delay_3 > save_time_delay) && (Check_3 == true))  {
    EEPROM.write(6, D3);
    EEPROM.write(14, V3_Save);
    EEPROM.write(22, V3_Save_FINE);
    EEPROM.write(30, R3_Save);
    Check_3 = false;
  }

  if ((millis() - save_timer_delay_4 > save_time_delay) && (Check_4 == true))  {
    EEPROM.write(7, D4);
    EEPROM.write(15, V4_Save);
    EEPROM.write(23, V4_Save_FINE);
    EEPROM.write(31, R4_Save);
    Check_4 = false;
  }







  if ((millis() - save_timer_reset_1 > save_time_reset) && (Check_13 == true))  {    //Save when Reset has changed and Save Time Has Expired
    EEPROM.write(24, constrain(R1,0,255));
    EEPROM.write(12, V1_Save);
    EEPROM.write(20, V1_Save_FINE);
    EEPROM.write(16, D1_Save);
    Check_13 = false;
    Serial.println(R1);
  }

  if ((millis() - save_timer_reset_2 > save_time_reset) && (Check_14 == true))  {
    EEPROM.write(25, constrain(R2,0,255));
    EEPROM.write(13, V2_Save);
    EEPROM.write(21, V2_Save_FINE);
    EEPROM.write(17, D2_Save);
    Check_14 = false;
  }

  if ((millis() - save_timer_reset_3 > save_time_reset) && (Check_15 == true))  {
    EEPROM.write(26, constrain(R3,0,255));
    EEPROM.write(14, V3_Save);
    EEPROM.write(22, V3_Save_FINE);
    EEPROM.write(18, D3_Save);
    Check_15 = false;
  }

  if ((millis() - save_timer_reset_4 > save_time_reset) && (Check_16 == true))  {
    EEPROM.write(27, constrain(R4,0,255));
    EEPROM.write(15, V4_Save);
    EEPROM.write(23, V4_Save_FINE);
    EEPROM.write(19, D4_Save);
    Check_16 = false;
  }









  if ((millis() - save_timer_division_1 > save_time_division) && (Check_5 == true))  {     //Save when Division has changed and Save Time Has Expired
    EEPROM.write(8, constrain(V1,0,255));
    EEPROM.write(16, D1_Save);
    EEPROM.write(20, V1_Save_FINE);
    EEPROM.write(28, R1_Save);
    Check_5 = false;
  }

  if ((millis() - save_timer_division_2 > save_time_division) && (Check_6 == true))  {
    EEPROM.write(9, constrain(V2,0,255));
    EEPROM.write(17, D2_Save);
    EEPROM.write(21, V2_Save_FINE);
    EEPROM.write(29, R2_Save);
    Check_6 = false;
  }

  if ((millis() - save_timer_division_3 > save_time_division) && (Check_7 == true))  {
    EEPROM.write(10, constrain(V3,0,255));
    EEPROM.write(18, D3_Save);
    EEPROM.write(22, V3_Save_FINE);
    EEPROM.write(30, R3_Save);
    Check_7 = false;
  }

  if ((millis() - save_timer_division_4 > save_time_division) && (Check_8 == true))  {
    EEPROM.write(11, constrain(V4,0,255));
    EEPROM.write(19, D4_Save);
    EEPROM.write(23, V4_Save_FINE);
    EEPROM.write(31, R4_Save);
    Check_8 = false;
  }




  if ((millis() - save_timer_division_5 > save_time_division) && (Check_9 == true))  {    //Save when Fine Division has changed and Save Time Has Expired
    EEPROM.write(8, constrain(V1,0,255));
    EEPROM.write(16, D1_Save);
    EEPROM.write(12, V1_Save);
    EEPROM.write(28, R1_Save);
    Check_9 = false;
  }

  if ((millis() - save_timer_division_6 > save_time_division) && (Check_10 == true))  {
    EEPROM.write(9, constrain(V2,0,255));
    EEPROM.write(17, D2_Save);
    EEPROM.write(13, V2_Save);
    EEPROM.write(29, R2_Save);
    Check_10 = false;
  }

  if ((millis() - save_timer_division_7 > save_time_division) && (Check_11 == true))  {
    EEPROM.write(10, constrain(V3,0,255));
    EEPROM.write(18, D3_Save);
    EEPROM.write(14, V3_Save);
    EEPROM.write(30, R3_Save);
    Check_11 = false;
  }

  if ((millis() - save_timer_division_8 > save_time_division) && (Check_12 == true))  {
    EEPROM.write(11, constrain(V4,0,255));
    EEPROM.write(19, D4_Save);
    EEPROM.write(15, V4_Save);
    EEPROM.write(31, R4_Save);
    Check_12 = false;
  }










  if ((clock_timer_1 > 0) && (millis() - clock_timer_1 > Pulse_Length + D1)) {   // Set Master Clock States low after Pulse_Lenghts Time
    MCS_1 = false;
    clock_timer_1 = 0;
  }

  if ((clock_timer_2 > 0) && (millis() - clock_timer_2 > Pulse_Length + D2)) {
    MCS_2 = false;
    clock_timer_2 = 0;
  }

  if ((clock_timer_3 > 0) && (millis() - clock_timer_3 > Pulse_Length + D3)) {
    MCS_3 = false;
    clock_timer_3 = 0;
  }

  if ((clock_timer_4 > 0) && (millis() - clock_timer_4 > Pulse_Length + D4)) {
    MCS_4 = false;
    clock_timer_4 = 0;
  }





  if ((reset_timer_1 > 0) && (millis() - reset_timer_1 > Pulse_Length + D1)) {   // Set Master Reset States low after Pulse_Lenghts Time
    MCS_5 = false;
    reset_timer_1 = 0;
  }

  if ((reset_timer_2 > 0) && (millis() - reset_timer_2 > Pulse_Length + D2)) {
    MCS_6 = false;
    reset_timer_2 = 0;
  }

  if ((reset_timer_3 > 0) && (millis() - reset_timer_3 > Pulse_Length + D3)) {
    MCS_7 = false;
    reset_timer_3 = 0;
  }

  if ((reset_timer_4 > 0) && (millis() - reset_timer_4 > Pulse_Length + D4)) {
    MCS_8 = false;
    reset_timer_4 = 0;
  }






  if ((MCS_1 == true) && (millis() - clock_timer_1 >= D1)) { //Set Clockpins HIGH or LOW with Delays
    digitalWrite(CLOCK_1, HIGH);
  }
  else {
    digitalWrite(CLOCK_1, LOW);
  }



  if ((MCS_2 == true) && (millis() - clock_timer_2 >= D2)) {
    digitalWrite(CLOCK_2, HIGH);
  }
  else {
    digitalWrite(CLOCK_2, LOW);
  }



  if ((MCS_3 == true) && (millis() - clock_timer_3 >= D3)) {
    digitalWrite(CLOCK_3, HIGH);
  }
  else {
    digitalWrite(CLOCK_3, LOW);
  }



  if ((MCS_4 == true) && (millis() - clock_timer_4 >= D4)) {
    digitalWrite(CLOCK_4, HIGH);
  }
  else {
    digitalWrite(CLOCK_4, LOW);
  }









  if ((MCS_5 == true) && (millis() - reset_timer_1 >= D1)) {  //Set Reset Pins High or Low (Only used for LED but could be used to send reset triggers out through jacks!!!)
    digitalWrite(RESET_1, HIGH);
  }
  else {
    digitalWrite(RESET_1, LOW);
  }



  if ((MCS_6 == true) && (millis() - reset_timer_2 >= D2)) {
    digitalWrite(RESET_2, HIGH);
  }
  else {
    digitalWrite(RESET_2, LOW);
  }



  if ((MCS_7 == true) && (millis() - reset_timer_3 >= D3)) {
    digitalWrite(RESET_3, HIGH);
  }
  else {
    digitalWrite(RESET_3, LOW);
  }



  if ((MCS_8 == true) && (millis() - reset_timer_4 >= D4)) {
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



      break;
  }


}
