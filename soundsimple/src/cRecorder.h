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
#include "cSend.h"
#include "cFile.h"
#include "chainsign/ckeysstorage.h"
#include <boost/circular_buffer.hpp>

#define CBUFF_SIZE 200
#define STEREO 2
#define MONO 1
#define EVENT_TIME 10
#define FIRST_FILES_TIME 5
#define NEXT_FILES_TIME 10
#define SAVING_LOG "saving_buff"
#define KEY_SIZE 2048

const std::string recDirName = "recordings/";
extern bool FLAG_signalHandler;

class cAlarm {
};

class cAlarmSoundRecorder: public sf::SoundBufferRecorder {
public:
	void setSimulationMode(bool simulationMode = false){
		this->simulationMode = simulationMode;
		_dbg3(simulationMode);
	}
	cAlarmSoundRecorder();
private:
	boost::circular_buffer<cSoundFrame> mRawBuffer = boost::circular_buffer<
			cSoundFrame>(CBUFF_SIZE);

	std::chrono::steady_clock::time_point mAlarmLastTime;
	std::chrono::steady_clock::duration diffToAlarm;

	bool isEvent = false;
	bool savedMinusFile = false;
	bool simulationMode = false;
	unsigned int mSavedFiles = 0;
	std::string message = "";
	
	// chainsign
	cKeysStorage mKeysStorage;
	
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
		samplesFromCBuff.reserve(1000000);
		time_t tt;
		tt = std::chrono::system_clock::to_time_t(mRawBuffer.at(0).getStartTime());
		for (auto sample : mRawBuffer) {
			auto chunk = sample.getSamplesVec();
			for (auto element : chunk)
				samplesFromCBuff.push_back(element);
		}
		return samplesFromCBuff;
	}

	void saveBuffToFile(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate, std::string date, std::string mess) {
		_mark("saveBuffToFile");
		for (int i = 0; i < SamplesCount; ++i) {
			//_mark("saveBuffToFile" << Samples[i]);
		}
		assert(Samples != nullptr && SamplesCount > 0);

		sf::SoundBuffer buff;
		buff.loadFromSamples(Samples, SamplesCount, MONO, SampleRate);
		assert(buff.getDuration() != sf::Time::Zero);
		int numberOfGoodSamples = 0;
		for (int i = 0; i < buff.getSampleCount(); ++i) {
			if (buff.getSamples()[i] < -10 || buff.getSamples()[i] > 10) {
				++numberOfGoodSamples;
			}
		}
		//_mark("numberOfGoodSamples " << numberOfGoodSamples);
		if (numberOfGoodSamples == 0) {
			return;
		}
		_dbg2_c(SAVING_LOG, "size of samples(get samples): " << sizeof(buff.getSamples()));
		_info_c(SAVING_LOG, "samples count: " << buff.getSampleCount() << ", duration: " << buff.getDuration().asSeconds());
		std::string filename = cFile::getFilename(date);
		_mark("filename: " << filename);
		//_mark("save buff to file " << filename);
		//_mark("sample rate " << buff.getSampleRate());
		//_mark("channel count " << buff.getChannelCount());
		if (!buff.saveToFile(filename)) _erro(filename << " not saved :( ");
		else {
			assert(this->message != "");
			_note("File saved " << filename);
			_note_c(SAVING_LOG, "File saved " << filename);
			
			_dbg1("start sign file " << filename);
			_dbg2("generate key");
			const std::string key_name(cFile::getWorkDir(filename) + "/key_" + std::to_string(mKeysStorage.getCurrentKey()) + ".pub");
			mKeysStorage.GenerateRSAKey(KEY_SIZE, key_name);
			_dbg2("sign key");
			// mKeysStorage.RSASignFile(key_name, key_name + ".sig", true);
			mKeysStorage.RSASignNormalFile(key_name, key_name + ".sig", true);
			_dbg2("sign file");
			mKeysStorage.RSASignNormalFile(filename, filename + ".sig", false);
			mKeysStorage.RemoveRSAKey();
			//_note("test verify file " << filename << " using " << key_name);
			//mKeysStorage.RSAVerifyFile(filename + ".sig", key_name);
			cSend::sendMailNotificationMessage(mess, filename);
		}

		mRawBuffer.clear();
	}


	void handleAlarm(std::shared_ptr<cSound> &sound) {
		mAlarmLastTime = std::chrono::steady_clock::now();
		assert(!mRawBuffer.empty());
		this->message = sound->getMessage();
		_info_c(SAVING_LOG, "message: " << message);
	}

	void handleEvent(std::shared_ptr<cSound> &sound) {
		unsigned int SampleRate = getSampleRate();
		diffToAlarm = std::chrono::steady_clock::now() - mAlarmLastTime;
		if (diffToAlarm < std::chrono::seconds(EVENT_TIME)) {
			isEvent = true;
			auto vecOfSamples = mergeCBuff();
			//_mark("size " << vecOfSamples.size());
			for (auto a : vecOfSamples) {
				if (a != 0) {
					//_mark(a);
				}
			}
			// saving last 20s
			if (!savedMinusFile) {
				_dbg2_c(SAVING_LOG, "saving 20s file");
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime(), message);
				savedMinusFile = true;
			}

			// saving 3 files (1s)
			else if (mSavedFiles < 3 && mRawBuffer.size() >= FIRST_FILES_TIME * 10) {
				_dbg2_c(SAVING_LOG, "saving 1s file");
				++mSavedFiles;
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime(), message);

			}

			// other files (10s)
			else if (mSavedFiles >= 3 && mRawBuffer.size() >= NEXT_FILES_TIME * 10) {
				_dbg2_c(SAVING_LOG, "saving 10s file");
				saveBuffToFile(vecOfSamples.data(), vecOfSamples.size(), SampleRate, sound->currentDateTime(), message);
			}
		}
		else {
			isEvent = false;
			savedMinusFile = false;
			mSavedFiles = 0;
			this->message = "";
		}
	}

	/**
	 * Audio samples are provided to the onProcessSamples function every 100 ms.
	 * This is currently hard-coded into SFML and you can't change that
	 * (unless you modify SFML itself).
	 *
	 * SamplesCount = size of Samples
	 */
	virtual bool onProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		for (int i = 0; i < SamplesCount; ++i) {
			if (Samples[i] != 0) {
				// _mark(Samples[i]);
			}
		}
		unsigned int SampleRate = getSampleRate();
		createAndSaveFrameToCBuff(Samples, SamplesCount);

		auto sound = std::make_shared<cSound>(isEvent);
		if (simulationMode) sound->setSimulationMode();

		auto wasAlarm = sound->ProccessRecording(Samples, SamplesCount, SampleRate);
		if (wasAlarm) {
			_mark("!!!ALARM!!!");
			handleAlarm(sound);
		}

		handleEvent(sound);

		// return true to continue the capture, or false to stop it
		return true;
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
	void setSimulationMode();

	const bool fromMicrophoneMode;

private:
	cAlarmSoundRecorder Recorder;

	void waitForExitKey();
	
	/// audioDevice_manage
	/// Working only on sfml (>= 2.3) version.
	/// manage == false -- only return current device name
	/// manage == true -- allows you to choose device
    std::string audioDevice_manage();
};

#endif /* CRECORDER_H_ */
