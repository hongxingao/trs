// =======================================
// Unit   : 基于 TCP 的通讯接口
// Version: 1.0.0.0 (build 2016.07.24)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _TCPUtils_H_
#define _TCPUtils_H_

// 在 windows 下需要加载 lib 文件
#if defined(_WIN64)
#pragma comment(lib, "TCPUtils64.lib")
#elif defined(_WIN32)
#pragma comment(lib, "TCPUtils32.lib")
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Linux 下无 __stdcall/__cdecl 修饰符, 为了兼容 Windows 下修饰符 */

#ifdef __linux

// __stdcall 宏定义
#ifndef __stdcall
#define __stdcall
#endif

// __cdecl 宏定义
#ifndef __cdecl
#define __cdecl
#endif

#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 常量定义 */

// 返回值及错误码
enum TU_TTCPResult
        {TU_trSuccess         =  1,       // 成功
         TU_trFailure         =  0,       // 失败
         TU_trUnknown         = -1,       // 未知错误
         TU_trNotExist        = -2,       // 不存在(如: 连接对象)
         TU_trNotConnect      = -3,       // 连接未打开
         TU_trNonsupport      = -4,       // 不支持
         TU_trVersion         = -5,       // 版本不兼容
         TU_trTimeout         = -6,       // 操作超时
         TU_trPassword        = -7,       // 密码错误
         TU_trIsExisted       = -8,       // 已存在
         TU_trIsRefused       = -9,       // 被拒绝
         TU_trIsIllegal       = -10,      // 不合法
         TU_trIsNullName      = -11,      // 名字为空
         TU_trAttrInvalid     = -12,      // 属性无效
         TU_trStateInvalid    = -13,      // 状态无效
         TU_trHandleInvalid   = -14,      // 句柄无效
         TU_trAccessIllegal   = -15};     // 存取非法

// 接口的属性项
enum TU_TTCPAttrib
        {TU_taVersion         = 0,        // 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
         TU_taConnCount       = 1,        // 连接个数
         TU_taServerCount     = 2,        // 服务端个数
         TU_taSendThreads     = 3,        // 所有连接的发送线程最大个数, 默认值: 8
         TU_taSendCacheSize   = 4,        // 所有连接的发送线程缓冲池尺寸, 默认值: 8
         TU_taRecvThreads     = 5,        // 所有客户端的接收线程最大个数, 默认值: 8
         TU_taRecvCacheSize   = 6};       // 所有客户端的接收线程缓冲池尺寸, 默认值: 8

// 对象的状态
enum TU_TTCPObjState
        {TU_tosInactive       = 0,        // 未打开
         TU_tosClosing        = 1,        // 正在关闭
         TU_tosOpening        = 2,        // 正在打开
         TU_tosOpened         = 3};       // 已打开

// 连接的类型
enum TU_TTCPConnType
        {TU_tctUnknown        = 0,        // (未知)
         TU_tctClient         = 1,        // TCP 客户端连接
         TU_tctSrvClient      = 2};       // TCP 服务端的客户连接

// 服务端的属性项
enum TU_TTCPServerAttrib
        {TU_tsaOwner          = -1,       // 服务端所属对象(可以通过属性接口设置)
         TU_tsaError          = 0,        // 打开服务失败返回的错误信息
         TU_tsaState          = 1,        // 服务端的状态(TU_TTCPObjState)
         TU_tsaLinger         = 2,        // TCP 句柄关闭后的拖延时长(秒), 默认值: 0
         TU_tsaBindAddr       = 3,        // 服务端的绑定IP地址, 默认值: ""
         TU_tsaBindPort       = 4,        // 服务端的侦听端口号, 默认值: 0
         TU_tsaListenQueue    = 5,        // 服务端的侦听连接队列尺寸, 默认值: 256
         TU_tsaRecvThreads    = 6,        // 接收线程最大个数, 默认值: 8
         TU_tsaRecvCacheSize  = 7,        // 接收线程缓冲池尺寸, 默认值: 8
         TU_tsaMaxClientCount = 8,        // 服务端连接的最大客户端个数, 默认值: 0 表示无限制
         TU_tsaSrvClientCount = 9,        // 当前服务已连接的客户端个数
         TU_tsaSendQueue      = 10,       // 连接的发送队列尺寸, 默认值: 64, 其中 0 表示无队列
         TU_tsaKeepTimeout    = 11,       // 连接的心跳检测超时时间(单位: 毫秒), 默认值: 30000
         TU_tsaKeepInterval   = 12,       // 连接的心跳检测重试间隔(单位: 毫秒), 默认值: 10000
         TU_tsaKeepRetryTimes = 13,       // 连接的心跳检测重试次数, 默认值: 3

         // 回调事件属性, 取值范围: [32..63]
         TU_tsaOnDisconnect   = 32,       // 服务的 OnDisconnect     事件, 断开侦听(TU_TTCPOnEvent)
         TU_tsaOnAccept       = 33,       // 服务的 OnAccept         事件, 接受连接(TU_TTCPOnAccept), 可以设置连接属性
         TU_tsaOnFreeClt      = 34,       // 服务的 OnFreeClt        事件, 释放连接(TU_TTCPOnEvent)
         TU_tsaOnCltDisconnect= 35,       // 连接的 OnCltDisconnect  事件, 断开连接(TU_TTCPOnEvent)
         TU_tsaOnCltRecvEvent = 36};      // 连接的 OnCltRecvEvent   事件, 接收事件(TU_TTCPOnEvent)

