/*
 * cSound.cpp
 *
 *  Created on: 17 lis 2014
 */

#include "cSound.h"
#include "gnuplot_i.hpp"

#include "cSend.h"

#define SND_LOG "detecting_sound"

using namespace std;

std::stack<std::pair<std::string, cSound::sendingMethod>> cSound::alarmsToSend;
int cSound::n = 0;
mutex cSound::mtx;

cSound::cSound(bool isEvent) :
		simulation_(false), minAlarm(1.), energy_(0), confirmation(0),
				wasAlarm(false), isEventNow(isEvent), noiseLvl(0), method(sendingMethod::XMPP)
{
	if (n == 0) createThreadForSendScript();
	n++;
}

void cSound::createThreadForSendScript() {
	_dbg1_c(SND_LOG, n);
	xmppScript = thread(&cSend::alarmHandler);
	xmppScript.detach();
}

bool cSound::ProccessRecording(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate) {
	double *in = new double[SamplesCount];
	convArrToDouble(in, Samples, SamplesCount); // Convert input array to double

	// fttw transform
	const auto fftw_size = SamplesCount;
	fftw_complex *out;
	fftw_plan p;
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * fftw_size);
	p = fftw_plan_dft_r2c_1d(fftw_size, in, out, FFTW_ESTIMATE);
	fftw_execute(p);

	// frequencies and magnitude vectors
	auto freq = calculateFrequencies(SampleRate, fftw_size);
	auto mag = calculateMagnitude(fftw_size, out);

	// alarm
	if (detectAlarm(mag, SampleRate, fftw_size)) alarm();

	// plot results
	if (simulation_) {
		//dealWithData(freq, mag);
		//plotResults(freq, mag);
	}

	// remove fftw data
	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);

	return wasAlarm;
}

bool cSound::detectAlarm(samples mag, unsigned int SampleRate, size_t fftw_size) {
	this->confirmation = Interpret(mag, SampleRate, fftw_size);
	bool isAlarm = false;
	this->reason = " ";
	bool justNoise = true;

	if (confirmation >= MIN_CONF) {
		isAlarm = true;
		justNoise = false;
		this->reason += "high confirmation level; ";
	}
	if (hasMagHigh(mag)) {
		isAlarm = true;
		justNoise = false;
		this->reason += "detected high magnitude; ";
	}
	if (energy_ >= MAX_STANDARD_NOISE) {
		isAlarm = true;
		this->reason += "energy [" + to_string(energy_) + "]"
				+ " higher than MAX_STANDARD_NOISE [" + to_string(MAX_STANDARD_NOISE)
				+ "]; ";

		this->method = sendingMethod::MAIL;
	}
	if (noiseLvl >= MAX_NOISE_LVL) {
		isAlarm = true;
		this->reason += "high noise level: " + to_string(noiseLvl) + "; ";
	}

	if (justNoise) this->method = sendingMethod::MAIL;

	return isAlarm;
}

bool cSound::hasMagHigh(samples mag) {
	bool makeAlarm = false;
	mag.back() = 0;
	for (int i = 0; i < 40; ++i)
		mag.pop_back();
	for (unsigned int i = 10; i < mag.size(); ++i) {
		if (mag.at(i) > minAlarm) {
			_dbg1("makeAlarm true " << i);
			makeAlarm = true;
		}
	}
	return makeAlarm;
}

void cSound::convArrToDouble(double *toArr, const sf::Int16 *fromArr, const size_t &size) {
	for (unsigned long int i = 0; i < size; i++) {
		toArr[i] = static_cast<double>(fromArr[i]);
	}
}

std::vector<double> cSound::calculateMagnitude(size_t fftw_size, const fftw_complex* out) {
	double r, i, magOne;
	std::vector<double> mag;
	double maxMag = 0;
	for (size_t ind = 0; ind < fftw_size; ind++) {
		i = out[ind][0];
		r = out[ind][1];
		magOne = sqrt((r * r) + (i * i));
		mag.push_back(magOne); // simple filter
		if (mag.at(ind) > maxMag) {
			maxMag = mag.at(ind);
		}
	}
	normalize(mag, maxMag, fftw_size);
	return mag;
}

void cSound::normalize(samples &mag, double maxMag, size_t fftw_size) {
	double energy = 0.;
	for (size_t i = 0; i < fftw_size; i++) {
		mag.at(i) /= maxMag;
		energy += mag.at(i);
	}
	energy_ = energy;

	_dbg2_c(SND_LOG, "energy: " << energy);
}

std::vector<double> cSound::calculateFrequencies(unsigned int SampleRate, size_t fftw_size) {
	std::vector<double> freq;
	double f = 0;
	for (size_t i = 0; i < fftw_size; i++) {
		f += (1.0 / fftw_size) * SampleRate;
		freq.push_back(f);
	}
	return freq;
}

