#ifndef COMMON_GLOBAL_DEFINE_H_
#define COMMON_GLOBAL_DEFINE_H_

#define DSP_MOD_CONFIG_PATH  "/usr/local/pmp/conf";

enum RequestType
{
    REQUEST_TYPE_BID         = 0,
    REQUEST_TYPE_WINNOTICE   = 1,
    REQUEST_TYPE_CLICK       = 2,
};

enum RESULT
{
    RESULT_ERROR             = -1,//
    RESULT_SUCCESS           = 0 ,//success
    RESULT_WARNING           = 1 ,//success, is part success
};

#define  RESULT_ISFAILED(rel) (rel<=RESULT_ERROR)
#define  RESULT_ISSUCCESSED(rel) (rel>=RESULT_SUCCESS)

#if defined(_DEBUG)
#define DebugLog ColorPrintf
#else
#define DebugLog(...)
#endif

#endif // COMMON_GLOBAL_DEFINE_H_

