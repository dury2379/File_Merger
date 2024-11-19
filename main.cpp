#include <fstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>
#include "Merger.h"

/*
 * For simplicity, hashmap<int, std::string> is defined
 * into a sort of alias called List. This is made to make
 * code look nice.
 */
typedef std::unordered_map<int, std::string> List;

void loadFile(const std::string& fileName, List* map);
void dumpToFile(const std::string& fileName, List* map);
void deleteList(List* map);

int main()
{
	std::string f1name;
	std::string f2name;
	std::string fRName;
	List f1Map;
	List f2Map;
	List fRMap;

	/*
	 * Enter filenames of first and second file to merge.
	 * If a file does not exist, then program will ask user to reenter the filename.
	*/

	do
	{
		std::cout << "Enter first file name: ";
		std::cin >> f1name;
	} while(!std::filesystem::exists(f1name));

	do
	{
		std::cout << "Enter second file name: ";
		std::cin >> f2name;
	} while(!std::filesystem::exists(f2name));

	/*
	 * Enter filename of output file. If file already exists then program
	 * will ask user to renter the filename.
	*/
	do
	{
		std::cout << "Enter name of output file: ";
		std::cin >> fRName;
	} while(std::filesystem::exists(fRName));

	/*
	 * Loads file in format of a hash map. Keys are line numbers (int) and values are
	 * lines themselves (std::string).
	*/
	loadFile(f1name, &f1Map);
	loadFile(f2name, &f2Map);

	/*
	 * Merges first file and second files hash maps into a new hashmap.
	 * It excludes duplicates. Resulting hash map has only unique values.
	*/
	merge(&f1Map, &f2Map, &fRMap);

	/*
	 * Takes a hashmap of format key: int (line number), value: std::string
	 * (line itself) and puts it into the file with name "fRName" (FileResultName
	 * or resulting file filename).
	*/
	dumpToFile(fRName, &fRMap);

	/*
	 * Delete all allocated memory.
	*/
	deleteList(&f1Map);
	deleteList(&f2Map);
	deleteList(&fRMap);

	return 0;
}

/*
 * Takes in a filename and empty hashmap. Opens the file.
 * Takes a line and puts it into the hashmap. Key is the
 * line number, starts from 1. Value is line itself. If
 * file could not be opened then it throws an exception,
 * message: File could not be opened. Closes the file
 * when it hits eof.
 */

void loadFile(const std::string& fileName, List* map)
{
	std::fstream file;
	int lineNum = 1;

	file.open(fileName, std::ios::in);
	if (!file.is_open())
	{
		throw std::runtime_error("File could not be opened");
	}

	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);
		map->insert(std::pair<int, std::string>(lineNum, line));
		lineNum++;
	}
	file.close();
}

/*
 * Takes in a filename and a hashmap of format key: int, value: std::string.
 * Opens file and writes each line in sequential order (keys are int that
 * indicate the line number). Adds a new line character to the end of every
 * line but the last. Closes the file.
 */

void dumpToFile(const std::string& fileName, List* map)
{
	std::fstream file;
	file.open(fileName, std::ios::out);

	for(int i = 1; i <= map->size(); i++)
	{
		file << map->at(i);
		if(i != map->size())
		{
			file << "\n";
		}
	}
	file.close();
}

/*
 * Deletes all values (std::string) in a hashmap.
 * Also deletes hashmap itself afterward.
 */

void deleteList(List* map)
{
	for(int i = 1; i <= map->size(); i++)
	{
		delete map->at(i);
	}
	delete map;
}