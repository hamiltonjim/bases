#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <unistd.h>

using namespace std;

bool isSigned = true;
bool bits64 = true;

template <class T>
void swapv(T& left, T& right) {
    T temp = left;
    left = right;
    right = temp;
}

string chars(long long number) {
    char foo[sizeof number];
    memset(foo, 0, sizeof foo);
    *(long long *)foo = number;
    
    if (BYTE_ORDER == 1234) {
            // reverse the bytes
        const int maxi = sizeof foo - 1;
        const int maxc = sizeof foo >> 1;
        for (int ctr = 0; ctr < maxc; ++ctr)
            swapv(foo[ctr], foo[maxi - ctr]);
    }
    
    return string(foo, sizeof foo);
}

string bytes(long long number) {
    string bytes;
    bytes.reserve(3 * sizeof number - 1);
    char foo[sizeof number];
    *(long long *)foo = number;
    
    if (BYTE_ORDER == 4321) {
            // reverse the bytes
        const int maxi = sizeof foo - 1;
        const int maxc = sizeof foo >> 1;
        for (int ctr = 0; ctr < maxc; ++ctr)
            swapv(foo[ctr], foo[maxi - ctr]);
    }
    
    for (int ctr = sizeof number; --ctr >= 0;) {
        const char *hexdigits = "0123456789ABCDEF";
        unsigned char byte = foo[ctr];
        if (bytes.length()) bytes += ' ';
        bytes += hexdigits[byte >> 4];
        bytes += hexdigits[byte & 0x0f];
    }
    
    return bytes;
}

void showUsage() {
    cerr << "Usage:" << endl
    << "bases [-usil] number ..." << endl
    << "\t-u or -s: treat as unsigned or signed (default signed)" << endl
    << "\t-i or -l: treat as 32-bit or 64-bit (default 64-bit)" << endl;
    
    cerr << "Use -- to end options if you use negative numbers." << endl;
}

int main (int argc, char * const argv[]) {
    
    // options
    int optionVal;
    while ((optionVal = getopt(argc, argv, "suli")) != -1) {
        switch (optionVal) {
            case 's':
                isSigned = true;
                break;
                
            case 'u':
                isSigned = false;
                break;
                
            case 'l':
                bits64 = true;
                break;
                
            case 'i':
                bits64 = false;
                break;
                
            default:
                showUsage();
                return 1;
                
        }
    }
    
    for (int ctr = optind; ctr < argc; ++ctr) {
        long long number;
        
        char * const nextArg = argv[ctr];
        if (isdigit(nextArg[0]) || (nextArg[0] == '-' && isdigit(nextArg[1]))) {
            if (bits64) {
                if (isSigned) {
                    number = strtoll(nextArg, NULL, 0);
                } else {
                    number = strtoull(nextArg, NULL, 0);
                }
            } else {
                if (isSigned) {
                    number = strtol(nextArg, NULL, 0);
                } else {
                    number = strtoul(nextArg, NULL, 0);
                }
            }
        } else {
            number = 0;
            for (int len = 0; len < 8 && nextArg[len]; ++len) {
                number <<= 8;
                number += nextArg[len];
            }
        }
        
        cout << argv[ctr] << ": "
        << dec << number // decimal
        << " 0" << oct << number
        << " 0x" << hex << number
        << " '" << chars(number) << "'"
        << " " << bytes(number)
        << endl;
    }
    return 0;
}
