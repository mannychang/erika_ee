#include "touch.h"

/****************************************************************************
  Global variables
****************************************************************************/

EE_UINT16 X_pos;
EE_UINT16 Y_pos;

// Array for X/Y-Coordinates
short int Reading_X[SAMPLES_FOR_ONE_TRUE_XY_PAIR];
short int Reading_Y[SAMPLES_FOR_ONE_TRUE_XY_PAIR];
volatile unsigned char i_array;
static short int Reading_low_level;
unsigned int horiz_width,vert_height;
volatile unsigned int Untouch_conditions;
volatile TouchFlow tf;
volatile char raw_ready;
unsigned int X_raw,Y_raw;

double cal_a,cal_b,cal_c;
double cal_d,cal_e,cal_f;

//unsigned char x_panel,y_panel;
volatile unsigned int x,y;

// computation begins
volatile unsigned int xd1,yd1;
volatile unsigned int xd2,yd2;
volatile unsigned int xd3,yd3;
volatile unsigned int xt1,yt1;
volatile unsigned int xt2,yt2;
volatile unsigned int xt3,yt3;

/**************************************************************************/
#ifndef __USE_LCD__

EE_UINT16 temp_count = 0;

void Delay( unsigned int delay_count )
{
	temp_count = delay_count +1;
	asm volatile("outer: dec _temp_count");
	asm volatile("cp0 _temp_count");
	asm volatile("bra z, done");
	asm volatile("do #3200, inner" );
	asm volatile("nop");
	asm volatile("inner: nop");
	asm volatile("bra outer");
	asm volatile("done:");
}

#endif // __USE_LCD__

/**************************************************************************/

void touch_adc_init(void)
{
	AD1CHS0 = STANDBY_PIN;
	AD1CON1bits.ASAM = 1;
	AD1CON1bits.FORM = 0;
	AD1CON1bits.SSRC = 0;
	AD1CON1bits.AD12B = 1;
	AD1CON2 = 0;
	AD1CON3bits.SAMC = 16;
	AD1CON3bits.ADRC = 0;
	AD1CON3bits.ADCS = 3;
	IFS0bits.AD1IF = 0;		// reset ADC interrupt flag
	IEC0bits.AD1IE = 1;		// enable ADC interrupts, disable this interrupt if the DMA is enabled
	ADC_TURN_ON;			// turn on ADC
}

/*void touch_set_dimensions(
		unsigned int pixel_horizontal_width,
		unsigned int pixel_vertical_height)
{
	horiz_width = pixel_horizontal_width;
	vert_height = pixel_vertical_height;

}*/

void touch_set_dimension(
		EE_UINT8 touch_axis,
		EE_UINT16 touch_range)
{
	if(touch_axis == TOUCH_X_AXIS)
	{
		horiz_width = pixel_horizontal_width;
	}
	else if(touch_axis == TOUCH_Y_AXIS)
	{
		vert_height = pixel_vertical_height;
	}
}


void touch_start()
{

#ifdef __USE_TOUCH_ALTERNATE__
	ClearBit(FAKE_PULL_UP_DDR,EE_T_FAKE_PULL_UP); // Set fake pull-up pin (RD8) as output
	SetBit(FAKE_PULL_UP_OUTPUT,EE_T_FAKE_PULL_UP); // Light up fake pull-up pin.
#endif
	touch_adc_init();
	Untouch_conditions = 0;
	X_raw = 0;
	Y_raw = 0;
	raw_ready = 0;
	i_array = 0;
	STANDBY_CONFIGURATION;
	tf.STANDBY = 1;
	SetRelAlarm(Alarm_Touch_Manager,10,10);
}

void touch_stop()
{
	CancelAlarm(Alarm_Touch_Manager);
}

void touch_calibrate()
{
	// touch input P1 (90%,50%)
	xd1 = (unsigned int)((float)horiz_width*0.9);
	yd1 = (unsigned int)((float)vert_height*0.5);
	// touch input P2 (50%,10%)
	xd2 = (unsigned int)((float)horiz_width*0.5);
	yd2 = (unsigned int)((float)vert_height*0.9);
	// touch input P3 (10%,90%)
	xd3 = (unsigned int)((float)horiz_width*0.1);
	yd3 = (unsigned int)((float)vert_height*0.1);

	/*

	EE_touch_stop();
	Delay(Delay_1S_Cnt/3);
	EE_touch_start();

	//  Outing: Touch P1!

	EE_led_on();
	raw_ready = 0;
	while(!raw_ready);
	xt1 = X_raw;
	yt1 = Y_raw;
	raw_ready = 0;
	EE_led_off();

	EE_touch_stop();
	Delay(Delay_1S_Cnt/3);
	EE_touch_start();

	//  Outing: Touch P2!

	EE_led_on();
	while(!raw_ready);
	xt2 = X_raw;
	yt2 = Y_raw;
	raw_ready = 0;
	EE_led_off();

	EE_touch_stop();
	Delay(Delay_1S_Cnt/3);
	EE_touch_start();

	//  Outing: Touch P2!

	EE_led_on();
	while(!raw_ready);
	xt3 = X_raw;
	yt3 = Y_raw;
	raw_ready = 0;
	EE_led_off();

	EE_touch_stop();

	*/

	/*xt1 = 2990;
	yt1 = 1890;
	xt2 = 2110;
	yt2 = 2770;
	xt3 = 1300;
	yt3 = 1040;*/

	xt1 = 3104;
	yt1 = 1944;
	xt2 = 2102;
	yt2 = 2876;
	xt3 = 1128;
	yt3 = 988;

	cal_a = ((long)yt1*xd3-(long)yt1*xd2-(long)yt2*xd3+(long)xd2*yt3-(long)xd1*yt3+(long)xd1*yt2);
	cal_a /= (-(long)xt1*yt3+(long)xt2*yt3-(long)xt2*yt1+(long)xt3*yt1-(long)xt3*yt2+(long)xt1*yt2);

	cal_b = cal_a*((float)xt3-xt2)+xd2-xd3;
	cal_b /= (yt2-yt3);

	cal_c = xd3-cal_a*xt3-cal_b*yt3;

	cal_d = -((long)yt2*yd3-(long)yt2*yd1-(long)yt1*yd3-(long)yt3*yd2+(long)yt3*yd1+(long)yt1*yd2);
	cal_d /= (-(long)yt2*xt3+(long)yt2*xt1+(long)yt1*xt3+(long)yt3*xt2-(long)yt3*xt1-(long)yt1*xt2);

	cal_e = cal_d*((float)xt3-xt2)+yd2-yd3;
	cal_e /= yt2-yt3;

	cal_f = yd3-cal_d*xt3-cal_e*yt3;
}

