// =======================================
// Unit   : TCPUtils objects
// Version: 1.0.0.0 (build 2016.09.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _TCPObjs_H_
#define _TCPObjs_H_

#include <string.h>
#include "TCPUtils.h"

namespace TCPUtils
{

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCP 相关的事件定义 */

class TTCPConnObj;
class TTCPServer;

// TObject - 空类
class TObject
{
};

// 连接事件
typedef void (TObject::*TDoConnEvent)(TTCPConnObj* AConnObj);
typedef struct
{
   TDoConnEvent   Method;
   void*          Object;
} TOnConnEvent;

// 接收数据事件
typedef void (TObject::*TDoRecvData)(TTCPConnObj* AConnObj, void* AData, long ASize);
typedef struct
{
   TDoRecvData    Method;
   void*          Object;
} TOnRecvData;

// 服务器事件
typedef void (TObject::*TDoSrvEvent)(TTCPServer* AServer);
typedef struct
{
   TDoSrvEvent    Method;
   void*          Object;
} TOnSrvEvent;

// 服务器接受连接事件
typedef void (TObject::*TDoAccept)(TTCPServer* AServer, TTCPConnObj* AConnObj,
                                         bool& AIsRefused);
typedef struct
{
   TDoAccept      Method;
   void*          Object;
} TOnAccept;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPConnObj - TCP 连接对象类 */

class TTCPConnObj
{
public:
   TTCPConnObj(void* AConnID = NULL);
   virtual ~TTCPConnObj();

   // 属性
   void*          Data() const         { return FData; }
   TTCPServer*    Server() const       { return FServer; }
   void*          ConnID() const       { return FConnID; }
   long           ConnType() const     { return FConnType; }// = TCPConnGetInt(FConnID, TU_tcaType, NULL);

   char*          Error() const        { return TCPConnGetStr(FConnID, TU_tcaError, NULL); }
   long           State() const        { return TCPConnGetInt(FConnID, TU_tcaState, NULL); }
   long           Timeout() const      { return TCPConnGetInt(FConnID, TU_tcaTimeout, NULL); }
   long           SendQueue() const    { return TCPConnGetInt(FConnID, TU_tcaSendQueue, NULL); }

   char*          CurrAddr() const     { return TCPConnGetStr(FConnID, TU_tcaCurrAddr, NULL); }
   long           CurrPort() const     { return TCPConnGetInt(FConnID, TU_tcaCurrPort, NULL); }
   char*          PeerAddr() const     { return TCPConnGetStr(FConnID, TU_tcaPeerAddr, NULL); }
   long           PeerPort() const     { return TCPConnGetInt(FConnID, TU_tcaPeerPort, NULL); }

   long           LastTick() const     { return TCPConnGetInt(FConnID, TU_tcaLastTick, NULL); }
   long           KeepTimeout() const  { return TCPConnGetInt(FConnID, TU_tcaKeepTimeout, NULL); }
   long           KeepInterval() const { return TCPConnGetInt(FConnID, TU_tcaKeepInterval, NULL); }
   long           KeepRetryTimes()const{ return TCPConnGetInt(FConnID, TU_tcaKeepRetryTimes, NULL); }

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   long           SetTimeout(long ATimeout)
                  { return TCPConnSetInt(FConnID, TU_tcaTimeout, ATimeout); }
   long           SetSendQueue(long AQueueSize)
                  { return TCPConnSetInt(FConnID, TU_tcaSendQueue, AQueueSize); }

   long           SetCurrAddr(const char* Addr)
                  { return TCPConnSetStr(FConnID, TU_tcaCurrAddr, Addr, (Addr != NULL) ? strlen(Addr) : 0); }
   long           SetCurrAddr(const char* Addr, long ALength)
                  { return TCPConnSetStr(FConnID, TU_tcaCurrAddr, Addr, ALength); }
   long           SetCurrPort(long APort)
                  { return TCPConnSetInt(FConnID, TU_tcaCurrPort, APort); }

