﻿#pragma once
#include"VietNamese.h"

class Word {
public:
	wstring key; //Ex: dog, cat...
	wstring type; // Noun, adj...
	wstring spelling; // /ˈap(ə)l/
	vector<wstring> definitions; //Just definitions
	Word();
	Word(wstring key, wstring type, wstring def);
	void addDefinition(wstring def);
	void removeDefinition(int idx);
};
