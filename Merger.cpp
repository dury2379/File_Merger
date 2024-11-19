//
// Created by den on 11/19/24.
//

#include "Merger.h"

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
 * and goes until program ether find the value (called entry), or
 * hits the end. If the value is found then it returns the key
 * (line number). If value is not found then it returns -1. Used
 * for searching tails of a hashmap for leading entry of the other
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