   long           SetPeerAddr(const char* Addr)
                  { return TCPConnSetStr(FConnID, TU_tcaPeerAddr, Addr, (Addr != NULL) ? strlen(Addr) : 0); }
   long           SetPeerAddr(const char* Addr, long ALength)
                  { return TCPConnSetStr(FConnID, TU_tcaPeerAddr, Addr, ALength); }
   long           SetPeerPort(long APort)
                  { return TCPConnSetInt(FConnID, TU_tcaPeerPort, APort); }

   long           SetKeepTimeout(long ATimeout)
                  { return TCPConnSetInt(FConnID, TU_tcaKeepTimeout, ATimeout); }
   long           SetKeepInterval(long AInterval)
                  { return TCPConnSetInt(FConnID, TU_tcaKeepInterval, AInterval); }
   long           SetKeepRetryTimes(long ARetryTimes)
                  { return TCPConnSetInt(FConnID, TU_tcaKeepRetryTimes, ARetryTimes); }

   // 打开/关闭连接
   long           Open()               { return TCPConnOpen(FConnID); }
   long           Close()              { return TCPConnClose(FConnID); }

   // 发送数据
   // 注: 1. 返回值为发送成功的数据尺寸, 若小于 0 则为错误码;
   //     2. 若 SendQueue == 0 时, 则表示已加入发送缓冲区尺寸;
   //     3. 否则数据优先加入发送缓冲区, 当缓冲区不够时则加入发送队列.
   long           Send(const void* AData, long ASize)
                  { return TCPConnSend(FConnID, AData, ASize); }

   // 设置事件
   long           SetOnRecvData(TDoRecvData AMethod, void* AObject);
   long           SetOnDisconnect(TDoConnEvent AMethod, void* AObject);

protected:
   // 执行事件方法
   void           DoRecvData();
   void           DoDisconnect();

private:
   void*          FData;               // 自定义数据
   TTCPServer*    FServer;             // 连接所属的 TTCPServer 对象
   void*          FConnID;             // TCP 连接 ID
   long           FConnType;           // 连接类型
   bool           FNeedFree;           // 是否需要释放

   // 事件
   TOnRecvData    FOnRecvData;         // 接收数据
   TOnConnEvent   FOnDisconnect;       // 断开连接

private:
   // 连接事件的回调函数
   static void __stdcall _TCPOnDisconnect(void* AConnID, TTCPConnObj* AConnObj);
   static void __stdcall _TCPOnRecvEvent(void* AConnID, TTCPConnObj* AConnObj);

private:
   friend class TTCPServer;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPServer - TCP 服务器类 */

class TTCPServer
{
public:
   TTCPServer();
   virtual ~TTCPServer();

   // 属性
   void*          Data() const         { return FData; }
   void*          SrvID() const        { return FSrvID; }
   long           State() const        { return TCPServerGetInt(FSrvID, TU_tsaState, NULL); }
   char*          Error() const        { return TCPServerGetStr(FSrvID, TU_tsaError, NULL); }

   char*          Addr() const         { return TCPServerGetStr(FSrvID, TU_tsaBindAddr, NULL); }
   long           Port() const         { return TCPServerGetInt(FSrvID, TU_tsaBindPort, NULL); }
   long           Linger() const       { return TCPServerGetInt(FSrvID, TU_tsaLinger, NULL); }
   long           SendQueue() const    { return TCPServerGetInt(FSrvID, TU_tsaSendQueue, NULL); }
   long           ListenQueue() const  { return TCPServerGetInt(FSrvID, TU_tsaListenQueue, NULL); }
   long           RecvThreads() const  { return TCPServerGetInt(FSrvID, TU_tsaRecvThreads, NULL); }
   long           CacheThreads() const { return TCPServerGetInt(FSrvID, TU_tsaRecvCacheSize, NULL); }

