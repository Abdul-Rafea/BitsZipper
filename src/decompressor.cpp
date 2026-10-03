#include <fstream>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

// Function Prototypes
int calcCharArraySize(char* charArray);
char* charArrayCombine(char* charArray1, char* charArray2);
void addNewChar(char*& charArray, int& size, char letter);
void addNewChar(int*& charArray, int& size, char letter);

// Main Function
int main() {
	char compressedFilename[100];
	char keyFilename[100];

	cout << "Enter file name to decompress: ";
	cin >> compressedFilename;
	cout << "Enter key file name: ";
	cin >> keyFilename;

	// Recording Start Time
	auto startTime = chrono::high_resolution_clock::now();

	// Opening Compressed And Key Files
	ifstream compressedFile(compressedFilename, ios::binary);
	if (!compressedFile.is_open()) {
		cout << "ERROR: Failed to open " << compressedFilename;
		return 0;
	}

	ifstream keyFile(keyFilename, ios::binary);
	if (!keyFile.is_open()) {
		cout << "ERROR: Failed to open " << keyFilename;
		return 0;
	}

	// Getting File Size
	int fileSize = 0;
	keyFile >> fileSize;
	keyFile.get(); // remove space

	// Calclating Char Map
	char letter;
	int charMapSize = 0;
	char* charMap = nullptr;
	
	while (keyFile.get(letter)) {
		addNewChar(charMap, charMapSize, letter);
	}

	//// Printing Char Map
	//cout << "Char Map: ";
	//for (int i = 0; i < charMapSize; i++) {
	//	cout << charMap[i];
	//}
	//cout << endl;
	
	// Calculate Max Bits Per Character
	int maxBits = int(ceil(log2(charMapSize)));
	
	if (maxBits == 0) { // handling edge case
		maxBits = 1;
	}

	// Unpacking Packed Characters And Calculating Character Array
	int charArraySize = 0;
	int* charArray = nullptr;
	char unpackedChar = 0;
	int usedBits = 0;
	while (compressedFile.get(letter) && charArraySize < fileSize) {
		for (int i = 0; i < 8; i++) {
			bool isBitOn = letter & (128 >> i); // ckecking bit state
			if (isBitOn) {
				unpackedChar = unpackedChar | (1 << (maxBits - usedBits - 1));
			}
			usedBits++;

			if (usedBits == maxBits) {
				addNewChar(charArray, charArraySize, unpackedChar);
				unpackedChar = 0;
				usedBits = 0;
			}

			if (charArraySize == fileSize) {
				break;
			}
		}
	}

	//// Printing Character Array
	//cout << "Char Array: ";
	//for (int i = 0; i < charArraySize; i++) {
	//	cout << charArray[i];
	//}
	//cout << endl;

	// Creating And Opening Decmpressed File
	char decompressedFileExtension[] = "(decompressed).txt";
	int keyFilenameLength = calcCharArraySize(keyFilename);
	keyFilename[keyFilenameLength - 9] = '\0';
	char* decompressedFilename = charArrayCombine(keyFilename, decompressedFileExtension);

	ofstream decompressedFile(decompressedFilename, ios::binary);
	if (!decompressedFile.is_open()) {
		cout << "ERROR: Failed to open" << decompressedFilename;
		return 0;
	}

	// Writting Character Array in Compressed File
	for (int i = 0; i < charArraySize; i++) {
		decompressedFile << charMap[charArray[i]];
	}

	// Recording End Time And Calcualting Time Duration
	auto endTime = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

	// Printing Decompression info
	cout << endl << "Decompression Info: -" << endl;
	cout << "  -> Time Taken: " << duration.count() << "ms" << endl;

	// Closing Opened Files
	compressedFile.close();
	keyFile.close();

	// Deallocating Memory
	delete[] charMap;
	delete[] charArray;

	return 0;
}

// Function Definitions
int calcCharArraySize(char* charArray) {
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
	for (int i = 0; i < calcCharArraySize(charArray1); i++) {
		charArray[i] = charArray1[i];
		filledIndex++;
	}

	// Copying Second Array
	for (int i = 0; i < calcCharArraySize(charArray2); i++) {
		charArray[filledIndex] = charArray2[i];
		filledIndex++;
	}

	// Adding Null Character At Last Index
	charArray[filledIndex] = '\0';

	return charArray;
}

void addNewChar(char*& charArray, int& size, char letter) {
	size++;
	char* oldCharMap = charArray; // pointing to old char map
	char* newCharMap = new char[size]; // creating new char map
	for (int i = 0; i < size - 1; i++) { // copying old array into new
		newCharMap[i] = charArray[i];
	}
	newCharMap[size - 1] = letter; // adding new letter
	charArray = newCharMap; // passing new array pointer
	delete[] oldCharMap;
}

void addNewChar(int*& charArray, int& size, char letter) {
	size++;
	int* oldCharMap = charArray; // pointing to old char map
	int* newCharMap = new int[size]; // creating new char map
	for (int i = 0; i < size - 1; i++) { // copying old array into new
		newCharMap[i] = charArray[i];
	}
	newCharMap[size - 1] = static_cast<int>(letter); // adding new letter
	charArray = newCharMap; // passing new array pointer
	delete[] oldCharMap;
}