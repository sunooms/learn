#ifndef __HY_DYNAMIC_DMP_PROTOCOL_H
#define __HY_DYNAMIC_DMP_PROTOCOL_H

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#include <inttypes.h>               // uint64_t打印可移植处理
#endif

#include <stdio.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <sstream>

#define MAX_BOBY_SIZE    65535
#define HY_DYNAMIC_DMP_VERSION 1
#define COMMOND_QUERY    10
#define COMMOND_RESPONSE 20

using namespace std;


typedef struct _HyDynamicDmpHeader{
    uint16_t          wVersion;              //接口协议版本号，当前为1，unsigned short  必填
    uint16_t          wCommand;             //命令号，当前为10，unsigned short  必填
    uint16_t          cResult;              //调用结果，返回包填写,0表示调用成功，1为包检验失败
    uint16_t          wLength;              //整个包长度，unsigned short， 必填

    _HyDynamicDmpHeader()
    {
        memset(this, 0, sizeof(_HyDynamicDmpHeader));
        wVersion = HY_DYNAMIC_DMP_VERSION;
        wCommand = COMMOND_QUERY;
    }

    static void host2net(struct _HyDynamicDmpHeader* header)
    {
        header->wVersion = (uint16_t)(htons(header->wVersion));
        header->wCommand = (uint16_t)(htons(header->wCommand));
        header->cResult = (uint16_t)(htons(header->cResult));
        header->wLength = (uint16_t)(htons(header->wLength));
    }

    static void net2host(struct _HyDynamicDmpHeader* header)
    {
        header->wVersion = (uint16_t)(ntohs(header->wVersion));
        header->wCommand = (uint16_t)(ntohs(header->wCommand));
        header->cResult = (uint16_t)(ntohs(header->cResult));
        header->wLength = (uint16_t)(ntohs(header->wLength));
    }
}HyDynamicDmpHeader;

typedef struct _HyDynamicDmpReqPackage
{
    HyDynamicDmpHeader stHead;
    char szBody[];
}HyDynamicDmpReqPackage;

typedef struct _HyDynamicDmpRspPackage
{
    HyDynamicDmpHeader stHead;
    char szBody[];
}HyDynamicDmpRspPackage;



#endif
