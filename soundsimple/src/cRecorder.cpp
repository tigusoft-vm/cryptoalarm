/*
 * cRecorder.cpp
 *
 *  Created on: 26 lis 2014
 */

#include "cRecorder.h"

using namespace std;

cRecorder::cRecorder() :
		fromMicrophoneMode(true)
{
	assert(sf::SoundRecorder::CanCapture()); // audio capture must be supported
}

cRecorder::~cRecorder()
{
	// TODO Auto-generated destructor stub
}

void cRecorder::startRecording() {
	_fact("start recording");
	Recorder.Start();
	{
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
