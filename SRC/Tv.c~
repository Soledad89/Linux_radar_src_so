#include "Tv.h"
#include <stdio.h>
#include <dlfcn.h>

void *tv_lib_handle = NULL;

int  OpenTVDLL(char *filename)
{
    if(!filename) return -1;

    tv_lib_handle = dlopen(filename, RTLD_LAZY);//./lib_so/
    if (!tv_lib_handle)
    {
       fprintf(stderr, "%s\n", dlerror());
      return -2;
    }  
 return 1;
}
void CloseTVDLL()
{

 if(tv_lib_handle)
	{
                dlclose(tv_lib_handle);
                tv_lib_handle = NULL;
	}
}

int InitTVFunctionPointer()
{
    int i=0;
    char *error;
    InitTV = dlsym(tv_lib_handle, "ML_GVID_InitTV");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return --i;
    }
   OpenTV = dlsym(tv_lib_handle, "ML_GVID_OpenTV");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return i;
    }   
   CloseTV = dlsym(tv_lib_handle, "ML_GVID_CloseTV");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return  --i;
    }
   SetTVXY = dlsym(tv_lib_handle, "ML_GVID_SetTVXY");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return  --i;
    }
   SetTVWH = dlsym(tv_lib_handle, "ML_GVID_SetTVWH");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return --i;
    }
   SetTVFrost = dlsym(tv_lib_handle, "ML_GVID_SetTVFrost");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return --i;
    }
   SetTVBright = dlsym(tv_lib_handle, "ML_GVID_SetTVBright");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return --i;
    }
   SetTVContrast = dlsym(tv_lib_handle, "ML_GVID_SetTVContrast");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return --i;
    }
   SetTVColor = dlsym(tv_lib_handle, "ML_GVID_SetTVColor");
    if ((error = dlerror()) != NULL)
    {
       fprintf(stderr, "%s\n", error);
       return --i;
    }
    return 1;
}

