#pragma once
#include <iostream>
#include "Bone.h"
#include <vector>
#include <fstream>
//git
std::string* getDest(std::string destPath, char splitchar = '/');
void swap(std::vector<vertex>& a);
struct FileInfo {
	bool exist;
	std::string ending;
};
FileInfo seeIfFileExist(std::string fileName);