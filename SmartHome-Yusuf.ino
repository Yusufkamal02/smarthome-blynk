/***************************************
 * Smart Home ESP32 dg Blynk IoT
 * Board  : ESP32 DEVKIT
 * Input  : MQ2, DHT22, Magnet Sensor
 * Output : LED, Buzzer, Motor DC, Blynk App
 * ESP32 Smart Home Aura Analytics
 ****************************************/
 //---GANTI SESUAI DENGAN TEMPLATE ID
 //---GANTI DEVICE NAME
 //---GANTI TOKEN BLYNK ANDA
#define BLYNK_TEMPLATE_ID "TMPL6OySZJqt8"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "-TSZN5Vnkko3Sc3_2Hy9gAlm0YAnrVjk"

#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

//---GANTI SESUAI DENGAN JARINGAN WIFI
//---HOTSPOT ANDA
char ssid[] = "iPhone";
char pass[] = "yusufganteng";

#define DHTPIN 4 
#define DHTTYPE DHT22
#define Lampu_depan 14
#define Lampu_tidur 27
#define Lampu_tengah 26
#define Lampu_belakang 25
#define Kipas_angin 33
#define Bell 32
#define MQ2_SENSOR  16
#define Magnet  17

DHT dht(DHTPIN, DHTTYPE);
char auth[] = BLYNK_AUTH_TOKEN;

LiquidCrystal_I2C lcd(0x27, 16, 2);
boolean st,fg,fm;
int humi, temp,u; 
int MQ2_SENSOR_Value = 0;
//==============================================
void cek_koneksi(){ //fungsi cek koneksi dg Blynk
  st=Blynk.connected();//baca status koneksi
  if(st==true){//jika terhubung
    lcd.clear();//set ke awal baris 2      
    lcd.print("Koneksi Sukses");//tulis ke LCD
  }
  else{ //jika tidak terhubung
    lcd.clear();//set ke awal baris 2      
    lcd.print("Koneksi Gagal");//tulisa ke LCD
  }
}
//=============================
void read_DHT22(){
  humi = dht.readHumidity();  
  temp = dht.readTemperature();
  if (isnan(humi) || isnan(temp)) {
    lcd.setCursor(3,0);
    lcd.print("--");
    lcd.setCursor(10,0);
    lcd.print("--");
    return;
  }
  else{  
    lcd.setCursor(3,0);
    lcd.print(temp);
    lcd.setCursor(10,0);
    lcd.print(humi);
    Blynk.virtualWrite(V0, temp);  
    Blynk.virtualWrite(V1, humi);  
  }  
}
//==============================
BLYNK_WRITE(V2)//Lampu depan
{ 
  int value1 = param.asInt();
  digitalWrite(Lampu_depan,value1);  
}
//==============================
BLYNK_WRITE(V3)//Lampu tengah
{ 
  int value2 = param.asInt();
  digitalWrite(Lampu_tengah,value2);  
}
//==============================
BLYNK_WRITE(V4)//Lampu belakang
{ 
  int value3 = param.asInt();
  digitalWrite(Lampu_belakang,value3);  
}
//-----------------
BLYNK_WRITE(V5)// Lampu tidur
{ 
  int value4 = param.asInt();  
  analogWrite(Lampu_tidur,value4);
}
//----------------
BLYNK_WRITE(V6)// Fan
{ 
  int value5 = param.asInt(); 
  analogWrite(Kipas_angin,value5);
}
//=============================
BLYNK_WRITE(V7)// Bell
{ 
  int value6 = param.asInt();
  digitalWrite(Bell,value6);  
}
//==============================
void cek_Magnet(){ 
  if ((digitalRead(Magnet)==HIGH)&&(fm==0)) 
       { 
         Serial.println("==>Pintu terbuka terdeteksi");         
         Blynk.logEvent("awas", "Pintu terbuka!");
         fm=1;   
         lcd.setCursor(7, 1);
         lcd.print("AWS PINTU");      
         delay(5000);
        }
     else if((digitalRead(Magnet)==LOW)&&(fm==1)) 
       {              
        fm=0;
        lcd.setCursor(7, 1);
        lcd.print("Aman      ");  
        delay(1000);
       }
}
//==============================
void cek_MQ(){
   MQ2_SENSOR_Value = digitalRead(MQ2_SENSOR);
    if ((MQ2_SENSOR_Value==0)&&(fg==0)) 
       { 
         Serial.println("==>Gas terdeteksi");         
         Blynk.logEvent("awas", "Gas bocor terdeteksi!");
         digitalWrite(Bell, HIGH); // Menyalakan buzzer saat gas terdeteksi
         Blynk.virtualWrite(V8, "AWAS GAS BOCOR!"); // Peringatan warning di widget Blynk (Pin V8)
         fg=1;   
         lcd.setCursor(7, 1);
         lcd.print("AWAS GAS");      
         delay(5000);
        }
     else if((MQ2_SENSOR_Value==1)&&(fg==1)) 
       {              
        digitalWrite(Bell, LOW); // Mematikan buzzer
        Blynk.virtualWrite(V8, "Aman"); // Status aman di widget Blynk (Pin V8)
        fg=0;
        lcd.setCursor(7, 1);
        lcd.print("Aman      ");  
        delay(1000);
       }
 }
//============================
void setup()
{  
  Serial.begin(115200);
  pinMode(Magnet, INPUT_PULLUP);  
  pinMode(MQ2_SENSOR, INPUT); 
  pinMode(Lampu_depan, OUTPUT);
  pinMode(Lampu_tidur, OUTPUT);
  pinMode(Lampu_tengah, OUTPUT);
  pinMode(Lampu_belakang, OUTPUT);
  pinMode(Kipas_angin, OUTPUT);
  pinMode(Bell, OUTPUT);
  lcd.begin(16,2); //set LCD ukuran 16x2
  lcd.init();  //inisialisasi LCD
  lcd.backlight(); //backlight LCD
  lcd.setCursor(0, 0); //set kursor di awal baris 1
  lcd.print(" = Smart Home = ");//tulis ke LCD
  lcd.setCursor(0, 1); //set kursos di awal baris 2     
  lcd.print("   Blynk IoT");//tulis ke LCD
  delay(2000); //tunggu 2s
  lcd.clear(); //hapus tampilan LCD     
  lcd.print("Tunggu Koneksi..");//tulis LCD 
  
  Blynk.begin(auth, ssid, pass);
  cek_koneksi(); //panggil fungsi cek koneksi 
  dht.begin();  
  delay(3000);
  lcd.clear();   
  lcd.print("T=   C, H=   %");
  lcd.setCursor(0, 1);  
  lcd.print("Status:Aman    ");
}
//=============================
void loop()
{  
  for(u=0;u<500;u++){
  Blynk.run();  
  cek_MQ();
  cek_Magnet();
  delay(4);
  }
  read_DHT22(); 
  delay(5);  
}


