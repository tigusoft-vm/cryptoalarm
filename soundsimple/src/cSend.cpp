/*
 * cSend.cpp
 *
 *  Created on: 28 lis 2014
 *     
 */

#include "cSend.h"

using namespace std;

cSend::cSend()
{
	// TODO Auto-generated constructor stub

}

cSend::~cSend()
{
	// TODO Auto-generated destructor stub
}

void cSend::alarmHandler() {
	while (true) {
		_note(cSound::alarmsToSend.size());
		if (cSound::alarmsToSend.empty()) sleep(1); // nothing to send
		else {
			_fact("sending");

			cSound::mtx.lock();
				const string mess = cSound::alarmsToSend.top().first;
				const auto method = cSound::alarmsToSend.top().second;
				cSound::alarmsToSend.pop();
			cSound::mtx.unlock();
			send(method, mess);
		}
	}
}

void cSend::send(cSound::sendingMethod method, const std::string &message) {
	switch(method) {
	case cSound::sendingMethod::XMPP:
		sendXMPPNotificationMessage(message);
		break;
	case cSound::sendingMethod::MAIL:
        sendMailNotificationMessage(message);
		break;
	}
}


void cSend::sendXMPPNotificationMessage(std::string mess) {
    const string sendScript = "./send.sh ";
	const string q = " \" ";
	_info("cmd");
	const string cmd = sendScript + q + mess + q;
	std::system(cmd.c_str());
}

void cSend::sendXMPPNotificationMessageInThread(std::string mess) {
	thread sendingThread(&cSend::sendXMPPNotificationMessage, mess);
	sendingThread.detach();
}

void cSend::sendSum(std::string filename) {
	if (!boost::filesystem::exists(filename)) {
		_warn("It seems file: " << filename << " doesn't exist, but I try to send checksum");
	}
	// TODO: sholud we use crypto++ library to get this sum?
	const string checkSumScript = " $(sha512sum recordings/2014-11-28.15:37:52.wav | tr \" \" \"-\" )";
	sendXMPPNotificationMessage(checkSumScript);
}

void cSend::sendMailNotificationMessage(std::string mess) {
    const string sendScript = "./sendmail.py ";
    const string q = " \" ";
    _info("cmd");
    std::list<std::string> attachList; // all *.tar.gz files in . dir
    boost::filesystem::directory_iterator dirIterator(".");
    boost::filesystem::directory_iterator endIterator;
    while (dirIterator != endIterator) {
        if (boost::filesystem::is_regular_file(dirIterator->status())) {
            std::stringstream ss;
            std::string fileName;
            ss << *dirIterator;
            ss >> fileName;
            if (fileName.find(".tar.gz") != std::string::npos) {
                attachList.emplace_back(std::move(fileName));
            }
        }
        dirIterator++;
    }
    string cmd = sendScript + q + mess + q;
    for (auto att : attachList) {
        cmd += " ";
        cmd += att;
    }
    std::system(cmd.c_str());
}
