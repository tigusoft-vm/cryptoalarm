/*
 * cLearn.h
 *
 *  Created on: 11 gru 2014
 *     
 */

#ifndef CLEARN_H_
#define CLEARN_H_

#include "libs.h"
#include "cSound.h"
#include "cSoundProperties.h"

class cLearn {
public:
	cLearn(std::string filename_);
	bool fileExist(std::string filename);
	virtual ~cLearn();
	void start();

private:
	const std::string filename;
	sf::SoundBuffer getBuffFromFile();
	std::shared_ptr<cSound> prepareSound(sf::SoundBuffer &Buffer);

};

#endif /* CLEARN_H_ */