// 连接的属性项
enum TU_TTCPConnAttrib
        {TU_tcaOwner          = -1,       // 连接所属对象(可以通过属性接口设置)
         TU_tcaError          = 0,        // 打开连接失败返回的错误信息(注: Type == TU_tctClient)
         TU_tcaType           = 1,        // 连接的类型(TU_TTCPConnType)
         TU_tcaState          = 2,        // 连接的状态(TU_TTCPObjState)
         TU_tcaSrvObj         = 3,        // 连接的服务端对象(注: Type == TU_tctSrvClient)
         TU_tcaLinger         = 4,        // 连接的句柄关闭后拖延时长(秒), 默认值: 0
         TU_tcaTimeout        = 5,        // 连接的打开超时(单位: 毫秒), 默认值: 30000
         TU_tcaPeerAddr       = 6,        // 连接的对端地址
         TU_tcaPeerPort       = 7,        // 连接的对端端口号
         TU_tcaCurrAddr       = 8,        // 连接的本地地址
         TU_tcaCurrPort       = 9,        // 连接的本地端口号
         TU_tcaLastTick       = 10,       // 连接的最后一次收发数据时 tick 值
         TU_tcaSendQueue      = 11,       // 连接的发送队列尺寸, 默认值: 64, 其中 0 表示无队列
         TU_tcaKeepTimeout    = 12,       // 连接的心跳检测超时时间(单位: 毫秒), 默认值: 30000
         TU_tcaKeepInterval   = 13,       // 连接的心跳检测重试间隔(单位: 毫秒), 默认值: 10000
         TU_tcaKeepRetryTimes = 14,       // 连接的心跳检测重试次数, 默认值: 3

         // 回调事件属性, 取值范围: [32..63]
         TU_tcaOnDisconnect   = 32,       // 连接的 OnDisconnect  事件, 断开连接(TU_TTCPOnEvent)
         TU_tcaOnRecvEvent    = 33};      // 连接的 OnRecvEvent   事件, 接收事件(TU_TTCPOnEvent)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 接口回调函数类型定义 */

// 对象事件的回调函数类型(如: OnRecvEvent, OnDisconnect, OnFreeClt 等等)
typedef void (__stdcall *TU_TTCPOnEvent)(void* AConnObj, void* AOwner);

