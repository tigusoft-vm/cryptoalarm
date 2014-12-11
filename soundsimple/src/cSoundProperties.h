/*
 * cSoundProperties.h
 *
 *  Created on: 1 gru 2014
 *     
 */

#ifndef CSOUNDPROPERTIES_H_
#define CSOUNDPROPERTIES_H_

#include "libs.h"

typedef std::vector<double> samples;

class cSoundProperties {
public:
	cSoundProperties(samples mag, samples freq);
	virtual ~cSoundProperties();

	void improve(const samples &mag, const samples &freq);
	void display() const;

	const int intervals_;

private:
	const samples mag_;
	const samples freq_;

	void getDataFromChunk(samples::iterator fromIt, samples::iterator toIt);
	void generateChatacteristicData(samples &mag);


	samples avgs_;
	samples sums_;
	samples maxs_;

	size_t size_;
};

#endif /* CSOUNDPROPERTIES_H_ */
