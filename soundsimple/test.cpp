#include <iostream>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <fstream>

#include <SFML/Audio.hpp>

#include <fftw3.h>

#include "gnuplot_i.hpp"

using namespace std;

const std::string currentDateTime();
void wait_for_key(); // Programm halts until keypress


class cAlarm {

};


class cAlarmSoundRecorder : public sf::SoundRecorder
{
    virtual bool OnStart()
    {
        cout << "Start sound recorder" << endl;
        return true;
    }

    virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount)
    {
//    	cout << "Processing chunk of size " << SamplesCount << endl;
    	unsigned int SampleRate = GetSampleRate();

  		// Convert input array to double
  		double *in = new double[SamplesCount];
  		for (size_t i = 0; i < SamplesCount; i++) {
  			in[i] = static_cast<double>(Samples[i]);
  		}

  		size_t N; // FFT size
  		N = SamplesCount;

  	  fftw_complex *out;
  		fftw_plan p;
  		out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  		p =	fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
  		fftw_execute(p);

  		// Calculate array of frequencies
  		std::vector<double> freq;
  		double f = 0;
  		for (size_t i = 0; i < N; i++) {
  			f += (1.0/N)*SampleRate;
  			freq.push_back(f);
  		}

  		//Calculate magnitude
  		double r, i, magOne;
  		std::vector<double> mag;
  		double maxMag = 0;
  		for (size_t ind = 0; ind < N; ind++) {
  			i = out[ind][0];
  			r = out[ind][1];
  			magOne = sqrt((r * r) + (i * i));
  			mag.push_back( magOne); // simple filter
  			if ( mag.at(ind) > maxMag ) {
  				maxMag = mag.at(ind);
  			}
  		}
  		//Normalize
  		for (size_t i = 0; i < N; i++) {
  			mag.at(i) /= maxMag;
  		}

  		//Detect Alarm
  		double threshold = 0.9; // magnitude threshold

  		int detectLow = 1010;
  		int detectHigh = 1040;

  		int LowInd = detectLow/( (1.0/N)*SampleRate );
  		int HighInd = detectHigh/( (1.0/N)*SampleRate );

  		vector<double> magRange;
  		vector<double> magX;

  		int confirmations = 0;
  		for (int i = LowInd; i < HighInd; i++) {
  			if (mag.at(i) > threshold) {
  				confirmations++;
  			}
  			magRange.push_back(mag.at(i));
  			magX.push_back(freq.at(i));
  		}

  		//std::cout << confirmations << std::endl;

  		if (confirmations) {
  			std::ofstream log;
  			log.open("log.txt", std::ios::app);
  			cout << currentDateTime() << "  ALARM DETECTED" << endl;
  			log << currentDateTime() << "  ALARM DETECTED" << endl;
  			log.close();
  		}

  		fftw_destroy_plan(p);
  		fftw_free(in); fftw_free(out);

      return true;
    }

    virtual void OnStop()
    {
    	 cout << "Stop sound recorder" << endl;
    }
};



