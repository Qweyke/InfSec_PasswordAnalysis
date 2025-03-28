#include <string>
#include <Windows.h>
#include <iostream>

namespace
{
	// ASCII extended, CP866
	constexpr uint8_t SPECIAL = 15 + 7 + 6 + 4;      // [33; 47] | [58; 64] | [91; 96] |  [123; 126]
	constexpr uint8_t DIGITS = 10;                   // [48; 57]
	constexpr uint8_t UPR_LAT_LETTERS = 26;          // [65; 90]
	constexpr uint8_t LWR_LAT_LETTERS = 26;          // [97; 122]
	constexpr uint8_t UPR_CYR_LETTERS = 32 + 1;      // [128; 159] | 240
	constexpr uint8_t LWR_CYR_LETTERS = 16 + 16 + 1; // [160; 175] | [224; 239] | 241

	// Variables for hacking time
	constexpr uint32_t SECS_FOR_COMBINATION = 1;
	constexpr uint32_t ATTEMPTS_BEFORE_LOCK = 5;
	constexpr uint32_t SECS_FOR_UNLOCK = 10;

	// Max value 2^64
	constexpr uint64_t MAX_VALUE = (std::numeric_limits<uint64_t>::max)();

} // namespace

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
			if (res >= MAX_VALUE / base)

				throw std::overflow_error("Exponentiation \"result\" variable overflow");

			res *= base;
		}

		if (base >= MAX_VALUE / base)

			throw std::overflow_error("Exponentiation \"base\" variable overflow");

		power /= 2;   // dispose of last bit
		base *= base; // iterate through each of binary places
	}

	std::cout << "Combinations quantity: " << res << "\n";
	return res;
}

void countHackTime(uint64_t cmbnQnty, uint32_t secsForAttempt, uint32_t attemptsBeforeLock, uint32_t lockTime)
{
	const uint32_t SECS_YEAR = 365 * 24 * 60 * 60;
	const uint32_t SECS_DAY = 24 * 60 * 60;
	const uint32_t SECS_HOURS = 60 * 60;
	const uint32_t SECS_MINUTES = 60;

	uint64_t totalSecs = 0;

	if (cmbnQnty > MAX_VALUE / secsForAttempt)
	{
		std::cout << "Overflow in multiplying secs and combinations. Setting total secs to MAX_VALUE.\n";
		totalSecs = MAX_VALUE;
	}
	else
	{
		totalSecs = cmbnQnty * secsForAttempt;
	}

	uint64_t lockPenalty = 0;
	if (cmbnQnty / attemptsBeforeLock > MAX_VALUE / lockTime)
	{
		std::cout << "Overflow in lock penalty. Setting lock penalty to MAX_VALUE.\n";
		lockPenalty = MAX_VALUE;
	}
	else
	{
		if (cmbnQnty % attemptsBeforeLock == 0)
		{
			lockPenalty = ((cmbnQnty / attemptsBeforeLock) * lockTime) - 1 * lockTime;
		}
		else
			lockPenalty = (cmbnQnty / attemptsBeforeLock) * lockTime;
	}

	if (totalSecs > MAX_VALUE - lockPenalty)
	{
		std::cout << "Overflow in suming lock penalty and secs for all attempts. Setting total secs to MAX_VALUE.\n";
		totalSecs = MAX_VALUE;
	}
	else
	{
		totalSecs += lockPenalty;
	}

	std::cout << totalSecs << " Total secs \n";

	uint64_t years = totalSecs / SECS_YEAR;
	totalSecs %= SECS_YEAR;

	uint64_t days = totalSecs / SECS_DAY;
	totalSecs %= SECS_DAY;

	uint64_t hours = totalSecs / SECS_HOURS;
	totalSecs %= SECS_HOURS;

	uint64_t minutes = totalSecs / SECS_MINUTES;
	totalSecs %= SECS_MINUTES;

	uint64_t secs = totalSecs;

	std::cout << years << " year(s) - " << days << " day(s) - " << hours << " hour(s) - " << minutes << " minute(s)  " << secs << " sec(s). \n";
}

void checkPswdStrength()
{

	UINT encodingInp = GetConsoleCP();

	std::cout << "Encoding of console: Inp-" << encodingInp << " Out-" << GetConsoleOutputCP() << "\n";

	if (encodingInp != 866)
	{
		SetConsoleCP(866);
		SetConsoleOutputCP(866);

		std::cout << "Encoding of console changed to: Inp-" << GetConsoleCP() << " Out-" << GetConsoleOutputCP() << "\n";
	}

	std::string pswd;
	std::cin >> pswd;

	uint8_t bitMask = 0;
	uint32_t size = 0;

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

		size++;
	}

	uint32_t alphPower = countAlphPower(bitMask);
	std::cout << "Power of the alphabet: " << alphPower << "\n";
	std::cout << "Password length: " << size << "\n";

	uint64_t cmbnQnty;

	try
	{
		cmbnQnty = doBinExp(alphPower, size);
	}
	catch (std::overflow_error)
	{
		std::cout << "Combinations quantity limit for C++ uint64_t exceeded, combinations quantity set to 2^64 \n";
		cmbnQnty = MAX_VALUE;
	}

	countHackTime(cmbnQnty, SECS_FOR_COMBINATION, ATTEMPTS_BEFORE_LOCK, SECS_FOR_UNLOCK);
}

int main()
{
	try
	{
		checkPswdStrength();
	}
	catch (std::exception& er)
	{
		std::cout << "Error: " << er.what() << "\n";
		return 1;
	}

	return 0;
}