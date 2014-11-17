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

	void Alarm(int level);

	const bool simulation_;

	struct alarmData {
		int from;
		int to;
		double sum;
		double avg;
		double max;
	};

private:
	const std::string currentDateTime();
	void wait_for_key();
	std::shared_ptr<alarmData> Check(const std::vector<double> &mag,  int from,  int to, unsigned int SampleRate, size_t N);


	int Interpret(const std::vector<double> &mag, unsigned int SampleRate, size_t N); // hardcoded values for specific hardware

	bool IsInRange(double var, double from, double to);

};

#endif /* CSOUND_H_ */
