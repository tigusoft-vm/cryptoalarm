#ifndef CSOUNDFRAME_H
#define CSOUNDFRAME_H

#include "libs.h"

using namespace std;

class cSoundFrame
{
	//	chrono::system_clock::time_point mBuffStarTime;
	//chrono::system_clock::time_point mAlarmLastTime;
	//bool mInEvent;
	//bool mInAlarm;
	vector<sf::Int16> SamplesVec;

public:
	cSoundFrame(const sf::Int16 *pSample, size_t SamplesCount);
	//    cSoundFrame(const cSoundFrame &other);
	~cSoundFrame();

	const vector<sf::Int16>& getSamplesVec() const
	{
		return SamplesVec;
	}
};

#endif
