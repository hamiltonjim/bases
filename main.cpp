#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

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

int main (int argc, char * const argv[]) {
    
    
    for (int ctr = 1; ctr < argc; ++ctr) {
        long long number;
        
        if (isdigit(argv[ctr][0])) {
            number = strtoll(argv[ctr], NULL, 0);
        } else {
            number = 0;
            char *parm = argv[ctr];
            for (int len = 0; len < 8 && parm[len]; ++len) {
                number <<= 8;
                number += parm[len];
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
