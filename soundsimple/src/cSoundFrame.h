#ifndef CSOUNDFRAME_H
#define CSOUNDFRAME_H

#include "libs.h"

using namespace std;

class cSoundFrame
{
	sf::Int16 *mSample;
	chrono::system_clock::time_point mBuffStarTime;
    //chrono::system_clock::time_point mAlarmLastTime;
    //bool mInEvent;
    //bool mInAlarm;
	size_t size(const sf::Int16 *sample);
	size_t SamplesCount;
	
public:
    void addFrame(const sf::Int16 *pSample, size_t SamplesCount);
    cSoundFrame(const sf::Int16 *pSample, size_t SamplesCount);
    cSoundFrame(const cSoundFrame &other);
    ~cSoundFrame();

	sf::Int16* getSample() const {
		return mSample;
	}

	size_t size();
	
	size_t getSamplesCount() {
		return SamplesCount;
	}
};

#endif
