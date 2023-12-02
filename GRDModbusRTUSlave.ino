#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "ModbusRtu.h"    /* Usage of ModBusRtu library allows you to implement the Modbus RTU protocol in your sketch. */
/*
   CONTROLLINO - Modbus RTU protocol Slave example for MAXI and MEGA, Version 01.00
   
   The sketch is relevant only for CONTROLLINO variants MAXI and MEGA (because of necessity of RS485 interface)!

   This sketch is intended as an example of the communication between devices via RS485 with utilization the ModbusRTU protocol.
   In this example the CONTROLLINO  is used as the Modbus slave!
   For more information about the Modbus protocol visit the website: http://modbus.org/
   
   Modbus master device can read Modbus 16bit registers (provided by the slave):
    0 - analog CONTROLLINO_A0 value (0 - 1024)
    1 - digital CONTROLLINO_D0 value (0/1)
    2 - Modbus messages received
    3 - Modbus messages transmitted

   Modbus master device can write Modbus 16bit registers:
    4 - relay CONTROLLINO_R0 (0/1)
    5 - relay CONTROLLINO_R1 (0/1)
    6 - relay CONTROLLINO_R2 (0/1)
    7 - relay CONTROLLINO_R3 (0/1)

   To easily evaluate this example you need a second CONTROLLINO as Modbus master running DemoModbusRTUMaster example sketch.
   Please note that both CONTROLLINOs need 12/24V external supply and you need to interconnect GND, -, + signals of RS485 screw terminal.

   Modbus Master-Slave library for Arduino (ModbusRtu.h) was taken from the website: https://github.com/smarmengol/Modbus-Master-Slave-for-Arduino
   It was necessary to modify setting of the PORTJ for pins DE and RE control. These pins are located at the PORJ and on the pins PIN6(DE) and PIN5(RE).

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MAXI/MEGA before Upload to your CONTROLLINO.
  (Please, refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  Created 30 March 2017
  by David

  https://controllino.biz/

  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/

// This MACRO defines Modbus slave address.
// For any Modbus slave devices are reserved addresses in the range from 1 to 247.
// Important note only address 0 is reserved for a Modbus master device!
#define SlaveModbusAdd  247

// This MACRO defines number of the comport that is used for RS 485 interface.
// For MAXI and MEGA RS485 is reserved UART Serial3.
#define RS485Serial     3

// The object ControllinoModbuSlave of the class Modbus is initialized with three parameters.
// The first parametr specifies the address of the Modbus slave device.
// The second parameter specifies type of the interface used for communication between devices - in this sketch is used RS485.
// The third parameter can be any number. During the initialization of the object this parameter has no effect.
Modbus ControllinoModbusSlave(SlaveModbusAdd, RS485Serial, 0);

// This uint16 array specified internal registers in the Modbus slave device.
// Each Modbus device has particular internal registers that are available for the Modbus master.
// In this example sketch internal registers are defined as follows:
// (ModbusSlaveRegisters 0 - 3 read only and ModbusSlaveRegisters 4 - 7 write only from the Master perspective):
// ModbusSlaveRegisters[0] - Read an analog value from the CONTROLLINO_A0 - returns value in the range from 0 to 1023.
// ModbusSlaveRegisters[1] - Read an digital value from the CONTROLLINO_D0 - returns only the value 0 or 1.
// ModbusSlaveRegisters[2] - Read the number of incoming messages - Communication diagnostic.
// ModbusSlaveRegisters[3] - Read the number of number of outcoming messages - Communication diagnostic.
// ModbusSlaveRegisters[4] - Sets the Relay output CONTROLLINO_R0 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[5] - Sets the Relay output CONTROLLINO_R1 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[6] - Sets the Relay output CONTROLLINO_R2 - only the value 0 or 1 is accepted.
// ModbusSlaveRegisters[7] - Sets the Relay output CONTROLLINO_R3 - only the value 0 or 1 is accepted.
uint16_t ModbusSlaveRegisters[58];

bool richiestoReset = false;
long int t1 = 0;
long int ElapsedTimeToReset = 7000;

// The setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
// In the setup function is carried out port setting and initialization of communication of the Modbus slave protocol.
void setup()
{
    pinMode(CONTROLLINO_R0, OUTPUT);    // Set the pin CONTROLLINO_R0 as output.
    pinMode(CONTROLLINO_R1, OUTPUT);    // Set the pin CONTROLLINO_R1 as output.
    pinMode(CONTROLLINO_R2, OUTPUT);    // Set the pin CONTROLLINO_R2 as output.
    pinMode(CONTROLLINO_R3, OUTPUT);    // Set the pin CONTROLLINO_R3 as output.
    pinMode(CONTROLLINO_R4, OUTPUT);
    pinMode(CONTROLLINO_R5, OUTPUT);
    pinMode(CONTROLLINO_R6, OUTPUT);
    pinMode(CONTROLLINO_R7, OUTPUT);
    pinMode(CONTROLLINO_R8, OUTPUT);
    pinMode(CONTROLLINO_R9, OUTPUT);
    pinMode(CONTROLLINO_R10, OUTPUT);
    pinMode(CONTROLLINO_R11, OUTPUT);
    pinMode(CONTROLLINO_R12, OUTPUT);
    pinMode(CONTROLLINO_R13, OUTPUT);
    pinMode(CONTROLLINO_R14, OUTPUT);
    pinMode(CONTROLLINO_R15, OUTPUT);
  
    pinMode(CONTROLLINO_D0, OUTPUT);   
    pinMode(CONTROLLINO_D1, OUTPUT);     
    pinMode(CONTROLLINO_D2, OUTPUT);     
    pinMode(CONTROLLINO_D3, OUTPUT);     
    pinMode(CONTROLLINO_D4, OUTPUT);     
    pinMode(CONTROLLINO_D5, OUTPUT);     
    pinMode(CONTROLLINO_D6, OUTPUT);     
    pinMode(CONTROLLINO_D7, OUTPUT);     
    pinMode(CONTROLLINO_D8, OUTPUT);     
    pinMode(CONTROLLINO_D9, OUTPUT);     
    pinMode(CONTROLLINO_D10, OUTPUT);     
    pinMode(CONTROLLINO_D11, OUTPUT);     
    pinMode(CONTROLLINO_D12, OUTPUT);     
    pinMode(CONTROLLINO_D13, OUTPUT);     
    pinMode(CONTROLLINO_D14, OUTPUT);     
    pinMode(CONTROLLINO_D15, OUTPUT);     
    pinMode(CONTROLLINO_D16, OUTPUT);     
    pinMode(CONTROLLINO_D17, OUTPUT);     
    pinMode(CONTROLLINO_D18, OUTPUT); 
    pinMode(CONTROLLINO_D19, OUTPUT);     
    pinMode(CONTROLLINO_D20, OUTPUT);     
    pinMode(CONTROLLINO_D21, OUTPUT);     
    pinMode(CONTROLLINO_D22, OUTPUT);     
    pinMode(CONTROLLINO_D23, OUTPUT);     

    pinMode(CONTROLLINO_A0, INPUT);     // Set the pin CONTROLLINO_A0 as input
    pinMode(CONTROLLINO_A1, INPUT);
    pinMode(CONTROLLINO_A2, INPUT);
    pinMode(CONTROLLINO_A3, INPUT);
    pinMode(CONTROLLINO_A4, INPUT);
    pinMode(CONTROLLINO_A5, INPUT);
    pinMode(CONTROLLINO_A6, INPUT);
    pinMode(CONTROLLINO_A7, INPUT);
    pinMode(CONTROLLINO_A8, INPUT);
    pinMode(CONTROLLINO_A9, INPUT);
    pinMode(CONTROLLINO_A10, INPUT);
    pinMode(CONTROLLINO_A11, INPUT);
    pinMode(CONTROLLINO_A12, INPUT);
    pinMode(CONTROLLINO_A13, INPUT);
    pinMode(CONTROLLINO_A14, INPUT);
    pinMode(CONTROLLINO_A15, INPUT);
    
    ControllinoModbusSlave.begin( 9600 );  // Start the communication over the ModbusRTU protocol. Baund rate is set at 9600.
}

void checkResetHw(){
  if(digitalRead(CONTROLLINO_A15)==1){
    richiestoReset = true;
  }
  else{
    t1 = millis();
    richiestoReset = false;
    return;
  }
  if(richiestoReset && (millis()-t1 > ElapsedTimeToReset)){
      richiestoReset = false;
      t1 = millis();
      digitalWrite(CONTROLLINO_R9,HIGH);
      delay(5000);
      digitalWrite(CONTROLLINO_R9,LOW);
  }
}

void autoReset()
{
  if(ModbusSlaveRegisters[49]==1){
      Serial.println("RICHIESTO RESET HARDWARE");
      /*
      digitalWrite(CONTROLLINO_R9, HIGH);*/
      delay(5000);
      ModbusSlaveRegisters[49] = 0;
      digitalWrite(CONTROLLINO_R9, LOW);
}
}

