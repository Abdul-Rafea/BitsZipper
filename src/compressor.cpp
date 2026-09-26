#include <fstream>
#include <iostream>

#include <vector>
#include <string>
#include <chrono>

using namespace std;

class Compressor {
private:
	vector<char> charMap;
	vector<int> bitArray;
	string filename;

public:
	Compressor(string Filename) {
		filename = Filename;
	}

	void setCharMap() {
		ifstream file(filename);
		if (!file.is_open()) {
			cout << "Failed to open filename";
			return;
		}

		char letter;
		while (file.get(letter)) {
			bool matchFound = false;
			for (int i = 0; i < charMap.size(); i++) {
				if (letter == charMap.at(i)) {
					matchFound = true;
				}
			}

			if (!matchFound) {
				charMap.push_back(letter);
			}
		}

		file.close();
	}

	void printCharMap() {
		cout << "Char Map : -" << endl;
		for (int i = 0; i < charMap.size(); i++) {
			cout << charMap.at(i) << " | " << i << endl;
		}
	}

	void setBitArray() {
		ifstream file(filename);
		if (!file.is_open()) {
			cout << "Failed to open filename";
			return;
		}

		char letter;
		while (file.get(letter)) {
			for (int i = 7; i >= 0; i--) {
				bool isBitOn = (letter & (1 << i)) != 0;

				if (isBitOn) {
					bitArray.push_back(1);
				}
				else {
					bitArray.push_back(0);
				}
			}
		}

		file.close();
	}

	void printBitArray() {
		cout << "Bit Array : ";
		for (int i = 0; i < bitArray.size(); i++) {
			cout << bitArray.at(i);
		}
	}
};

// Main Function
int main() {
	string filename;
	cout << "Enter filename to compress: ";
	cin >> filename;

	auto startTime = chrono::high_resolution_clock::now(); // record start time

	Compressor compressor = Compressor(filename);
	compressor.setCharMap();
	compressor.printCharMap();
	compressor.setBitArray();
	compressor.printBitArray();

	auto endTime = chrono::high_resolution_clock::now(); // record end time
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime); // calcuate time duration

	cout << endl << "Time takes: " << duration.count() << " ms";
	return 0;
}