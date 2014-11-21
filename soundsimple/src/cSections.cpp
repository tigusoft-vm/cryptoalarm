/*
 * cSections.cpp
 *
 *  Created on: 21 lis 2014
 *      Author: abby
 */

#include "cSections.h"

using namespace std;

cSections::cSections() {
	keywords.push_back("from");
	keywords.push_back("to");
	keywords.push_back("max");
	keywords.push_back("avg");
	keywords.push_back("sum");
}

cSections::~cSections()
{
	// TODO Auto-generated destructor stub
}

cSections::cSection::cSection(int f, int t) :
		from(f), to(t), max(0,0), avg(0,0), sum(0,0)
{


}

bool cSections::getFromFile(const std::string& filename) {




}

std::ostream& operator<<(std::ostream& os, const cSections::cSection& section) {
	char a='['; char z=']'; char s=' ';
	os << a << section.from << s << "-" << section.to << z << ","
			<< a << "max= " << section.max.first << "-" << section.max.second << z << s
			<< a << "avg= " << section.avg.first << "-" << section.avg.second << z << s
			<< a << "sum= " << section.sum.first << "-" << section.avg.second << z << s;


	//os << ctask.day_time_ << " " << task::states_.at(ctask.type_) << " "
	//		<< ctask.name_;
	return os;
}
