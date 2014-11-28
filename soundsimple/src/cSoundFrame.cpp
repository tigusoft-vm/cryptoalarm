#include "cSoundFrame.h"
#include "libs.h"

cSoundFrame::~cSoundFrame() {
	// TODO
}

cSoundFrame::cSoundFrame(const sf::Int16* pSample, size_t SamplesCount) {
	assert(pSample != NULL);
	for (int i = 0; i < SamplesCount; ++i)
		SamplesVec.push_back(pSample[i]);
}

