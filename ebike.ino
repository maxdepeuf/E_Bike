#include "VescUart.h"
#include "font_pic.h"
#include <TouchScreen.h> //touch library
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
LCDWIKI_KBV mylcd(ILI9486, A3, A2, A1, A0, A4); //model,cs,cd,wr,rd,reset
//define some colour values
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 906
#define TS_MAXX 116

#define TS_MINY 92
#define TS_MAXY 952
// We have a status line for like, is FONA working
#define STATUS_X 10
#define STATUS_Y 65


boolean switch_flag_1 = true, switch_flag_2 = true, switch_flag_3 = true, switch_flag_4 = true, switch_flag_5 = true, switch_flag_6 = true;
int16_t menu_flag = 1, old_menu_flag;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

/** Initiate VescUart class */
VescUart UART;
int k;
int vitesse;
int vitesse_last;
int batterie;
int batterie_last;
int ia;
int ie;
int ja;
int je;
int x;
long t_vitesse;
long t_batterie;
long t = millis();
long t_chrono;
long t_chrono_s;
long t_chrono_m;
long t_chrono_h;

      int x_batt = 200;
      int y_batt = 40;
      int xpos_batt = 110;
      int ypos_batt = 305;
      int edge_batt = 3;
      int pol_batt = 3;
void setup() {
  // put your setup code here, to run once:
  /** Setup Serial port to display data */
  Serial.begin(9600);
  mylcd.Init_LCD();
  // mylcd.Set_Rotation(2);
  mylcd.Fill_Screen(BLACK);
  //    mylcd.Set_Draw_color(WHITE);
  //    for(int i = 1; i<480;i=i+10)
  //    {
  //          mylcd.Set_Draw_color(WHITE);
  //       mylcd.Draw_Line(0, i, 160, i+90);
  //       mylcd.Draw_Line(320, i, 160, i+90);
  //           mylcd.Set_Draw_color(BLACK);
  //              mylcd.Draw_Line(0, i, 160, i+90);
  //       mylcd.Draw_Line(320, i, 160, i+90);
  //
  //     }

  //  mylcd.Set_Rotation(2);
  //  mylcd.Fill_Screen(WHITE);
  //  mylcd.Set_Text_Back_colour(WHITE);
  //  mylcd.Set_Text_colour(BLACK);
  //  mylcd.Set_Text_Size(2);
  //  mylcd.Print_String("Battery Voltage: ", 0 , 200);
  //  mylcd.Print_String("Battery Current: ", 0 , 220);
  //  mylcd.Print_String("Motor Current:   ", 0 , 240);
  //  mylcd.Print_String("Capacity:        ", 0 , 260);
  //  mylcd.Print_String("Capacity Charge: ", 0 , 280);
  //  mylcd.Print_String("Duty Cycle:      ", 0 , 300);
  //  mylcd.Print_String("RPM:             ", 0 , 320);
  //  mylcd.Print_String("Tachometer:      ", 0 , 340);
  //  mylcd.Print_String("Tachometer Abs:  ", 0 , 360);
  //
  //  mylcd.Set_Text_Size(2);
  /** Setup UART port (Serial1 on Atmega32u4) */
  Serial1.begin(115200);

  while (!Serial) {
    ;
  }

  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);
  //show_picture(const uint8_t *color_buf,int16_t buf_size,int16_t x1,int16_t y1,int16_t x2,int16_t y2)




  temp_ind(15, 303, 10, 44, 1);
  temp_ind(36, 303, 10, 44, 1);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Print_String("R", 17, 288);
  mylcd.Print_String("M", 38, 288);
  mylcd.Set_Text_Size(1);


  //temp_ind(10, 380, 20, 50, 2);
  for (int x = 0; x <= 255; x = x + 20) {
    mylcd.Set_Draw_color(x, 0, 255 - x);
    //mylcd.Draw_Fast_HLine(map((x / 2), 0, 511, 0, x_batt) + xpos_batt, ypos_batt, y_batt);
    /// mylcd.Draw_Fast_HLine(xpos_batt, map((x), 0, 255, 0, y_batt - 1) + ypos_batt, x_batt);
    mylcd.Fill_Rectangle(15, 348, 15 + 9, 347 - map((x), 0, 255, 0, 44));
    mylcd.Print_String(String(x, DEC), 11, 355);
  }

  for (int x = 0; x <= 255; x = x + 20) {
    mylcd.Set_Draw_color(x, 0, 255 - x);
    //mylcd.Draw_Fast_HLine(map((x / 2), 0, 511, 0, x_batt) + xpos_batt, ypos_batt, y_batt);
    /// mylcd.Draw_Fast_HLine(xpos_batt, map((x), 0, 255, 0, y_batt - 1) + ypos_batt, x_batt);
    mylcd.Fill_Rectangle(36, 348, 36 + 9, 347 - map((x), 0, 255, 0, 44));
    mylcd.Print_String(String(x, DEC), 11, 355);
    mylcd.Print_String(String(x, DEC), 32, 355);
  }