   long           MaxClients() const   { return TCPServerGetInt(FSrvID, TU_tsaMaxClientCount, NULL); }
   long           ClientCount() const  { return TCPServerGetInt(FSrvID, TU_tsaSrvClientCount, NULL); }

   long           KeepTimeout() const  { return TCPServerGetInt(FSrvID, TU_tsaKeepTimeout, NULL); }
   long           KeepInterval() const { return TCPServerGetInt(FSrvID, TU_tsaKeepInterval, NULL); }
   long           KeepRetryTimes()const{ return TCPServerGetInt(FSrvID, TU_tsaKeepRetryTimes, NULL); }

   // 设置属性
   void           SetData(void* AData) { FData = AData; }
   long           SetAddr(const char* Addr)
                  { return TCPServerSetStr(FSrvID, TU_tsaBindAddr, Addr, (Addr != NULL) ? strlen(Addr) : 0); }
   long           SetAddr(const char* Addr, long ALength)
                  { return TCPServerSetStr(FSrvID, TU_tsaBindAddr, Addr, ALength); }
   long           SetPort(long APort)
                  { return TCPServerSetInt(FSrvID, TU_tsaBindPort, APort); }
   long           SetLinger(long ALinger)
                  { return TCPServerSetInt(FSrvID, TU_tsaLinger, ALinger); }
   long           SetSendQueue(long AQueueSize)
                  { return TCPServerSetInt(FSrvID, TU_tsaSendQueue, AQueueSize); }
   long           SetListenQueue(long AListenQueue)
                  { return TCPServerSetInt(FSrvID, TU_tsaListenQueue, AListenQueue); }
   long           SetRecvThreads(long ACount)
                  { return TCPServerSetInt(FSrvID, TU_tsaRecvThreads, ACount); }
   long           SetCacheThreads(long ACount)
                  { return TCPServerSetInt(FSrvID, TU_tsaRecvCacheSize, ACount); }

   long           SetMaxClients(long AMaxCount)
                  { return TCPServerSetInt(FSrvID, TU_tsaMaxClientCount, AMaxCount); }

   long           SetKeepTimeout(long ATimeout)
                  { return TCPServerSetInt(FSrvID, TU_tsaKeepTimeout, ATimeout); }
   long           SetKeepInterval(long AInterval)
                  { return TCPServerSetInt(FSrvID, TU_tsaKeepInterval, AInterval); }
   long           SetKeepRetryTimes(long ARetryTimes)
                  { return TCPServerSetInt(FSrvID, TU_tsaKeepRetryTimes, ARetryTimes); }

   // 打开/关闭 TCP 服务端
   long           Open()               { return TCPServerOpen(FSrvID); }
   long           Close()              { return TCPServerClose(FSrvID); }

   // 设置事件
   long           SetOnDisconnect(TDoSrvEvent AMethod, void* AObject);
   long           SetOnAccept(TDoAccept AMethod, void* AObject);
   long           SetOnFreeClt(TDoConnEvent AMethod, void* AObject);
   long           SetOnCltRecvData(TDoRecvData AMethod, void* AObject);

protected:
   // 执行事件方法
   void           DoDisconnect();
   void           DoAccept(TTCPConnObj* AConnObj, bool& AIsRefused);
   void           DoFreeClt(TTCPConnObj* AConnObj);

private:
   void*          FData;               // 自定义数据
   void*          FSrvID;              // 服务器 ID

   // 事件
   TOnSrvEvent    FOnDisconnect;       // 断开侦听
   TOnAccept      FOnAccept;           // 接受连接
   TOnConnEvent   FOnFreeClt;          // 释放连接
   TOnRecvData    FOnCltRecvData;      // 接收数据

private:
   // 服务器事件的回调函数
   static void __stdcall _SrvOnDisconnect(void* ASrvID, TTCPServer* AServer);
   static long __stdcall _SrvOnAccept(void* ASrvID, void* AConnID);
   static void __stdcall _SrvOnFreeClt(void* AConnID, TTCPConnObj* AConnObj);
};

/* end namespace */

}

#endif
