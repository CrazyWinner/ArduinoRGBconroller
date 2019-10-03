#include <EEPROM.h>

#define modeCount 24

boolean savedIsOn;
byte savedMode;
long saveCounter = 0;
boolean necessarySave = false;
byte red = 255;
byte green = 255;
byte blue = 255;
byte step = 0;
byte mode = 0;
int counter = 0;
boolean PWMinit = true;
boolean increaseMode = false;
long lastTime = 0;
boolean isOn = true;
int buttonCounter = 0;

void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  initFastPWM();
  attachInterrupt(digitalPinToInterrupt(2), changeMode, FALLING);
  load();

}
void save() {
  if (savedMode != mode) {
    EEPROM.write(50, mode);
    savedMode = mode;
  }
  if (savedIsOn != isOn) {
    EEPROM.write(51, isOn);
    savedIsOn = isOn;
  }
  necessarySave = false;
}
void load() {
  mode = EEPROM.read(50);
  isOn = EEPROM.read(51);
  savedMode = mode;
  savedIsOn = isOn;
}
void initFastPWM() {
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); //fast pwm
  TCCR2B = _BV(CS22); //fast pwm
}
void writeFastPWM(byte yaz) {
  if (yaz == 0) {
    OCR2A = 0;
    OCR2B = 0;
    digitalWrite(11, LOW);
    PWMinit = false;
  } else {
    if (!PWMinit) {
      initFastPWM();
      PWMinit = true;
    }
    OCR2A = yaz;
    OCR2B = yaz;
  }

}
void reset() {
  step = 0;
  counter = 0;
  increaseMode = false;
  red = 255;
  green = 255;
  blue = 255;
}

void changeMode() {
  if (isOn) {
    if (millis() - lastTime < 250) {
      if (mode > 0) {
        mode -= 1;
      } else {
        mode = modeCount - 1;
      }
      necessarySave = true;
      saveCounter = millis();
      reset();

    } else {
      increaseMode = true;
    }
    lastTime = millis();
  }
}

void check() {
  if (isOn && digitalRead(2) != LOW) {
    if (increaseMode && millis() - lastTime >= 250) {
      if (mode < modeCount - 1) {
        mode++;
      } else {
        mode = 0;
      }
      necessarySave = true;
      saveCounter = millis();
      reset();
    }
  }
  if (digitalRead(2) == LOW) {
    if (buttonCounter < 1000) buttonCounter++;
    if (buttonCounter == 1000) {
      isOn = !isOn;
      buttonCounter++;
      necessarySave = true;
      reset();
      saveCounter = millis();
    }
  } else {
    buttonCounter = 0;
  }
  if (millis() - saveCounter > 10000 && necessarySave) {
    save();
  }
}
void loop() {
  check();
  if (counter == 0)
    switch (mode) {
    case 0:
      red = 0;
      counter = 10;
      break;
    case 1:
      green = 0;
      counter = 10;
      break;
    case 2:
      blue = 0;
      counter = 10;
      break;
    case 3:
      red = 0;
      green = 0;
      counter = 10;
      break;
    case 4:
      red = 0;
      blue = 0;
      counter = 10;
      break;
    case 5:
      blue = 0;
      green = 0;
      counter = 10;
      break;
    case 6:
      blue = 0;
      green = 0;
      red = 0;
      counter = 10;
      break;
    case 7:
      counter = 2;
      mode1();
      break;
    case 8:
      counter = 5;
      mode1();
      break;
    case 9:
      counter = 10;
      mode1();
      break;
    case 10:
      counter = 2;
      mode2();
      break;
    case 11:
      counter = 5;
      mode2();
      break;
    case 12:
      counter = 10;
      mode2();
      break;
    case 13:
      mode3();
      break;
    case 14:
      mode4();
      break;
    case 15:
      mode5();
      break;
    case 16:
      mode6();
      break;
    case 17:
      mode7();
      break;
    case 18:
      mode8();
      break;
    case 19:
      mode9();
      break;
    case 20:
      mode10();
      break;
    case 21:
      mode11();
      break;
    case 22:
      mode12();
      break;
    case 23:
      mode13();
      break;
    }
  if (isOn) {
    analogWrite(5, 255 - red);
    analogWrite(6, 255 - blue);
    writeFastPWM(255 - green);
  } else {
    analogWrite(5, 0);
    analogWrite(6, 0);
    writeFastPWM(0);
  }
  if (counter > 0) counter--;
  delay(1);
}

void mode1() {
  if (step == 0) {
    if (red != 0) {
      red--;
    } else {
      step++;
    }
  } else
  if (step == 1) {
    if (green != 0) {
      green--;
    } else {
      step++;
    }
  } else
  if (step == 2) {
    if (red != 255) {
      red++;
    } else {
      step++;
    }
  } else
  if (step == 3) {
    if (blue != 0) {
      blue--;
    } else {
      step++;
    }
  } else
  if (step == 4) {
    if (green != 255) {
      green++;
    } else {
      step++;
    }
  } else
  if (step == 5) {
    if (red != 0) {
      red--;
    } else {
      step++;
    }
  } else
  if (step == 6) {
    if (green != 0) {
      green--;
    } else {
      step++;
    }
  } else
  if (step == 7) {
    if (green != 255) {
      red++;
      green++;
      blue++;
    } else {
      step = 0;
    }
  }
}

