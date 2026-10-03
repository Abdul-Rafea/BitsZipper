#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

// Function Prototypes
int charArraySize(char* charArray);
char* charArrayCombine(char* charArray1, char* charArray2);

// Main Function
int main() {
	char filename[100];
	cout << "Enter filename to compress: ";
	cin >> filename;

	// Opening File
	ifstream file(filename, ios::binary);
	if (!file.is_open()) {
		cout << "ERROR: Failed to open: " << filename;
		return 0;
	}
	
	// Recording Start Time
	auto startTime = chrono::high_resolution_clock::now();
	
	// Calculating Character Map And File Size
	char letter;
	int charMapSize = 0;
	char* charMap = nullptr;
	int fileSize = 0;
	while (file.get(letter)) {
		bool matchFound = false;
		
		for (int i = 0; i < charMapSize; i++) {
			if (letter == charMap[i]) {
				matchFound = true;
			}
		}

		if (!matchFound) {
			charMapSize++;
			char* oldCharMap = charMap;
			char* newCharMap = new char[charMapSize]; // creating new array

			for (int i = 0; i < charMapSize - 1; i++) { // copying old array data
				newCharMap[i] = charMap[i];
			}

			newCharMap[charMapSize - 1] = letter; // adding new letter
			charMap = newCharMap; // passing new array pointer
			delete[] oldCharMap;
		}

		fileSize++;
	}

	// Calculate Max Bits Per Character
	int maxBits = int(ceil(log2(charMapSize)));
	if (maxBits == 0) { // handling edge case
		maxBits = 1;
	}
	
	// Reseting ifstream
	file.clear();                 // clear EOF flag
	file.seekg(0, ios::beg); // set pointer to beginning of file
	
	// Calculating Bit Array
	int bitArraySize = 0;
	bool* bitArray = nullptr;
	while (file.get(letter)) {
		for (int i = 0; i < charMapSize; i++) {
			if (letter == charMap[i]) {
				for (int j = maxBits - 1; j >= 0; j--) {
					bool isBitOn = i & (1 << j);
					
					bitArraySize++;
					bool* oldBitArray = bitArray;
					bool* newBitArray = new bool[bitArraySize]; // creating new array
					for (int k = 0; k < bitArraySize - 1; k++) { // copying old array into new
						newBitArray[k] = bitArray[k];
					}
					newBitArray[bitArraySize - 1] = isBitOn; // setting mew bit
					bitArray = newBitArray; // passing new pointer
					delete[] oldBitArray;
				}
			}
		}
	}

	//// Printing Bit Array
	//cout << "Bit Array: ";
	//for (int i = 0; i < bitArraySize; i++) {
	//	cout << bitArray[i];
	//}
	
	// Creating And Opening Compressed And Key Files
	char compressedFileExtension[] = "(compressed).txt";
	char keyFileExtension[] = "(key).txt";
	char filenameLegth = charArraySize(filename);
	filename[filenameLegth - 4] = '\0';
	char* compressedFileName = charArrayCombine(filename, compressedFileExtension);
	char* keyFileName = charArrayCombine(filename, keyFileExtension);

	ofstream compressedFile(compressedFileName, ios::binary);
	if (!compressedFile.is_open()) {
		cout << "ERROR: Failed to open " << compressedFileName;
		return 0;
	}
	
	ofstream keyFile(keyFileName, ios::binary);
	if (!keyFile.is_open()) {
		cout << "ERROR: Failed to open " << keyFileName;
		return 0;
	}

	// Writting Key File
	keyFile << fileSize << " ";
	for (int i = 0; i < charMapSize; i++) {
		keyFile << charMap[i];
	}

	// Packing Characters And Caculating Compressed File Size
	char packedChar = 0;
	int fillCount = 0;
	int compressedFileSize = 0;
	for (int i = 0; i < bitArraySize; i++) {
		if (fillCount == 8) { // writing in compressed file when packedChar is filled
			compressedFile << packedChar;
			packedChar = 0;
			fillCount = 0;
			compressedFileSize++;
		}

		packedChar = packedChar | (bitArray[i] << (7 - fillCount));
		fillCount++;
	}
	 
	if (fillCount > 0) { // handling EOF state
		compressedFile << packedChar;
		compressedFileSize++;
	}

	// Recording End Time And Calcualting Time Duration
	auto endTime = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

	// Printing Compresson Details
	double compressionRatio = (static_cast<double>(compressedFileSize) / fileSize) * 100;

	cout << endl << "Compression Info: -" << endl;
	cout << "  -> Time Take: " <<duration.count() << " ms" << endl;
	cout << "  -> File Size: " << fileSize << endl;
	cout << "  -> Compressed File Size: " << compressedFileSize << endl;
	cout << "  -> Compression Ratio: " << compressionRatio << "%";

	// Closing Opened Files
	file.close();
	compressedFile.close();
	keyFile.close();

	// Deallocating Memeory
	delete[] charMap;
	delete[] bitArray;
	delete[] compressedFileName;
	delete[] keyFileName;

	return 0;
}

// Function Definitions
int charArraySize(char* charArray) {
	int size = 0;
	while (charArray[size] != '\0') {
		size++;
	}

	return size;
}

char* charArrayCombine(char* charArray1, char* charArray2) {
	char* charArray = new char[100];
	int filledIndex = 0;

	// Copying First Array
	for (int i = 0; i < charArraySize(charArray1); i++) {
		charArray[i] = charArray1[i];
		filledIndex++;
	}

	// Copying Second Array
	for (int i = 0; i < charArraySize(charArray2); i++) {
		charArray[filledIndex] = charArray2[i];
		filledIndex++;
	}
	
	// Adding Null Character At Last Index
	charArray[filledIndex] = '\0';

	return charArray;
}