
int marioMelody[] = {2637, 2637, 0, 2637, 0, 2093, 2637, 0, 3136};


void melody()
{
  int size = sizeof(marioMelody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++)
  {
    tone(BUZZER, marioMelody[thisNote], 90);
    delay(120);
  }
  delay(500);
}

void buzz()
{
  tone(BUZZER, 1800);
}

void buzz(int buzzDuration)
{
  noTone(BUZZER);//turn buzzer off
  tone(BUZZER, 1800, buzzDuration);//Make a tone for 1 second
}

void buzzOff()//lol
{
  noTone(BUZZER);//turn buzzer off
}



