/*
 * cSound.h
 *
 *  Created on: 17 lis 2014
 */

#ifndef CSOUND_H_
#define CSOUND_H_

#include "libs.h"

#include "cSections.h"

#define MIN_CONF 5
#define MAX_STANDARD_NOISE 250
#define MAX_NOISE_LVL 5

typedef std::vector<double> samples;

class cSend;

class cSound {
	friend class cSend;
	public:
	cSound(bool s);
	cSound();
	virtual ~cSound();

	bool ProccessRecording(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate);
	const std::string currentDateTime();
	void alarm();

	void setSimulationMode() {
		simulation_ = true;
	}

	struct alarmData {
		int from;
		int to;
		double sum;
		double avg;
		double max;
	};
	std::string getMessage();


private:
	static std::mutex mtx;
	static int n;

	enum sendingMethod {
		XMPP, MAIL
	};

	static std::stack<std::pair<std::string, sendingMethod>> alarmsToSend;

	sendingMethod method;
	bool simulation_;
	const double minAlarm;
	bool wasAlarm;
	std::thread xmppScript;
	double energy_;
	int confirmation;
	bool isEventNow;
	int noiseLvl;
	std::string reason;
	bool learnMode;

	samples mag;
	samples freq;

	void createThreadForSendScript();
	void wait_for_key(); // used in plot function
	std::shared_ptr<alarmData> getSection(const samples &mag, int from, int to, unsigned int SampleRate, size_t N);
	int Interpret(const samples &mag, unsigned int SampleRate, size_t N); // hardcoded values for specific hardware
	bool IsInRange(double var, double from, double to);
	void convArrToDouble(double *toArr, const sf::Int16 *fromArr, const size_t &size);
	std::vector<double> calculateMagnitude(size_t fftw_size, const fftw_complex *out);
	std::vector<double> calculateFrequencies(unsigned int SampleRate, size_t fftw_size);
	void normalize(samples &mag, double maxMag, size_t fftw_size);
	void plotResults(const samples &x, const samples &y);
	bool detectAlarm(samples mag, unsigned int SampleRate, size_t fftw_size);
	bool hasMagHigh(samples mag);

	// plotResult();
	int autodetect(size_t fftw_size, const samples &freq, const samples &mag, unsigned int SampleRate); // old way to detect alarm, not used at now

public:
	void setLearnMode(bool learnMode){
		this->learnMode = learnMode;
	}

};

#endif /* CSOUND_H_ */