int main(int argc, char* argv[]){

	bool fromMic = true;
	if (argc > 1)
		fromMic = false;
	if (fromMic) {
		if (!sf::SoundRecorder::CanCapture()) {
		   cerr << "System don't support audio capture";
		   return -1;
		}
		cAlarmSoundRecorder Recorder;

		Recorder.Start();

		string userInput;
		int a = 0;
		while(true) {
			cout << "q to quit" << endl;
			cin >> userInput;
			a++;
			if (userInput == "q" || a == 10) break;
		}

		Recorder.Stop();
	}
	else {
		while(1) {
		sf::SoundBuffer Buffer;

		if (argc > 1) {
			string fileName = argv[1];
			if (!Buffer.LoadFromFile(fileName)) {
			    // Error...
			}
		}
		else {
			//if (!Buffer.LoadFromFile("ktd-wavtest_8pcm_v2.wav"))
			if (!Buffer.LoadFromFile("data.wav"))
			//if (!Buffer.LoadFromFile("sine.wav"))
			{
			    // Error...
			}
		}

		unsigned int SampleRate = Buffer.GetSampleRate();
		unsigned int Channels   = Buffer.GetChannelsCount();
		float Duration = Buffer.GetDuration();
		std::size_t SamplesCount = Buffer.GetSamplesCount();

		cout << "Sample rate: " << SampleRate << endl;
		cout << "Channels: " << Channels << endl;
		cout << "Duration: " << Duration << endl;
		cout << "Samples count: " << SamplesCount << endl;

		const sf::Int16* Samples = Buffer.GetSamples();
		if (0) { // Display samples after loading
			for (size_t i = 0; i < SamplesCount; i++) {
				cout << i << ":" <<Samples[i] << " ";
			}
		cout << endl;
		}

		// Convert input array to double
		double *in = new double[SamplesCount];
		for (size_t i = 0; i < SamplesCount; i++) {
			in[i] = static_cast<double>(Samples[i]);
		}

		if (0) { // Display samples after conversion
			for (size_t i = 0; i < SamplesCount; i++) {
				cout << i << ":" <<in[i] << " ";
			}
		cout << endl;
		}

		size_t N; // FFT size
		N = SamplesCount;

	  fftw_complex *out;
		fftw_plan p;
		out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
		p =	fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
		fftw_execute(p);

		// Calculate array of frequencies
		std::vector<double> freq;
		double f = 0;
		for (size_t i = 0; i < N; i++) {
			f += (1.0/N)*SampleRate;
			freq.push_back(f);
		}

		//Calculate magnitude
		double r, i, magOne;
		std::vector<double> mag;
		double maxMag = 0;
		//cout << endl << "FFT: "<< endl;
		for (size_t ind = 0; ind < N; ind++) {
			i = out[ind][0];
			r = out[ind][1];
			magOne = sqrt((r * r) + (i * i));
			mag.push_back( magOne * magOne ); // simple filter
			if ( mag.at(ind) > maxMag ) {
				maxMag = mag.at(ind);
			}
			//cout << ind << setiosflags(ios::fixed) << setprecision(2) << ":" "\ti" << "=" << i <<  "\tr" << "=" << r << "\tmag" << "=" << mag[ind] << "\t\tfreq" << "=" << freq[ind] << endl;
		}
		//Normalize
		for (size_t i = 0; i < N; i++) {
			mag.at(i) /= maxMag;
		}

		//Detect Alarm
		double threshold = 0.5; // magnitude threshold

		int detectLow = 100;
		int detectHigh = 1100;

		int LowInd = detectLow/( (1.0/N)*SampleRate );
		int HighInd = detectHigh/( (1.0/N)*SampleRate );

		vector<double> magRange;
		vector<double> magX;

		int confirmations = 0;

		for (int i = LowInd; i < HighInd; i++) {
			if (mag.at(i) > threshold) {
				confirmations++;
			}
			magRange.push_back(mag.at(i));
			magX.push_back(freq.at(i));
		}
		cout << "confirmations: " << confirmations << endl;
		if (confirmations > 3) {
			std::ofstream log;
			log.open("log.txt", std::ios::app);
			cout << currentDateTime() << "  ALARM DETECTED" << endl;
			log << currentDateTime() << "  ALARM DETECTED" << endl;
			log.close();
		}
/*
		// Plot results
		if (1) {
			try {
				Gnuplot g1("spectrum");

				cout << endl << endl << "spectrum plot" << endl;
				g1.set_grid();
				g1.set_style("points").plot_xy(freq,mag," ");

				//g1.set_style("points").plot_xy(magX,magRange,"magnitude");

				wait_for_key ();
			}
			catch (GnuplotException & ge) {
				cout << ge.what() << endl;
			}
		}
*/
		fftw_destroy_plan(p);
		fftw_free(in); fftw_free(out);
	}}
}


// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}


void wait_for_key ()
{
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

