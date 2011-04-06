// This file includes several parts of file ptrace.c of 'scanmem' proyect.

#include "memory.h"

bool attach(pid_t target)
{
    int status;

    /* attach, to the target application, which should cause a SIGSTOP */
    if (ptrace(PTRACE_ATTACH, target, NULL, NULL) == -1L) {
        show_error("failed to attach to %d, %s\n", target, strerror(errno));
        return false;
    }

    /* wait for the SIGSTOP to take place. */
    if (waitpid(target, &status, 0) == -1 || !WIFSTOPPED(status)) {
        show_error("there was an error waiting for the target to stop.\n");
        show_error("%s\n", strerror(errno));
        return false;
    }

    /* everything looks okay */
    return true;

}

bool detach(pid_t target)
{
    // addr is ignore under Linux, but should be 1 under FreeBSD in order to let the child process continue at what it had been interrupted
    return ptrace(PTRACE_DETACH, target, 1, 0) == 0;
}

/* read region using /proc/pid/mem */
ssize_t readregion(pid_t target, void *buf, size_t count, unsigned long offset)
{
    char mem[32];
    int fd;
    ssize_t len = 0;

    /* print the path to mem file */
    snprintf(mem, sizeof(mem), "/proc/%d/mem", target);

    /* attempt to open the file */
    if ((fd = open(mem, O_RDONLY)) == -1) {
        show_error("unable to open %s.\n", mem);
        return -1;
    }

    /* try to honour the request */
    int tmp;
    while ((size_t)len < count)
    {
        tmp = pread(fd, (void*)((long)buf + len), count - len , offset + len);
        if (tmp == -1)
            break;
        len += tmp;
    }

    /* clean up */
    close(fd);

    return len;
}


/* read region using /proc/pid/mem */
ssize_t writeregion(pid_t target, void *buf, size_t count, unsigned long offset)
{
    // Calculate the nearest size of void*
    size_t size = 0;
    while (size*sizeof(void*) < count)
        size++;
    // Copying the real data:
    long * tmp = (long*)calloc(sizeof(void*),size);
    readregion(target, tmp, size, offset);
    // Changing real data to new data:
    memcpy(tmp,buf,count);
    // Write new data:
    for (unsigned long i = 0; i < size; i++)
    {
        if (ptrace(PTRACE_POKEDATA, target, offset + i*sizeof(void*), tmp[i]) == -1L)
        {
            show_error("POKEDATA failed.\n");
            show_error("%s\n", strerror(errno));
            return false;
        }
    }
    return size;
}

/* Returns readable regions */
vector<s_map> readableRegions(pid_t target)
{
    char mapsfile[32];
    vector<s_map> result;
    string tmp;
    stringstream sstr;

    /* print the path to maps file */
    snprintf(mapsfile, sizeof(mapsfile), "/proc/%d/maps", target);
    ifstream mapsf(mapsfile);

    if (mapsf.bad())
    {
        show_error("%s: can not open file",mapsfile);
        return result;
    }
    while(!mapsf.eof())
    {
        s_map tmpsmap;
        // Get first line
        getline(mapsf,tmp);
        // Erase the '-' so "000000000-000000000" become "000000000 000000000"
        tmp[tmp.find('-')] = ' ';
        // Put it in stringstream:
        sstr.str(tmp);
        sstr >> tmpsmap.start >> tmpsmap.end >> tmpsmap.perms >> tmpsmap.offset;
        if (permissionFlags(tmpsmap.perms)&F_READ_ONLY)
            result.push_back(tmpsmap);
    }
    return result;
}

/* Returns writeable (and readable) regions */
vector<s_map> writeableRegions(pid_t target)
{
    char mapsfile[32];
    vector<s_map> result;
    string tmp;
    stringstream sstr;

    /* print the path to maps file */
    snprintf(mapsfile, sizeof(mapsfile), "/proc/%d/maps", target);
    ifstream mapsf(mapsfile);

    if (mapsf.bad())
    {
        show_error("%s: can not open file",mapsfile);
        return result;
    }
    while(!mapsf.eof())
    {
        s_map tmpsmap;
        // Get first line
        getline(mapsf,tmp);
        // Erase the '-' so "00000000-00000000" become "00000000 00000000"
        tmp[tmp.find('-')] = ' ';
        // Put it in stringstream:
        sstr.str(tmp);
        sstr >> tmpsmap.start >> tmpsmap.end >> tmpsmap.perms >> tmpsmap.offset;
        if (permissionFlags(tmpsmap.perms)&F_WRITE_ONLY)
            result.push_back(tmpsmap);
    }
    return result;
}

