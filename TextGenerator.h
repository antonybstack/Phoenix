#pragma once

#include <string>
#include <array>
#include <vector>
#include "ConsoleEngine.h"

std::vector<std::wstring> letterArr(char c) {
	std::vector<std::wstring> LETTER;

	switch (c)
	{
	case ' ':
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		break;

	case '0':

		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L".ooo.");
		break;

	case '1':

		LETTER.push_back(L".o.");
		LETTER.push_back(L"oo.");
		LETTER.push_back(L".o.");
		LETTER.push_back(L".o.");
		LETTER.push_back(L".o.");
		LETTER.push_back(L".o.");
		LETTER.push_back(L".o.");
		LETTER.push_back(L".o.");
		LETTER.push_back(L"ooo");
		break;

	case '2':

		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"...o.");
		LETTER.push_back(L"..o..");
		LETTER.push_back(L".o...");
		LETTER.push_back(L"o....");
		LETTER.push_back(L"o....");
		LETTER.push_back(L"ooooo");
		break;

	case '3':

		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L".ooo.");
		break;

	case '4':

		LETTER.push_back(L"....o.");
		LETTER.push_back(L"...oo.");
		LETTER.push_back(L"..o.o.");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L"oooooo");
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"....o.");
		break;

	case '5':

		LETTER.push_back(L"ooooo");
		LETTER.push_back(L"o....");
		LETTER.push_back(L"o....");
		LETTER.push_back(L"o....");
		LETTER.push_back(L"oooo.");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"oooo.");
		break;
	case '6':

		LETTER.push_back(L".ooo..");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.oo..");
		LETTER.push_back(L"oo..o.");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L".ooo..");
		break;
	case '7':

		LETTER.push_back(L"oooooo");
		LETTER.push_back(L".....o");
		LETTER.push_back(L".....o");
		LETTER.push_back(L".....o");
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"..o...");
		LETTER.push_back(L".o....");
		LETTER.push_back(L"o.....");
		break;
	case '8':

		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L".ooo.");
		break;
	case '9':

		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L"o...o");
		LETTER.push_back(L".oooo");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		LETTER.push_back(L"....o");
		break;

	case 'A':

		LETTER.push_back(L"..oo..");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"oooooo");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		break;

	case 'B':

		LETTER.push_back(L"ooooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"ooooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"ooooo.");
		break;

	case 'C':

		LETTER.push_back(L".oooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".oooo.");
		break;

	case 'D':

		LETTER.push_back(L"oooo..");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L"oooo..");
		break;

	case 'E':
		LETTER.push_back(L".ooooo");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".oooo.");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".ooooo");
		break;

	case 'F':

		LETTER.push_back(L"oooooo");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"oooo..");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		break;

	case 'G':

		LETTER.push_back(L".oooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o..ooo");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".oooo.");
		break;

	case 'H':

		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"oooooo");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		break;

	case 'I':

		LETTER.push_back(L".ooooo");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L".ooooo");
		break;

	case 'J':

		LETTER.push_back(L"oooooo");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"o..o..");
		LETTER.push_back(L"o..o..");
		LETTER.push_back(L".oo...");
		break;

	case 'K':

		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L"o..o..");
		LETTER.push_back(L"ooo...");
		LETTER.push_back(L"o.o...");
		LETTER.push_back(L"o..o..");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		break;

	case 'L':

		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"oooooo");
		break;

	case 'M':

		LETTER.push_back(L"o....o");
		LETTER.push_back(L"oo..oo");
		LETTER.push_back(L"o.oo.o");
		LETTER.push_back(L"o.oo.o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		break;

	case 'N':

		LETTER.push_back(L"oo...o");
		LETTER.push_back(L"oo...o");
		LETTER.push_back(L"o.o..o");
		LETTER.push_back(L"o.o..o");
		LETTER.push_back(L"o.oo.o");
		LETTER.push_back(L"o..o.o");
		LETTER.push_back(L"o..o.o");
		LETTER.push_back(L"o...oo");
		LETTER.push_back(L"o...oo");
		break;

	case 'O':

		LETTER.push_back(L"..oo..");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L"..oo..");
		break;

	case 'P':

		LETTER.push_back(L"ooooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"ooooo.");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		break;

	case 'Q':

		LETTER.push_back(L".oooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o..o.o");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L".ooo.o");
		break;

	case 'R':

		LETTER.push_back(L"ooooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"ooooo.");
		LETTER.push_back(L"o..o..");
		LETTER.push_back(L"o...o.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		break;

	case 'S':

		LETTER.push_back(L".oooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L".oooo.");
		LETTER.push_back(L".....o");
		LETTER.push_back(L".....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".oooo.");
		break;

	case 'T':

		LETTER.push_back(L"oooooo");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		break;

	case 'U':

		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".oooo.");
		break;

	case 'V':

		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L"..oo..");
		break;

	case 'W':

		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o.oo.o");
		LETTER.push_back(L"o.oo.o");
		LETTER.push_back(L".o..o.");
		break;

	case 'X':

		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L"..oo..");
		LETTER.push_back(L".o..o.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L"o....o");
		break;

	case 'Y':

		LETTER.push_back(L"o.....o");
		LETTER.push_back(L".o...o.");
		LETTER.push_back(L"..o.o..");
		LETTER.push_back(L"...o...");
		LETTER.push_back(L"...o...");
		LETTER.push_back(L"...o...");
		LETTER.push_back(L"...o...");
		LETTER.push_back(L"...o...");
		LETTER.push_back(L"...o...");
		break;

	case 'Z':

		LETTER.push_back(L"oooooo");
		LETTER.push_back(L".....o");
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"..o...");
		LETTER.push_back(L".o....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"oooooo");
		break;

	case '$':

		LETTER.push_back(L"..o..");
		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"o.o.o");
		LETTER.push_back(L"o.o..");
		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"..o.o");
		LETTER.push_back(L"o.o.o");
		LETTER.push_back(L".ooo.");
		LETTER.push_back(L"..o..");
		break;

	case '!':

		LETTER.push_back(L".o");
		LETTER.push_back(L".o");
		LETTER.push_back(L".o");
		LETTER.push_back(L".o");
		LETTER.push_back(L".o");
		LETTER.push_back(L".o");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L".o");
		break;

	case '?':

		LETTER.push_back(L".oooo.");
		LETTER.push_back(L"o....o");
		LETTER.push_back(L".....o");
		LETTER.push_back(L".....o");
		LETTER.push_back(L"...oo.");
		LETTER.push_back(L"..o...");
		LETTER.push_back(L"......");
		LETTER.push_back(L"......");
		LETTER.push_back(L"..o...");
		break;

	case '\"':
		LETTER.push_back(L"o.o");
		LETTER.push_back(L"o.o");
		LETTER.push_back(L"o.o");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		break;

	case '\'':
		LETTER.push_back(L"o");
		LETTER.push_back(L"o");
		LETTER.push_back(L"o");
		LETTER.push_back(L".");
		LETTER.push_back(L".");
		LETTER.push_back(L".");
		LETTER.push_back(L".");
		LETTER.push_back(L".");
		LETTER.push_back(L".");
		break;

	case ',':
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L"...");
		LETTER.push_back(L".o.");
		LETTER.push_back(L"o..");
		break;

	case '.':
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"..");
		LETTER.push_back(L"oo");
		LETTER.push_back(L"oo");
		break;

	case '\\':
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".o....");
		LETTER.push_back(L"..o...");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"....o.");
		break;

	case '/':
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"....o.");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"...o..");
		LETTER.push_back(L"..o...");
		LETTER.push_back(L".o....");
		LETTER.push_back(L".o....");
		LETTER.push_back(L"o.....");
		LETTER.push_back(L"o.....");
		break;
	}
	return LETTER;
}