#define MINPRESSURE 10
#define MAXPRESSURE 1000

  mylcd.Set_Text_Size(2);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Draw_color(WHITE);
  mylcd.Fill_Circle(160, 140, 10);

  for (int i = 210; i >= -30; i = i - 40) {
    if (i <= 50) {
      mylcd.Set_Draw_color(RED);
      mylcd.Set_Text_colour(RED);
    } else {
      mylcd.Set_Draw_color(WHITE);
      mylcd.Set_Text_colour(WHITE);
    }
    mylcd.Print_String(String(k, DEC), 149. + (140)*cos((i * PI) / 180), 140 - (140)*sin((i * PI) / 180));
    k = k + 10;
    for (int j = -4; j <= 4; j++) {
      for (int k = 0; k <= 20; k++) {
        mylcd.Draw_Pixel(160. + (120. - k)*cos((j + i * PI) / 180.), 140. - (120. - k)*sin((j + i * PI) / 180.));
      }
    }
    if (i > -30) {
      for (int g = 0; g <= 40; g = g + 10) {
        for (int j = -2; j <= 2; j++) {
          for (int k = 0; k <= 10; k++) {
            mylcd.Draw_Pixel(160. + (120. - k)*cos((j + (i - g) * PI) / 180.), 140. - (120. - k)*sin((j + (i - g) * PI) / 180.));
          }
        }
      }
    }

  }


  for (int i = 210; i >= -30; i = i - 10) {
    mylcd.Set_Draw_color(RED);
    mylcd.Fill_Triangle( 160 + 95 * cos((i * PI) / 180), 140 - 95 * sin((i * PI) / 180),
                         160 + 15 * cos(((i * PI) / 180) - 0.18), 140 - 15 * sin(((i * PI) / 180) - 0.18),
                         160 + 15 * cos(((i * PI) / 180) + 0.18), 140 - 15 * sin(((i * PI) / 180) + 0.18));
    delay(1);
    mylcd.Set_Draw_color(BLACK);
    mylcd.Fill_Triangle( 160 + 95 * cos((i * PI) / 180), 140 - 95 * sin((i * PI) / 180),
                         160 + 15 * cos(((i * PI) / 180) - 0.18), 140 - 15 * sin(((i * PI) / 180) - 0.18),
                         160 + 15 * cos(((i * PI) / 180) + 0.18), 140 - 15 * sin(((i * PI) / 180) + 0.18));
  }

  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(5);
  // mylcd.Print_String("Km/h", 130,185);
  mylcd.Set_Text_Back_colour(BLACK);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Print_String(String(30, DEC), 134, 170); //105,170);135, 170);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("km/h", 128, 212);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String("09:20", 255, 6);
  // mylcd.Print_String(String(__DATE__), 65, 290);
  //mylcd.Print_String(String(__TIME__), 97, 310);


  mylcd.Set_Text_Size(10);

  mylcd.Set_Text_Size(10);
  mylcd.Set_Draw_color(RED);
  //for (int x = 0; x <= 60; x=x+2) {
  //int  i=map(x,0,60,210,-30);
  //  mylcd.Set_Draw_color(RED);
  //  mylcd.Fill_Triangle( 160 + 95 * cos((i * PI) / 180), 140 - 95 * sin((i * PI) / 180),
  //                       160 + 15 * cos(((i * PI) / 180) - 0.4), 140 - 15 * sin(((i * PI) / 180) - 0.4),
  //                       160 + 15 * cos(((i * PI) / 180) + 0.4), 140 - 15 * sin(((i * PI) / 180) + 0.4));
  // mylcd.Print_String(String(x, DEC), 105, 170); //105,170);
  // delay(100);
  //   mylcd.Set_Draw_color(BLACK);
  //  mylcd.Fill_Triangle( 160 + 95 * cos((i * PI) / 180), 140 - 95 * sin((i * PI) / 180),
  //                       160 + 15 * cos(((i * PI) / 180) - 0.4), 140 - 15 * sin(((i * PI) / 180) - 0.4),
  //                       160 + 15 * cos(((i * PI) / 180) + 0.4), 140 - 15 * sin(((i * PI) / 180) + 0.4));
  //}


  // //mylcd.Draw_Rectangle(10,300,300,350);
  //// for (int x = 0; x <= 255; x++) {
  //  //mylcd.Set_Draw_color(x ,0,255-x);
  //  int x=50;
  // mylcd.Set_Draw_color(255-x ,x,0);
  // mylcd.Draw_Fast_VLine(x,320,50);
  // //mylcd.Set_Draw_color(0 ,0,255);
  // //for (int y=0;y<=255-x;y++){
  // //mylcd.Draw_Fast_VLine(x+y,320,50);
  // //}
  // delay(10);
  // //}
  int x_batt = 200;
  int y_batt = 40;
  int xpos_batt = 110;
  int ypos_batt = 305;
  int edge_batt = 3;
  int pol_batt = 3;
  mylcd.Set_Text_Size(pol_batt);
  mylcd.Set_Draw_color(WHITE);
  for (int x = 1; x <= edge_batt; x = x + 1) {
    mylcd.Draw_Rectangle(xpos_batt - 2 - x, ypos_batt - 2 - x, (x_batt / 2) + xpos_batt + 1 + x, y_batt + ypos_batt + 1 + x);
  }
  mylcd.Fill_Rectangle((x_batt / 2) + edge_batt + xpos_batt, ypos_batt + y_batt / 10, (x_batt / 2) + edge_batt + xpos_batt  + 3 * (edge_batt), ypos_batt + 9 * y_batt / 10);


  for (int x = 0; x <= 511; x = x + 10) {
    if (x <= 255) {
      mylcd.Set_Draw_color(255, x, 0);
      mylcd.Set_Text_colour(255, x, 0);
    } else {
      mylcd.Set_Draw_color(255 - (x - 256), 255, 0);
      mylcd.Set_Text_colour(255 - (x - 256), 255, 0);
    }
    //mylcd.Draw_Fast_VLine(map((x / 2), 0, 511, 0, x_batt) + xpos_batt, ypos_batt, y_batt);
    mylcd.Fill_Rectangle(xpos_batt, ypos_batt , xpos_batt + map((x / 2), 0, 511, 0, x_batt), ypos_batt + y_batt - 1);

    // mylcd.Fill_Rectangle(xpos_batt+x_batt/5-4, ypos_batt+y_batt/4-4,xpos_batt+x_batt/4+pol_batt*18, ypos_batt+y_batt/4+pol_batt*8);
    if (map(x, 0, 511, 0, 100) >= 100) {
      mylcd.Print_String("FULL", 133, 255);
    } else {
      mylcd.Print_String(String(map(x, 0, 511, 0, 100)) + "%", 133, 255);
    }
  }
  //
  //
  //
  //
  //  temp_ind(10, 380, 20, 50, 2);
  //  //void temp_ind(int xpos_batt,int ypos_batt,int x_batt,int y_batt,int edge_batt){
  t = millis();

  mylcd.Set_Text_Size(3);
  mylcd.Set_Text_colour(WHITE);

  show_picture(lamp_off, sizeof(lamp_off) / 2, 1, 1, 1 + 41, 1 + 49);
  //show_picture(night, sizeof(night) / 2, 270, 1, 270 + 49, 1 + 49);
  show_picture(warning, sizeof(warning) / 2, 210, 215, 210 + 49, 215 + 49);
  show_picture(time_off, sizeof(time_off) / 2, 1, 430, 1 + 49, 430 + 49);
  show_picture(temp, sizeof(temp) / 2, 6, 233, 6 + 49, 233 + 49);
  show_picture(fan_1, sizeof(fan_1) / 2, 53, 300, 53 + 49, 300 + 49);
  show_picture(fan_2, sizeof(fan_2) / 2, 53, 300, 53 + 49, 300 + 49);
  show_picture(slope_off, sizeof(slope_off) / 2, 270, 430, 270 + 49, 430 + 49);
  show_picture(slope_p, sizeof(slope_p) / 2, 270, 430, 270 + 49, 430 + 49);
  show_picture(brake, sizeof(brake) / 2, 68, 220, 68 + 49, 220 + 49);
  show_picture(ebrake, sizeof(ebrake) / 2, 230, 300, 230 + 49, 300 + 49);
  //show_picture(econ, sizeof(econ) / 2, 270, 290, 270 + 49, 290 + 49);
  show_picture(dist, sizeof(dist) / 2, 1, 378, 1 + 49, 378 + 49);
  delay(500);
  mylcd.Set_Draw_color(BLACK);
