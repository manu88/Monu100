#include "Tlc5940.h"

// TLC5940/Arduino mega pins
/*
VPRG  : D50->tlc27
XERR  : D10->tlc16
SIN   : D51->tlc26
SCLK  : D52->tlc25
XLAT  : D11->tlc24
BLANK : D12->tlc23
GSCLK : D9->tlc18
*/



#define MIC_OUTPUT_ENABLE 30
#define MIC_STROBE 28
#define MIC_CLOCK 26
#define MIC_DATA 24

//MIC pins
/*
LOGIC SUPPLY - vcc
LOAD SUPPLY - vcc
GROUND - gnd
OUT1-8 - led - 500R - gnd
*/

#define WIDTH 15
#define HEIGHT 30
#define GS_MAX 4095

int bright = 4095;
int values[WIDTH][HEIGHT];

void setup() {

  pinMode(MIC_OUTPUT_ENABLE, OUTPUT);
  pinMode(MIC_STROBE, OUTPUT);
  pinMode(MIC_CLOCK, OUTPUT);
  pinMode(MIC_DATA, OUTPUT);

  Tlc.init();
  
  for(int i=0; i<HEIGHT; i++)
  {
  	for(int j=0; j<WIDTH; j++)
  	{
  		values[j][i]=0;
  	}
  }
  values[0][0]=GS_MAX;
  values[1][1]=GS_MAX;
  values[2][0]=GS_MAX;
  values[3][1]=GS_MAX;
}


void loop()
{	

	for(int i=0; i<HEIGHT; i++)
	{
		digitalWrite(MIC_OUTPUT_ENABLE, HIGH); 
		//Tlc.clear();
		for(int j=0; j<WIDTH; j++)
		{
			int val = values[j][i];
			Tlc.set(j,val);
		}
		Tlc.update();		
		delayMicroseconds(900);		
		if(i==0)  
		{
			mic_transfer_byte(0,0,0,0,0,0,0,1);
		}
		else if(i==1)
		{
			mic_transfer_byte(0,0,0,0,0,0,1,0);	
		}
		else 
		{
			mic_transfer_byte(0,0,0,0,0,0,0,0);	
		}
		digitalWrite(MIC_OUTPUT_ENABLE, LOW);
		delayMicroseconds(50);
	}


	// mic_transfer_byte(0,0,0,0,0,0,0,0);
	// delay(500);
	// mic_transfer_byte(0,0,0,0,0,0,0,1);
	// delay(500);
	// mic_transfer_byte(0,0,0,0,0,0,1,0);
	// delay(500);
	// mic_transfer_byte(0,0,0,0,0,0,1,1);
	// delay(500);
}

//


void mic_transfer_byte(uint8_t a,uint8_t b,uint8_t c,uint8_t d,uint8_t e,uint8_t f,uint8_t g,uint8_t h){
  //digitalWrite(MIC_OUTPUT_ENABLE, HIGH);  
  mic_transfer_bit(a);
  mic_transfer_bit(b);
  mic_transfer_bit(c);
  mic_transfer_bit(d);
  mic_transfer_bit(e);
  mic_transfer_bit(f);
  mic_transfer_bit(g);
  mic_transfer_bit(h);
  digitalWrite(MIC_STROBE, HIGH);
  digitalWrite(MIC_STROBE, LOW);
  //digitalWrite(MIC_OUTPUT_ENABLE, LOW);
}

void mic_transfer_bit(uint8_t bit){
  digitalWrite(MIC_DATA, bit ? HIGH : LOW);
  digitalWrite(MIC_CLOCK, HIGH);
  digitalWrite(MIC_CLOCK, LOW);
}

