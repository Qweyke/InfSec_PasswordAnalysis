#include <string>
#include <Windows.h>
#include <iostream>

// ASCII extended, CP866
constexpr uint8_t SPECIAL = 15 + 7 + 6 + 4;      // [33; 47] | [58; 64] | [91; 96] |  [123; 126]
constexpr uint8_t DIGITS = 10;                   // [48; 57]
constexpr uint8_t UPR_LAT_LETTERS = 26;          // [65; 90]
constexpr uint8_t LWR_LAT_LETTERS = 26;          // [97; 122]
constexpr uint8_t UPR_CYR_LETTERS = 32 + 1;      // [128; 159] | 240
constexpr uint8_t LWR_CYR_LETTERS = 16 + 16 + 1; // [160; 175] | [224; 239] | 241

// Variables for hacking time
constexpr uint32_t SECS_FOR_COMBINATION = 2;
constexpr uint32_t ATTEMPTS_BEFORE_LOCK = 3;
constexpr uint32_t SECS_FOR_UNLOCK = 15;

uint32_t countAlphPower(const uint8_t bitMask)
{
	uint32_t alphPower = 0;

	if (bitMask & (1 << 0))
		alphPower += SPECIAL;

	if (bitMask & (1 << 1))
		alphPower += DIGITS;

	if (bitMask & (1 << 2))
		alphPower += UPR_LAT_LETTERS;

	if (bitMask & (1 << 3))
		alphPower += LWR_LAT_LETTERS;

	if (bitMask & (1 << 4))
		alphPower += UPR_CYR_LETTERS;

	if (bitMask & (1 << 5))
		alphPower += LWR_CYR_LETTERS;

	return alphPower;
}

// Fast (binary) exponentiation method
uint64_t doBinExp(uint64_t base, uint32_t power)
{
	uint64_t res = 1; // if power == 0

	while (power > 0) // till last bit
	{

		if (power % 2 == 1) // check last bit
		{
			if (res >= (std::numeric_limits<uint64_t>::max)() / base)

				throw std::runtime_error("Exponentiation \"result\" variable overflow");

			res *= base;
		}

		if (base >= (std::numeric_limits<uint64_t>::max)() / base)
			throw std::runtime_error("Exponentiation \"base\" variable overflow");

		power /= 2;   // dispose of last bit
		base *= base; // iterate through each of binary places
	}

	return res;
}

void countHackTime(uint64_t cmbnQnty, uint32_t secsForAttempt, uint32_t attemptsBeforeLock, uint32_t lockTime)
{
	const uint32_t SECS_YEAR = 365 * 24 * 60 * 60;
	const uint32_t SECS_DAY = 24 * 60 * 60;
	const uint32_t SECS_HOURS = 60 * 60;
	const uint32_t SECS_MINUTES = 60;

	double totalSecs = (static_cast<double>(cmbnQnty) * secsForAttempt) + ((static_cast<double>(cmbnQnty) / attemptsBeforeLock) * secsForAttempt * lockTime);

	uint32_t years = static_cast<uint32_t>(totalSecs) / SECS_YEAR;
	totalSecs -= years * SECS_YEAR;

	uint32_t days = static_cast<uint32_t>(totalSecs) / SECS_DAY;
	totalSecs -= days * SECS_DAY;

	uint32_t hours = static_cast<uint32_t>(totalSecs) / SECS_HOURS;
	totalSecs -= hours * SECS_HOURS;

	uint32_t minutes = static_cast<uint32_t>(totalSecs) / SECS_MINUTES;
	totalSecs -= minutes * SECS_MINUTES;

	uint32_t secs = static_cast<uint32_t>(std::round(totalSecs));

	std::cout << years << " year(s) - " << days << " day(s) - " << hours << " hour(s) - " << minutes << " minute(s)  " << secs << " sec(s). \n";
}

void checkPswdStrength(const std::string& pswd)
{

	UINT encoding = GetConsoleCP();

	if (encoding == 866)
	{
		std::cout << "DOS Cyrillic Russian - CP" << encoding << "\n";

		uint8_t bitMask = 0;

		for (unsigned char ch : pswd)
		{
			if ((ch >= '!' && ch <= '/') || (ch >= ':' && ch <= '@') || (ch >= '[' && ch <= '`') || (ch >= '{' && ch <= '~'))

				bitMask |= 1 << 0; // SPECIAL

			else if (ch >= '0' && ch <= '9')

				bitMask |= 1 << 1; // DIGITS

			else if (ch >= 'A' && ch <= 'Z')

				bitMask |= 1 << 2; // UPR_LAT_LETTERS

			else if (ch >= 'a' && ch <= 'z')

				bitMask |= 1 << 3; // LWR_LAT_LETTERS

			else if ((ch >= 128 && ch <= 159) || ch == 240)

				bitMask |= 1 << 4; // UPR_CYR_LETTERS

			else if ((ch >= 160 && ch <= 175) || (ch >= 224 && ch <= 239) || ch == 241)

				bitMask |= 1 << 5; // LWR_CYR_LETTERS
		}

		uint32_t alphPower = countAlphPower(bitMask);
		uint64_t cmbnQnty = doBinExp(alphPower, static_cast<uint32_t>(pswd.size()));

		std::cout << "Power of the alphabet: " << alphPower << "\n";
		std::cout << "Password length: " << pswd.size() << "\n";
		std::cout << "Combinations quantity: " << cmbnQnty << "\n";

		countHackTime(cmbnQnty, 2, 3, 10);
	}

	else
		throw std::runtime_error("Unsupported encoding: " + std::to_string(encoding));
}

int main()
{
	std::string input;
	std::cin >> input;
	std::cout << "\n";

	try
	{
		checkPswdStrength(input);
	}
	catch (std::runtime_error& Er)
	{
		std::cout << "Error: " << Er.what() << "\n";
		return 1;
	}

	return 0;
}