/*
 * File:   home.c
 * Author: dell
 *
 * Created on June 20, 2018, 9:14 AM
 */


#include <xc.h>
#include <stdbool.h>
#define _XTAL_FREQ 16000000
#define RS RC0
#define RW RC1
#define EN RC2
#define dataport PORTD

void uart_init();
void lcd_on();
void enable_pulse();
void lcd_data(unsigned char);
void lcd_text(unsigned char*);
void lcd_cmd(unsigned char);
unsigned char uart_recv();

void main(void) {
    //unsigned char inst[4];
    bool bulb_status=false;
    bool fan_status=false;
    TRISD=0x00;
    TRISB=0x00;
    TRISA=0x00;
    TRISCbits.RC0=0;
    TRISCbits.RC1=0;
    TRISCbits.RC2=0;
    TRISCbits.RC7=1;
    RW=0;
    uart_init();
    lcd_on();
    while(1){
        unsigned char ch=uart_recv();
        switch(ch){
            case 'a':
                lcd_cmd(0x01);
                lcd_text("BULB ON");
                lcd_cmd(0xC0);
                PORTAbits.RA3=1;
                bulb_status=true;
                if(fan_status)
                {PORTBbits.RB7=1;
                lcd_text("FAN ON");
                }
                else
                    lcd_text("FAN OFF");
                break;
            case 'b':
                lcd_cmd(0x01);
                if(bulb_status)
                {PORTAbits.RA3=1;
                lcd_text("BULB ON");
                }
                else
                    lcd_text("BULB OFF");
                lcd_cmd(0xC0);
                lcd_text("FAN ON");
                PORTBbits.RB7=1;
                fan_status=true;
                break;
            case 'c':
                lcd_cmd(0x01);
                lcd_text("BULB OFF");
                lcd_cmd(0xC0);
                PORTAbits.RA3=0;
                bulb_status=false;
                if(fan_status)
                {PORTBbits.RB7=1;
                lcd_text("FAN ON");
                }
                else
                    lcd_text("FAN OFF");
                break;
            case 'd':
                lcd_cmd(0x01);
                if(bulb_status)
                {  PORTAbits.RA3=1;
                 lcd_text("BULB ON");
                }
                else
                    lcd_text("BULB OFF");
                lcd_cmd(0xC0);
                lcd_text("FAN OFF");
                PORTBbits.RB7=0;
                fan_status=false;
                break;
        }
    }
    return;
}
void lcd_text(unsigned char *text)
{
    while(*text)
    {
        lcd_data(*text++);
    }
}
unsigned char uart_recv()
{
    while(RCIF==0);
    RCIF=0;
    return(RCREG);
}
void uart_init()
{
    TXSTA=0x24;
    RCSTA=0x90;
    BAUDCON=0x40;
    SPBRG=103;
}
void lcd_on()
{
    lcd_cmd(0x38);
    __delay_ms(1);
    lcd_cmd(0x0E);
    __delay_ms(1);
}
void lcd_cmd(unsigned char cmd)
{
    RS=0;
    dataport=cmd;
    enable_pulse();
}
void enable_pulse()
{
    EN=1;
    __delay_ms(5);
    EN=0;
    __delay_ms(5);
}
void lcd_data(unsigned char data)
{
    RS=1;
    dataport=data;
    enable_pulse();
}
