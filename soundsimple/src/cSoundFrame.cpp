#include "cSoundFrame.h"
#include "libs.h"

void cSoundFrame::addFrame(const sf::Int16 *pSample, size_t SamplesCount) {
	mSample = new sf::Int16[size(pSample)];
    memcpy(mSample, pSample, sizeof pSample);
    this->SamplesCount = SamplesCount;
}

cSoundFrame::cSoundFrame(const cSoundFrame &other)
{
	//_dbg1("cpy constructor");
	this->SamplesCount = other.SamplesCount;
	this->mSample = new sf::Int16 [sizeof(other.mSample) / sizeof(sf::Int16)];
	memcpy(this->mSample, other.mSample, sizeof(other.mSample));
}

cSoundFrame::~cSoundFrame()
{
    delete []mSample;
}

cSoundFrame::cSoundFrame(const sf::Int16* pSample, size_t SamplesCount) {
	addFrame(pSample, SamplesCount);
}

size_t cSoundFrame::size() {
	return size(mSample);
}

size_t cSoundFrame::size(const sf::Int16* sample) {
	if(sample == nullptr) _erro("this pointer is nullptr!!");
	return sizeof(sample) / sizeof(signed short);
}
