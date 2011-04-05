#include "reader.h"

// ANY VALUE IMPLEMENTATION

// All values (long,float and double).

any_value::any_value()
{
    sz = sizeof(long) + sizeof(float) + sizeof(double);
    buff = malloc(sz);
    vt = ALL;
}

// Long declaration:
any_value::any_value(long v)
{
    sz = sizeof(v);
    buff = malloc(sz);
    vt = BYTE8;
}

// Pointer declaration:
any_value::any_value(any_value * v)
{
    // Memcpy all the class:
    memcpy(this,v,sizeof(any_value));
    // But buff can't be memcopied, so create new one:
    buff = malloc(sz);
    // And restore its data:
    memcpy(buff,v->getPointer(),sz);
}

// Destructor:
any_value::~any_value()
{
    if (buff)
        free(buff);
}

// Misc functions:

size_t any_value::size()
{
    return sz;
}

// Operators:
bool any_value::operator==(any_value& v)
{
    if (numberValues(vt) != numberValues(v.getValueType()))
        return false;
    if (intValues(vt) != intValues(v.getValueType()))
        return *(long*)buff == (long)v;
    if (intValues(vt))
        return *(long*)buff == (long)v;
    return false;
}

bool any_value::operator>(any_value& v)
{
    if (numberValues(vt) != numberValues(v.getValueType()))
        return false;
    if (intValues(vt) != intValues(v.getValueType()))
        return *(long*)buff > (long)v;
    if (intValues(vt))
        return *(long*)buff > (long)v;
    return false;
}

bool any_value::operator<(any_value& v)
{
    if (numberValues(vt) != numberValues(v.getValueType()))
        return false;
    if (intValues(vt) != intValues(v.getValueType()))
        return *(long*)buff < (long)v;
    if (intValues(vt))
        return *(long*)buff < (long)v;
    return false;
}


// Casters:
any_value::operator long()
{
    if (intValues(vt) || (vt == ALL))
    {
        long result;
        memcpy(&result,buff,sizeof(result));
        return result;
    }
    if (vt == FLOAT)
    {
        float result;
        memcpy(&result,buff,sizeof(result));
        return (long)result;
    }
    if (vt == DOUBLE)
    {
        double result;
        memcpy(&result,buff,sizeof(result));
        return (long)result;
    }
    return 0L;
}

// READER IMPLEMENTATION:

reader::reader(FILE * fs,bool snpsht,pid_t target, bool writeable)
{
    sn = NULL;
    sc = NULL;
    // Init all values:
    if (fs == NULL)
    {
        f = snapshot(target,writeable);
        snap = true;
        local = true;
    }
    else
    {
        local = false;
        snap = snpsht;
        f = fs;
    }
    if (snap)
        sn = new s_snapshot_header;
    else
        sc = new s_scan_header;
    // Define structures:
    if (snap)
    {
        fread(sn,sizeof(s_snapshot_header),1,f);
        bytesLeft = sn->size;
    }
    else
        fread(sc,sizeof(s_scan_header),1,f);
    // Get the full size of the FILE:
    long oldPos = ftell(f);
    fseek(f,0,SEEK_END);
    fullSize = ftell(f);
    // And restore it old position:
    fseek(f,oldPos,SEEK_SET);
    show_info("File size of reader is: %lu\n",fullSize);
}

reader::~reader()
{
    if (local) fclose(f);
    if (sn) free(sn);
    if (sc) free(sc);
}

void * reader::getActualMemoryPointer()
{
    // Calculate real position of the block from the start of the memory:
    if (snap) return (void*)((long)sn->start + ((long)sn->size - bytesLeft) - 1);
    return NULL;
}

bool reader::eof(size_t sizeofvalue)
{
    return (sizeofvalue > (fullSize - ftell(f)));
}

FILE * reader::getFile()
{
    return f;
}

reader& reader::operator>>(any_value &extract) throw (const char *)
{
    const size_t size = extract.size();
    if (snap)
    {
        // Check if we got to the end of the current block:
        if (bytesLeft < size)
        {
            // Try to jump to the next memory block:
            if (fseek(f,bytesLeft,SEEK_CUR))
                throw ("Error reading snapshot, check it using eof!");
            // Read block's header:
            fread(sn,sizeof(s_snapshot_header),1,f);
            // Get the size of the new block:
            bytesLeft = sn->size;
        }
        // Read value, and go one byte forward:

        fread(extract.getPointer(),size,1,f);
        fseek(f,1-size,SEEK_CUR);
        // Substract one byte of the counter:
        bytesLeft--;
    }
    return *this;
}
