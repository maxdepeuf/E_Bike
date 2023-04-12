#include "VescUart.h"
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

VescUart UART;

LCDWIKI_KBV mylcd(ILI9486,A3,A2,A1,A0,A4);
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int state_Acc_Brake = 0;
int state_Reg = 0;
int state_Light = 0;
int Pot_Value = 0;
int Pot_Value_F = 0;
int gaz;
int Gaz_Pin = 9;
int Gaz_Pin2 = 6;
int State_bat = 7;
long t_co;
long t_f;
char inChar = 0;
String stringData;
bool newline = false;
String data_in = "";
String data_in2 = "";
String command = "";
String data_out = "";
boolean newData = false;
int state_uart;
float speed_reg;

long speed_uart;
float volt_uart;
float intmot_uart;
double vmin_batt = 33.6;
double vmax_batt = 42.0;
float Pot_Threshold = 10;

float Acc_Current;
float Acc_Current_F;
float Acc_Current_Max = 50;
float Brake_Current_Max = 30;
const int numReadings = 10;
long t;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;
float r;
const int RunningAverageCount = 10;
float RunningAverageBuffer[RunningAverageCount];
int NextRunningAverage;
const byte bat_led[] = {45, 47, 49, 51, 53};
int i;
long t_led_bat = 0;
int nb;
void setup() {
  Serial.begin(9600);//Serial
  //Serial1.begin(38400);//Ble
 // Serial2.begin(38400);//µSkate
  Serial1.begin(115200);//µSkate
  while (!Serial) {
    ;
  }
  UART.setSerialPort(&Serial1);


}

void loop() {

  if ( UART.getVescValues() ) {
    volt_uart = ((UART.data.inpVoltage));
  }
  Serial.println(volt_uart);

}
