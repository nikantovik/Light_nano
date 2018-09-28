// Плата управления освещением объекта "Гараж"
#include <Arduino.h>
#include <UIPEthernet.h>
#include "CyberLib.h"

#define relay1_on D14_High // Реле включения
#define relay1_off D14_Low
#define relay2_on D15_Low // Реле выключения
#define relay2_off D15_High
#define Reset D17_Low // сброс контроллера.

int Timer_Reset=0; // переменная для подсчета времени контроля наличия связи с компом


EthernetServer server = EthernetServer(2500);

int Status = 0;

boolean flag = true;
boolean flag2 = true;
int Opros = 0;



void timer_reset () { // Функция по прерыванию таймера 1
Timer_Reset++;
if (Timer_Reset>=30) Reset ; //вызываем reset;
}

void setup()
{
  uint8_t mac[6] = {0xDE, 0xAD, 0xBE, 0xEF, 0xED, 0xFC};
  IPAddress myIP(192, 168, 1, 86);

  Ethernet.begin(mac, myIP);
  server.begin();

  D14_Out; // Выход первого реле
  D15_Out; //Выход второго реле и включение
  D16_In; D16_High; // Вход телесостояния пускателя и подтяжка к плюсу
  D17_Out; D17_High; // Настраиваем пин для управления сбросом контроллера



  Status = D16_Read; // Проверка состояния пускателя

  if (Status == HIGH)
    flag = false;
  else
    flag = true;
 
}

void loop()
{
  StartTimer1(timer_reset, 1000000); // инициализируем таймер 1 раз в секунду.
  Start
  EthernetClient client = server.available();
  if (client)
  {
    if (client.available())
    {
      char val = client.read();
      if (val == 'L' & flag2 == true)
      {
        server.write('Q');
        delay(400);
      }
      if (val == 'H' & flag2 == false)
      {
        server.write('W');
        delay(400);
      }
      if (val == 'H' & flag2 == true)
      {
        relay1_on;
        delay(1000);
        relay1_off;
        flag2 = false;
        delay(500);
        Status = D16_Read;
        if (Status == HIGH)
        {
          flag2 = true;
          server.write('N');
          delay(400);
        }
      }
      if (val == 'L' & flag2 == false)
      {
        relay2_off;
        delay(1000);
        relay2_on;
        flag2 = true;
        delay(500);
        Status = D16_Read;
        if (Status == LOW)
        {
          flag2 = false;
          server.write('Y');
          delay(400);
        }
      }

      if (val == 'Z') // проверка связи
      {
        server.write('X');
        delay(400); Timer_Reset=0;
      }
      if (val == 'O')
      {
        if (Status == HIGH)
          flag = false;
        else
          flag = true;
      }
    }
  }

  Status = D16_Read;

  if (Status == LOW & flag == true)
  {
    server.write('P');
    delay(400);
    flag = false;
    flag2 = false;
  }

  if (Status == HIGH & flag == false)
  {
    server.write('S');
    delay(400);
    flag = true;
    flag2 = true;
  }
  End
  
}
