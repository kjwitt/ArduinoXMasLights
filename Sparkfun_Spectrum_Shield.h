#ifndef SPARKFUN_SPECTRUM_SHIELD_H
#define SPARKFUN_SPECTRUM_SHIELD_H

#include <Arduino.h>

class Sparkfun_Spectrum_Shield
{
private:
	int reset_pin_;
	int strobe_pin_;
	int right_pin_;
	int left_pin_;

public:
	int right_channel[7];
	int left_channel[7];
	int average[7];

	void Init();
	void Update();

	Sparkfun_Spectrum_Shield();
	Sparkfun_Spectrum_Shield(int reset,int strobe,int right,int left);
};


#endif