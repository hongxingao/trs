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

/* TTCPConnObj - TCP ���Ӷ����� */

// ---------------- ��̬���� ----------------
// ���ӵ� OnDisconnect �ص��¼�
void __stdcall TTCPConnObj::_TCPOnDisconnect(void* AConnID, TTCPConnObj* AConnObj)
{
   AConnObj->DoDisconnect();
}

// ���ӵ� OnRecvData �ص��¼�
void __stdcall TTCPConnObj::_TCPOnRecvEvent(void* AConnID, TTCPConnObj* AConnObj)
{
   AConnObj->DoRecvData();
}

// ---------------- ���캯������������ ----------------
// ���캯��
TTCPConnObj::TTCPConnObj(void* AConnID)
{
   // ��ʼ��
   FData                = NULL;
   FServer              = NULL;
   FConnID              = NULL;
   FConnType            = TU_tctUnknown;
   FNeedFree            = false;

   // ��ʼ���¼�
   FOnRecvData.Method   = NULL;
   FOnRecvData.Object   = NULL;
   FOnDisconnect.Method = NULL;
   FOnDisconnect.Object = NULL;

   // ���ö������
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

// ��������
TTCPConnObj::~TTCPConnObj()
{
   // �ÿ�
   void* pConn = FConnID;
   FConnID     = NULL;

   // �ж��Ƿ�ǿ�
   if (pConn != NULL)
   {
      // �ر�����
      TCPConnClose(pConn);

      // �������
      if (TCPConnGetObj(pConn, TU_tcaOwner, NULL) == this)
         TCPConnSetObj(pConn, TU_tcaOwner, NULL);

      // �ͷ�����
      if (FNeedFree)
         TCPClientFree(pConn);
      else
         TCPConnDecRef(pConn);
   }
}

// ---------------- ˽�к��� ----------------
// ���� OnDisconnect �¼�
void TTCPConnObj::DoDisconnect()
{
   if (FOnDisconnect.Method != NULL)
      try
      {
         ((TObject*)FOnDisconnect.Object->*FOnDisconnect.Method)(this);
      }
      catch (...) {}
}

// ���� OnRecvData �¼�
void TTCPConnObj::DoRecvData()
{
   // ��������
   char arrData[8192];
   long intSize = TCPConnRecv(FConnID, arrData, sizeof(arrData));

   // �����¼�
   if ((intSize > 0) && (FOnRecvData.Method != NULL))
      try
      {
         ((TObject*)FOnRecvData.Object->*FOnRecvData.Method)(this, arrData, intSize);
      }
      catch (...) {}
}

// ---------------- ���к��� ----------------
// ���� OnRecvData �¼�
long TTCPConnObj::SetOnRecvData(TDoRecvData AMethod, void* AObject)
{
   // ��ʼ��
   long result = TU_trUnknown;

   // ������
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

   // ���ؽ��
   return result;
}

// ���� OnDisconnect �¼�
long TTCPConnObj::SetOnDisconnect(TDoConnEvent AMethod, void* AObject)
{
   // ��ʼ��
   long result = TU_trUnknown;

   // ������
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

   // ���ؽ��
   return result;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TTCPServer - TCP �������� */

// ---------------- ��̬��Ա ----------------

// ---------------- ��̬���� ----------------
// ����� OnDisconnect �ص��¼�
void __stdcall TTCPServer::_SrvOnDisconnect(void* ASrvID, TTCPServer* AServer)
{
   AServer->DoDisconnect();
}

// ����� OnAccept �ص��¼�
long __stdcall TTCPServer::_SrvOnAccept(void* ASrvID, void* AConnID)
{
   // ��ʼ��
   long result       = -1;
   bool boolRefused  = true;

   // ȡ�������
   TTCPServer* objSrv = (TTCPServer*)TCPServerGetObj(ASrvID, TU_tsaOwner, NULL);
   if (objSrv != NULL)
   {
      TTCPConnObj* objConn  = new TTCPConnObj(AConnID);
      if (objConn != NULL)
      {
         // ���� OnAccept �¼�
         try
         {
            objSrv->DoAccept(objConn, boolRefused);
            result = 0;
         }
         catch (...) {}

         // �ж��Ƿ�ܾ�
         if (boolRefused)
            delete objConn;
         else
            result = 1;
      }
   }

   // ���ؽ��
   return result;
}

// ����� OnFreeClt �ص��¼�
void __stdcall TTCPServer::_SrvOnFreeClt(void* AConnID, TTCPConnObj* AConnObj)
{
   // ���� OnFreeClt �¼�
   try
   {
      TTCPServer* objSrv = AConnObj->Server();
      if (objSrv != NULL)
         objSrv->DoFreeClt(AConnObj);
   }
   catch (...) {}

   // �ͷŶ���
   delete AConnObj;
}

// ---------------- ���캯������������ ----------------
// ���캯��
TTCPServer::TTCPServer()
{
   // ��ʼ��
   FData                   = NULL;
   FOnDisconnect.Method    = NULL;
   FOnDisconnect.Object    = NULL;
   FOnAccept.Method        = NULL;
   FOnAccept.Object        = NULL;
   FOnFreeClt.Method       = NULL;
   FOnFreeClt.Object       = NULL;
   FOnCltRecvData.Method   = NULL;
   FOnCltRecvData.Object   = NULL;

   // ���� TCP ����˶���, �����ù���
   FSrvID   = TCPServerCreate();
   if (FSrvID != NULL)
      TCPServerSetObj(FSrvID, TU_tsaOwner, this);
}

// ��������
TTCPServer::~TTCPServer()
{
   // �ж��Ƿ�ǿ�
   if (FSrvID != NULL)
   {
      // �ر�
      TCPServerClose(FSrvID);

      // �������
      TCPServerSetObj(FSrvID, TU_tsaOwner, NULL);

      // �ͷŶ���
      TCPServerFree(FSrvID);
   }
}

// ---------------- ˽�к��� ----------------

// ---------------- �������� ----------------
// ���� OnDisconnect �¼�
void TTCPServer::DoDisconnect()
{
   if (FOnDisconnect.Method != NULL)
      try
      {
         ((TObject*)FOnDisconnect.Object->*FOnDisconnect.Method)(this);
      }
      catch (...) {}
}

// ���� OnAccept �¼�
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

// ���� OnFreeClt �¼�
void TTCPServer::DoFreeClt(TTCPConnObj* AConnObj)
{
   if (FOnFreeClt.Method != NULL)
      try
      {
         ((TObject*)FOnFreeClt.Object->*FOnFreeClt.Method)(AConnObj);
      }
      catch (...) {}
}

// ---------------- ���к��� ----------------
// ���� OnDisconnect �¼�
long TTCPServer::SetOnDisconnect(TDoSrvEvent AMethod, void* AObject)
{
   // ��ʼ��
   long result = TU_trUnknown;

   // ������
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

   // ���ؽ��
   return result;
}

// ���� OnAccept �¼�
long TTCPServer::SetOnAccept(TDoAccept AMethod, void* AObject)
{
   // ��ʼ��
   long result = TU_trUnknown;

   // ������
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

   // ���ؽ��
   return result;
}

// ���� OnFreeClt �¼�
long TTCPServer::SetOnFreeClt(TDoConnEvent AMethod, void* AObject)
{
   // ��ʼ��
   long result = TU_trUnknown;

   // ������
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

   // ���ؽ��
   return result;
}

// ���� OnCltRecvData �¼�
long TTCPServer::SetOnCltRecvData(TDoRecvData AMethod, void* AObject)
{
   // ��ʼ��
   long result = TU_trUnknown;

   // ������
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

   // ���ؽ��
   return result;
}

/* end namespace */

}
