#include "libs.h"

#include "cRecorder.h"

using namespace std;



int main(int argc, char* argv[]) {
	bool fromMic = true;
	//if (argc > 1) fromMic = false;

	cRecorder rec;
	rec.startRecording();


	/*
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
	}*/
}



