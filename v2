#include<LPC17xx.h>
unsigned int f1 = 0;
unsigned int f2 = 1;

#define RS_CTRL 0x08000000
#define EN_CTRL 0x10000000
#define DT_CTRL 0x07800000
unsigned long int temp1=0,temp2=0,i,j;
unsigned char flag1=0,flag2=0;
unsigned char msg[]={"WELCOME"};
void lcd_write();
void port_write();
void delay_lcd(unsigned int);
unsigned long int init_command[]={0x30,0x30,0x30,0x20,0x28,0x0c,0x06,0x01,0x80};

//IR SENSOR 1 Connected to P2.12
//IR SENSOR 2 Connected to P2.13


LPC_GPIO2->FIODIR = 0x00000FF0; //for LEDs
unsigned int space = 10;        // No of people that a room can accomodate
unsigned int NO_OF_PEOPLE = 0;  // No of people inside the room initially 0

int main(){
while(1)
{

   SystemInit();
    SystemCoreClockUpdate();
    LPC_GPIO0->FIODIR=DT_CTRL | RS_CTRL | EN_CTRL;
    flag1=0;
    for (i=0;i<9;i++){
        temp1=init_command[i];
        lcd_write();
    }
    if((LPC_GPIO2->FIOPIN & (1<<12)) && f1 == 0)
    {
        f1=1;
        /*if(space > 0)
        {
            flag1 = 1;
            if(flag2 == 0)
            {
                space = space - 1;
                NO_OF_PEOPLE += 1;
                if(NO_OF_PEOPLE > 1)
                {
                    LPC_GPIO2-FIOPIN = 0x00000FF0;
                }
            }
        }*/
    }
     if((LPC_GPIO2->FIOPIN&(1<<13))&&(f2==0))
    {
        f2=1;
    }
    if((LPC_GPIO2->FIOPIN&(1<<13))&&(f1==1))
    {
        f2=0;
        if(space > 0)
        {
            
            {
                space = space - 1;
                NO_OF_PEOPLE += 1;
                if(NO_OF_PEOPLE > 1)
                {
                    LPC_GPIO2-FIOPIN = 0x00000FF0;
                }
            }
        }
         if(NO_OF_PEOPLE > 0)
    {
        LPC_GPIO2->FIOPIN = 0x00000FF0;
        
    }

    else
    {
        NO_OF_PEOPLE = 0;
        LPC_GPIO2-FIOCLR = 0x00000FF0;
        
    }
        
    }

    if((LPC_GPIO2->FIOPIN&(1<<12))&&(f2==1))
    {
        f1=0;
          space = space + 1;
         NO_OF_PEOPLE -= 1;
          if(NO_OF_PEOPLE > 0)
    {
        LPC_GPIO2->FIOPIN = 0x00000FF0;
        
    }

    else
    {
        NO_OF_PEOPLE = 0;
        LPC_GPIO2-FIOCLR = 0x00000FF0;
        
    }
    }
   
    
   flag1=0;
   temp1=0X80;
   lcd_write();
   flag1=1;
   sprintf(msg,"%d",NO_OF_PEOPLE);
   i=0;
   while(msg[i]!='/0')
   {
    temp1=msg[i++];
    lcd_write();
   }
   flag1=0;
   temp1=0XC0;
   lcd_write();
   flag1=1;
   sprintf(msg,"%d",space);
   i=0;
   while(msg[i]!='/0')
   {
    temp1=msg[i++];
    lcd_write();
   }

    /////////////LCD CODE TO DISPLAY THE SPACE AND THE NO_OF_PEOPLE//////////////////
    //////The two variables that are to displayed are "NO_OF_PEOPLE" And "Space"///////
}
}
void lcd_write(void){
    flag2=(flag1==1)?0:((temp1==0x30)||(temp1==0x20)) ? 1:0;
    temp2=temp1&0xf0;
    temp2=temp2<<19;
    port_write();
    if(!flag2){
        temp2=temp1&0x0f;
        temp2=temp2<<23;
        port_write();
    }
}
void port_write(void){
    LPC_GPIO0->FIOPIN=temp2;
    if(flag1==0){
        LPC_GPIO0->FIOCLR=RS_CTRL;
    }
    else{
        LPC_GPIO0->FIOSET=RS_CTRL;
    }
    LPC_GPIO0->FIOSET=EN_CTRL;
    delay_lcd(25);
    LPC_GPIO0->FIOCLR=EN_CTRL;
    delay_lcd(5000);
}
void delay_lcd(unsigned int r1){
    unsigned int r;
    for(r=0;r<r1;r++);
    return;
}
