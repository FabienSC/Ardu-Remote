

void sigLED(bool r, bool g, bool b)
{
  if(r)
    digitalWrite(LED_SIG_R, HIGH);
  else
    digitalWrite(LED_SIG_R, LOW);
  if(g)
    digitalWrite(LED_SIG_G, HIGH);
  else
    digitalWrite(LED_SIG_G, LOW);
  if(b)
    digitalWrite(LED_SIG_B, HIGH);
  else
    digitalWrite(LED_SIG_B, LOW);
}

void auxLED(bool r, bool g, bool b)
{
  if(r)
    digitalWrite(LED_AUX_R, HIGH);
  else
    digitalWrite(LED_AUX_R, LOW);
  if(g)
    digitalWrite(LED_AUX_G, HIGH);
  else
    digitalWrite(LED_AUX_G, LOW);
  if(b)
    digitalWrite(LED_AUX_B, HIGH);
  else
    digitalWrite(LED_AUX_B, LOW);
}

