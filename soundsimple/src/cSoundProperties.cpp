/*
 * cSoundProperties.cpp
 *
 *  Created on: 1 gru 2014
 *     
 */

#include "cSoundProperties.h"

using namespace std;

cSoundProperties::cSoundProperties(samples mag, samples freq) :
		mag_(mag), freq_(freq), intervals_(100)
{
	_dbg1("freq size = " << freq_.size() << ", mag size = " << mag_.size());
	size_ = std::min(mag_.size(), freq.size());
	//generateChatacteristicData(mag);

}

cSoundProperties::~cSoundProperties()
{
	// TODO Auto-generated destructor stub
}

void cSoundProperties::generateChatacteristicData() {
	for (int i = 0; i < size_ - intervals_; i += intervals_) {
		auto chunk = chunkOfMag(i, i + intervals_);

	}
}


samples cSoundProperties::chunkOfMag(int from, int to) {
	assert(to > from);
	assert(to <= freq_.size());
	assert(from >= 0);

	samples chunkMag;
	double maxMag = 0.;
	double sum=0.;
	for (auto f : freq_) {
		if (f >= from && f < to) {
			assert(mag_.size() > f);
			chunkMag.push_back(mag_.at(f));
			maxMag = std::max(maxMag, mag_.at(f));
			sum += mag_.at(f);
		}
	}
	avgs_.push_back(sum / chunkMag.size());
	sums_.push_back(sum);
	maxs_.push_back(maxMag);

	return chunkMag;
}
