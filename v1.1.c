#include <LPC17xx.h>
#include <stdio.h> // Include stdio.h for sprintf function

unsigned int flag1 = 0;
unsigned int flag2 = 0;
#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

LPC_GPIO0->FIODIR = 0x00000FF0;
unsigned long int temp1=0,temp2=0,i,j;
unsigned char flag1L=0,flag2L=0;
unsigned char msg[]={"No of People:"};

void lcd_write();
void port_write();
void delay_lcd(unsigned int);
unsigned long int init_command[]={0x30,0x30,0x30,0x20,0x28,0x0c,0x06,0x01,0x80};
unsigned int count = 0;

void init_GPIO(void) {
    // Set GPIO directions: IR pins as inputs, LCD control pins as outputs
    LPC_GPIO0->FIODIR |= (DT_CTRL | RS_CTRL | EN_CTRL);
    LPC_GPIO0->FIODIR &= ~DT_CTRL; // IR pins as inputs
}

void init_LCD(void) {
    for (i = 0; i < 9; i++) {
        temp1 = init_command[i];
        lcd_write();
    }
}

void update_LCD_display(void) {
    flag1L = 0;
    temp1 = 0x80;
    lcd_write();
    flag1L = 1;
    sprintf(msg, "%d", count);
    i = 0;
    while (msg[i] != '\0') {
        temp1 = msg[i++];
        lcd_write();
    }
}

int main() {
    init_GPIO(); // Initialize GPIO pins
    init_LCD();  // Initialize LCD
    
    while (1) {
        SystemInit();
        SystemCoreClockUpdate();

        if (!(LPC_GPIO2->FIOPIN & (1 << 12)) && flag1 == 0) {
            flag1 = 1;
        }

        if (!(LPC_GPIO2->FIOPIN & (1 << 13)) && flag1 == 1) {
            count = count + 1;
            flag2 = 1;
        }

        if (!(LPC_GPIO2->FIOPIN & (1 << 13)) && flag2 == 0) {
            flag2 = 1;
        }

        if (!(LPC_GPIO2->FIOPIN & (1 << 12)) && flag2 == 1) {
            count = count - 1;
            flag1 = 1;
        }

        if (flag1 == 1 && flag2 == 1) {
            flag1 = 0;
            flag2 = 0;
        }

        if (count > 0) {
            LPC_GPIO2->FIOPIN = 0x00000FF0;
        } else {
            LPC_GPIO2->FIOCLR = 0x00000FF0;
        }

        update_LCD_display();
    }
}

void lcd_write(void) {
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
    temp2 = temp1 & 0xf0;
    temp2 = temp2 << 19;
    port_write();
    if (!flag2) {
        temp2 = temp1 & 0x0f;
        temp2 = temp2 << 23;
        port_write();
    }
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2;
    if (flag1 == 0) {
        LPC_GPIO0->FIOCLR = RS_CTRL;
    } else {
        LPC_GPIO0->FIOSET = RS_CTRL;
    }
    LPC_GPIO0->FIOSET = EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = EN_CTRL;
    delay_lcd(5000);
}

void delay_lcd(unsigned int r1) {
    unsigned int r;
    for (r = 0; r < r1; r++);
}