void mode2() {
  if (step == 0) {
    if (red != 0) {
      red--;
    } else {
      step++;
    }
  } else
  if (step == 1) {
    if (red != 255) {
      red++;
    } else {
      step++;
    }
  } else
  if (step == 2) {
    if (green != 0) {
      green--;
    } else {
      step++;
    }
  } else
  if (step == 3) {
    if (green != 255) {
      green++;
    } else {
      step++;
    }
  } else
  if (step == 4) {
    if (blue != 0) {
      blue--;
    } else {
      step++;
    }
  } else
  if (step == 5) {
    if (blue != 255) {
      blue++;
    } else {
      step = 0;
    }
  }

}

void mode3() {
  if (step == 0) {
    red = 255;
    counter = 900;
    step++;
  } else
  if (step == 1) {
    red = 0;
    counter = 100;
    step = 0;
  }

}
void mode4() {
  if (step == 0) {
    blue = 255;
    counter = 900;
    step++;
  } else
  if (step == 1) {
    blue = 0;
    counter = 100;
    step = 0;
  }

}
void mode5() {
  if (step == 0) {
    green = 255;
    counter = 900;
    step++;
  } else
  if (step == 1) {
    green = 0;
    counter = 100;
    step = 0;
  }

}

void mode6() {
  if (step == 0) {
    blue = 255;
    red = 255;
    counter = 900;
    step++;
  } else
  if (step == 1) {
    red = 0;
    counter = 100;
    step++;
  } else
  if (step == 2) {
    red = 255;
    green = 255;
    counter = 900;
    step++;
  } else
  if (step == 3) {
    green = 0;
    counter = 100;
    step++;
  } else
  if (step == 4) {
    green = 255;
    blue = 255;
    counter = 900;
    step++;
  } else
  if (step == 5) {
    blue = 0;
    counter = 100;
    step = 0;
  }
}

void mode7() {
  if (step == 0) {
    blue = 255;
    counter = 900;
    step++;
  } else
  if (step == 1) {
    red = 0;
    counter = 100;
    step++;
  } else
  if (step == 2) {
    red = 255;
    green = 0;
    counter = 100;
    step++;
  } else
  if (step == 3) {
    green = 255;
    blue = 0;
    counter = 100;
    step = 0;
  }

}

void mode8() {
  counter = 300;
  if (step == 0) {
    green = 255;
    blue = 255;
    red = 0;
    step++;
  } else
  if (step == 1) {
    green = 0;
    step++;
  } else
  if (step == 2) {
    red = 255;
    step++;
  } else
  if (step == 3) {
    blue = 0;
    step++;
  } else
  if (step == 4) {
    green = 255;
    step++;
  } else
  if (step == 5) {
    red = 0;
    step++;
  } else
  if (step == 6) {
    red = 0;
    green = 0;
    blue = 0;
    step = 0;
  }

}

void mode9() {
  if (step % 2 == 0) {
    red = 0;
    counter = 1000 / (16 - step);
  } else {
    red = 255;
    counter = 100;
  }

  if (step < 14) {
    step++;
  } else {
    step = 0;
  }
}
void mode10() {
  if (step % 2 == 0) {
    green = 0;
    counter = 1000 / (16 - step);
  } else {
    green = 255;
    counter = 100;
  }

  if (step < 14) {
    step++;
  } else {
    step = 0;
  }
}
void mode11() {
  if (step % 2 == 0) {
    blue = 0;
    counter = 1000 / (16 - step);
  } else {
    blue = 255;
    counter = 100;
  }

  if (step < 14) {
    step++;
  } else {
    step = 0;
  }
}
void mode12() {
  if (step < 16) {
    if (step % 2 == 0) {
      red = 0;
      counter = 1000 / (16 - step);
    } else {
      red = 255;
      counter = 100;
    }
  } else
  if (step > 15 && step < 32) {
    if (step % 2 == 0) {
      green = 0;
      counter = 1000 / (32 - step);
    } else {
      green = 255;
      counter = 100;
    }
  } else {
    if (step % 2 == 0) {
      blue = 0;
      counter = 1000 / (48 - step);
    } else {
      blue = 255;
      counter = 100;
    }

  }
  if (step < 47) {
    step++;
  } else {
    step = 0;
  }
}

void mode13() {

  counter = 100;
  if (step % 2 == 0) {
    if (step / 2 == 0) {
      red = 0;
    } else
    if (step / 2 == 1) {
      red = 0;
      green = 0;
    } else
    if (step / 2 == 2) {
      green = 0;
    } else
    if (step / 2 == 3) {
      green = 0;
      blue = 0;
    } else
    if (step / 2 == 4) {
      blue = 0;
    } else
    if (step / 2 == 5) {
      blue = 0;
      red = 0;
    } else
    if (step / 2 == 6) {
      red = 0;
      green = 0;
      blue = 0;
    }
  } else {
    red = 255;
    green = 255;
    blue = 255;
    counter = 900;
  }

  if (step < 13) {
    step++;
  } else {
    step = 0;
  }
}
