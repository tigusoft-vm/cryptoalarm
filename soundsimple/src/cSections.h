/*
 * cSections.h
 *
 *  Created on: 21 lis 2014
 *      Author: abby
 */

#ifndef CSECTIONS_H_
#define CSECTIONS_H_

#include "libs.h"

typedef std::pair<double,double> range;

class cSections {
public:
	cSections();


	class cSection {
	public:
		cSection(int f, int t);
		friend std::ostream& operator<<(std::ostream &out, const cSection &section);
	private:
		int from;
		int to;
		range max;
		range avg;
		range sum;
	};

	virtual ~cSections();
	bool interpretLine(std::string line);


private:
	std::vector <cSection> sections;
	std::vector <std::string> keywords;
	bool getFromFile(const std::string &filename);
	void getOptionalData();
};

#endif /* CSECTIONS_H_ */
