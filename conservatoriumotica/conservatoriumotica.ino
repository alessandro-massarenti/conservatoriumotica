// Valve control pins
const int EU_valve = 4; //Emergency unload valve control pin
const int L_valve = 3;  //load valve control pin
const int U_valve = 2;  //Watering unload valve

// Sensor pins
const int TL_sensor = A1; //Tank level
const int humidity = A0;  // Analog input pin that senses Vout

int lst;
int A[3];

void setup()
{
  // setup
  Serial.begin(9600); // InitialiL_aprie serial communications at 9600 bps
  pinMode(EU_valve, OUTPUT);
  pinMode(L_valve, OUTPUT);
  pinMode(U_valve, OUTPUT);
  digitalWrite(EU_valve, HIGH);
  digitalWrite(L_valve, HIGH);
  digitalWrite(U_valve, HIGH);
}

int rollingAvg(int input)
{

  long long sum = 0;

  A[lst] = input;
  for (int i = 0; i < 10; i++)
    sum = sum + A[i];
  lst++;
  if (lst == 10)
    lst = 0;
  return sum;
}

/**
  a tankRoutine
  Tank life parameters
  int M = 87; //Maximum water level of the tank
  int z = 70; // Median maximum level
  int x = 40; // Median minimum level
  int m = 20; //Minimum water level of the tank
*/
void tankRoutine(bool &L_apri, bool &EU_apri, int &l1, int M = 87, int z = 70, int x = 40, int m = 20)
{
  int l = map(analogRead(TL_sensor), 0, 1023, 0, 100); //mappa i valori letti dal sensore legandoli a valori percentuali

  if (l != l1)
  {
    if (!L_apri && l <= m)
      L_apri = true;
    if (L_apri && l > x)
      L_apri = false;
    L_apri ? digitalWrite(L_valve, LOW) : digitalWrite(L_valve, HIGH);

    if (!EU_apri && l >= M)
      EU_apri = true;
    if (EU_apri && l < z)
      EU_apri = false;
    EU_apri ? digitalWrite(EU_valve, LOW) : digitalWrite(EU_valve, HIGH);
  }
  l1 = l;
}

// Main function
void loop()
{
  int sensorValue = 0; // humidity default value
  float Vin = 5;       // Input voltage
  float Vout = 0;      // Vout default value
  float Rref = 47000;  // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
  float R = 0;

  // Greenhouse life parameters
  int h = 0; //Current humidity level in greenhouse
  int h1;
  unsigned long t1 = 0;
  bool low_moisture = false;

  // Auxiliary
  bool L_apri = false;
  bool EU_apri = false;
  int l1 = 0;

  while (true)
  {
    unsigned long timestamp = millis();

    tankRoutine(L_apri, EU_apri, l1);

    //Moisture reading.
    if ((timestamp - t1) >= 10)
    {
      sensorValue = rollingAvg(analogRead(humidity));  // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
      Vout = (Vin * sensorValue) / 1023;   // Convert Vout to volts
      R = Rref * (1 / ((Vin / Vout) - 1)); // Formula to calculate tested resistor's value
      if (!low_moisture && R > 10000)
        low_moisture = true;
      if (low_moisture && R < 5000)
        low_moisture = false;
      low_moisture ? digitalWrite(U_valve, LOW) : digitalWrite(U_valve, HIGH);
      t1 = millis();
    }
    Serial.print("Level: ");
    Serial.print(l1); // Give level in Serial Monitor
    Serial.print(" ");
    Serial.print("light: ");
    Serial.print(analogRead(2)); // Give level in Serial Monitor
    Serial.print(" ");
    Serial.print("R: ");
    Serial.println(R); // Give calculated resistance in Serial Monitor
    delay(1);          // Clock
  }
  Serial.println("Qualcosa è andato storto, ora riavvio");
}
