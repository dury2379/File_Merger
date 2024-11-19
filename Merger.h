//
// Created by den on 11/19/24.
//

#ifndef MERGER_H
#define MERGER_H

#include <unordered_map>
#include <string>

/*
 * For simplicity, hashmap<int, std::string> is defined
 * into a sort of alias called List. This is made to make
 * code look nice.
 */
typedef std::unordered_map<int, std::string> List;

/*
 * Merges two hashmaps with no duplicates.
 */
void merge(List* f1Map, List* f2Map, List* RMap);

/*
 * Searches for the string (std::string) in the values of hashmap.
 * Because keys are line numbers find starts at index (default 1)
 * and goes until program ether find the value (called entry), or
 * hits the end. If the value is found then it returns the key
 * (line number). If value is not found then it returns -1. Used
 * for searching tails of a hashmap for leading entry of the other
 * hashmap.
 */

int find(List* map, std::string* entry, int index);

#endif //MERGER_H
