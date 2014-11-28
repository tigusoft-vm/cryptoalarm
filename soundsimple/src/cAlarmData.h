/*
 * cAlarmData.h
 *
 *  Created on: 26 lis 2014
 */

#ifndef CALARMDATA_H_
#define CALARMDATA_H_

#include "libs.h"



class cAlarmData {
public:
	virtual ~cAlarmData();
	cAlarmData();
	void add(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate);
	void clearAll();
	void save();
private:
	std::vector <sf::Int16*> Samples;
	std::vector <size_t> SamplesCounts;
	std::vector <unsigned int> SampleRates;


};

#endif /* CALARMDATA_H_ */
