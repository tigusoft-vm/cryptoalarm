/*
 * cSend.h
 *
 *  Created on: 28 lis 2014
 *     
 */

#ifndef CSEND_H_
#define CSEND_H_

#include "libs.h"
#include "cSound.h"

class cSend {
public:
	cSend();
	virtual ~cSend();

	static void alarmHandler();
	static void sendXMPPNotificationMessage(std::string mess);
	static void sendXMPPNotificationMessageInThread(std::string mess);
	static void sendSum(std::string filename);
	static void send(cSound::sendingMethod method, const std::string &message);
};

#endif /* CSEND_H_ */
