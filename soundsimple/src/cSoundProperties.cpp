/*
 * cSoundProperties.cpp
 *
 *  Created on: 1 gru 2014
 *     
 */

#include "cSoundProperties.h"

using namespace std;

cSoundProperties::cSoundProperties(samples mag, samples freq) :
		mag_(mag), freq_(freq), intervals_(20)
{
	_dbg1("freq size = " << freq_.size() << ", mag size = " << mag_.size());
	size_ = std::min(mag_.size(), freq.size());
	//generateChatacteristicData(mag);

	auto vec = chunkOfMag(300, 2000);
}

cSoundProperties::~cSoundProperties()
{
	// TODO Auto-generated destructor stub
}


samples cSoundProperties::chunkOfMag(int from, int to) {
	assert(to > from);
	assert(to <= freq_.size());
	assert(from >= 0);

	samples chunkMag;
	double maxMag = 0.;
	for (auto f : freq_) {
		if (f >= from && f < to) {
			assert(mag_.size() > f);
			chunkMag.push_back(mag_.at(f));
			maxMag = std::max(maxMag, mag_.at(f));
		}
	}
	_dbg1(maxMag);
	return chunkMag;
}
