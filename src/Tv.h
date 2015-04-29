
#ifndef P264_TV_DLL_H
#define P264_TV_DLL_H


#ifdef __cplusplus
extern "C" {
#endif

int  OpenTVDLL(char *filename);
void CloseTVDLL();
int InitTVFunctionPointer();

extern void *tv_lib_handle;
typedef int status;

status   (*InitTV)();
status   (*OpenTV)(char VideoChannel, short theX, short theY, short theWidth, short theHeight);
status   (*CloseTV)(char VideoChannel);
status   (*SetTVXY)(char VideoChannel, short theX, short theY);
status   (* SetTVWH)(char VideoChannel, short theWidth, short theHeight);
status   (*SetTVFrost)(char VideoChannel, char theForst);
status   (*SetTVBright)(char VideoChannel, unsigned char theBright);
status   (*SetTVContrast)(char VideoChannel, unsigned char theContrast);
status   (*SetTVColor)(char VideoChannel, unsigned char theColor);


#ifdef __cplusplus
}
#endif

#endif //P264_TV_DLL_H