// 服务端接受连接事件的回调函数类型(OnAccept)
// 注: 1. 服务端的客户连接对象 AConnObj 由 Server 创建和释放;
//     2. 若事件返回值 > 0 则表示接受连接, 否则拒绝连接;
//     3. 若服务端未设置 OnAccept 事件则无法打开服务端对象.
typedef long (__stdcall *TU_TTCPOnAccept)(void* ASrvObj, void* AConnObj);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef __cplusplus
extern "C"
{
#endif

/* 接口相关函数 */

// 接口初始化/释放函数
void  __stdcall TCPInitialize();
void  __stdcall TCPUninitialize();

// 接口版本信息, 格式: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    返回版本号, 如: 0x0708090A 表示 7.8.9.10
char* __stdcall TCPGetVersion(long* AVersion);

// 取当前操作的最后错误码(注: 用于返回值非错误码的函数)
long  __stdcall TCPGetLastError();

// 取接口属性
// 注: 返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸
long  __stdcall TCPGetInt(long Attrib, long* ARetCode/* = NULL*/);
void* __stdcall TCPGetObj(long Attrib, long* ARetCode/* = NULL*/);
char* __stdcall TCPGetStr(long Attrib, long* ARetCode/* = NULL*/);

// 设置接口属性
long  __stdcall TCPSetInt(long Attrib, long  AValue);
long  __stdcall TCPSetObj(long Attrib, void* AValue);
long  __stdcall TCPSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 写 debug 信息到 KD 的相关函数 */

// 判断 Debug 是否已经打开
long  __stdcall TCPDbgOpened();

// 写 Debug 行
void  __stdcall TCPDbgWrite(const char* AInfo);

// 格式化写 Debug
void  __cdecl   TCPDbgWriteFmt(const char* AFormat, ...);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCP 服务端的相关函数 */

// 创建/释放 TCP 服务端
void* __stdcall TCPServerCreate();
void  __stdcall TCPServerFree(void* ASrvObj);

// 打开/关闭 TCP 服务端
long  __stdcall TCPServerOpen(void* ASrvObj);
long  __stdcall TCPServerClose(void* ASrvObj);

// 取 TCP 服务端属性
// 注: 返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸
long  __stdcall TCPServerGetInt(void* ASrvObj, long Attrib, long* ARetCode/* = NULL*/);
void* __stdcall TCPServerGetObj(void* ASrvObj, long Attrib, long* ARetCode/* = NULL*/);
char* __stdcall TCPServerGetStr(void* ASrvObj, long Attrib, long* ARetCode/* = NULL*/);

// 设置 TCP 服务端属性
long  __stdcall TCPServerSetInt(void* ASrvObj, long Attrib, long  AValue);
long  __stdcall TCPServerSetObj(void* ASrvObj, long Attrib, void* AValue);
long  __stdcall TCPServerSetStr(void* ASrvObj, long Attrib,
                          const char* AValue,  long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 连接的相关函数 */

// 创建/释放 TCP 客户端, 返回连接对象
void* __stdcall TCPClientCreate();
void  __stdcall TCPClientFree(void* AConnObj);

// 连接对象引用计数增/减 1
// 注: 1. 若 TCPConnIncRef 返回TU_trSuccess时, 则必须调用 TCPConnDecRef 减引用;
//     2. TCPConnDecRef 函数不检查 AConnObj 参数是否合法, 即 AConnObj 必须被函
//        数 TCPConnIncRef 成功加引用.
long  __stdcall TCPConnIncRef(void* AConnObj);
void  __stdcall TCPConnDecRef(void* AConnObj);

// 打开/关闭连接
long  __stdcall TCPConnOpen(void* AConnObj);
long  __stdcall TCPConnClose(void* AConnObj);

// 接收数据
// 注: 1. 为了函数性能不检查 AConnObj 参数是否合法, 即 AConnObj 必须存在;
//     2. 返回值为接收成功的数据尺寸, 若小于 0 则为错误码;
//     3. 此函数必须在 OnRecvEvent 事件中调用.
long  __stdcall TCPConnRecv(void* AConnObj, void* AData, long ASize);

// 发送数据
// 注: 1. 为了函数性能不检查 AConnObj 参数是否合法, 即 AConnObj 必须存在;
//     2. 返回值为发送成功的数据尺寸, 若小于 0 则为错误码;
//     3. 若 SendQueue == 0 时, 则表示已加入发送缓冲区尺寸;
//     4. 否则数据优先加入发送缓冲区, 当缓冲区不够时则加入发送队列.
long  __stdcall TCPConnSend(void* AConnObj, const void* AData, long ASize);

// 取连接属性
// 注: 返回值为属性值, ARetCode 为失败的返回码, 允许为空, 当字符串类型成功时为值尺寸
long  __stdcall TCPConnGetInt(void* AConnObj, long Attrib, long* ARetCode/* = NULL*/);
void* __stdcall TCPConnGetObj(void* AConnObj, long Attrib, long* ARetCode/* = NULL*/);
char* __stdcall TCPConnGetStr(void* AConnObj, long Attrib, long* ARetCode/* = NULL*/);

// 设置连接属性
long  __stdcall TCPConnSetInt(void* AConnObj, long Attrib, long  AValue);
long  __stdcall TCPConnSetObj(void* AConnObj, long Attrib, void* AValue);
long  __stdcall TCPConnSetStr(void* AConnObj, long Attrib,
                        const char* AValue,   long ASize);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
