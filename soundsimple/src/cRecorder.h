/*
 * cRecorder.h
 *
 *  Created on: 26 lis 2014
 */

#ifndef CRECORDER_H_
#define CRECORDER_H_

#include "libs.h"
#include "cSound.h"
#include "cAlarmData.h"
#include "cSoundFrame.h"
#include <boost/circular_buffer.hpp>

class cAlarm {
};


class cAlarmSoundRecorder: public sf::SoundBufferRecorder {
	
	cAlarmData mAlarmData;
	
	boost::circular_buffer<cSoundFrame> mRawBuffer = boost::circular_buffer<cSoundFrame>(100);
	//sf::Int16 *cpyBuff;
	time_t mBuffStarTime;
	
	bool mInAlarm;
	time_t mAlarmLastTime;
	bool mInEvent;
	time_t mEventStarTime;
	
	virtual bool OnStart() {
		std::cout << "Start sound recorder" << std::endl;
		return true;
	}

	/**
	 * Audio samples are provided to the onProcessSamples function every 100 ms.
	 * This is currently hard-coded into SFML and you can't change that
	 * (unless you modify SFML itself).
	 */
	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		unsigned int SampleRate = GetSampleRate();

		//cpyBuff = new sf::Int16 [sizeof(Samples) / sizeof(sf::Int16)]
		//memcpy(cpyBuff, 
		//mRawBuffer.push_back();

		auto sound = std::make_shared<cSound>(false);
		sf::SoundBuffer buff = GetBuffer();
		buff.LoadFromSamples(Samples, SamplesCount, 2, SampleRate);

		auto wasAlarm = sound->ProccessRecording(Samples, SamplesCount, SampleRate);

		if (wasAlarm) {
			mAlarmData.add(Samples, SamplesCount, SampleRate);
			saveBuffToFile(Samples, SamplesCount, SampleRate, sound->currentDateTime());
		}


		// return true to continue the capture, or false to stop it
		return true;
	}

	void saveBuffToFile(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate, std::string filename) {
		filename += ".wav";
		sf::SoundBuffer buff = GetBuffer();

		_info("samples count: " << buff.GetSamplesCount() << ", duration: " << buff.GetDuration());
		if(!buff.SaveToFile(filename)) _erro(filename << " not saved :( ");
	}



	virtual void OnStop() {
		std::cout << "Stop sound recorder" << std::endl;
	}
	
};

class cRecorder {
public:
	cRecorder();
	virtual ~cRecorder();

	const bool fromMicrophoneMode;
	void startRecording();

private:
	cAlarmSoundRecorder Recorder;

	void waitForExitKey();
};

#endif /* CRECORDER_H_ */
