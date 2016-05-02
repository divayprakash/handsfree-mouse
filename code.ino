#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl ; 
int cursorMovePin = 12 ;

void setup() 
{
  pinMode(cursorMovePin, INPUT) ;
  pinMode(13, OUTPUT) ;
  digitalWrite(13, LOW) ;
  adxl.powerOn() ;
  adxl.setActivityThreshold(75) ; 
  adxl.setInactivityThreshold(75) ;
  adxl.setTimeInactivity(10) ; 
  adxl.setActivityX(1) ;
  adxl.setActivityY(1) ;
  adxl.setActivityZ(1) ;
  adxl.setInactivityX(1) ;
  adxl.setInactivityY(1) ;
  adxl.setInactivityZ(1) ;
  adxl.setTapDetectionOnX(1) ;
  adxl.setTapDetectionOnY(1) ;
  adxl.setTapDetectionOnZ(1) ;
  adxl.setTapThreshold(50) ; 
  adxl.setTapDuration(15) ; 
  adxl.setDoubleTapLatency(80) ;
  adxl.setDoubleTapWindow(200) ; 
  adxl.setFreeFallThreshold(7) ; 
  adxl.setFreeFallDuration(45) ; 
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN ) ;
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,  ADXL345_INT1_PIN ) ;
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN ) ;
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN ) ;
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN ) ;
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1) ;
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1) ;
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1) ;
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1) ;
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1) ;
  Mouse.begin() ;
}

void loop() 
{
  int flagLeft = 0  ;
  if (digitalRead(cursorMovePin) == HIGH)
  {
    int x, y, z ;  
    adxl.readAccel(&x, &y, &z) ; 
    int x1 = y ;
    int y1 = -x ;
    int x2 = x1 - x % 100 ;
    int y2 = y1 - y % 100 ;
    Mouse.move(x2 / 50, y2 / 50, 0) ;
    delay(4) ;  
    byte interrupts = adxl.getInterruptSource() ;
    if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP))
    {
      if (adxl.isTapSourceOnZ())
      {
        if (Mouse.isPressed(MOUSE_MIDDLE)) Mouse.release(MOUSE_MIDDLE) ;
        else Mouse.press(MOUSE_MIDDLE) ;
      }
    }
  }
  else
  {
    byte interrupts = adxl.getInterruptSource() ;
    if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP))
    {
      if (adxl.isTapSourceOnY()) Mouse.click(MOUSE_RIGHT) ;
      else if (adxl.isTapSourceOnX()) Mouse.click(MOUSE_LEFT) ;
      else if (adxl.isTapSourceOnZ())
      {
        if (Mouse.isPressed(MOUSE_MIDDLE)) Mouse.release(MOUSE_MIDDLE) ;
        if (Mouse.isPressed(MOUSE_LEFT)) Mouse.release(MOUSE_LEFT) ;
        else Mouse.press(MOUSE_LEFT) ;
      }
    }
    if (adxl.triggered(interrupts, ADXL345_DOUBLE_TAP))
    {
      if (adxl.isTapSourceOnX())
      { 
        Mouse.click(MOUSE_LEFT) ;
        Mouse.click(MOUSE_LEFT) ;
      }
    } 
  }
}