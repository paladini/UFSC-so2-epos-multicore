// EPOS OStream Implementation

#include <utility/ostream.h>
#include <machine.h>

extern "C" { void _panic(); }

__BEGIN_UTIL

const char OStream::_digits[] = "0123456789abcdef";

void OStream::preamble()
{
    static char tag[] = "<0>: ";

    int me = Machine::cpu_id();
    int last = CPU::cas(_lock, -1, me);
    for(int i = 0, owner = last; (i < 10) && (owner != me); i++, owner = CPU::cas(_lock, -1, me));
    if(last != me) {
        tag[1] = '0' + Machine::cpu_id();
        print(tag);
    }
}

void OStream::trailler()
{
    static char tag[] = " :<0>";

    if(_lock != -1) {
        tag[3] = '0' + Machine::cpu_id();
        print(tag);

        _lock = -1;
    }
    if(_error)
        _panic();
}


int OStream::itoa(int v, char * s)
{
    unsigned int i = 0;

    if(v < 0) {
        v = -v;
        s[i++] = '-';
    }

    return utoa(static_cast<unsigned int>(v), s, i);
} 


int OStream::utoa(unsigned int v, char * s, unsigned int i)
{
    unsigned int j;

    if(!v) {
        s[i++] = '0';
        return i;
    }

    if(v > 256) {
        if(_base == 8 || _base == 16)
            s[i++] = '0';
        if(_base == 16)
            s[i++] = 'x';
    }

    for(j = v; j != 0; i++, j /= _base);
    for(j = 0; v != 0; j++, v /= _base)
        s[i - 1 - j] = _digits[v % _base];

    return i;
}


int OStream::llitoa(long long int v, char * s)
{
    unsigned int i = 0;

    if(v < 0) {
        v = -v;
        s[i++] = '-';
    }

    return llutoa(static_cast<unsigned long long int>(v), s, i);
} 


int OStream::llutoa(unsigned long long int v, char * s, unsigned int i)
{
    unsigned long long int j;

    if(!v) {
        s[i++] = '0';
        return i;
    }

    if(v > 256) {
        if(_base == 8 || _base == 16)
            s[i++] = '0';
        if(_base == 16)
            s[i++] = 'x';
    }

    for(j = v; j != 0; i++, j /= _base);
    for(j = 0; v != 0; j++, v /= _base)
        s[i - 1 - j] = _digits[v % _base];

    return i;
}


int OStream::ptoa(const void * p, char * s)
{
    unsigned int j, v = reinterpret_cast<unsigned int>(p);

    s[0] = '0';
    s[1] = 'x';

    for(j = 0; j < sizeof(void *) * 2; j++, v >>= 4)
        s[2 + sizeof(void *) * 2 - 1 - j]
            = _digits[v & 0xf];

    return j + 2;
}    

__END_UTIL
