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
		_dbg2("Sound frame size: " << mSoundFrame.size());
	}

	std::shared_ptr<sf::Int16> mergeCBuff() {
		_dbg1("mergeCBuff");
		size_t size = 0;
		for (auto frame: mRawBuffer) {
			size += frame.size();
		}
		_dbg1("all ok with size: " << size);
		auto bigBuff = std::shared_ptr<sf::Int16>(new sf::Int16 [size], [](sf::Int16* p){_dbg1("delete bigBuff"); delete []p;});
		_dbg1("bigBuff created");
		assert(bigBuff != nullptr);

		sf::Int16 *ptr = bigBuff.get();
		for (auto frame: mRawBuffer) {
			memcpy(ptr, frame.getSample(), frame.size());
			ptr += frame.size();
		}
		assert(bigBuff != nullptr);
		return bigBuff;
	}

	size_t mergeSampleCount() {
		_dbg1("mergeSampleCount");
		size_t size = 0;
		for (auto frame: mRawBuffer) {
			size += frame.getSamplesCount();
		}
		return size;
	}

	/**
	 * Audio samples are provided to the onProcessSamples function every 100 ms.
	 * This is currently hard-coded into SFML and you can't change that
	 * (unless you modify SFML itself).
	 */
	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		unsigned int SampleRate = GetSampleRate();

		_dbg3("samples count " << SamplesCount << ", sample rate: " << SampleRate);
		createAndSaveFrameToCBuff(Samples, SamplesCount);

		auto sound = std::make_shared<cSound>(false);
		
		auto wasAlarm = sound->ProccessRecording(Samples, SamplesCount, SampleRate);

		if (wasAlarm) { 
			 assert(!mRawBuffer.empty());
			 auto bigSample = mergeCBuff();
			_dbg2("end of mergeCBuff");

			saveBuffToFile(bigSample.get(), mergeSampleCount(), SampleRate, sound->currentDateTime());
		}

		// return true to continue the capture, or false to stop it
		return true;
	}

	void saveBuffToFile(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate, std::string filename) {
		_dbg3("Start save to file");
		_dbg3("size of samples: " << sizeof(Samples));
		filename += ".wav";
		sf::SoundBuffer buff;
		buff.LoadFromSamples(Samples, SamplesCount, STEREO, SampleRate);
		_info("samples count: " << buff.GetSamplesCount() << ", duration: " << buff.GetDuration());

		_info("samples count: " << buff.GetSamplesCount() << ", duration: " << buff.GetDuration());
		if (!buff.SaveToFile(filename)) _erro(filename << " not saved :( ");
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
