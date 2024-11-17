#include <fstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>

typedef std::unordered_map<int, std::string> List;

void loadFile(const std::string& fileName, List* map);
void merge(List* f1Map, List* f2Map, List* RMap);
int find(List* map, std::string* entry, int index);
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
	 * Meerges first file and second files hash maps into a new hashmap.
	 * It excludes duplicates. Resulting hash map has only quniue values.
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
		// std::cout << map->at(lineNum) << std::endl;
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
 * Merges two hashmaps with no duplicates.
 */

void merge(List* f1Map, List* f2Map, List* RMap)
{
	// all counter initialized to 1 because line count starts from 1.
	int counter1, counter2, counterR;
	counter1 = counter2 = counterR = 1;

	while(counter1 <= f1Map->size() || counter2 <= f2Map->size())
	{
		// If code exhausted f1Map, fill RMap with remainder of f2Map.
		if(counter1 > f1Map->size())
		{
			RMap->insert(std::pair<int, std::string>(
				counterR, f2Map->at(counter2)));
			counterR++;
			counter2++;
		}
		// If code exhausted f2Map, fill RMap with remainder of f2Map.
		else if(counter2 > f2Map->size())
		{
			RMap->insert(std::pair<int, std::string>(
				counterR, f1Map->at(counter1)));
			counterR++;
			counter1++;
		}
		// If both maps still have something then check if two leading
		// entries are the same.
		else if(f1Map->at(counter1) == f2Map->at(counter2))
		{
			// If yes then put one copy into RMap and increment all
			// counters.
			RMap->insert(std::pair<int, std::string>(
				counterR, f1Map->at(counter1)));
			counterR++;
			counter1++;
			counter2++;
		}
		else
		{
			// If entries differ then check if leading entry of f1Map exists in the
			// tail of f2Map (FirstEntrInSecondsTail). Also, do same check vice versa
			// (SecondEntrInFirstsTail).
			int FirstEntrInSecondsTail = find(f2Map, &f1Map->at(counter1), counter2);
			int SecondEntrInFirstsTail = find(f1Map, &f2Map->at(counter2), counter1);

			// If both entries are not unique relative to other tail
			if(FirstEntrInSecondsTail != -1 && SecondEntrInFirstsTail != -1)
			{
				// Then ignore both entries
				counter1++;
				counter2++;
			}
			else
			{
				// If leading entry of the f1Map is unique relative to f2Map tail
				if (FirstEntrInSecondsTail == -1)
				{
					// Then add f1Map leading entry to the RMap
					RMap->insert(std::pair<int, std::string>(
					counterR, f1Map->at(counter1)));
					// Increment both counterR and counter1
					counterR++;
					counter1++;
				}
				else
				{
					// If f1Map leading entry is not unique to f2Map tail
					// then ignore the f1Map leading entry
					counter1++;
				}
				// If leading entry of the f2Map is unique relative to f1Map tail
				if (SecondEntrInFirstsTail == -1)
				{
					// Then add f2Map leading entry to the RMap
					RMap->insert(std::pair<int, std::string>(
						counterR, f2Map->at(counter2)));
					// Increment counterR and counter2
					counterR++;
					counter2++;
				}
				else
				{
					// If f2Map leading entry is not unique relative to f1Map tail
					// then ignore the f2Map leading entry
					counter2++;
				}
			}
		}
	}
}

/*
 * Searches for the string (std::string) in the values of hashmap.
 * Because keys are line numbers find starts at index (default 1)
 * and goes until it ether find the value (called entry), or hits
 * the end. If the value is found then it returns the key (line
 * number). If value is not found then it returns -1. Used for
 * searching tails of a hashmap for leading entry of the other
 * hashmap.
 */

int find(List* map, std::string* entry, int index = 1)
{
	while(index <= map->size())
	{
		if(*entry == map->at(index))
			return index;
		index++;
	}
	return -1;
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