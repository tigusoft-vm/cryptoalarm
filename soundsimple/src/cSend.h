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
	static const std::string sendScript;
	static void alarmHandler();
	static void sendXMPPNotificationMessage(std::string mess);
	static void sendXMPPNotificationMessageInThread(std::string mess);
	static void sendSum(std::string filename);
	static void send(cSound::sendingMethod method, const std::string &message);
    static void sendMailNotificationMessage(std::string mess, std::string rec);
    static void sendMailHandleErrors(std::string toSend, int n);
    static void execute(std::string cmd);

};

#endif /* CSEND_H_ */
