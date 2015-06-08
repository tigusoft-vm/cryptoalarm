/*
 * cRecorder.cpp
 *
 *  Created on: 26 lis 2014
 */

#include "cRecorder.h"
#include <boost/filesystem.hpp>

using namespace std;

cRecorder::cRecorder() :
		fromMicrophoneMode(true)
{
	assert(sf::SoundBufferRecorder::CanCapture()); // audio capture must be supported
	if (!boost::filesystem::exists(recDirName)) boost::filesystem::create_directory(recDirName);
	cSend::simulationMode = false;
}

cRecorder::~cRecorder()
{
	// TODO Auto-generated destructor stub
}

void cRecorder::startRecording() {
	_fact("start recording");
	Recorder.Start();
	{
		/**
		 * Sound is recording in new thread
		 * (implemented in sfml).
		 *
		 * This thread waits for exit key
		 */
		waitForExitKey();
	}
	Recorder.Stop();
}

void cRecorder::waitForExitKey() {
	_dbg2("Waiting for exit key");
	string userInput;
	int a = 0;
	while (true) {
		cout << "q to quit" << endl;
		cin >> userInput;
		a++;
		if (userInput == "q" || a == 10) break; // why a==10?
	}
}

void cRecorder::setSimulationMode() {
	_warn("SIMULATION MODE, ONLY FOR TESTING");
	Recorder.setSimulationMode(true);
	cSend::simulationMode = true;
}

cAlarmSoundRecorder::cAlarmSoundRecorder()
{
	mKeysStorage.GenerateRSAKey(KEY_SIZE, cFile::getHomeDir() + "Alarm_data/key_" + std::to_string(mKeysStorage.getCurrentKey()) + ".pub");
}
