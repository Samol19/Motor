#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;
class IOManager
{
public:
	static bool readFileToBuffer(string filepath, vector<unsigned char>& buffer);

};

