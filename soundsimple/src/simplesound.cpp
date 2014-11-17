#include "libs.h"

#include "cSound.h"

using namespace std;

class cAlarm {

};

class cAlarmSoundRecorder: public sf::SoundRecorder {
	virtual bool OnStart() {
		cout << "Start sound recorder" << endl;
		return true;
	}

	virtual bool OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount) {
		//cout << "Processing chunk of size " << SamplesCount << endl;
		//_info(*Samples << " , count " << SamplesCount);
		unsigned int SampleRate = GetSampleRate();

		auto snd = std::make_shared<cSound>(false);
		snd->ProccessRecording(Samples, SamplesCount, SampleRate);

		return true;
	}

	virtual void OnStop() {
		cout << "Stop sound recorder" << endl;
	}
};


int main(int argc, char* argv[]) {
	bool fromMic = true;
	if (argc > 1)
	fromMic = false;
	if (fromMic) {
		_fact("microphone mode");
		if (!sf::SoundRecorder::CanCapture()) {
			cerr << "System don't support audio capture";
			return -1;
		}
		cAlarmSoundRecorder Recorder;

		Recorder.Start();

		string userInput;
		int a = 0;
		while (true) {
			cout << "q to quit" << endl;
			cin >> userInput;
			a++;
			if (userInput == "q" || a == 10) break;
		}

		Recorder.Stop();
	}
	else {
		_fact("from recording");
		cSound snd(true);

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
		unsigned int Channels = Buffer.GetChannelsCount();
		float Duration = Buffer.GetDuration();
		std::size_t SamplesCount = Buffer.GetSamplesCount();

		cout << "Sample rate: " << SampleRate << endl;
		cout << "Channels: " << Channels << endl;
		cout << "Duration: " << Duration << endl;
		cout << "Samples count: " << SamplesCount << endl;

		const sf::Int16* Samples = Buffer.GetSamples();

		snd.ProccessRecording(Samples, SamplesCount, SampleRate);
	}
}



