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
    sv = NULL;
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
    {
        sc = new s_scan_header;
        sv = new s_value;
    }
    // Define structures:
    if (snap)
    {
        fread(sn,sizeof(s_snapshot_header),1,f);
        // Get size of memory block.
        bytesLeft = sn->size;
        // And write ability
        write = sn->perms&F_WRITE_ONLY;
    }
    else
    {
        fread(sc,sizeof(s_scan_header),1,f);
        // vsize * 2 because we have actual value and old value.
        bytesLeft = sc->vsize * 2;
    }
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
    if (sv) free(sv);
}

void * reader::getActualMemoryPointer()
{
    // Calculate real position of the block from the start of the memory:
    if (snap) return (void*)((long)sn->start + ((long)sn->size - bytesLeft) - 1);
    if (!snap) return sv->start;
    return NULL;
}

bool reader::eof(size_t sizeofvalue)
{
    return (sizeofvalue > (fullSize - ftell(f)));
}

void reader::getOld(any_value &v)
{
    // If snapshot, the old value and the actual value are the same.
    // So I need to rewind a byte and extract again:
    if (snap)
    {
        fseek(f,-1,SEEK_CUR);
        this->operator>>(v);
    }
    // If it is a scan, old value is next to actual value, which must be already
    // readed, so I need only to extract next bytes:
    if (!snap)
    {
        // Extract value:
        fread(v.getPointer(),v.size(),1,f);
        bytesLeft-=v.size();
    }
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
            // And write ability
            write = sn->perms&F_WRITE_ONLY;
        }
        // Read value, and go one byte forward:

        fread(extract.getPointer(),size,1,f);
        fseek(f,1-size,SEEK_CUR);
        // Substract one byte from the counter:
        bytesLeft--;
    }
    else
    {
        // Check if we are behind a s_value:
        if (bytesLeft == 0)
        {
            // We ended extracting bytes, get next value:
            fread(sv,sizeof(s_value),1,f);
            // Reset bytesLeft:
            bytesLeft = sc->vsize;
            // Reset write ability:
            write = sv->perms&F_WRITE_ONLY;
            // Extract the size of the value:
            fread(extract.getPointer(),size,1,f);
            // Substract bytes from the counter
            bytesLeft-=size;
        }
        // Else, we are behind the old value; jump next value:
        else
        {
            fseek(f,size,SEEK_CUR);
            // Now we are behind a s_value:
            fread(sv,sizeof(s_value),1,f);
            // Reset bytesLeft:
            bytesLeft = sc->vsize;
            // Reset write ability:
            write = sv->perms&F_WRITE_ONLY;
            // Extract the size of the value:
            fread(extract.getPointer(),size,1,f);
            // Substract bytes from the counter
            bytesLeft-=size;
        }
    }
    return *this;
}
