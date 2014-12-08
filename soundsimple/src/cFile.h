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
