#ifndef HEX_TOOLS_H
#define HEX_TOOLS_H
#include <string>
#include <cmath>
using namespace std;

namespace hexTools{
	int hexCharToInt(char charDigit){
		int res;
		int digit = charDigit - 48;
		if(digit <= 9 && digit >= 0)
			res = digit;
		else{
			switch(charDigit){
				case 'a':
					res = 10;
					break;
				case 'b':
					res = 11;
					break;
				case 'c':
					res = 12;
					break;
				case 'd':
					res = 13;
					break;
				case 'e':
					res = 14;
					break;
				case 'f':
					res = 15;
					break;
			}
		}
		return res;
	}

	int hexToInt(string hexWord){
		int res = 0;
		for(size_t i = 0; i < hexWord.size(); i++)
			res += hexCharToInt(hexWord[i]) * (int)pow(16.0, (double)i);
		return res;
	}

}
#endif
