#ifndef __C__MessageDigest__
#define __C__MessageDigest__

#include <stdio.h>
#include <string>
#include <vector>
#include <CoreFoundation/CoreFoundation.h>

using namespace std;

class MessageDigest
{
public:
    static string messageDigestStringFromData(vector<uint8_t> data);
    static CFStringRef createMessageDigestStringFromData(CFDataRef data); //need to CFRelease data
};

#endif /* defined(__C__MessageDigest__) */
