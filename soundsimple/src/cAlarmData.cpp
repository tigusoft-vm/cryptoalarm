/*
 * cAlarmData.cpp
 *
 *  Created on: 26 lis 2014
 *      Author: abby
 */

#include "cAlarmData.h"

cAlarmData::~cAlarmData()
{
	// TODO Auto-generated destructor stub
}

cAlarmData::cAlarmData()
{
	// TODO Auto-generated constructor stub

}

void cAlarmData::add(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate) {
	auto SamplesCopy = new sf::Int16 [sizeof Samples]; //(Samples);
	std::memcpy(SamplesCopy, Samples, sizeof Samples);
	this->Samples.push_back(*&SamplesCopy);
	this->SamplesCounts.push_back(SamplesCount);
	this->SampleRates.push_back(SampleRate);
}

void cAlarmData::clearAll() {
	this->SampleRates.clear();
	this->SamplesCounts.clear();
	this->SampleRates.clear();
}

void cAlarmData::save() {

}

