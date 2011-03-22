#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <GL/gl.h>
#include <GL/glu.h>


typedef void (*gl)(GLenum mode);

static gl glBegin_orig;

extern "C" void glBegin( GLenum mode )
{
    exit(-2);
    /*glBegin_orig = (gl)dlsym(RTLD_NEXT,"glBegin");
    if (mode == GL_POLYGON)
    {
        glBegin_orig(mode);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(1.0);
        glColor3f(0.0f, 0.7f, 0.0f);
    }
    else
    {
        glBegin_orig(mode);
        glColor3f(0.0f, 0.7f, 0.0f);
    }*/
}
