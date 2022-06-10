#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;

template<typename T>
bool IsContain(vector<T*>& vec, T* element)
{
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == element)
			return true;
	}
	return false;
}

template<typename T>
int GetIndex(vector<T*>& vec, T* element)
{
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == element)
			return i;
	}
	return -1;
}
template<typename T>
void DeleteElement(vector<T*>& vec, T* element) {
	vec.erase(vec.begin() + GetIndex(vec, element));
}

template<typename T>
typename vector<T*>::iterator BinarySearch(vector<T*>& vec, typename vector<T*>::iterator start, typename vector<T*>::iterator end, T* element) {
	typename vector<T*>::iterator middle =  start + ((end - start) / 2);
	if ((*middle)->_index == element->_index) 
		return middle;
	if (start == end && (*middle)->_index != element->_index)
		return vec.end();
	if ((*middle)->_index < element->_index)
		return BinarySearch(vec, middle, end, element);
	if ((*middle)->_index > element->_index)
		return BinarySearch(vec, start, middle, element);
}