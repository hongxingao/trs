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

/* TCP ��ص��¼����� */

class TTCPConnObj;
class TTCPServer;

// TObject - ����
class TObject
{
};

// �����¼�
typedef void (TObject::*TDoConnEvent)(TTCPConnObj* AConnObj);
typedef struct
{
   TDoConnEvent   Method;
   void*          Object;
} TOnConnEvent;

// ���������¼�
typedef void (TObject::*TDoRecvData)(TTCPConnObj* AConnObj, void* AData, long ASize);
typedef struct
{
   TDoRecvData    Method;
   void*          Object;
} TOnRecvData;

// �������¼�
typedef void (TObject::*TDoSrvEvent)(TTCPServer* AServer);
typedef struct
{
   TDoSrvEvent    Method;
   void*          Object;
} TOnSrvEvent;

// ���������������¼�
typedef void (TObject::*TDoAccept)(TTCPServer* AServer, TTCPConnObj* AConnObj,
                                         bool& AIsRefused);
typedef struct
{
   TDoAccept      Method;
   void*          Object;
} TOnAccept;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPConnObj - TCP ���Ӷ����� */

class TTCPConnObj
{
public:
   TTCPConnObj(void* AConnID = NULL);
   virtual ~TTCPConnObj();

   // ����
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

   // ��������
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

   // ��/�ر�����
   long           Open()               { return TCPConnOpen(FConnID); }
   long           Close()              { return TCPConnClose(FConnID); }

   // ��������
   // ע: 1. ����ֵΪ���ͳɹ������ݳߴ�, ��С�� 0 ��Ϊ������;
   //     2. �� SendQueue == 0 ʱ, ���ʾ�Ѽ��뷢�ͻ������ߴ�;
   //     3. �����������ȼ��뷢�ͻ�����, ������������ʱ����뷢�Ͷ���.
   long           Send(const void* AData, long ASize)
                  { return TCPConnSend(FConnID, AData, ASize); }

   // �����¼�
   long           SetOnRecvData(TDoRecvData AMethod, void* AObject);
   long           SetOnDisconnect(TDoConnEvent AMethod, void* AObject);

protected:
   // ִ���¼�����
   void           DoRecvData();
   void           DoDisconnect();

private:
   void*          FData;               // �Զ�������
   TTCPServer*    FServer;             // ���������� TTCPServer ����
   void*          FConnID;             // TCP ���� ID
   long           FConnType;           // ��������
   bool           FNeedFree;           // �Ƿ���Ҫ�ͷ�

   // �¼�
   TOnRecvData    FOnRecvData;         // ��������
   TOnConnEvent   FOnDisconnect;       // �Ͽ�����

private:
   // �����¼��Ļص�����
   static void __stdcall _TCPOnDisconnect(void* AConnID, TTCPConnObj* AConnObj);
   static void __stdcall _TCPOnRecvEvent(void* AConnID, TTCPConnObj* AConnObj);

private:
   friend class TTCPServer;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPServer - TCP �������� */

class TTCPServer
{
public:
   TTCPServer();
   virtual ~TTCPServer();

   // ����
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

   // ��������
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

   // ��/�ر� TCP �����
   long           Open()               { return TCPServerOpen(FSrvID); }
   long           Close()              { return TCPServerClose(FSrvID); }

   // �����¼�
   long           SetOnDisconnect(TDoSrvEvent AMethod, void* AObject);
   long           SetOnAccept(TDoAccept AMethod, void* AObject);
   long           SetOnFreeClt(TDoConnEvent AMethod, void* AObject);
   long           SetOnCltRecvData(TDoRecvData AMethod, void* AObject);

protected:
   // ִ���¼�����
   void           DoDisconnect();
   void           DoAccept(TTCPConnObj* AConnObj, bool& AIsRefused);
   void           DoFreeClt(TTCPConnObj* AConnObj);

private:
   void*          FData;               // �Զ�������
   void*          FSrvID;              // ������ ID

   // �¼�
   TOnSrvEvent    FOnDisconnect;       // �Ͽ�����
   TOnAccept      FOnAccept;           // ��������
   TOnConnEvent   FOnFreeClt;          // �ͷ�����
   TOnRecvData    FOnCltRecvData;      // ��������

private:
   // �������¼��Ļص�����
   static void __stdcall _SrvOnDisconnect(void* ASrvID, TTCPServer* AServer);
   static long __stdcall _SrvOnAccept(void* ASrvID, void* AConnID);
   static void __stdcall _SrvOnFreeClt(void* AConnID, TTCPConnObj* AConnObj);
};

/* end namespace */

}

#endif
