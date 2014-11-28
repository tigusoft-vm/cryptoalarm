#include "cSoundFrame.h"
#include "libs.h"

void cSoundFrame::addFrame(const sf::Int16 *pSample) {
	mSample = new sf::Int16[size(pSample)];
    memcpy(mSample, pSample, sizeof pSample);
}

cSoundFrame::~cSoundFrame()
{
    delete []mSample;
}

cSoundFrame::cSoundFrame(const sf::Int16* pSample) {
	addFrame(pSample);
}

size_t cSoundFrame::size() {
	return size(mSample);
}

size_t cSoundFrame::size(const sf::Int16* sample) {
	if(sample == nullptr) _erro("this pointer is nullptr!!");
	return sizeof(sample) / sizeof(signed short);
}
