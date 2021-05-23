// =======================================
// Unit   : TCPUtils objects
// Version: 1.0.0.0 (build 2016.09.02)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#include "TCPObjs.h"

namespace TCPUtils
{

/* begin namespace */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPConnObj - TCP 连接对象类 */

// ---------------- 静态函数 ----------------
// 连接的 OnDisconnect 回调事件
void __stdcall TTCPConnObj::_TCPOnDisconnect(void* AConnID, TTCPConnObj* AConnObj)
{
   AConnObj->DoDisconnect();
}

// 连接的 OnRecvData 回调事件
void __stdcall TTCPConnObj::_TCPOnRecvEvent(void* AConnID, TTCPConnObj* AConnObj)
{
   AConnObj->DoRecvData();
}

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TTCPConnObj::TTCPConnObj(void* AConnID)
{
   // 初始化
   FData                = NULL;
   FServer              = NULL;
   FConnID              = NULL;
   FConnType            = TU_tctUnknown;
   FNeedFree            = false;

   // 初始化事件
   FOnRecvData.Method   = NULL;
   FOnRecvData.Object   = NULL;
   FOnDisconnect.Method = NULL;
   FOnDisconnect.Object = NULL;

   // 设置对象关联
   if (AConnID == NULL)
   {
      FConnID     = TCPClientCreate();
      if (FConnID != NULL)
      {
         FNeedFree   = true;
         FConnType   = TU_tctClient;
         TCPConnSetObj(FConnID, TU_tcaOwner, this);
      }
   }
   else if (TCPConnIncRef(AConnID) == TU_trSuccess)
   {
      FConnID     = AConnID;
      FConnType   = TCPConnGetInt(AConnID, TU_tcaType, NULL);
      TCPConnSetObj(FConnID, TU_tcaOwner, this);
   }
}

// 析构函数
TTCPConnObj::~TTCPConnObj()
{
   // 置空
   void* pConn = FConnID;
   FConnID     = NULL;

   // 判断是否非空
   if (pConn != NULL)
   {
      // 关闭连接
      TCPConnClose(pConn);

      // 清空数据
      if (TCPConnGetObj(pConn, TU_tcaOwner, NULL) == this)
         TCPConnSetObj(pConn, TU_tcaOwner, NULL);

      // 释放连接
      if (FNeedFree)
         TCPClientFree(pConn);
      else
         TCPConnDecRef(pConn);
   }
}

// ---------------- 私有函数 ----------------
// 激发 OnDisconnect 事件
void TTCPConnObj::DoDisconnect()
{
   if (FOnDisconnect.Method != NULL)
      try
      {
         ((TObject*)FOnDisconnect.Object->*FOnDisconnect.Method)(this);
      }
      catch (...) {}
}

// 激发 OnRecvData 事件
void TTCPConnObj::DoRecvData()
{
   // 接收数据
   char arrData[8192];
   long intSize = TCPConnRecv(FConnID, arrData, sizeof(arrData));

   // 激发事件
   if ((intSize > 0) && (FOnRecvData.Method != NULL))
      try
      {
         ((TObject*)FOnRecvData.Object->*FOnRecvData.Method)(this, arrData, intSize);
      }
      catch (...) {}
}

// ---------------- 公有函数 ----------------
// 设置 OnRecvData 事件
long TTCPConnObj::SetOnRecvData(TDoRecvData AMethod, void* AObject)
{
   // 初始化
   long result = TU_trUnknown;

   // 检查参数
   if (FConnID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &TTCPConnObj::_TCPOnRecvEvent : NULL;
      result = TCPConnSetObj(FConnID, TU_tcaOnRecvEvent, pFunc);
      if (result != TU_trSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnRecvData.Object = AObject;
         FOnRecvData.Method = AMethod;
      }
      else
      {
         FOnRecvData.Method = NULL;
         FOnRecvData.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnDisconnect 事件
long TTCPConnObj::SetOnDisconnect(TDoConnEvent AMethod, void* AObject)
{
   // 初始化
   long result = TU_trUnknown;

   // 检查参数
   if (FConnID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &TTCPConnObj::_TCPOnDisconnect : NULL;
      result = TCPConnSetObj(FConnID, TU_tcaOnDisconnect, pFunc);
      if (result != TU_trSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnDisconnect.Object = AObject;
         FOnDisconnect.Method = AMethod;
      }
      else
      {
         FOnDisconnect.Method = NULL;
         FOnDisconnect.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPServer - TCP 服务器类 */

// ---------------- 静态成员 ----------------

// ---------------- 静态函数 ----------------
// 服务的 OnDisconnect 回调事件
void __stdcall TTCPServer::_SrvOnDisconnect(void* ASrvID, TTCPServer* AServer)
{
   AServer->DoDisconnect();
}

// 服务的 OnAccept 回调事件
long __stdcall TTCPServer::_SrvOnAccept(void* ASrvID, void* AConnID)
{
   // 初始化
   long result       = -1;
   bool boolRefused  = true;

   // 取服务对象
   TTCPServer* objSrv = (TTCPServer*)TCPServerGetObj(ASrvID, TU_tsaOwner, NULL);
   if (objSrv != NULL)
   {
      TTCPConnObj* objConn  = new TTCPConnObj(AConnID);
      if (objConn != NULL)
      {
         // 激发 OnAccept 事件
         try
         {
            objSrv->DoAccept(objConn, boolRefused);
            result = 0;
         }
         catch (...) {}

         // 判断是否拒绝
         if (boolRefused)
            delete objConn;
         else
            result = 1;
      }
   }

   // 返回结果
   return result;
}

// 服务的 OnFreeClt 回调事件
void __stdcall TTCPServer::_SrvOnFreeClt(void* AConnID, TTCPConnObj* AConnObj)
{
   // 激发 OnFreeClt 事件
   try
   {
      TTCPServer* objSrv = AConnObj->Server();
      if (objSrv != NULL)
         objSrv->DoFreeClt(AConnObj);
   }
   catch (...) {}

   // 释放对象
   delete AConnObj;
}

// ---------------- 构造函数和析构函数 ----------------
// 构造函数
TTCPServer::TTCPServer()
{
   // 初始化
   FData                   = NULL;
   FOnDisconnect.Method    = NULL;
   FOnDisconnect.Object    = NULL;
   FOnAccept.Method        = NULL;
   FOnAccept.Object        = NULL;
   FOnFreeClt.Method       = NULL;
   FOnFreeClt.Object       = NULL;
   FOnCltRecvData.Method   = NULL;
   FOnCltRecvData.Object   = NULL;

   // 创建 TCP 服务端对象, 并设置关联
   FSrvID   = TCPServerCreate();
   if (FSrvID != NULL)
      TCPServerSetObj(FSrvID, TU_tsaOwner, this);
}

// 析构函数
TTCPServer::~TTCPServer()
{
   // 判断是否非空
   if (FSrvID != NULL)
   {
      // 关闭
      TCPServerClose(FSrvID);

      // 清除关联
      TCPServerSetObj(FSrvID, TU_tsaOwner, NULL);

      // 释放对象
      TCPServerFree(FSrvID);
   }
}

// ---------------- 私有函数 ----------------

// ---------------- 保护函数 ----------------
// 激发 OnDisconnect 事件
void TTCPServer::DoDisconnect()
{
   if (FOnDisconnect.Method != NULL)
      try
      {
         ((TObject*)FOnDisconnect.Object->*FOnDisconnect.Method)(this);
      }
      catch (...) {}
}

// 激发 OnAccept 事件
void TTCPServer::DoAccept(TTCPConnObj* AConnObj, bool& AIsRefused)
{
   AIsRefused  = true;
   if (FOnAccept.Method != NULL)
      try
      {
         AConnObj->FServer       = this;
         AConnObj->FOnRecvData   = FOnCltRecvData;
         ((TObject*)FOnAccept.Object->*FOnAccept.Method)(this, AConnObj, AIsRefused);
      }
      catch (...) {}
}

// 激发 OnFreeClt 事件
void TTCPServer::DoFreeClt(TTCPConnObj* AConnObj)
{
   if (FOnFreeClt.Method != NULL)
      try
      {
         ((TObject*)FOnFreeClt.Object->*FOnFreeClt.Method)(AConnObj);
      }
      catch (...) {}
}

// ---------------- 公有函数 ----------------
// 设置 OnDisconnect 事件
long TTCPServer::SetOnDisconnect(TDoSrvEvent AMethod, void* AObject)
{
   // 初始化
   long result = TU_trUnknown;

   // 检查参数
   if (FSrvID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &TTCPServer::_SrvOnDisconnect : NULL;
      result = TCPServerSetObj(FSrvID, TU_tsaOnDisconnect, pFunc);
      if (result != TU_trSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnDisconnect.Object = AObject;
         FOnDisconnect.Method = AMethod;
      }
      else
      {
         FOnDisconnect.Method = NULL;
         FOnDisconnect.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnAccept 事件
long TTCPServer::SetOnAccept(TDoAccept AMethod, void* AObject)
{
   // 初始化
   long result = TU_trUnknown;

   // 检查参数
   if (FSrvID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &TTCPServer::_SrvOnAccept : NULL;
      result = TCPServerSetObj(FSrvID, TU_tsaOnAccept, pFunc);
      if (result != TU_trSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnAccept.Object  = AObject;
         FOnAccept.Method  = AMethod;
      }
      else
      {
         FOnAccept.Method  = NULL;
         FOnAccept.Object  = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnFreeClt 事件
long TTCPServer::SetOnFreeClt(TDoConnEvent AMethod, void* AObject)
{
   // 初始化
   long result = TU_trUnknown;

   // 检查参数
   if (FSrvID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &TTCPServer::_SrvOnFreeClt : NULL;
      result = TCPServerSetObj(FSrvID, TU_tsaOnFreeClt, pFunc);
      if (result != TU_trSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnFreeClt.Object = AObject;
         FOnFreeClt.Method = AMethod;
      }
      else
      {
         FOnFreeClt.Method = NULL;
         FOnFreeClt.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

// 设置 OnCltRecvData 事件
long TTCPServer::SetOnCltRecvData(TDoRecvData AMethod, void* AObject)
{
   // 初始化
   long result = TU_trUnknown;

   // 检查参数
   if (FSrvID != NULL)
   {
      void* pFunc = (AMethod != NULL) ? &TTCPConnObj::_TCPOnRecvEvent : NULL;
      result = TCPServerSetObj(FSrvID, TU_tsaOnCltRecvEvent, pFunc);
      if (result != TU_trSuccess)
         ;
      else if (AMethod != NULL)
      {
         FOnCltRecvData.Object = AObject;
         FOnCltRecvData.Method = AMethod;
      }
      else
      {
         FOnCltRecvData.Method = NULL;
         FOnCltRecvData.Object = NULL;
      }
   }

   // 返回结果
   return result;
}

/* end namespace */

}