#ifdef __LOW_LEVEL_MEASUREMENT__
TASK(Touch_Manager)
{
	if(CONVERSION_DONE)
	{
		CONVERSION_RESET;
		if(tf.STANDBY)
		{
			Reading_low_level = ADC1BUF0;
			if(Reading_low_level < MAXIMUM_LOW_LEVEL)
			{
				tf.STANDBY = 0;
				tf.YPOS = 1;
				if(tf.COMPLETE)
				{
					store_valid_data();
				}
				Y_POS_CONFIGURATION;
				AD1CHS0 = ADC_Y;
				Untouch_conditions = 0;
			} else
			{
				tf.COMPLETE = 0;
				if(MAXIMUM_UNTOUCH_CONDITIONS == Untouch_conditions )
				{
					X_raw = 0;
					Y_raw = 0;
				} else Untouch_conditions++;
			}

		} else if(tf.YPOS)
		{
			Reading_Y[i_array] = ADC1BUF0;
			tf.YPOS = 0;
			tf.XPOS = 1;
			X_POS_CONFIGURATION;
			AD1CHS0 = ADC_X;
		} else if(tf.XPOS)
		{
			Reading_X[i_array] = ADC1BUF0;
			tf.XPOS = 0;
			tf.STANDBY = 1;
			tf.COMPLETE = 1;
			STANDBY_CONFIGURATION;
			AD1CHS0 = STANDBY_PIN;
		}
	} else
	{
		START_CONVERSION;
	}
}
#endif // __LOW_LEVEL_MEASUREMENT__

ISR2(_ADC1Interrupt)
{
	IFS0bits.AD1IF = 0;
	ActivateTask(Touch_Manager);
}

void sorted_insertion(short int Array[])
{
	signed char i;
	unsigned char j;
	short int current;

	// Sort Array[]
	for(j=1; j<SAMPLES_FOR_ONE_TRUE_XY_PAIR; j++)
	{
		  current = Array[j];
		  i = j - 1;
		  while(i>=0 && Array[i]>current)
		  {
				Array[i+1] = Array[i];
				i--;
		  }
		  Array[i+1] = current;
	}
}

void store_valid_data(void)
{
        // Store valid Data in Array

	if(i_array==(END_OF_ARRAY))
    {
		// Array Management

		// Sort X/Y Readings To Get Median
		sorted_insertion(Reading_X);
		sorted_insertion(Reading_Y);

		X_raw = Reading_X[MEDIAN];
		Y_raw = Reading_Y[MEDIAN];
		raw_ready = 1;

		// Translation to (PX,PY)

		//GetResource(mutex_posRead);

		X_pos = cal_a*X_raw+cal_b*Y_raw+cal_c;
		Y_pos = cal_d*X_raw+cal_e*Y_raw+cal_f;
		
		if(X_pos<0)
			X_pos = 0;
		else if(X_pos>horiz_width-1)
			X_pos = horiz_width-1;
			
		if(Y_pos<0)
			Y_pos = 0;
		else if(Y_pos>vert_height-1)
			Y_pos = vert_height-1;
				
		/*uposRead.x = cal_a*X_raw+cal_b*Y_raw+cal_c;
		uposRead.y = cal_d*X_raw+cal_e*Y_raw+cal_f;

		sposRead.x = (signed int)uposRead.x - horiz_width/2;
		sposRead.y = (signed int)uposRead.y - vert_height/2;

		if(uposRead.x > horiz_width)
			uposRead.x = horiz_width;
		if(uposRead.y > vert_height)
			uposRead.y = vert_height;*/

		//ReleaseResource(mutex_posRead);

		i_array = 0;

    } else	i_array++;
}

EE_UINT16 touch_get_position(EE_UINT8 axis)
{
	if(axis==TOUCH_X_AXIS)
		return X_pos;
	else if(axis==TOUCH_Y_AXIS)
		return Y_pos;	
}
