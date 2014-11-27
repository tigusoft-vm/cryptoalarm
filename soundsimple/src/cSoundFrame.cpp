#include "cSoundFrame.h"
#include "libs.h"

void cSoundFrame::addFrame(const sf::Int16 *pSample)
{
    memcpy(mSample, pSample, sizeof pSample);
}

cSoundFrame::~cSoundFrame()
{
    delete []mSample;
}