// The loop function runs over and over again forever
void loop()
{
     
    // This instance of the class Modbus checks if there are any incoming data.
    // If any frame was received. This instance checks if a received frame is Ok.
    // If the received frame is Ok the instance poll writes or reads corresponding values to the internal registers (ModbusSlaveRegisters).
    // Main parameters of the instance poll are address of the internal registers and number of internal registers.
    ControllinoModbusSlave.poll(ModbusSlaveRegisters, 58);

    // While are not received or sent any data, the Modbus slave device periodically reads the values of analog and digital outputs.
    // Also it updates the other values of registers.
    //TODO USARE UN CICLO
    ModbusSlaveRegisters[0] = digitalRead(CONTROLLINO_A0);          
    ModbusSlaveRegisters[1] = digitalRead(CONTROLLINO_A1);          
    ModbusSlaveRegisters[2] = digitalRead(CONTROLLINO_A2);          
    ModbusSlaveRegisters[3] = digitalRead(CONTROLLINO_A3);          
    ModbusSlaveRegisters[4] = digitalRead(CONTROLLINO_A4);          
    ModbusSlaveRegisters[5] = digitalRead(CONTROLLINO_A5);          
    ModbusSlaveRegisters[6] = digitalRead(CONTROLLINO_A6);          
    ModbusSlaveRegisters[7] = digitalRead(CONTROLLINO_A7);          
    ModbusSlaveRegisters[8] = digitalRead(CONTROLLINO_A8);         
    ModbusSlaveRegisters[9] = digitalRead(CONTROLLINO_A9);          
    ModbusSlaveRegisters[10] = digitalRead(CONTROLLINO_A10);
    ModbusSlaveRegisters[11] = digitalRead(CONTROLLINO_A11);
    //-----------------------------------------------------------------
    ModbusSlaveRegisters[12] = digitalRead(CONTROLLINO_A12);          
    ModbusSlaveRegisters[13] = digitalRead(CONTROLLINO_A13);          
    ModbusSlaveRegisters[14] = digitalRead(CONTROLLINO_A14);          
    ModbusSlaveRegisters[15] = digitalRead(CONTROLLINO_A15);  
    //-----------------------------------------------------------------
   
    

    //-----------------------------------------------------------------

    digitalWrite(CONTROLLINO_D0, ModbusSlaveRegisters[16]);   
    digitalWrite(CONTROLLINO_D1, ModbusSlaveRegisters[17]);     
    digitalWrite(CONTROLLINO_D2, ModbusSlaveRegisters[18]);     
    digitalWrite(CONTROLLINO_D3, ModbusSlaveRegisters[19]);     
    digitalWrite(CONTROLLINO_D4, ModbusSlaveRegisters[20]);
         
    digitalWrite(CONTROLLINO_D5, ModbusSlaveRegisters[21]);
    digitalWrite(CONTROLLINO_D6, ModbusSlaveRegisters[22]);     
    digitalWrite(CONTROLLINO_D7, ModbusSlaveRegisters[23]);     
    digitalWrite(CONTROLLINO_D8, ModbusSlaveRegisters[24]);     
    digitalWrite(CONTROLLINO_D9, ModbusSlaveRegisters[25]);
         
    digitalWrite(CONTROLLINO_D10, ModbusSlaveRegisters[26]);     
    digitalWrite(CONTROLLINO_D11, ModbusSlaveRegisters[27]);     
    digitalWrite(CONTROLLINO_D12, ModbusSlaveRegisters[28]);     
    digitalWrite(CONTROLLINO_D13, ModbusSlaveRegisters[29]);     
    digitalWrite(CONTROLLINO_D14, ModbusSlaveRegisters[30]); 
        
    digitalWrite(CONTROLLINO_D15, ModbusSlaveRegisters[31]);     
    digitalWrite(CONTROLLINO_D16, ModbusSlaveRegisters[32]);     
    digitalWrite(CONTROLLINO_D17, ModbusSlaveRegisters[33]);     
    digitalWrite(CONTROLLINO_D18, ModbusSlaveRegisters[34]); 
    digitalWrite(CONTROLLINO_D19, ModbusSlaveRegisters[35]);
         
    digitalWrite(CONTROLLINO_D20, ModbusSlaveRegisters[36]);     
    digitalWrite(CONTROLLINO_D21, ModbusSlaveRegisters[37]);     
    digitalWrite(CONTROLLINO_D22, ModbusSlaveRegisters[38]);     
    digitalWrite(CONTROLLINO_D23, ModbusSlaveRegisters[39]);     
    
    digitalWrite(CONTROLLINO_R0, ModbusSlaveRegisters[40]); 
    digitalWrite(CONTROLLINO_R1, ModbusSlaveRegisters[41]); 
    digitalWrite(CONTROLLINO_R2, ModbusSlaveRegisters[42]); 
    digitalWrite(CONTROLLINO_R3, ModbusSlaveRegisters[43]); 
    digitalWrite(CONTROLLINO_R4, ModbusSlaveRegisters[44]); 
    
    digitalWrite(CONTROLLINO_R5, ModbusSlaveRegisters[45]); 
    digitalWrite(CONTROLLINO_R6, ModbusSlaveRegisters[46]); 
    digitalWrite(CONTROLLINO_R7, ModbusSlaveRegisters[47]);
    digitalWrite(CONTROLLINO_R8, ModbusSlaveRegisters[48]);
    digitalWrite(CONTROLLINO_R9, ModbusSlaveRegisters[49]);

    digitalWrite(CONTROLLINO_R10, ModbusSlaveRegisters[50]); 
    digitalWrite(CONTROLLINO_R11, ModbusSlaveRegisters[51]); 
    digitalWrite(CONTROLLINO_R12, ModbusSlaveRegisters[52]);
    digitalWrite(CONTROLLINO_R13, ModbusSlaveRegisters[53]);
    digitalWrite(CONTROLLINO_R14, ModbusSlaveRegisters[54]);
    digitalWrite(CONTROLLINO_R15, ModbusSlaveRegisters[55]);

    ModbusSlaveRegisters[56] = ControllinoModbusSlave.getInCnt();    // Read the number of incoming messages.
    ModbusSlaveRegisters[57] = ControllinoModbusSlave.getOutCnt();   // Read the number of outcoming messages

    autoReset();
    checkResetHw();
}

/* End of the example. Visit us at https://controllino.biz/ or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2017-03-31: The sketch was successfully tested with Arduino 1.8.1, Controllino Library 1.1.0 and CONTROLLINO MAXI and MEGA. */
