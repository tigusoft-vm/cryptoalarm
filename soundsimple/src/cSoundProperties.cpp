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
}

cSoundProperties::~cSoundProperties()
{
	// TODO Auto-generated destructor stub
}

void cSoundProperties::generateChatacteristicData(const samples &mag) {
	// 0 ... 20 ... 40 ... 60
	for (int i = 0; i < mag.size(); i += intervals_) {
		samples::iterator toIt;

		// less than intervals_ elements in mag_ vector
		if (i >= mag.size()) toIt = mag.end();
		else
			toIt = mag.begin() + intervals_ + i;

		auto fromIt = mag.begin() + i;
		getDataFromChunk(fromIt, toIt);
	}
}

void cSoundProperties::getDataFromChunk(samples::iterator fromIt, samples::iterator toIt) {
	double avg = 0., sum = 0., maxv = 0.;
	int i = 0;

	for (auto it = fromIt; it < toIt; ++it) {
		maxv = std::max(maxv, *it);
		sum += *it;
		++i;
	}

	avg = sum / i;

	avgs_.push_back(avg);
	sums_.push_back(sum);
	maxs_.push_back(maxv);
}
