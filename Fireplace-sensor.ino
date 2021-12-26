const int LDR_PIN = A0;

const int reads = 50;
const int timeBetweenReads = 100;

int readings[reads];
int highestVarience;
int currentVarience;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  highestVarience = 0;
  
  //Get readings for (reads * timeBetweenReads) milliseconds
  for (int i = 0; i < reads; i++)
  {
    readings[i] = analogRead(LDR_PIN);
    delay(timeBetweenReads);
  }

  //Get highest varience between readings (highest 'flicker')
  for(int i = 0; i < reads-1; i++)
  {
    currentVarience = abs(readings[i] - readings[i + 1]);
    if(currentVarience > highestVarience)
    {
      highestVarience = currentVarience;
    }
  }

  Serial.print(highestVarience);
  
}
