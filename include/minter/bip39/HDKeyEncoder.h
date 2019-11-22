/*! 
 * bip39. 2018
 * 
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef BIP39_KEY_ENCODER_H
#define BIP39_KEY_ENCODER_H

#include "minter/crypto/secp256k1.h"
#include "minter/crypto/base58.h"
#include "minter/crypto/ecdsa.h"
#include "minter/crypto/hmac.h"
#include "minter/crypto/curves.h"
#include "bip39_core.h"
#include "utils.h"
#include "Bip39Mnemonic.h"

namespace minter {

struct BTCNetwork {
  std::string name;
  std::string bech32;
  uint32_t bip32[2];
  uint8_t pubKeyHash;
  uint8_t scriptHash;
  uint8_t wif;
};

static const BTCNetwork MainNet = {
    "bitcoin",
    "bc",
    {0x0488b21e, 0x0488ade4},
    0x00,
    0x05,
    0x80
};

static const BTCNetwork TestNet = {
    "testnet",
    "tb",
    {0x043587cf, 0x04358394},
    0x6f,
    0xc4,
    0xef
};

static const std::string MASTER_SECRET = "Bitcoin seed";

class BIP39_CORE_API Derivation {
 public:
    Derivation(const std::string &path);
    Derivation(std::string &&path);
    Derivation(const char* path);

    std::string path;
};

class BIP39_CORE_API HDKey {
 public:
    HDKey();
    ~HDKey();
    HDKey(const HDKey &other);
    HDKey(HDKey &&other) noexcept;
    HDKey &operator=(minter::HDKey other);

    void clear();

    FixedData<33> publicKey;
    FixedData<32> privateKey;
    FixedData<32> chainCode;
    Bip32Key extPrivateKey;
    Bip32Key extPublicKey;
    BTCNetwork net;
    uint8_t depth;
    uint32_t index;
    uint32_t fingerprint;
    const curve_info *curve;
};

class BIP39_CORE_API HDKeyEncoder {
 public:
    static Data64 makeBip39Seed(const std::string &mnemonicWords);
    static Data64 makeBip39Seed(const std::vector<std::string> &mnemonicWords);
    static HDKey makeBip32RootKey(const char* mnemonic, BTCNetwork net = minter::MainNet);
    static HDKey makeBip32RootKey(const Data64 &seed, BTCNetwork net = minter::MainNet);
    static void makeExtendedKey(HDKey &rootKey, const Derivation &derivation);
 private:
    static void derive(HDKey &key, uint32_t index);
    static HDKey fromSeed(const Data &seed);
    static HDKey fromSeed(const char *seed);
    static HDKey fromSeed(const uint8_t *seed, size_t seedLength);
    static void derivePath(HDKey &key, const std::string &path, bool priv = true);
    static uint32_t fetchFingerprint(HDKey &key);
    static void fillPublicKey(HDKey &key);
    static void serialize(HDKey &key, uint32_t fingerprint, uint32_t version, bool publicKey = false);
};
}

#endif //BIP39_KEY_ENCODER_H
