#ifndef READER_H
#define READER_H

#include "memory.h"

// Snapshot FILE system is:
// s_snapshot_header - memory block - s_snapshot_header - memory block ... eof

/* This structure must be at start of each block of memory in an snapshot */
struct s_snapshot_header
{
    char perms;         // Permission flags of the block
    unsigned long size; // Size of the block
    void * start;       // Where the block is in memory
};

// Scan FILE system is:
// s_scan_header - s_value - actual value - first value - s_value - actual value - first value - s_value - actual value ... eof

/* This structure is the scan file header using when searching for values */
struct s_scan_header
{
    unsigned long values;       // How much values the program found
    size_t vsize;               // Size of values
};

/* This structure must be before any found value */
struct s_value
{
    char perms;     // Permission flags of the value.
    void * start;   // Where the value is in memory.
};

// Integer values:
#define intValues(value_type) ((value_type >= BYTE) && (value_type <= BYTE8))
// Floating point values:
#define floatValues(value_type) ((value_type == DOUBLE) || (value_type == FLOAT))
// Number values:
#define numberValues(value_type) (((value_type >= BYTE) && (value_type <= DOUBLE)) || value_type == ALL)


enum VALUE_TYPE
{
    BINARY = 0, BYTE = 1, BYTE2 = 2, BYTE4 = 4, BYTE8 = 8, FLOAT, DOUBLE, TEXT, BYTEARRAY, ALL
};


class any_value
{
private:
    VALUE_TYPE vt;
    void * buff;
    size_t sz;
public:
    // All values:
    any_value();
    // The rest of values:
    any_value(bool v);
    any_value(char v);
    any_value(short v);
    any_value(int v);
    any_value(long v);
    any_value(float v);
    any_value(double v);
    any_value(char * v);
    any_value(void * v);
    // A any_value of the same type as another any_value:
    any_value(any_value * v);
    // Destructor:
    ~any_value();
    // Pointer:
    void * getPointer(){return buff;}
    int getValueType(){return vt;}
    // Size:
    size_t size();

    bool operator ==(any_value& v);
    bool operator >(any_value& v);
    bool operator >=(any_value& v);
    bool operator <(any_value& v);
    bool operator <=(any_value& v);

    operator bool();
    operator char();
    operator short();
    operator int();
    operator long();
    operator float();
    operator double();
    operator char *();
};

// This class is used to read from a FILE * of an snapshot or an older scan.
class reader
{
private:
    // FILE pointer
    FILE * f;
    // If local, we take care of the FILE pointer deallocating.
    bool local;
    // If snapshot and not scan:
    bool snap;
    // EOF bit:
    bool write;
    // Structures needed:
    s_scan_header * sc;
    s_snapshot_header * sn;
    s_value * sv;
    // How much bytes are  there left until the end of the block.
    // Used with snapshots
    unsigned long bytesLeft;
    // full size of FILE:
    unsigned long fullSize;

public:
    reader(FILE * fs, bool snapshot,pid_t target, bool writeable = false);
    ~reader();
    bool eof(size_t sizeofvalue);
    void * getActualMemoryPointer();
    // Get old value.
    void getOld(any_value &v);
    FILE * getFile(){return f;}
    bool getWritePerms(){return write;}

    // Operators:
    reader& operator>>(any_value &extract) throw (const char *);

};

#endif // READER_H
