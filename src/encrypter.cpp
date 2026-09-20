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
	vector<char> charMap;
	char letter;

	// Checking If File Is Open
	if (!inputFile.is_open()) {
		cout << "ERROR: cannot open output file" << endl;
		return 0;
	}

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

	//printVector(charMap);

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
	
	// Cheking If In ASCII Range
	if (maxBits > 256) {
		cout << "ERROR: file is greater than ASCII Range";
		return 0;
	}
	
	// Creating Output File
	ofstream outputFile(outputFileName);
	// Checking If File Exists And Is Open
	if (filesystem::exists(baseName)) { 
		cout << "ERROR: file cannnot be created" << endl;
		return 0;
	}
	else if (!outputFile.is_open()) {
		cout << "ERROR: cannot open output File" << endl;
		return 0;
	}

	//Writting charMap in Output File
	for (int i = 0; i < charMap.size(); i++) {
		outputFile << charMap.at(i);
	}
	outputFile << "\n";

	// Compressing Chacters And Writting In outputFile
	unsigned char newChar = static_cast<char>(0);
	int usedBits = 0;

	while (inputFile.get(letter)) {
		for (int i = 0; i < charMap.size(); i++) {
			if (letter == charMap.at(i)) {
				char targetChar = static_cast<char>(i); // convert character code to character
				bool is_full = (usedBits + maxBits) > 8; // check if newChar has sapce for targetChar
				if (!is_full) {
					targetChar >> usedBits; // shift bits to the right
					newChar | targetChar; // using OR to add targetChar into newChar
					usedBits += maxBits;
				}
				else {
					//Check Free Space
					int freeSpace = 8 - usedBits;
					
					if (freeSpace == 0) {
						outputFile << newChar;
						newChar = targetChar;
					}
					else {
						char tempTargetChar = targetChar >> (8 - maxBits); // shift to right most
						tempTargetChar >> (8 - freeSpace); // shift right by free sapces

						newChar | tempTargetChar; // add into newChar
						outputFile << newChar;
						targetChar << freeSpace; // shift left by free spaces
						newChar = targetChar;
						usedBits = 8 - freeSpace;
					}
				}

				break;
			}
		}

		// Writing newChar in outputFile
		if (usedBits == 8) {
			outputFile << newChar;

			newChar = static_cast<char>(0);
			usedBits = 0;
		}

		// End OF File 
		outputFile << newChar;
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