//  mylcd.Fill_Rectangle(53, 300, 102, 349);
//  mylcd.Fill_Rectangle(68, 220, 117, 268);
//  mylcd.Fill_Rectangle(210, 215, 259, 264);
//  mylcd.Fill_Rectangle(230, 300, 279, 349);

mylcd.Fill_Rectangle(xpos_batt, ypos_batt, xpos_batt+x_batt/2, ypos_batt+y_batt);

  
  t_vitesse = millis();
 t_batterie = millis();

  
}
void loop() {
  if (millis() - t_batterie >= 1100) {
    t_batterie = millis();
    batterie=batterie+10;
    if (batterie > 100) {
      batterie = 0;
    }

      mylcd.Set_Text_Size(pol_batt);
     
       int  je=map(batterie,0,100,0,511);
if(je<ja){
 mylcd.Set_Draw_color(BLACK);
 mylcd.Fill_Rectangle(xpos_batt, ypos_batt , xpos_batt + map((ja / 2), 0, 511, 0, x_batt), ypos_batt + y_batt - 1);
}
       
        if (je <= 255) {
          mylcd.Set_Draw_color(255, je, 0);
          mylcd.Set_Text_colour(255, je, 0);
        } else {
          mylcd.Set_Draw_color(255 - (je - 256), 255, 0);
          mylcd.Set_Text_colour(255 - (je - 256), 255, 0);
        }
        //mylcd.Draw_Fast_VLine(map((x / 2), 0, 511, 0, x_batt) + xpos_batt, ypos_batt, y_batt);
        mylcd.Fill_Rectangle(xpos_batt, ypos_batt , xpos_batt + map((je / 2), 0, 511, 0, x_batt), ypos_batt + y_batt - 1);
  
        // mylcd.Fill_Rectangle(xpos_batt+x_batt/5-4, ypos_batt+y_batt/4-4,xpos_batt+x_batt/4+pol_batt*18, ypos_batt+y_batt/4+pol_batt*8);
        if(batterie>=100){
          mylcd.Print_String("FULL",133, 255);
        }else{
          if(batterie>=10){
        mylcd.Print_String(" "+String(batterie,DEC) + "%", 133, 255);
        }
      else{
        mylcd.Print_String("  "+String(batterie,DEC) + "%", 133, 255);
      }
        }
    ja=je;
  }
if (millis() - t_vitesse >= 200) {
  t_vitesse=millis();
    mylcd.Set_Text_Size(5);
    vitesse=vitesse+1;
    if (vitesse > 60) {
      vitesse = 0;
    }
    if (vitesse > 40) {
       mylcd.Set_Text_colour(RED);
    }else{
      mylcd.Set_Text_colour(WHITE);
    }
    if (vitesse < 10) {
      mylcd.Print_String(String(vitesse, DEC), 147, 170); ; //105,170);
    } else {
      mylcd.Print_String(String(vitesse, DEC), 134, 170); ; //105,170);
    }


    ie = map(vitesse, 0, 60, 210, -30);

    mylcd.Set_Draw_color(BLACK);
    mylcd.Fill_Triangle( 160 + 95 * cos((ia * PI) / 180), 140 - 95 * sin((ia * PI) / 180),
                         160 + 15 * cos(((ia * PI) / 180) - 0.4), 140 - 15 * sin(((ia * PI) / 180) - 0.4),
                         160 + 15 * cos(((ia * PI) / 180) + 0.4), 140 - 15 * sin(((ia * PI) / 180) + 0.4));

    mylcd.Set_Draw_color(RED);
    mylcd.Fill_Triangle( 160 + 95 * cos((ie * PI) / 180), 140 - 95 * sin((ie * PI) / 180),
                         160 + 15 * cos(((ie * PI) / 180) - 0.4), 140 - 15 * sin(((ie * PI) / 180) - 0.4),
                         160 + 15 * cos(((ie * PI) / 180) + 0.4), 140 - 15 * sin(((ie * PI) / 180) + 0.4));
    ia = ie;






    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Size(3);
    if (t_chrono_m < 10) {
      mylcd.Print_String("0" + String(t_chrono_m, DEC), 60, 445);
    } else {
      mylcd.Print_String(String(t_chrono_m, DEC), 60, 445);
    }
    if (t_chrono_s < 10) {
      mylcd.Print_String(":0" + String(t_chrono_s, DEC), 96, 445);
    } else {
      mylcd.Print_String(":" + String(t_chrono_s, DEC), 96, 445);
    }

    double distance = (30 / 3.6) * t_chrono;
    mylcd.Print_String(String(distance, 1), 60, 390);
  }




  //
  //show_picture(fan_1, sizeof(fan_1) / 2, 53, 300, 53 + 49, 300 + 49);
  //delay(100);
  //  show_picture(fan_2, sizeof(fan_2) / 2, 53, 300, 53 + 49, 300 + 49);
  //  delay(100);
      int x_batt = 200;
      int y_batt = 40;
      int xpos_batt = 110;
      int ypos_batt = 305;
      int edge_batt = 3;
      int pol_batt = 3;
      mylcd.Set_Text_Size(pol_batt);
  //// x=random(0,511);
 



  t_chrono = (millis() - t) / 1000;
  t_chrono_h = t_chrono / 3600;
  t_chrono_m = (t_chrono - 3600 * t_chrono_h) / 60 ;
  t_chrono_s = t_chrono % 60;


  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
  {
    //p.x = my_lcd.Get_Display_Width()-map(p.x, TS_MINX, TS_MAXX, my_lcd.Get_Display_Width(), 0);
    //p.y = my_lcd.Get_Display_Height()-map(p.y, TS_MINY, TS_MAXY, my_lcd.Get_Display_Height(), 0);
    p.x = map(p.x, TS_MINX, TS_MAXX, mylcd.Get_Display_Width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, mylcd.Get_Display_Height(), 0);

    if (is_pressed(1, 1, 42, 50, p.x, p.y))
    {
      if (switch_flag_1)
      { //display off
        show_picture(lamp_on, sizeof(lamp_on) / 2, 1, 1, 42, 50);
        switch_flag_1 = false;
      }
      else
      { //display on
        show_picture(lamp_off, sizeof(lamp_off) / 2, 1, 1, 42, 50);
        switch_flag_1 = true;
      }
      delay(100);
    }


    if (is_pressed(270, 300, 320, 350, p.x, p.y))
    {
      if (switch_flag_2)
      { //display off
        mylcd.Set_Draw_color(WHITE);
        mylcd.Draw_Rectangle(270, 300, 320, 350);
        switch_flag_2 = false;
      }
      else
      { //display on
        mylcd.Set_Draw_color(BLACK);
        mylcd.Draw_Rectangle(270, 300, 320, 350);
        switch_flag_2 = true;
      }
      delay(100);
    }

    if (is_pressed(1, 430, 50, 479, p.x, p.y)) {
      t = millis();
    }

    //UART.setCurrent(current);
    //UART.setBrakeCurrent(current);
    //UART.setRPM(current);
    //UART.printVescValues();

    //mylcd.Get_Display_Height();
    //Get_Display_Width();
    //  for (int i = 1; i < 100; i++) {
    //    mylcd.Print_String(String(i, DEC) + " V", 190, 200);
    //
    //    mylcd.Fill_Rectangle(10, 10, 50, i);
    //    //mylcd.Print_Number_Int(i, 0, 0, 0, ' ', 3);
    //    //mylcd.Print_Number_Float(millis(),0,0, 100, '.', 3, ' ');
    //
    //
    //  }
    // mylcd.Fill_Screen(WHITE);
    //  if ( UART.getVescValues() ) {
    //    Serial.println(UART.data.rpm);
    //    Serial.println(UART.data.inpVoltage);
    //    Serial.println(UART.data.ampHours);
    //    Serial.println(UART.data.tachometerAbs);
    //    Serial.println(UART.data.tachometerAbs);
    //    Serial.println(UART.data.avgMotorCurrent);
    //    Serial.println(UART.data.avgInputCurrent);
    //    Serial.println(UART.data.dutyCycleNow);
    //    Serial.println(UART.data.ampHoursCharged);
    //  }
  }

  //
  //  show_picture(fan1, sizeof(fan1) / 2, 60, 370, 60 + 49, 370 + 49);
  //  delay(100);
  //  show_picture(fan2, sizeof(fan2) / 2, 60, 370, 60 + 49, 370 + 49);
  //  delay(100);
}


