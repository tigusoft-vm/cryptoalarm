#ifndef CSOUNDFRAME_H
#define CSOUNDFRAME_H

#include "libs.h"

using namespace std;

class cSoundFrame
{
	vector<sf::Int16> SamplesVec;

public:
	cSoundFrame();
	cSoundFrame(const sf::Int16 *pSample, size_t SamplesCount);
	std::chrono::system_clock::time_point mSatrtTime = std::chrono::system_clock::now();
	~cSoundFrame();

	const vector<sf::Int16>& getSamplesVec() const
	{
		return SamplesVec;
	}
	
	const std::chrono::system_clock::time_point getStartTime()
	{
		return mSatrtTime;
	}
};

#endif
