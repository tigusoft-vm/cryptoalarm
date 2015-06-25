#include "libs.h"
#include "cRecorder.h"
#include "cLearn.h"
#include "cFile.h"
#include "chainsign/ckeysstorage.h"

#include "boost/parameter.hpp"

#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>

#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/files.h>
#include <crypto++/base64.h>

using namespace std;
namespace po = boost::program_options;

bool FLAG_signalHandler = false;

/// C Signal_Handler <csignal>
void signalHandler(int signum)
{
	FLAG_signalHandler = true;
	//std::cout << "\b\bInterrupt signal (" << signum << ") received.\n";
	//_fact("\b\bInterrupt signal (" << signum << ") received.\n");
//	cout << "q to quit" << endl;
}

cKeysStorage keyStorage;

int verify(std::string firstKey, const std::string &path) // verify keys, get name of 1st pub file, return last good key
{
	//_scope_fact("firstKey " << firstKey << " path " << path);
	std::cout << "start verify keys" << std::endl;
	//std::ifstream pubFile;
	//system(std::string("cp " + firstKey + " " + mOutDir + firstKey).c_str()); // copy 1st key to out dir
/*	
	std::string fileName = instance;
	bool good = true;
	int keyNumber = 2;
	while (firstKey.front() != '-')
	{
		instance += firstKey.front();
		firstKey.erase(firstKey.begin());
	}
*/
	const std::string prefixKeyName(path + "key_"); // key_1.pub, key_2.pub ...
	const std::string suffixKeyName(".pub");
	bool good = true;
	int keyNumber = 2;
	std::string fileName;
	
	std::cout << "start loop" << std::endl; 
	unsigned int lastGoodKey = 1;
	while (good) {
		fileName = prefixKeyName + std::to_string(keyNumber) + suffixKeyName;
		//_mark("verify fileName " << fileName);
		std::cout << "file name " << fileName << std::endl;
		std::ifstream pubFile;
		pubFile.open(fileName);
		if(!pubFile.good()) // no file
		{
			std::cout << "No found " << fileName << std::endl;
			break;
		}
		
		std::cout << "start verify " << fileName << std::endl;
		//good = keyStorage.RSAVerifyFile(fileName);
		//_mark("sig file " << fileName + ".sig");
		good = keyStorage.RSAVerifyNormalFile(fileName, fileName + ".sig", path);
		//RSAVerifyNormalFile(const std::string& inputFilename, const std::string& signatureFilename);
		if (good) {
			lastGoodKey = keyNumber;
			//std::cout << "mv cmd " << "mv " + fileName + " " + mOutDir + fileName << std::endl;
			//fileName.erase(fileName.end() - 4, fileName.end()); // rm ".sig"
			//system(std::string("cp " + fileName + " " + mOutDir + fileName).c_str());
		}
		keyNumber++;
	}
	
	std::cout << "Last good key: " << lastGoodKey << std::endl;
	if (lastGoodKey > 1)
	{
		std::cout << "OK" << std::endl;
		return lastGoodKey;
	}
	else
		return -1;
}

unsigned int verifyOneFile(const std::string &fileName) //fileName = normal file
{	
	//_scope_mark("fileName = " << fileName);
	//std::cout << "RSAVerifyFile " << keyStorage.RSAVerifyFile(fileName, instance) << std::endl;
	
	std::string mainDir(fileName);
	std::string::iterator it = mainDir.end();
	it--;
	while (*it != '/') {
		mainDir.erase(it);
		it--;
	}
	
	std::string firstPubKey = mainDir + "key_1.pub";
	
	std::cout << "Start keys verification" << std::endl;
	std::cout << "first pub key " << firstPubKey << std::endl;
	
	//_mark("mainDir " << mainDir);
	//firstPubKey.insert(0, mainDir);
	int lastGoodKey;
	int ret = verify(firstPubKey, mainDir); // verify keys
	lastGoodKey = ret;
	if (ret == -1) {
		std::cout << "***keys verification error***" << std::endl;
		return 2;
	}
	
	std::cout << "file name " << fileName << " using " << fileName + ".sig" << std::endl;
	if (!boost::filesystem::exists(fileName + ".sig")) {
		std::cout << "***ERROR reading file " << fileName + ".sig" << "***" << std::endl;
		return 5;
	}
	ret = keyStorage.RSAVerifyNormalFile(fileName, fileName + ".sig", mainDir);
	//std::cout << ret << std::endl;
	if (ret == 0) {
		std::cout << "***file verification error***" << std::endl;
		return 3;
	}
	
	if (ret > lastGoodKey) {
		std::cout << "***file verification error***" << std::endl;
		std::cout << "***pub key < last good key***" << std::endl;
		return 4;
	}
	
	std::cout << "OK" << std::endl;
	return 1;
}

void startAlarm(bool simulation) {
	cRecorder rec;
	if (simulation) rec.setSimulationMode();
	rec.startRecording();
}

int handleCommand(const po::variables_map &vm, const po::options_description &desc, int argc) {
	try {
		if (vm.count("help") || argc == 1) cout << desc << endl;

		else if (vm.count("active-alarm")) startAlarm(false);

		else if (vm.count("simulation-mode")) startAlarm(true);

		else if (vm.count("learn")) {
			string rec = vm["learn"].as<string>();
			_info("file: " << rec);
			cLearn learn(rec);
			learn.start();
		}
		else if (vm.count("verify")) {
			std::string path;
			path = vm["verify"].as<string>();
			_dbg1("verify " << path);
			return verifyOneFile(path);
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
	FLAG_signalHandler = false;
	signal(SIGINT, signalHandler);  
	
	// Checking, creating if necessary Alarm_data directory.
	struct passwd *pw = getpwuid(getuid());
	string alarm_Data_dir = pw->pw_dir;
	alarm_Data_dir += "/Alarm_data";
	if(!boost::filesystem::is_directory(alarm_Data_dir)){
		auto d = boost::filesystem::create_directory(alarm_Data_dir);
		_dbg2(d);
	}
	try {
		string opt;

		po::options_description desc("Options");
		desc.add_options()
		("help,h", "print help messages")
		("active-alarm,a", "starting alarm")
		("simulation-mode,s", "start detecting alarm without notification")
		("learn,l", po::value<string>(), "learn new alarm from recording")
		("verify,v",po::value<string>(), "verify files")
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

