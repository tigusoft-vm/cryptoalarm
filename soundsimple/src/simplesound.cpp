#include "libs.h"
#include "cRecorder.h"
#include "boost/parameter.hpp"


using namespace std;

int main(int argc, char* argv[]) {
	cRecorder rec;
	rec.setLearningMode();
	rec.startRecording();

	return 0;
}



