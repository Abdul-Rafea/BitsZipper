#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

using namespace std;

class Decompressor {
private:
	string filename;
	vector<char> charMap;
	int maxBits;

public:
	Decompressor(string fileName) {
		filename = fileName;
		maxBits = 0;
	}
};

// Main Function
int main() {
	string filename;

	cout << "Enter file name to decompress: ";
	cin >> filename;

	return 0;
}