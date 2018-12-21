/*
Program by Anthony Rodriguez
Github: ZalChan
Purpose: Hide files in plain sight by using your knowledge of c++ :D	
 */
#include <iostream>
#include <dirent.h>
#include <fstream>
using namespace std;

/*----------------------------------------------------------*/
const char *FolderPath = "../../../../Pictures/qWallpaper"; //Configure
string sFolderPath(FolderPath); 							//
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

inline void encrypt_char(char& in);
inline void decrypt_char(char& in);

int main(int argc, char**argv)  
{
	if (argc < 2 || (argv[1][0] != 'e' && argv[1][0] != 'd') || argv[1][1] != '\0') {
		return 1;
	}
	bool encrypt_mode = (argv[1][0] == 'e' ? true : false);
	DIR *dir;
	struct dirent *ent;
	int imageCount = 0;
	ifstream readingFolderState("FolderState.txt");
	if (readingFolderState.is_open()) {
		string line;
		getline(readingFolderState, line);
		if (line == "folder is encrypted" && encrypt_mode) {
			cout << "Folder already encrypted!\n";
			readingFolderState.close();
			return 1;
		}
		if (line == "folder is not encrypted" && !encrypt_mode) {
			cout << "Folder is already decrypted\n";
			readingFolderState.close();
			return 1;
		}
		readingFolderState.close();
	}
	if ((dir = opendir (FolderPath)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			imageCount++;
			string nthFile = ent->d_name;
			string filePath = sFolderPath+ '/' +nthFile;
			if (filePath == sFolderPath+"/." || filePath == sFolderPath+"/..") {
				continue;
			}
			fstream file(filePath, ios::in | ios::out);
			file.seekg(0, ios::end);
			int size = file.tellg();
			file.seekg(0, ios::beg);
			char* fileData = new char[size]; 
			file.read(fileData, size);
			for (int i = 0; i < size; i++) {
				if (encrypt_mode) 
					encrypt_char(fileData[i]);
				else 
					decrypt_char(fileData[i]);
			}
			file.seekp(0, ios::beg); 
			file.write(fileData, size);
			delete[] fileData;
			file.close();
		}
		closedir (dir);
	} else {
		return EXIT_FAILURE;
	}
	ofstream writingFolderState("FolderState.txt");
	if (writingFolderState.is_open()) {
		if (encrypt_mode) {
			writingFolderState << "folder is encrypted";
		}
		else {
			writingFolderState << "folder is not encrypted";
		}
	}
	if (encrypt_mode)
		cout << "Encrypted " << imageCount << " files!\n";
	else
		cout << "Restored " << imageCount << " files!\n";
}

inline void encrypt_char(char& in) {
	in += 3;
}

inline void decrypt_char(char& in) {
	in -= 3;
}
