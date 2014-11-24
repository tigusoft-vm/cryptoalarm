/*
 * cSound.cpp
 *
 *  Created on: 17 lis 2014
 *      Author: abby
 */

#include "cSound.h"
#include "gnuplot_i.hpp"
#define MIN_CONF 5

using namespace std;

cSound::cSound(bool s) :
		simulation_(s), minAlarm(1.), numberOfActiveThreads(1)
{
	//_info("constructor");
}

void cSound::ProccessRecording(const sf::Int16* Samples, std::size_t SamplesCount, unsigned int SampleRate) {
	//_info(*Samples << " , count " << SamplesCount << ", " << SampleRate);
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

	detectAlarm(mag, SampleRate, fftw_size);

	// Plot results
	if (simulation_) plotResults(freq, mag);

	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);
}

void cSound::detectAlarm(samples mag, unsigned int SampleRate, size_t fftw_size) {
	auto confirmations = Interpret(mag, SampleRate, fftw_size);
	bool makeAlarm = analyseData(mag);
	if (confirmations >= MIN_CONF || makeAlarm) alarm(confirmations);
}

bool cSound::analyseData(samples mag) {
	bool makeAlarm = false;
	mag.back() = 0;
	//_note("mag.size() " << mag.size());
	//_note("minAlarm " << minAlarm);
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
	double energy=0;
	for (size_t i = 0; i < fftw_size; i++) {
		mag.at(i) /= maxMag;
		energy += mag.at(i);
		//if (mag.at(i) >= threshold) _dbg1(mag.at(i) << "\t" << freq.at(i));
	}
	_dbg2("energy: " << energy);
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

	int confLvl = 0;

	const auto range1 = getSection(mag, 550, 1200, SampleRate, N);
	if (IsInRange(range1->max, 0.85, 1.)) {
		confLvl += 3;
		_dbg1("r1 max: " << range1->max);
	}

	if (IsInRange(range1->avg, 0.006, 0.012)) {
		confLvl += 2;
		_dbg1("r1 avg: " << range1->avg);
	}

	const auto range2 = getSection(mag, 2800, 3500, SampleRate, N);
	if (IsInRange(range2->max, 0.2, 1.)) {
		confLvl += 2;
		_dbg1("r2 max: " << range2->max);
	}

	if (IsInRange(range2->avg, 0.002, 0.004)) {
		confLvl++;
		_dbg1("r2 avg: " << range2->avg);
	}

	const auto range3 = getSection(mag, 3770, 4200, SampleRate, N);
	if (IsInRange(range3->max, 0.05, 1.)) {
		confLvl += 2;
		_dbg1("r3 max: " << range3->max);
	}
	if (IsInRange(range3->avg, 0.002, 0.004)) {
		confLvl++;
		_dbg1("r3 avg: " << range2->avg);
	}

	// detector #2
	const auto range4 = getSection(mag, 2150, 2650, SampleRate, N);
	if (IsInRange(range4->sum, 15., 50.)) {
		confLvl += 2;
		_dbg1("r4 sum: " << range2->sum);
	}

	_dbg3("conflvl: " << confLvl);

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
	//_note("max: " << range->max << ", avg: " << range->avg << ", sum: " << range->sum);
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

void cSound::sendXMPPNotificationAlarm(int level) {
	_scope_info("sending XMPP notification");
	numberOfActiveThreads++;

	while (numberOfActiveThreads >= MAX_THREADS) {
		std::chrono::milliseconds duration(20000);
		this_thread::sleep_for(duration);
		_warn("threads: " << numberOfActiveThreads << ", sleeping");
	}

	std::stringstream cmd;
	cmd << "./send.sh \" " << currentDateTime() << "  ALARM DETECTED: " << level << "\" " << endl;
	_dbg3("command: [" << cmd.str());
	std::system(cmd.str().c_str());

	numberOfActiveThreads--;
}

void cSound::alarm(int level) {
	_mark("alarm (confirmations): " << level);

	thread xmppScript(&cSound::sendXMPPNotificationAlarm, this, level);
//	xmppScript.join();
	xmppScript.detach();

//	std::future <void> xmppScript = std::async(std::launch::async, cSound::sendXMPPNotificationAlarm, level);
//	xmppScript.wait();

//	sendXMPPNotificationAlarm(level);

	std::ofstream log;
	log.open("log.txt", std::ios::app);
	cout << currentDateTime() << "  ALARM DETECTED: " << level << endl;
	log << currentDateTime() << "  ALARM DETECTED: " << level << endl;
	log.close();
}

bool cSound::IsInRange(double var, double from, double to) {
	if (from > to) return false;
	if (var >= from && var <= to) return true;
	else
		return false;
}

void cSound::plotResults(const samples &x, const samples &y) {
	try {
		Gnuplot g1("spectrum");

		cout << endl << endl << "spectrum plot" << endl;
		g1.set_grid();
		g1.set_style("points").plot_xy(x, y, " ");

		//g1.set_style("points").plot_xy(magX,magRange,"magnitude");

		if (simulation_) wait_for_key();
		else
			sleep(2);
	}
	catch (GnuplotException & ge) {
		cout << ge.what() << endl;
	}
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

int cSound::autodetect(size_t fftw_size, const samples& freq, const samples& mag, unsigned int SampleRate) {
	double threshold = 0.5; // magnitude threshold
	//Detect Alarm
	int detectLow = 4500;
	int detectHigh = 5500;

	int LowInd = detectLow / ((1.0 / fftw_size) * SampleRate);
	//_dbg3("Low: " << LowInd);
	int HighInd = detectHigh / ((1.0 / fftw_size) * SampleRate);
	//_dbg3("High: " << HighInd);

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
