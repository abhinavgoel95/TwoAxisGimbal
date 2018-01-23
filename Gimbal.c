#include <reg51.h>
sbit outerServo = P3^6;
sbit innerServo = P0^7;
//NEW
sbit XEOC = P0^5;
sbit XRD = P0^ 4;
sbit XWR = P0^3;
sbit YEOC = P0^2;
sbit YRD = P0^1;
sbit YWR = P0^0;
void delay(char count)
{
	for(;count>0;count--)
	{
		TL0 = 0x18;
		TH0 = 0xFC;
		TR0 = 1;
		while(TF0 == 0);
		TR0 = 0;
		TF0 = 0;
	}
}
void delay50(char count)
{
	for(;count > 0; count--)
	{
		TL1 = 0x9C;
		TH1 = 0xFF;
		TR1 = 1;
		while(TF1 == 0);
		TR1 = 0;
		TF1 = 0;
	}
}
main()
{
	unsigned char stable;
	unsigned char accX, accY, accX1, accY1;
	stable = 38;
	P1 = 0xFF;
	P2 = 0xFF; 
	TMOD = 0x11;
	while(1)
	{
		//NEW
		XRD = 1;
		YRD = 1;		  //Read initally High
		XWR = 0;
		delay50(1);		  //To start conversion
		XWR  = 1;
		YWR = 0;
		delay50(1);		  //To start conversion
		YWR  = 1;
		while(XEOC == 1); //Wait for EOC
		XEOC = 1;
		XRD = 0;		  //Read H-L
		while(YEOC == 1); //Wait for EOC
		YEOC = 1;		  //Ready for next
		YRD = 0;		  //Read H-L
		delay50(1);

		accX = P2;
		accY = P1;
		if(accX > 0x8C)
		{
			while(accX > 0x8C)
			{
				
				//NEW
				XRD = 1;
				XWR = 0;
				delay50(1);//To start conversion
				XWR  = 1;
				while(XEOC == 1); //Wait for EOC
				XEOC = 1;
				XRD = 0;
				delay50(1);
				
				accX1 = P2;
				accX = accX1;
				outerServo = 1;
				delay(2);
				outerServo = 0;
				delay(40);
			}
			outerServo = 1;
			delay(3);
			outerServo = 0;
			delay(40);
		}
		else if(accX < 0x78)
		{
			while(accX < 0x78)
			{
				//NEW
				XRD = 1;
				XWR = 0;
				delay50(1);		  //To start conversion
				XWR  = 1;
				while(XEOC == 1); //Wait for EOC
				XEOC = 1;
				XRD = 0;
				delay50(1);

				accX1 = P2;
				accX = accX1;
				outerServo = 1;
				delay(4);
				outerServo = 0;
				delay(40);
			}
			outerServo = 1;
			delay(3);
			outerServo = 0;
			delay(40);
		}
		else
		{
			outerServo = 1;
			delay(3);
			outerServo = 0;
			delay(40);
		}
		if(accY > 0x87)
		{
			while(accY > 0x87)
			{
				stable++;
				if(stable > 47)
					stable = 38;
				innerServo = 1;
				delay50(stable);
				innerServo = 0;
				delay(40);

				//NEW
				YRD = 1;
				YWR = 0;
				delay50(1); //To start conversion
				YWR  = 1;
				while(YEOC == 1); //Wait for EOC
				YEOC = 1;
				YRD = 0;
				delay50(1);

				accY1 = P1;
				accY = accY1;
			}
		}
		else if(accY < 0x7D)
		{
			while(accY < 0x7D)
			{
				stable--;
				innerServo = 1;
				delay50(stable);
				innerServo = 0;
				delay(40);

				//NEW
				YRD = 1;
				YWR = 0;
				delay50(1); //To start conversion
				YWR  = 1;
				while(YEOC == 1); //Wait for EOC
				YEOC = 1;
				YRD = 0;
				delay50(1);
				
				accY1 = P1;
				accY = accY1;
			}
		}
	}
}

