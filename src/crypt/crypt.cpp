#include "crypt.h"

#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>

namespace tracker
{
    using byte = CryptoPP::byte;

    class AESImpl
    {
    public:
        explicit AESImpl(const std::string& key);
        ~AESImpl();
        AESImpl(const AESImpl&) = delete;
        AESImpl& operator=(const AESImpl&) = delete;

        void Init(const char* key, size_t size);

        std::string Encrypt(const void* input, size_t size);
        std::string Decrypt(const void* input, size_t size);

    private:
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption enc_;
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption dec_;
        byte iv_[CryptoPP::AES::BLOCKSIZE] = {0};
    };

    AESImpl::AESImpl(const std::string& key)
    {
        Init(key.data(), key.size());
    }

    AESImpl::~AESImpl() = default;

    void AESImpl::Init(const char* key, size_t size)
    {
        enc_.SetKeyWithIV(reinterpret_cast<const byte*>(key), size, iv_);
        dec_.SetKeyWithIV(reinterpret_cast<const byte*>(key), size, iv_);
    }


    std::string AESImpl::Encrypt(const void* input, size_t size)
    {
        std::string cipher;
        try
        {
            CryptoPP::StringSource ss(reinterpret_cast<const byte*>(input), size, true,
                                      new CryptoPP::StreamTransformationFilter(enc_,
                                                                               new CryptoPP::StringSink(cipher),
                                                                               CryptoPP::StreamTransformationFilter::PKCS_PADDING));
        }
        catch (const CryptoPP::Exception& e)
        {
            return "";
        }
        return cipher;
    }

    std::string AESImpl::Decrypt(const void* input, size_t size)
    {
        std::string recovered;
        try
        {
            CryptoPP::StringSource ss(reinterpret_cast<const byte*>(input), size, true,
                                      new CryptoPP::StreamTransformationFilter(dec_,
                                                                               new CryptoPP::StringSink(recovered),
                                                                               CryptoPP::StreamTransformationFilter::PKCS_PADDING));
        }
        catch (const CryptoPP::Exception& e)
        {
            return "";
        }
        return recovered;
    }

    std::string AEScrypt::GetKey(const std::string& salt, const std::string& password)
    {
        CryptoPP::SecByteBlock key(CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf;
        pbkdf.DeriveKey(key, key.size(), 0,
                        reinterpret_cast<const CryptoPP::byte*>(password.data()), password.size(),
                        reinterpret_cast<const CryptoPP::byte*>(salt.data()), salt.size(),
                        1000, 0.0);
        return std::string(reinterpret_cast<char*>(key.BytePtr()), key.size());
    }

    AEScrypt::AEScrypt(const std::string& key) : impl_(std::make_unique<AESImpl>(key))
    {
    }

    AEScrypt::~AEScrypt()
    {
    }

    std::string AEScrypt::Encrypt(const std::string& input)
    {
        return impl_->Encrypt(input.data(), input.size());
    }

    std::string AEScrypt::Decrypt(const std::string& input)
    {
        return impl_->Decrypt(input.data(), input.size());
    }

    std::string AEScrypt::Encrypt(const void* input, size_t size)
    {
        return impl_->Encrypt(input, size);
    }

    std::string AEScrypt::Decrypt(const void* input, size_t size)
    {
        return impl_->Decrypt(input, size);
    }
}
