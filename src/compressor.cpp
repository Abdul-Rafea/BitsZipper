#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <cmath>

using namespace std;

class Compressor {
	private:
		string filename;
		string compressedFilename;
		vector<char> charMap;
		vector<int> bitArray;
		vector<char> charArray;
		int maxBits;

	public:
		Compressor(string filename) {
			this->filename = filename;
			maxBits = 0;
		}

		void setCharMap() {
			ifstream file(filename);
			if (!file.is_open()) {
				cout << "ERROR: Failed to open (" << filename << ")" << endl;
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

			maxBits = int(ceil(std::log2(charMap.size()))); // caculating maxBits
			if (maxBits == 0) { // handling edge case
				maxBits = 1;
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
				cout << "ERROR: Failed to open (" << filename << ")" << endl;
				return;
			}

			char letter;
			while (file.get(letter)) { // reading every letter
				for (int i = 0; i < charMap.size(); i++) { // looping through charMap
					if (letter == charMap.at(i)) {
						for (int j = maxBits - 1; j >= 0; j--) { // cheking all bits of letter
							bool isBitOn = i & (1 << j);
							bitArray.push_back(isBitOn);
						}
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

		void bitArrayToCharArray() {
			int count = 0;
			char packedChar = 0;
			for (int i = 0; i < bitArray.size(); i++) {
				// Cheking if packedChar is filled
				if (count == 8) {
					charArray.push_back(packedChar);
					count = 0;
					packedChar = 0;
				}
			
				packedChar = packedChar | (bitArray.at(i) << (7 - count));
				count++;
			}
		
			// Handling End Of File 
			if (count > 0) {
				charArray.push_back(packedChar);
			}

			cout << "SUCCESS: " << "cmpressed " << filename << endl;
		}

		void printCharArray() {
			cout << "Char Array: ";
			for (int i = 0; i < charArray.size(); i++) {
				cout << charArray.at(i);
			}
		}

		void createKeyFile() {
			size_t dotPosition = filename.rfind('.');
			string keyFilename = filename.substr(0, dotPosition) + "(key).txt";
			ofstream file(keyFilename, ios::binary);

			if (!file.is_open()) {
				cout << "ERROR: Failed to open (" << keyFilename << ")" << endl;
				return;
			}

			// Writting charMap in key file 
			for (int i = 0; i < charMap.size(); i++) {
				file << charMap.at(i);
			}

			cout << "  -> " << keyFilename << " created" << endl;
		}

		void createCompressedFile() {
			size_t dotPosition = filename.rfind('.');
			string compressedFileName = filename.substr(0, dotPosition) + "(compressed).bin";
			ofstream file(compressedFileName, ios::binary);

			if (!file.is_open()) {
				cout << "ERROR: Failed to open (" << compressedFileName << ")" << endl;
				return;
			}

			// Writting charArray in compressed file 
			for (int i = 0; i < charArray.size(); i++) {
				file << charArray.at(i);
			}

			cout << "  -> " << compressedFileName << " created" << endl;
			compressedFilename = compressedFileName;
		}

		void printCmpressionDetails() {
			filesystem::path filePath = filename;
			filesystem::path compressedFilePath = compressedFilename;

			try {
				double fileSize = static_cast<double>(filesystem::file_size(filePath));
				double compressedFileSize = static_cast<double>(filesystem::file_size(compressedFilePath));
				double compressionRatio = (compressedFileSize / fileSize) * 100;

				cout << "Compression Info: " << endl;
				cout << "  -> " << filename << " size: " << fileSize << " bytes" << endl;
				cout << "  -> " << compressedFilename << " size: " << compressedFileSize << " bytes" << endl;
				cout << "  -> Compression Ratio: " << compressionRatio << "%" << endl;
			}
			catch (const filesystem::filesystem_error& error) {
				std::cerr << "ERROR: " << error.what() << endl;
			}
		}
};

// Main Function
int main() {
	char filename[100];
	cout << "Enter filename to compress: ";
	cin >> filename;
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "ERROR: Failed to open: " << filename; 
	}

	auto startTime = chrono::high_resolution_clock::now(); // record start time

	Compressor compressor = Compressor(filename);
	
	compressor.setCharMap();
	//compressor.printCharMap();

	compressor.setBitArray();
	//compressor.printBitArray();

	compressor.bitArrayToCharArray();
	//compressor.printCharArray();

	compressor.createKeyFile();
	compressor.createCompressedFile();

	compressor.printCmpressionDetails();

	auto endTime = chrono::high_resolution_clock::now(); // record end time
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime); // calcuate time duration
	cout << endl << "Time taken: " << duration.count() << " ms" << endl;

	return 0;
}