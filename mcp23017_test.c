/*********************************************************************
 * mcp23017_test.c : Interface with MCP23017 I/O Extender Chip
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <sys/ioctl.h>
#include "/usr/include/linux/i2c-dev.h"
#include "/usr/include/linux/i2c.h"

#include "mcp23017.c"                             //mcp23017 handlers

const char *i2cNode1 = "/dev/i2c-1";        //linux i2c node for the Hyperpixel display on Pi 4
const char *i2cNode2 = "/dev/i2c-11";       //linux i2c node for the Hyperpixel display on Pi 4
int  mcp23017_addr = 0x20;	            //MCP23017 I2C Address



// Configure the MCP23017 GPIO Extender :

int mcp23017_init(int add) 
{
 int resp; 
 resp=i2c_init(i2cNode1);	               			//Initialize i2c node for a pi with normal display
 if(resp<0)                                   //not found, try for pi with Hypepixel4 display
 {
  resp=i2c_init(i2cNode2);	               		//Initialize i2c node fo4r a pi with normal display
   if(resp<0)
    {
     return -1;                               //neither found so return error
    }
 }
 resp= mcp23017_readport(mcp23017_addr,GPIOA);      //dummy read to check if device is present
 if(resp<0)
 {
   return -1;
 }
 mcp23017_writereg(add,IODIR,GPIOA,0xFF);      //Port A all inputs
 mcp23017_writereg(add,GPPU,GPIOA,0xFF);      //Port A all pullups enabled
 mcp23017_writereg(add,IODIR,GPIOB,0x00);      //Port B all outputs  
 mcp23017_writeport(add,GPIOB,0);             //Zero all outputs 
 return 0;
}




// Main program :

int main(int argc,char **argv) 
{
int value;
	value=mcp23017_init(mcp23017_addr);			      // Configure MCP23017  
  
  if(value<0)
  {
     printf("Error Failed to open MCP23017\n");
     return -1;
  }
  
while(1)
{
value=mcp23017_readbit(mcp23017_addr,GPIOA,0);
mcp23017_writebit(mcp23017_addr,GPIOB,7,value);
printf("value= %d\n",value);
}


	i2c_close();				/* Close I2C driver */
}

