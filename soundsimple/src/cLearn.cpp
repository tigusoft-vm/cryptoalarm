/*
 * cLearn.cpp
 *
 *  Created on: 11 gru 2014
 *     
 */

#include "cLearn.h"

using namespace std;

cLearn::cLearn(std::string filename_) :
		filename(filename_)
{
}

bool cLearn::fileExist(std::string filename) {
	if (!boost::filesystem::exists(filename)) {
		_erro(filename << " file doesn't exist!");
		return false;
	}
	return true;
}

void cLearn::start() {
	auto Buffer = getBuffFromFile();
	auto snd = prepareSound(Buffer);

	cSoundProperties sndProp(snd->getMag(), snd->getFreq());

}

sf::SoundBuffer cLearn::getBuffFromFile() {
	if (!fileExist(this->filename)) throw invalid_argument("File not found");
	sf::SoundBuffer buffer;
	buffer.loadFromFile(this->filename);
	return buffer;
}

cLearn::~cLearn() {
	// TODO Auto-generated destructor stub
}

std::shared_ptr<cSound> cLearn::prepareSound(sf::SoundBuffer& Buffer) {
	auto snd = std::make_shared<cSound>(false);

	const sf::Int16* Samples = Buffer.getSamples();
	unsigned int SampleRate = Buffer.getSampleRate();
	unsigned int Channels = Buffer.getChannelCount();
	sf::Time Duration = Buffer.getDuration();
	std::size_t SamplesCount = Buffer.getSampleCount();

	cout << "Sample rate: " << SampleRate << endl;
	cout << "Channels: " << Channels << endl;
	cout << "Duration: " << Duration.asSeconds () << endl;
	cout << "Samples count: " << SamplesCount << endl;


	snd->ProccessRecording(Samples, SamplesCount, SampleRate);
	return snd;
}
