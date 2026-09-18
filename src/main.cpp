#include <fstream>
#include <iostream>
#include <filesystem>

#include <vector>
#include <string>
#include <cmath>

using namespace std;

void printVector(vector<char> charMap);

int main() {
	string inputFileName = "fileInput/main.txt";
	ifstream inputFile(inputFileName);

	// Checking If File Is Open
	if (!inputFile.is_open()) {
		cout << "ERROR: cannot open output file" << endl;
		return 0;
	}
	
	vector<char> charMap;
	char letter;

	// Reading Input File
	while (inputFile.get(letter)) {
		bool found = false;

		for (int i = 0; i < charMap.size(); i++) {
			if (letter == charMap.at(i)) {
				found = true;
			}
		}

		// Only Add If Unique Character
		if (!found) {
			charMap.push_back(letter);
		}
	}

	printVector(charMap);

	// Creating Ouput File Name
	int dotIndex = inputFileName.rfind('.');
	int slashIndex = inputFileName.rfind('/');
	string extension = inputFileName.substr(dotIndex);
	string baseName = inputFileName.substr(slashIndex + 1, dotIndex) + "(Zip)" + extension;
	string outputFileName = "fileOutput" + baseName;

	// Reset ifstream To Start Of File
	inputFile.clear();               // clears EOF and error flags
	inputFile.seekg(0, std::ios::beg); // moves stream pointer back to the beginning of file

	int maxBits = ceil(log2(charMap.size())); // calculate max bits per charaacter
	
	// Cheking If in ASCII Range
	if (maxBits > 256) {
		cout << "ERROR: file is greter than ASCII Range";
		return 0;
	}
	
	// Creating Output File
	ofstream outputFile(outputFileName);


	if (filesystem::exists(baseName)) { 
		cout << "ERROR: file cannnot be created" << endl;
		return 0;
	}

	// Checking If File Is Open
	if (!outputFile.is_open()) {
		cout << "ERROR: cannot open output File" << endl;
		return 0;
	}

	//Writting charMap in Output File
	outputFile << "{ ";
	for (int i = 0; i < charMap.size(); i++) {
		outputFile << charMap.at(i);
	}
	outputFile << " }";

	// Writing Chacters In outputFile
	char nonfilledChar;
	int usedBits = 0;

	while (inputFile.get(letter)) {
		char newChar;

		// Finding And Coverting Code To Character
		for (int i = 0; i < charMap.size(); i++) {
			if (letter == charMap.at(i)) {
				 newChar = static_cast<char>(i);
				 usedBits += maxBits;
				break;
			}
		}
		
		if()
	}

	// Closing Opened Files
	inputFile.close();
	outputFile.close();
}

void printVector(vector<char> charMap) {
	for (int i = 0; i < charMap.size(); i++) {
		cout << "[" << charMap.at(i) << "]" << " : " << i << endl;
	}
}