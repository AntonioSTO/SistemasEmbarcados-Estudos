int ssegPin[] = {18, 5, 17, 16, 4, 0, 2, 15}; // a, b, c, d, e, f, g, ponto
int comumDisp[] = {19, 21, 3, 1, 22, 23};    // comum dos displays

int segUni = 0, segDez = 0, minUni = 0, minDez = 0, horaUni = 0, horaDez = 0;
int cs0 = 34, cs1 = 35, cs2 = 32;
volatile bool cs0State = false, cs1State = false, cs2State = false;
int dp = 1;

hw_timer_t *timer = NULL;
uint8_t timer_id = 0;
uint16_t prescaler = 80;
int threshold = 1000000; // autoreload

void timer_isr(){
  //funcao de interrupcao
  cs0State = digitalRead(cs0);
  cs1State = digitalRead(cs1);
  cs2State = digitalRead(cs2);

  if(cs0State == LOW){
    dp = 1;
    segUni++;
    if (segUni == 10){
      segUni = 0;
      segDez++;
      if (segDez == 6){
        segDez = 0;
        minUni++;
        if (minUni == 10){
          minUni = 0;
          minDez++;
          if (minDez == 6){
            horaUni++;
            if (horaUni == 10){
              horaUni = 0;
              horaDez++;
            } 
          }
        }
      }
    }
  } else if(cs1State == LOW){
    dp = 1;
    segUni++;
    if (segUni == 10){
      segUni = 0;
      segDez++;
      if (segDez == 6){
        segDez = 0;
        minUni++;
        if (minUni == 10){
          minUni = 0;
          minDez++;
          if (minDez == 6){
            horaUni++;
            if (horaUni == 10){
              horaUni = 0;
              horaDez++;
              if (horaDez == 2 and horaUni == 4){
                timerWrite(timer,0);
                segUni = 0;
                segDez = 0; 
                minUni = 0;
                minDez = 0;
                horaUni = 0;
                horaDez = 0;
              }
            } 
          }
        }
      }
    }
  } else if(cs2State == LOW){
    segUni++;
    if (segUni == 10){
      segUni = 0;
      segDez++;
      if (segDez == 6){
        segDez = 0;
        minUni++;
        if (minUni == 10){
          minUni = 0;
          minDez++;
          if (minDez == 6){
            horaUni++;
            if (horaUni == 10){
              horaUni = 0;
              horaDez++;
              if (horaDez == 1 and horaUni == 2){
                timerWrite(timer,0);
                segUni = 0;
                segDez = 0; 
                minUni = 0;
                minDez = 0;
                horaUni = 0;
                horaDez = 0;
                dp = 0;
              }
            } 
          }
        }
      }
    }   
  } else {
      timerWrite(timer,0);
      segUni = 0;
      segDez = 0; 
      minUni = 0;
      minDez = 0;
      horaUni = 0;
      horaDez = 0;
    }
}

void displayNumber(int num, int disp) {
  int digitConfig[][8] = {
      {LOW, LOW, LOW, LOW, LOW, LOW, HIGH, dp},     // 0
      {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH, dp}, // 1
      {LOW, LOW, HIGH, LOW, LOW, HIGH, LOW, dp},    // 2
      {LOW, LOW, LOW, LOW, HIGH, HIGH, LOW, dp},    // 3
      {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW, dp},    // 4
      {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW, dp},    // 5
      {LOW, HIGH, LOW, LOW, LOW, LOW, LOW, dp},     // 6
      {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH, dp},  // 7
      {LOW, LOW, LOW, LOW, LOW, LOW, LOW, dp},      // 8
      {LOW, LOW, LOW, LOW, HIGH, LOW, LOW, dp}      // 9
  };

  for (int i = 0; i < 8; i++) {
    digitalWrite(ssegPin[i], digitConfig[num][i]);
  }

  digitalWrite(comumDisp[disp], HIGH);
  digitalWrite(comumDisp[disp], LOW);
}

void setup() {
  timer = timerBegin(timer_id, prescaler, true);
  timerAttachInterrupt(timer, &timer_isr, true);

  pinMode(cs0, INPUT_PULLUP);
  pinMode(cs1, INPUT_PULLUP);
  pinMode(cs2, INPUT_PULLUP);

  for (int i = 0; i < 8; i++) {
    pinMode(ssegPin[i], OUTPUT);
  }

  for (int i = 0; i < 6; i++) {
    pinMode(comumDisp[i], OUTPUT);
  }
}

void loop() {
  cs0State = digitalRead(cs0);
  cs1State = digitalRead(cs1);
  cs2State = digitalRead(cs2);

  if (cs0State == LOW || cs1State == LOW || cs2State == LOW) {
    timerAlarmWrite(timer, threshold, true);
    timerAlarmEnable(timer);

    displayNumber(segUni, 0);
    displayNumber(segDez, 1);
    displayNumber(minUni, 2);
    displayNumber(minDez, 3);
    displayNumber(horaUni, 4);
    displayNumber(horaDez, 5);
  }

  delay(100);
}
