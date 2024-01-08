#pragma once
#include <memory>
#include <string>

namespace tracker
{
    class Crypt
    {
    public:
        Crypt() = default;
        virtual ~Crypt() = default;
        virtual std::string Encrypt(const std::string& input) = 0;
        virtual std::string Decrypt(const std::string& input) = 0;
        virtual std::string Encrypt(const void* input, size_t size) = 0;
        virtual std::string Decrypt(const void* input, size_t size) = 0;
    };

    class AESImpl;

    class AEScrypt : public Crypt
    {
    public:
        static std::string GetKey(const std::string& salt, const std::string& password);
        explicit AEScrypt(const std::string& key);

        ~AEScrypt() override;
        std::string Encrypt(const std::string& input) override;
        std::string Decrypt(const std::string& input) override;
        std::string Encrypt(const void* input, size_t size) override;
        std::string Decrypt(const void* input, size_t size) override;

    private:
        std::unique_ptr<AESImpl> impl_;
    };
};