int cSound::Interpret(const samples &mag, unsigned int SampleRate, size_t N) {
	bool printDebug = false;
	int confLvl = 0;

	const double typicalEnergyNow = 3 * getSection(mag, 50, 8000, SampleRate, N)->avg; // totall level of sound/noise
	const double noiseEnergyNow = typicalEnergyNow * 0.3; // a treshold of sound in current sample

	if (typicalEnergyNow > 0.01) {
		_info_c(SND_LOG, "Noise lvl " << noiseLvl);
		++noiseLvl;
	}
	if (typicalEnergyNow > 0.02) {
		_info_c(SND_LOG, "Noise lvl " << noiseLvl);
		++noiseLvl;
	}
	if (typicalEnergyNow > 0.05) {
		_info_c(SND_LOG, "Noise lvl " << noiseLvl);
		++noiseLvl;
	}
	if (typicalEnergyNow > 0.08) {
		_info_c(SND_LOG, "Noise lvl " << noiseLvl);
		++noiseLvl;
	}
	if (typicalEnergyNow > 0.10) {
		_info_c(SND_LOG, "Noise lvl " << noiseLvl);
		++noiseLvl;
	}
	if (typicalEnergyNow > 0.25) {
		_info_c(SND_LOG, "Noise lvl " << noiseLvl);
		++noiseLvl;
	}
	if (noiseLvl >= 4) {
		_info_c(SND_LOG, "noise");_info_c(SND_LOG, "Noise! "<<noiseLvl);
	}

	const auto range1 = getSection(mag, 550, 1200, SampleRate, N);
	if (IsInRange(range1->max, 0.85, 1.)) {
		confLvl += 3;
		if (printDebug) _dbg1("r1 max: " << range1->max);
	}

	if (IsInRange(range1->avg, 0.006, 0.012)) {
		confLvl += 2;
		if (printDebug) _dbg1("r1 avg: " << range1->avg);
	}

	const auto range2 = getSection(mag, 2800, 3500, SampleRate, N);
	if (IsInRange(range2->max, 0.2, 1.)) {
		confLvl += 2;
		if (printDebug) _dbg1("r2 max: " << range2->max);
	}

	if (IsInRange(range2->avg, 0.002, 0.004)) {
		confLvl++;
		if (printDebug) _dbg1("r2 avg: " << range2->avg);
	}

	const auto range3 = getSection(mag, 3770, 4200, SampleRate, N);
	if (IsInRange(range3->max, 0.05, 1.)) {
		confLvl += 2;
		if (printDebug) _dbg1("r3 max: " << range3->max);
	}
	if (IsInRange(range3->avg, 0.002, 0.004)) {
		confLvl++;
		if (printDebug) _dbg1("r3 avg: " << range2->avg);
	}

	// detector #2
	const auto range4 = getSection(mag, 2150, 2650, SampleRate, N);
	if (IsInRange(range4->sum, 15., 50.)) {
		confLvl += 2;
		if (printDebug) _dbg1("r4 sum: " << range2->sum);
	}

	if (printDebug) _dbg3("conflvl: " << confLvl);

	return confLvl;
}

std::shared_ptr<cSound::alarmData> cSound::getSection(const samples &mag, int from, int to, unsigned int SampleRate, size_t N) {
	auto range = make_shared<cSound::alarmData>();
	range->sum = 0.;
	range->max = 0.;

	assert(from < to);

	from /= (1.0 / N) * SampleRate;
	to /= (1.0 / N) * SampleRate;

	range->from = from;
	range->to = to;

	for (auto i = from; i <= to; ++i) {
		range->sum += mag.at(i);
		if (mag.at(i) > range->max) range->max = mag.at(i);
	}

	range->avg = range->sum / (to - from);
	return range;
}

const std::string cSound::currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void cSound::alarm() {
	_mark_c(SND_LOG, "alarm (confirmations): " << this->confirmation);
	this->wasAlarm = true;
	ostringstream message;
	message << currentDateTime() << "  ALARM DETECTED: " << this->confirmation;

	if (simulation_) reason += " <!SIMULATION!>";
	if (!isEventNow) { 	// is event, don't send message
		pair<string, sendingMethod> sending(message.str() + reason, this->method);

		mtx.lock();
		{
			alarmsToSend.push(sending);
		}
		mtx.unlock();
	}

	std::ofstream log;
	log.open("log.txt", std::ios::app);
	cout << message.str() + reason << endl;
	log << message.str() + reason << endl;
	log.close();
}

bool cSound::IsInRange(double var, double from, double to) {
	if (from > to) return false; // error
	if (var >= from) return true; // is loud
	return false;
}

void cSound::plotResults(const samples &x, const samples &y) {
	try {
		Gnuplot g1("spectrum");

		cout << endl << endl << "spectrum plot" << endl;
		g1.set_grid();
		g1.set_style("points").plot_xy(x, y, " ");

		//g1.set_style("points").plot_xy(magX,magRange,"magnitude");

		wait_for_key();
	}
	catch (GnuplotException & ge) {
		cout << ge.what() << endl;
	}
	_dbg1("end of plotting func");
}

void cSound::wait_for_key() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
	cout << endl << "Press any key to continue..." << endl;

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	_getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
	cout << endl << "Press ENTER to continue..." << endl;

	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
#endif
	return;
}

std::string cSound::getMessage() {
	string mess = "";
	if (wasAlarm) mess = "Confirmation: " + to_string(this->confirmation) + ", reason: " + this->reason;
	return mess;
}

int cSound::autodetect(size_t fftw_size, const samples& freq, const samples& mag, unsigned int SampleRate) {
	double threshold = 0.5; // magnitude threshold
	//Detect Alarm
	int detectLow = 4500;
	int detectHigh = 5500;

	int LowInd = detectLow / ((1.0 / fftw_size) * SampleRate);
	int HighInd = detectHigh / ((1.0 / fftw_size) * SampleRate);

	vector<double> magRange;
	vector<double> magX;

	int confirmations = 0;

	// ==
	vector<double> freqs;

	// ==
	double energy = 0.;

	for (int i = LowInd; i < HighInd; i++) {
		energy += mag.at(i);
		if (mag.at(i) > threshold) {
			confirmations++;
			freqs.push_back(i);
		}
		magRange.push_back(mag.at(i));
		magX.push_back(freq.at(i));
	}
	_dbg2("energy (sum of mags): " << energy);
	return confirmations;
}

cSound::~cSound() {
	//_dbg3("destructor");
}
