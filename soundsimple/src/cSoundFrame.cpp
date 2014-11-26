#include "cSoundFrame.h"

void cSoundFrame::addFrame(sf::Int16 *pSample)
{
	memcpy(mSample, pSample, sizeof(pSample));
}
