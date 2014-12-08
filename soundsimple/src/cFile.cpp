/*
 * cFile.cpp
 *
 *  Created on: 8 gru 2014
 *     
 */

#include "cFile.h"
using namespace std;

cFile::cFile()
{
	// TODO Auto-generated constructor stub

}

cFile::~cFile()
{
	// TODO Auto-generated destructor stub
}

std::vector<std::string> cFile::splitString(string& toSplit, string delimiter) {
	toSplit += delimiter;
	string token;
	size_t pos = 0;
	vector<string> data;

	while ((pos = toSplit.find(delimiter)) != string::npos) {
		token = toSplit.substr(0, pos);
		toSplit.erase(0, pos + delimiter.length());
		data.push_back(token);
	}
	return data;
}

std::string cFile::getFilename(std::string date) {
	// 2014-12-01.15:36:23.wav -> 2014-12-04_11-39-52.wav
	date.replace(date.find("."), 1, "_");
	date.replace(date.find(":"), 1, "-");
	date.replace(date.find(":"), 1, "-");
	auto filename = date;

	auto parts = cFile::splitString(date, "_");
	auto path = dealWithDir(parts.at(0), parts.at(1));

	string fullfilename = path + "/" + filename + ".wav";

	_note(fullfilename);
	return fullfilename;
}

std::string cFile::getHomeDir() {
	struct passwd *pw = getpwuid(getuid());
	string home = pw->pw_dir;
	home += "/";
	return home;
}

std::string cFile::dealWithDir(const std::string& date, const std::string& time) {
	_note(getHomeDir());
	std::string path = getHomeDir() + "Alarm_data/" + date + "/" + time;

	auto d = boost::filesystem::create_directories(path);
	_dbg2(d);

	return path;
}
