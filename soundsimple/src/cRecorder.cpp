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
    audioDevice_manage();
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
		while(!FLAG_signalHandler) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		};
		//waitForExitKey();
	}
	Recorder.stop();
}

void cRecorder::waitForExitKey() {
	_dbg2("Waiting for exit key");
	string userInput;
	int a = 0;

	while(true) {
		cout << "q to quit" << endl;
		cin >> userInput;
		a++;
		if (userInput == "q" || a == 10 || FLAG_signalHandler) {
			break;
		}
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
	//std::cout << "******* list of regulear files" << std::endl;
	boost::filesystem::path recDirPath(cFile::getHomeDir() + "Alarm_data/");
	boost::filesystem::directory_iterator end_iter;
	for( boost::filesystem::directory_iterator dir_iter(recDirPath); dir_iter != end_iter; ++dir_iter) {
		if (boost::filesystem::is_regular_file(dir_iter->status())) {
			std::string filename(cFile::getHomeDir() + "Alarm_data/");
			filename += dir_iter->path().filename().string();
			if (filename.find(".prv") != std::string::npos) {
				std::cout << "************read prv key " << filename << std::endl;
				mKeysStorage.loadRSAPrivKey(filename);
				boost::filesystem::remove(filename);
				break;
			}
			//std::cout << *dir_iter << std::endl;
		}
	}
	//std::cout << "******* end of list" << std::endl;
}

cAlarmSoundRecorder::~cAlarmSoundRecorder() {
	std::cout << "save prv key to " << cFile::getHomeDir() + "Alarm_data/" << std::endl;
	mKeysStorage.saveRSAPrivKey(cFile::getHomeDir() + "Alarm_data/");
}

string cRecorder::audioDevice_manage() {
	// Checking audo devices
	char choice = 0;
	std::cout << "Your default audio device is: " << Recorder.getDefaultDevice() << std::endl;
	std::cout << "Do you wanna change audio device? (Y/n): ";
	std::cin >> choice;
	
	do {
        while (choice == 'Y' || choice == 'y') {
            int device_nr = -1;
			std::cout << "\nList of audio devices: " << std::endl;
            std::vector<std::string> devicesVector(cAlarmSoundRecorder::getAvailableDevices());
            std::sort(devicesVector.begin(), devicesVector.end());

            int i = 0;
            for (auto &dev : devicesVector) {
                std::cout << i << ":" << '\t' << dev << std::endl;
                ++i;
			}

			std::cout << "\nYour choice: ";
			std::cin >> device_nr;
            if (device_nr >= 0 && device_nr < devicesVector.size()) {
                bool is_change = Recorder.setDevice(devicesVector.at(device_nr));
				choice = 'N';
                if (!is_change) std::cout << "Fail to change device" << std::endl;
				std::cout 	<< "Now your current audio device is: "
                        << Recorder.getDevice() << '\n' << std::endl;
			}
            else {
				std::cout << "Bad device number, try again." << std::endl;
			}
		}
        if (!(choice == 'n' || choice == 'N')){
			std::cout << "Please reply Y (for Yes) or N (for No).\n(Y/n): ";
			std::cin >> choice;
			continue;
		}
    } while (!(choice == 'n' || choice == 'N'));
	
    return Recorder.getDevice();
}
