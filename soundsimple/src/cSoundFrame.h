#ifndef CSOUNDFRAME_H
#define CSOUNDFRAME_H

#include "libs.h"

using namespace std;

class cSoundFrame
{
	sf::Int16 *mSample;
	chrono::system_clock::time_point mBuffStarTime;
	chrono::system_clock::time_point mAlarmLastTime;
	bool mInEvent;
	bool mInAlarm;
	
public:
	void addFrame(sf::Int16 *pSample);
};

#endif
