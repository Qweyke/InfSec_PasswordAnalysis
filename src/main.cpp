#include <string>
#include <Windows.h>
#include <iostream>

// ASCII extended, 866
constexpr uint8_t SPECIAL = 15 + 7 + 6 + 4;      // [33; 47] | [58; 64] | [91; 96] |  [123; 126]
constexpr uint8_t DIGITS = 10;                   // [48; 57]
constexpr uint8_t UPR_LAT_LETTERS = 26;          // [65; 90]
constexpr uint8_t LWR_LAT_LETTERS = 26;          // [97; 122]
constexpr uint8_t UPR_CYR_LETTERS = 32 + 1;      // [128; 159] | 240
constexpr uint8_t LWR_CYR_LETTERS = 16 + 16 + 1; // [160; 175] | [224; 239] | 241

uint8_t checkMaskBits(const uint8_t mask)
{
	uint32_t pswPwr = 0;

	if (mask & (1 << 0))
		pswPwr += SPECIAL;

	if (mask & (1 << 1))
		pswPwr += DIGITS;

	if (mask & (1 << 2))
		pswPwr += UPR_LAT_LETTERS;

	if (mask & (1 << 3))
		pswPwr += LWR_LAT_LETTERS;

	if (mask & (1 << 4))
		pswPwr += UPR_CYR_LETTERS;

	if (mask & (1 << 5))
		pswPwr += LWR_CYR_LETTERS;

	return pswPwr;
}

uint32_t checkPswdStrength(const std::string& pswd)
{
	UINT encoding = GetConsoleOutputCP();

	std::cout << pswd << "\n";

	if (encoding == 866)
	{
		std::cout << "DOS Cyrillic Russian - " << encoding << "\n";

		uint8_t mask = 0;

		for (unsigned char ch : pswd)
		{
			if (!(mask & (1 << 0)) && ((ch >= '!' && ch <= '/') || (ch >= ':' && ch <= '@') || (ch >= '[' && ch <= '`') || (ch >= '{' && ch <= '~')))
			{
				mask += 1 << 0; // SPECIAL
			}

			else if (!(mask & (1 << 1)) && (ch >= '0' && ch <= '9'))
			{
				mask += 1 << 1; // DIGITS
			}

			else if (!(mask & (1 << 2)) && (ch >= 'A' && ch <= 'Z'))
			{
				mask += 1 << 2; // UPR_LAT_LETTERS
			}

			else if (!(mask & (1 << 3)) && (ch >= 'a' && ch <= 'z'))
			{
				mask += 1 << 3; // LWR_LAT_LETTERS
			}

			else if (!(mask & (1 << 4)) && ((ch >= 128 && ch <= 159) || ch == 240))
			{
				mask += 1 << 4; // UPR_CYR_LETTERS
			}

			else if (!(mask & (1 << 5)) && ((ch >= 160 && ch <= 175) || (ch >= 224 && ch <= 239) || ch == 241))
			{
				mask += 1 << 5; // LWR_CYR_LETTERS
			}
		}

		return checkMaskBits(mask);
	}

	else
		throw std::runtime_error("Unsupported encoding: " + std::to_string(encoding));
}

uint32_t countHackTime(uint32_t speed, uint32_t tries, uint32_t lockTime)
{
	return 0;
}

int main()
{
	// UINT encodingInp = GetConsoleOutputCP();
	// UINT encodingOut = GetConsoleOutputCP();

	// std::cout << "Output: " << GetConsoleOutputCP() << "\n Inp: " << GetConsoleCP() << "\n";

	std::string input;
	std::cin >> input;

	uint32_t pswdStrength = checkPswdStrength(input);

	std::cout << "Password strength is " << pswdStrength << "\n";

	return 0;
}