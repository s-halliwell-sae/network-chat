#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <string>

class Encrypter
{
public:
	//Hash without a salt
	std::string DietHash(const std::string& data);
	std::string DietHash(const char* data);

	std::string SaltedHash(const std::string& data);
	std::string SaltedHash(const char* data);

private:
	std::string salt = "SAEQChatProgram";

	std::string Hash(const std::string& data);

};

#endif //ENCRYPTER_H