std::wstring OutputText(std::string sentence) {
	std::vector<std::vector<std::wstring>> LETTERS;
	for (int i = 0; i < sentence.length(); i++) {
		std::vector<std::wstring> LETTER = letterArr(toupper(sentence[i]));

		LETTERS.push_back(LETTER);
		if (sentence[i] != '!' || '?' || '\"' || '\'' || ',' || '.' || '\\' || '/') {
			std::vector<std::wstring> LETTERSPACE = letterArr(' ');
			LETTERS.push_back(LETTERSPACE);
		}
	}
	std::wstring sText;

	int index = 0;
	for (unsigned short i = 0; i < 9; i++) {
		for (auto& LETTER : LETTERS) // access by reference to avoid copying
		{
			sText += LETTER.at(index);
		}
		index++;
	}
	//}

	//std::array<std::array<std::wstring, 9>, 5> ArrayOfLetterArrays = { *LETTER,*LETTER,*LETTER,*LETTER,*LETTER };
	//std::wstring* test[5] = { LETTER, LETTER, LETTER, LETTER, LETTER };
	//std::wstring sText;
	//short index = 0;
	//for (unsigned short i = 0; i < 9; i++) {
	//	//for (unsigned short j = 0; j < 9; j++) {
	//	sText += LETTER[i];
		//}
	//}

	return sText;
}
