#include "libs.h"
#include "cRecorder.h"
#include "cLearn.h"

#include "boost/parameter.hpp"

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

void startAlarm(bool simulation, bool learnMode) {
	cRecorder rec;
	if (simulation) rec.setSimulationMode();
	if (learnMode) rec.setLearningMode();
	rec.startRecording();
}

int handleCommand(const po::variables_map &vm, const po::options_description &desc, int argc) {
	try {
		if (vm.count("help") || argc == 1) cout << desc << endl;

		else if (vm.count("active-alarm")) startAlarm(false, false);

		else if (vm.count("simulation-mode")) startAlarm(true, false);

		else if (vm.count("learn-mode")) startAlarm(false, true);

		else if (vm.count("learn")) {
			string rec = vm["learn"].as<string>();
			_info("file: " << rec);
			cLearn learn(rec);
			learn.start();
		}
	}
	catch (po::error& e) {
		std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
		return 2;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	int cmd = 0;
	try {
		string opt;

		po::options_description desc("Options");
		desc.add_options()
		("help,h", "print help messages")
		("active-alarm,a", "starting alarm")
		("simulation-mode,s", "start detecting alarm without notification")
		("learn,l", po::value<string>(), "learn new alarm from recording")
		("learn-mode,m", "learn mode" )
		;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
		cmd = handleCommand(vm, desc, argc);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << ", application will now exit" << std::endl;
		return 1;
	}

	return cmd;
}

