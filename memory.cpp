/*
 $Id: ptrace.c,v 1.22 2007-06-07 15:16:46+01 taviso Exp taviso $

 Copyright (C) 2006,2007,2009 Tavis Ormandy <taviso@sdf.lonestar.org>
 Copyright (C) 2009           Eli Dupree <elidupree@charter.net>
 Copyright (C) 2009,2010      WANG Lu <coolwanglu@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

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
        show_info("%s\n", strerror(errno));
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
    ssize_t len;

    /* print the path to mem file */
    snprintf(mem, sizeof(mem), "/proc/%d/mem", target);

    /* attempt to open the file */
    if ((fd = open(mem, O_RDONLY)) == -1) {
        show_error("unable to open %s.\n", mem);
        return -1;
    }

    /* try to honour the request */
    int tmp;
    while (len < count)
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
            show_info("%s\n", strerror(errno));
            return false;
        }
    }
    return size;
}


