#include <string>
#include <Windows.h>
#include <iostream>

uint32_t handleMaskBits()
{
}

uint32_t checkPswdStrength(const std::string& pswd)
{
	UINT encoding = GetConsoleOutputCP();
	if (encoding == 866)
	{
		std::cout << "DOS Cyrillic Russian - 866"
		          << "\n";
		uint32_t pswdPower = 0;

		uint32_t mask = 0;

		for (char ch : pswd)
		{
			if (ch >= 33 && ch < 48)
			{
				pswdPower += 15;
			}

			else if (ch >= 48 && ch < 58)
			{
				pswdPower += 10;
			}

			else if (ch >= 48 && ch < 58)
			{
				pswdPower += 10;
			}
		}
	}

	else
	{
		std::cout << "Encoding: " << encoding << "\n";
		return 0;
	}

	return 0;
}

uint32_t countHackTime(uint32_t speed, uint32_t tries, uint32_t lockTime)
{
	return 0;
}

int main()
{
	// std::string input;
	// std::cin >> input;

	return 0;
}