/*
 * cSend.cpp
 *
 *  Created on: 28 lis 2014
 *     
 */

#include "cSend.h"

using namespace std;

const string cSend::sendScript = "../scripts/send.sh ";

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

	}
}


void cSend::sendXMPPNotificationMessage(std::string mess) {
	const string q = " \" ";
	_info("cmd");
	const string cmd = sendScript + " xmpp " + q + mess + q;
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

void cSend::sendMailNotificationMessage(std::string mess, std::string rec) {
	_dbg1_c("send_log", "mess" << mess);
	_dbg2_c("send_log", "rec" << rec);
	const string q = " \"  ";
	const string cmd = sendScript + " mail " + q + mess +  q + rec;
	_dbg2(cmd);

	thread systh(&cSend::sendMailHandleErrors, cmd.c_str(), 0);
	systh.detach();

}

void cSend::sendMailHandleErrors(std::string toSend, int n) {
	auto err = std::system(toSend.c_str());
	// log toSend
	if(err == 0) {
		// all ok, log this
		_dbg1_c("send_log", "OK");
	}
	else {
		if(!n) {
			_erro_c("send_log", "Problem with sending, I try send this once again");
			sendMailHandleErrors(toSend, ++n);
		}
		else {
			_erro_c("send_log", "Can't send e-mail, I send notification via XMPP");
			sendXMPPNotificationMessageInThread(toSend);
		}
	}
}

void cSend::execute(std::string cmd)  {
	thread systh(std::system, cmd.c_str());
	systh.detach();
}
