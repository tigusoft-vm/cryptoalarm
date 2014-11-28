#include "libs.h"

#include "cRecorder.h"

using namespace std;



int main(int argc, char* argv[]) {
	bool fromMic = true;
	//if (argc > 1) fromMic = false;

	cRecorder rec;
	rec.startRecording();

	return 0;
}



