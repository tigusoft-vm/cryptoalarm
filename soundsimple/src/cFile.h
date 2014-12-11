/*
 * cFile.h
 *
 *  Created on: 8 gru 2014
 *     
 */

#ifndef CFILE_H_
#define CFILE_H_

#include "libs.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/**
 * Saving files (*.wav files at now) in $HOME:
 * Preparing filenames and directories (using boost filesystem)
 *
 *	Alarm_data/
 *	`-- 2014-12-11
 *   	`-- 10-21-00
 *       	`-- 2014-12-11_10-21-00.wav
 */

class cFile {
public:
	cFile();
	virtual ~cFile();

	static std::vector <std::string> splitString(std::string &toSplit, std::string delimiter);
	static std::string getFilename(std::string date);
	static std::string getHomeDir();
	static std::string dealWithDir(const std::string &date, const std::string &time);
};

#endif /* CFILE_H_ */
