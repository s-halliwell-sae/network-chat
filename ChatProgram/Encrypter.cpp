#include "Encrypter.h"
#include "MurmurHash3.h"

std::string Encrypter::DietHash(const std::string& data)
{
	return Hash(data);
}

std::string Encrypter::DietHash(const char* data)
{
	std::string tempStr;
	tempStr.assign(data);
	return Hash(tempStr);
}

std::string Encrypter::SaltedHash(const std::string& data)
{
	return Hash(data + salt);
}

std::string Encrypter::SaltedHash(const char* data)
{
	std::string tempStr;
	tempStr.assign(data);
	tempStr += salt;
	return Hash(tempStr);
}

std::string Encrypter::Hash(const std::string& data)
{
	void* vp = (void*)data.c_str();
	void* vpOut = new char[128 / 8];
	MurmurHash3_x86_128(vp, data.length(), 0, vpOut);

	std::string endStr;
	endStr.assign((char*)vpOut, 128 / 8);
	return endStr;
}