/*
 * cRecorder.h
 *
 *  Created on: 26 lis 2014
 */

#ifndef CRECORDER_H_
#define CRECORDER_H_

#include "libs.h"
#include "cSound.h"
#include "cSoundFrame.h"
#include <boost/circular_buffer.hpp>

#define CBUFF_SIZE 200
#define STEREO 2

const std::string recDirName = "recordings/";

class cAlarm {
};

class cAlarmSoundRecorder: public sf::SoundBufferRecorder {
	boost::circular_buffer<cSoundFrame> mRawBuffer = boost::circular_buffer<
			cSoundFrame>(CBUFF_SIZE);
	//std::shared_ptr<sf::Int16> mBigSample;

	virtual bool OnStart() {
		std::cout << "Start sound recorder" << std::endl;
		return true;
	}

	void createAndSaveFrameToCBuff(const sf::Int16* Samples, std::size_t SamplesCount) {
		cSoundFrame mSoundFrame(Samples, SamplesCount);
		mRawBuffer.push_back(mSoundFrame);

	}

	std::vector<sf::Int16> mergeCBuff() {
		std::vector<sf::Int16> samplesFromCBuff;
		for (auto sample : mRawBuffer) {
			auto chunk = sample.getSamplesVec();
			samplesFromCBuff.assign(chunk.begin(), chunk.end());
		}
		return samplesFromCBuff;
	}


	/**
	 * Audio samples are provided to the onProcessSamples function every 100 ms.
	 * This is currently hard-coded into SFML and you can't change that
	 * (unless you modify SFML itself).
	 *
	 * SamplesCount = size of Samples
	 */
	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		unsigned int SampleRate = GetSampleRate();
		createAndSaveFrameToCBuff(Samples, SamplesCount);

		auto sound = std::make_shared<cSound>(false);
		auto wasAlarm = sound->ProccessRecording(Samples, SamplesCount, SampleRate);
		if (wasAlarm) {
			assert(!mRawBuffer.empty());
			auto vecOfSamples = mergeCBuff();
			saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime());
		}

		// return true to continue the capture, or false to stop it
		return true;
	}

	void saveBuffToFile(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate, std::string filename) {
		assert(Samples != nullptr && SamplesCount > 0);
		_dbg3("Start save to file");
		_dbg3("size of samples: " << sizeof(Samples));
		filename += ".wav";
		sf::SoundBuffer buff;
		buff.LoadFromSamples(Samples, SamplesCount, STEREO, SampleRate);

		_info("samples count: " << buff.GetSamplesCount() << ", duration: " << buff.GetDuration());
		if (!buff.SaveToFile(recDirName+filename)) _erro(filename << " not saved :( ");
	}

	virtual void OnStop() {
		std::cout << "Stop sound recorder" << std::endl;
	}

};

class cRecorder {
public:
	cRecorder();
	virtual ~cRecorder();
	void startRecording();

	const bool fromMicrophoneMode;

private:
	cAlarmSoundRecorder Recorder;

	void waitForExitKey();
};

#endif /* CRECORDER_H_ */