void show_picture(const uint8_t *color_buf, int16_t buf_size, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  mylcd.Set_Addr_Window(x1, y1, x2, y2);
  mylcd.Push_Any_Color(color_buf, buf_size, 1, 1);
}


void temp_ind(int xpos_batt, int ypos_batt, int x_batt, int y_batt, int edge_batt) {
  mylcd.Set_Draw_color(WHITE);
  for (int x = 0; x <= edge_batt; x = x + 1) {
    mylcd.Draw_Rectangle(xpos_batt - 2 - x, ypos_batt - 2 - x, (x_batt) + xpos_batt + 1 + x, y_batt + ypos_batt + 1 + x);
  }

  //  for (int x = 0; x <= 255; x = x + 1) {
  //    mylcd.Set_Draw_color(x, 0, 255-x);
  //    //mylcd.Draw_Fast_HLine(map((x / 2), 0, 511, 0, x_batt) + xpos_batt, ypos_batt, y_batt);
  //   /// mylcd.Draw_Fast_HLine(xpos_batt, map((x), 0, 255, 0, y_batt - 1) + ypos_batt, x_batt);
  //    mylcd.Fill_Rectangle(xpos_batt, ypos_batt,xpos_batt+x_batt,ypos_batt+map((x), 0, 255, 0, y_batt - 1));
  //  }
}

boolean is_pressed(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t px, int16_t py)
{
  if ((px > x1 && px < x2) && (py > y1 && py < y2))
  {


    return true;
  }
  else
  {

    return false;
  }
}
