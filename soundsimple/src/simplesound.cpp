#include "libs.h"
#include "cRecorder.h"
#include "cLearn.h"
#include "cFile.h"
#include "chainsign/ckeysstorage.h"

#include "boost/parameter.hpp"

#include <boost/program_options.hpp>
#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/files.h>
#include <crypto++/base64.h>

using namespace std;
namespace po = boost::program_options;



unsigned int verifyOneFile(const std::string &sigFileName) //fileName = sig file
{	
	std::cout << "start verify " << sigFileName << std::endl;
	std::ifstream inputFile(sigFileName);
	if(!inputFile.good()) // no file
	{
		std::cout << "No found " << sigFileName << std::endl;
		return -1;
	}
	std::string tmp;
	inputFile >> tmp;
	inputFile >> tmp; // number of pub file
	const std::string pubFile(cFile::getHomeDir() + "Alarm_data/key_" + tmp + ".pub");
	std::cout << "pubFile " << pubFile << std::endl;
	
	for (int i = 0; i < 4; ++i)
	{
		inputFile >> tmp;
		//std::cout << tmp << " ";
		inputFile >> tmp;
		//std::cout << tmp << std::endl;
	}
	
	const std::string clearDataFilePath(tmp);
	inputFile >> tmp;
	inputFile >> tmp;
	const std::string sig2File(tmp);
	
	std::cout << "clearDataFilePath " << clearDataFilePath << std::endl;
	std::cout << "sig2File " << sig2File << std::endl;
	
	//load signature
	using namespace CryptoPP;
	std::string signedTxt;
	FileSource(clearDataFilePath.c_str(), true, new StringSink(signedTxt)); 
	std::string signature;
	FileSource(sig2File.c_str(), true, new StringSink(signature)); 
	
	//Read public key
	CryptoPP::ByteQueue bytes;
	FileSource file(pubFile.c_str(), true, new Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	RSA::PublicKey pubKey;
	pubKey.Load(bytes);
	AutoSeededRandomPool rng;
	RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);
	std::string combined(signedTxt);
	combined.append(signature);
	
	try
	{
		StringSource(combined, true, 
			new SignatureVerificationFilter(verifier, NULL, SignatureVerificationFilter::THROW_EXCEPTION) );
		std::cout << "Signature OK" << std::endl;
		return 1;
	}
	catch(SignatureVerificationFilter::SignatureVerificationFailed &err)
	{
		std::cout << "verify error " << err.what() << std::endl;
		return -1;
	}
	/*cKeysStorage keyStorage;
	ret = keyStorage.RSAVerifyFile(fileName, );
	//std::cout << ret << std::endl;
	if (ret == 0)
		return 3;
	*/
	//std::cout << "OK" << std::endl;	
	//return 0;
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

