#include <LPC17xx.h>
#include <stdio.h> // Include stdio.h for sprintf function

unsigned int flag1 = 0;
unsigned int flag2 = 0;

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000

unsigned int space = 10;        // No of people that a room can accomodate
unsigned int NO_OF_PEOPLE = 0;  // No of people inside the room initially 0
unsigned long int temp1 = 0, temp2 = 0, i;

// Function to initialize GPIO pins
void init_GPIO(void) {
    // Set GPIO directions: IR pins as inputs, LCD control pins as outputs
    LPC_GPIO0->FIODIR |= (DT_CTRL | RS_CTRL | EN_CTRL);
}

// Function to initialize LCD
void init_LCD(void) {
    unsigned long int init_command[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0c, 0x06, 0x01, 0x80};
    for (i = 0; i < 9; i++) {
        temp1 = init_command[i];
        lcd_write();
    }
}

// Function to write data to LCD
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

// Function to write to port
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

// Function for delay
void delay_lcd(unsigned int r1) {
    unsigned int r;
    for (r = 0; r < r1; r++);
}

// Function to update LCD display with space and number of people
void update_LCD_display(void) {
    char msg[20]; // Buffer to store message for LCD display
    sprintf(msg, "Space: %d, People: %d", space, NO_OF_PEOPLE);
    for (i = 0; msg[i] != '\0'; i++) {
        temp1 = msg[i];
        flag1 = 0;
        lcd_write();
    }
}

int main() {
    init_GPIO(); // Initialize GPIO pins
    init_LCD();  // Initialize LCD
    
    while (1) {
        if (!(LPC_GPIO2->FIOPIN & (1 << 12)) && flag1 == 0) {
            if (space > 0) {
                flag1 = 1;
                if (flag2 == 0) {
                    space--;
                    NO_OF_PEOPLE++;
                    if (NO_OF_PEOPLE > 1) {
                        LPC_GPIO2->FIOPIN = 0x00000FF0;
                    }
                }
            }
        } else {
            // LCD code to display that no space is available
            for (i = 0; i < 16 && "No space available"[i] != '\0'; i++) {
                temp1 = "No space available"[i];
                flag1 = 1;
                lcd_write();
            }
        }

        if (!(LPC_GPIO2->FIOPIN & (1 << 13)) && flag2 == 0) {
            flag2 = 1;
            if (flag1 == 0) {
                space++;
                NO_OF_PEOPLE--;
            }
        }

        if (flag1 == 1 && flag2 == 1) {
            flag1 = 0;
            flag2 = 0;
        }

        if (NO_OF_PEOPLE > 0) {
            LPC_GPIO2->FIOPIN = 0x00000FF0;
        } else {
            NO_OF_PEOPLE = 0;
            LPC_GPIO2->FIOPIN &= ~0x00000FF0;
        }

        update_LCD_display();
    }
}
