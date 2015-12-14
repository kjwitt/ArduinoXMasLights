#include "Sparkfun_Spectrum_Shield.h"

Sparkfun_Spectrum_Shield::Sparkfun_Spectrum_Shield()
{
	reset_pin_ = 5;
	strobe_pin_ = 4;
	right_pin_ = 1;
	left_pin_ = 0;
}

Sparkfun_Spectrum_Shield::Sparkfun_Spectrum_Shield(int reset,int strobe,int right,int left)
{
	reset_pin_ = reset;
	strobe_pin_ = strobe;
	right_pin_ = right;
	left_pin_ = left;
}

void Sparkfun_Spectrum_Shield::Init()
{

	//Initialize pins for Spectrum Analyzer
	pinMode(reset_pin_, OUTPUT);
	pinMode(strobe_pin_, OUTPUT);
	digitalWrite(strobe_pin_, HIGH);
	digitalWrite(reset_pin_, HIGH);

  	//Initialize spectrum analyzer
	digitalWrite(strobe_pin_, LOW);
	delay(1);
	digitalWrite(reset_pin_, HIGH);
	delay(1);
	digitalWrite(strobe_pin_, HIGH);
	delay(1);
	digitalWrite(strobe_pin_, LOW);
	delay(1);
	digitalWrite(reset_pin_, LOW);
	delay(5);
}

void Sparkfun_Spectrum_Shield::Update()
{
	// Band 0 = Lowest Frequencies
	byte band;
	for (band = 0; band < 7; band++)
	{
    	right_channel[band] = (analogRead(left_pin_) + analogRead(left_pin_) ) >> 1; //Read twice and take the average by dividing by 2
    	left_channel[band] = (analogRead(right_pin_) + analogRead(right_pin_) ) >> 1; //Read twice and take the average by dividing by 2
    	average[band] = (right_channel[band]+left_channel[band])/2;
    	digitalWrite(strobe_pin_, HIGH);
    	digitalWrite(strobe_pin_, LOW);
	}
}