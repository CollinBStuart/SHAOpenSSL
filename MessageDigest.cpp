#include "MessageDigest.h"
#import <CommonCrypto/CommonDigest.h>
#import <CommonCrypto/CommonCryptor.h>
#import <Security/Security.h>
#import <iostream>
#include <openssl/sha.h>

string MessageDigest::messageDigestStringFromData(vector<uint8_t> data)
{
    string resultString;
    
    if ( ! data.empty())
    {
        unsigned char sha1Hash[SHA_DIGEST_LENGTH];
        SHA_CTX context;
        bzero(&context, sizeof(context));
        bool success = SHA1_Init(&context);
        if (success)
        {
            success = SHA1_Update(&context, data.data(), data.size());
        }
        if (success)
        {
            success = SHA1_Final(sha1Hash, &context);
        }
        
        
        if (success)
        {
            resultString.reserve(SHA_DIGEST_LENGTH);
            for (std::size_t i = 0; i != SHA_DIGEST_LENGTH; ++i)
            {
                resultString += "0123456789abcdef"[sha1Hash[i] / 16];
                resultString += "0123456789abcdef"[sha1Hash[i] % 16];
            }
        }
    }
    
    return resultString;
}

CFStringRef MessageDigest::createMessageDigestStringFromData(CFDataRef chunkData)
{
    string resultString;
    
    if (chunkData)
    {
        CFIndex chunkDataLength = CFDataGetLength(chunkData);
        vector<uint8_t> vector;
        vector.resize(chunkDataLength);
        CFDataGetBytes(chunkData, CFRangeMake(0, chunkDataLength), &vector[0]);
        resultString = messageDigestStringFromData(vector);
    }
    
    /* Common Crypto Way.... non-portable
    uint8_t sha1Hash[CC_SHA1_DIGEST_LENGTH];
    CC_SHA1_CTX sha1Context;
    bzero(&sha1Context, sizeof(sha1Context));
    CC_SHA1_Init(&sha1Context);
    CC_SHA1_Update(&sha1Context, CFDataGetBytePtr(chunkData), (CC_LONG)chunkDataLength);
    CC_SHA1_Final(sha1Hash, &sha1Context);
    CFMutableStringRef messageDigestString = CFStringCreateMutable(kCFAllocatorDefault, 0);
    CFDataRef hashData = CFDataCreate(kCFAllocatorDefault, sha1Hash, CC_SHA1_DIGEST_LENGTH);
    CFStringRef hashRepresentationString = CFCopyDescription(hashData);
    CFStringAppend(messageDigestString, hashRepresentationString);
    CFRelease(hashRepresentationString);
    CFRelease(hashData);
    
    string hashString = CFStringGetCStringPtr(messageDigestString, kCFStringEncodingUTF8);
    CFRelease(messageDigestString);
    
    string findString = "bytes = 0x";
    unsigned long location = hashString.find(findString);
    if (location != string::npos)
    {
        hashString = hashString.substr(location + findString.length(), 40);
    }
     */
    
    return CFStringCreateWithCString(kCFAllocatorDefault, resultString.c_str(), kCFStringEncodingUTF8);
}