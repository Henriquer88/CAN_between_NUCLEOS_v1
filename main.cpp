#include "mbed.h"

//-------Declara uso da serial---------------------
Serial pc(USBTX, USBRX); // tx, rx

//--------Declara uso da CAN----------------------
CAN can1(PB_8, PB_9);

//--------Declara uso do LED nativo da placa------
DigitalOut LED(PA_5);

//-----Declara uso do botao nativo da placa--------
DigitalIn SW(PC_13);

//------Dado da mensagem chamada de info que sera usada---------------
static char info =  0x55;

int main() 
{
    //------construção da mensagem CAN (ID, dado, tamanho em bits, Tipo de dado, Modo CAN)--- 
    CANMessage msg(5, &info, 8, CANData, CANStandard);
    
    //pisca o led
    LED = 1;
    wait(0.5);
    LED = 0;
    wait(0.5);
    
    //---boas vindas------------
    pc.printf("Ola!!!\n");
    
    while(1) 
    {
        //---checa se foi apertado o botão, com debounce
        if(SW == 0)
        {
            wait_ms(200);
            if(SW == 0)
            {
                //se a mensagem foi enviada
                if(can1.write(msg))
                {
                    pc.printf("Mensagem enviada\n");            
                }
                
                //se a mensagem nao foi enviada
                else
                {
                    pc.printf("Mensagem NAO enviada\n");                   
                }               
            }
        } 
    
        //se a mensagem for recebida
        if(can1.read(msg)) 
        {
            //printa o valor da mensagem
            pc.printf("Mensagem recebida: %x\n", msg.data[0]);  
            //pisca o led
            for(int i = 0; i < 5; i++)
            {
                LED = 1;
                wait_ms(300);
                LED = 0;
                wait_ms(300);
            }
        }
    }
}
