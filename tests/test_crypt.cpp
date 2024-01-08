#include <gtest/gtest.h>
#include "src/crypt/crypt.h"

TEST(CryptTest, Test1)
{
    std::string key = tracker::AEScrypt::GetKey("salt", "password");
    EXPECT_TRUE(!key.empty());

    tracker::Crypt* crypt = new tracker::AEScrypt(key);
    std::string plain = "hello world";
    std::string cipher = crypt->Encrypt(plain);
    EXPECT_TRUE(!cipher.empty());
    std::string recovered = crypt->Decrypt(cipher);
    EXPECT_TRUE(!recovered.empty());
    EXPECT_EQ(plain, recovered);
    delete crypt;
}