FILE * snapshot(pid_t target, bool writeable)
{
    // Attach program for read
    attach(target);
    // Create tmp file
    FILE * result = tmpfile();
    if (!result)
    {
        show_error("Xeat Engine failed to create a temporary file\n");
        return NULL;
    }
    // Create tmp buffer
    void * buffer = malloc(BUFFER_SIZE);
    // Get readable regions or writable regions depending on writeable value:
    vector<s_map> regions = (writeable ? writeableRegions(target) : readableRegions(target));
    if (regions.size() < 1)
    {
        show_error("Xeat Engine failed to get any %s memory region from pid %i\n",(writeable ? "writeable" : "readable"),target);
        return NULL;
    }
    // Loop throught all regions:

    for (vector<s_map>::iterator it = regions.begin(); it < regions.end(); it++)
    {
        // Create a header to write on snapshot file:
        s_snapshot_header tmp;
        // Initialize it:
        tmp.perms = permissionFlags(it->perms);
        tmp.size = (unsigned long)it->end - (unsigned long)it->start;
        tmp.start = it->start;
        // Write it as header:
        fwrite(&tmp,sizeof(tmp),1,result);
        // Start copying the region:
        long reading = BUFFER_SIZE;
        for (unsigned long l = (unsigned long)it->start; l < (unsigned long)it->end;l+=BUFFER_SIZE)
        {
            reading = BUFFER_SIZE;
            if ((l + BUFFER_SIZE) > (unsigned long)it->end)
                reading = (unsigned long)it->end - l;
            readregion(target,buffer,reading,l);
            fwrite(buffer,reading,1,result);
        }
    }
    // Move file pointer to beginning
    fseek(result,0,SEEK_SET);
    // Free pointer
    free(buffer);
    // Detach program
    detach(target);
    return result;
}

// Returns a FILE with a scan. If no ocurrences found,
// the return value is NULL.
FILE * searchValue(pid_t target,FILE * lastScan, bool readonly, any_value& buff, SCAN_TYPE stype, long * ocurrences)
{
    // Create tmp file
    FILE * result = tmpfile();
    if (!result)
    {
        show_error("Xeat Engine failed to create a temporary file\n");
        return NULL;
    }

    FILE * sn;
    if (lastScan == NULL)
        sn = snapshot(target,!readonly);
    else
        sn = lastScan;
    if (!sn)
    {
        show_error("Snapshot failed\n");
    }
    reader r(sn,true,target,true);
    if (stype == UNKOWN)
        return sn;
    // Temporal value for extraction:
    any_value tmp = new any_value(&buff);
    // Structures used for writing into file:
    s_scan_header sc;
    s_value sv;
    // Set all zeros:
    sc.values = 0;
    sc.vsize = 0;
    // Write empty s_scan_header (Will be overwrited later)
    fwrite(&sc,sizeof(s_scan_header),1,result);
    // Checking ALL MEMORY OF THE PROGRAM:
loop:
    while(!r.eof(buff.size()))
    {
        r >> tmp;
        if (stype == EXACT)
            if (buff == tmp)
                goto store;
        if (stype == BIGGER)
            if (buff > tmp)
                goto store;
        if (stype == SMALLER)
            if (buff < tmp)
                goto store;
    }

    if (false)
    {
store:
        // Initiate s_value:
        sv.perms = (readonly ? F_READ_ONLY : F_READ_WRITE);
        sv.start = r.getActualMemoryPointer();
        // Increment total values found:
        sc.values++;
        // Store s_value
        fwrite(&sv,sizeof(s_value),1,result);
        // Store actual value
        fwrite(tmp.getPointer(),tmp.size(),1,result);
        // Store old value:
        r.getOld(tmp);
        fwrite(tmp.getPointer(),tmp.size(),1,result);
        goto loop;
    }
    // Definite some undefined values:
    *ocurrences = sc.values;
    sc.vsize = sizeof(buff.size());
    // Rewrite the scan header:
    fseek(result,0,SEEK_SET);
    fwrite(&sc,sizeof(s_scan_header),1,result);
    // Iterate within all values to write them into files:

    // We asume the oblity of freeing the file:
    if (lastScan == NULL)
        fclose(sn);
    return result;
}
