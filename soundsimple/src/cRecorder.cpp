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
	assert(sf::SoundBufferRecorder::isAvailable()); // audio capture must be supported
	if (!boost::filesystem::exists(recDirName)) boost::filesystem::create_directory(recDirName);
	cSend::simulationMode = false;
		
}

cRecorder::~cRecorder()
{
	// TODO Auto-generated destructor stub
}

void cRecorder::startRecording() {
	
	
	_fact("start recording");
	Recorder.start();
	{
		/**
		 * Sound is recording in new thread
		 * (implemented in sfml).
		 *
		 * This thread waits for exit key
		 */
		waitForExitKey();
	}
	Recorder.stop();
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

string cRecorder::audioDevice_manage(bool manage){
	if(manage == false) return Recorder.getDevice();
	// Checking audo devices
	char choice = 0;
	std::cout << "Your default audio device is: " << Recorder.getDefaultDevice() << std::endl;
	std::cout << "Do you wanna change audio device? (Y/n): ";
	std::cin >> choice;
	
	do {
		while(choice == 'Y' || choice == 'y'){
			int device_nr;
			std::cout << "\nList of audio devices: " << std::endl;
			for(int i = 0; i < Recorder.getAvailableDevices().size(); i++){
				std::cout << i+1 << ":" << '\t' << Recorder.getAvailableDevices()[i] << std::endl;
			}
			std::cout << "\nYour choice: ";
			std::cin >> device_nr;
			if(device_nr >= 1 && device_nr <= Recorder.getAvailableDevices().size()){
				bool is_change = Recorder.setDevice(Recorder.getAvailableDevices()[device_nr-1]);
				choice = 'N';
				if(!is_change) std::cout << "Fail to change device" << std::endl;
				std::cout 	<< "Now your current audio device is: "
						<< Recorder.getDevice() << '\n' << std::endl;
			}
			else{
				std::cout << "Bad device number, try again." << std::endl;
			}
		}
		if(!(choice == 'n' || choice == 'N')){
			std::cout << "Please reply Y (for Yes) or N (for No).\n(Y/n): ";
			std::cin >> choice;
			continue;
		}
	} while(!(choice == 'n' || choice == 'N'));
	
	return Recorder.getDevice();
}