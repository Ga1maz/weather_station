#include <Wire.h>                                            
#include <LiquidCrystal_I2C.h>                                
#include <SPI.h>                                              
#include <SD.h>                                               
#include <iarduino_DHT.h>                                     
#include <iarduino_RTC.h>                                     
int8_t      DHT_result;                                 
uint16_t    LUM_result;                                 
bool        MSD_result;                                 
uint8_t     LCD_symbol[8]    = {B00110,B01001,B01001,B00110,B00000,B00000,B00000,B00000};   
const uint8_t     PIN_temperature  = 2;                       
const uint8_t     PIN_adapterSD_CS = 10;                      
const uint8_t     PIN_illumination = 0;                      
LiquidCrystal_I2C lcd(0x27,16,2);                             
iarduino_RTC      time(RTC_DS1307);                           
iarduino_DHT      sensor(PIN_temperature);                   
File              myFile;                                    
void setup(){ 
  MSD_result = SD.begin(PIN_adapterSD_CS);                   
  time.begin();                                               
  lcd.init();                                                 
  lcd.backlight();                                           
  lcd.createChar(1, LCD_symbol);                              
  lcd.setCursor(0, 0);                                        
  lcd.print(F("Almaz Ganiev"));                                
  lcd.setCursor(0, 1);                                   
  lcd.print(F("Ga1maz.ru"));                                   
  lcd.print(MSD_result?"OK":"");                          
  delay(2000);                                                
  lcd.clear();                                                
} 
void loop(){ 
  if(time.seconds!=atoi(time.gettime("s"))){                  
    DHT_result = sensor.read();                              
    LUM_result = 1024-analogRead(PIN_illumination);         
                           lcd.setCursor(0, 0);               
                           lcd.print(F("t=     \1C"));        
                           lcd.setCursor(2, 0);               
    if(DHT_result==DHT_OK){lcd.print(sensor.tem);}            
    else                  {lcd.print(F("??.??"));}            
                           lcd.setCursor(0, 1);               
                           lcd.print(F("PH=     %"));         
                           lcd.setCursor(3, 1);               
    if(DHT_result==DHT_OK){lcd.print(sensor.hum);}            
    else                  {lcd.print(F("??.??"));}           
                           lcd.setCursor(10, 0);              
                           lcd.print(F("*=    "));            
                           lcd.setCursor(12, 0);              
                           lcd.print(LUM_result);             
                           lcd.setCursor(10, 1);              
                           lcd.print(time.gettime("H:i"));    
                           lcd.setCursor(12, 1);              
    if(time.seconds%2){    lcd.print(" ");}                   
    if(MSD_result){                                           
      if(time.seconds==0){                                    
        if(time.minutes%5==0){                                
            myFile = SD.open(F("iarduino.txt"),FILE_WRITE);   
         if(myFile){                                          
            myFile.print(time.gettime("Y-m-d H.i:"));         
            myFile.print(F(" температура "));                 
            myFile.print(sensor.tem); 
            myFile.print(F("°С, влажность "));                
            myFile.print(sensor.hum); 
            myFile.print(F("%, освещённость "));              
            myFile.print(LUM_result); 
            myFile.println("."); 
            myFile.close();                                   
          } 
        } 
      } 
    } 
  }else{delay(200);}                                          
}
