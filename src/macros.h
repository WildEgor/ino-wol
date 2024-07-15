// Useful macros
#ifndef macros_h
#define macros_h

  #define SKETCH_NAME "INO WOL"
  #define SW_VERSION "v1.0.0"
  #define SKETCH_AUTHOR "Kartashov Egor @wildegor"
  
  #define SERIAL_PRINTING true // Serial debugging is enabled
  #define ESP_SERIAL_BAUD 115200 // Serial speed between ESP and PC

  #if SERIAL_PRINTING
    #define SERIAL_DEBUG  (Serial.begin(ESP_SERIAL_BAUD))
    #define Sprintln(a) (Serial.println(a))
    #define Sprint(a) (Serial.print(a))
    #define Sprintf(...) (Serial.printf(__VA_ARGS__))
    #define BLYNK_PRINT Serial
  #else
    #define SERIAL_DEBUG
    #define Sprintln(a) 
    #define Sprint(a) 
    #define Sprintf(...) 
  #endif
#endif 
