/*
 * cSound.h
 *
 *  Created on: 17 lis 2014
 *      Author: abby
 */

#ifndef CSOUND_H_
#define CSOUND_H_

#include "libs.h"

class cSound {
public:
	cSound(bool s);
	virtual ~cSound();

	void ProccessRecording(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate);
	void ProccessRecordingTest(const sf::SoundBuffer Buffer);

	void Check(const std::vector <double> &mag, const unsigned int &from, const unsigned int &to);

	const bool simulation_;


private:
	const std::string currentDateTime();
	void wait_for_key();


};

#endif /* CSOUND_H_ */
