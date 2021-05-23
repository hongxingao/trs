// =======================================
// Unit   : ���� TCP ��ͨѶ�ӿ�
// Version: 1.0.0.0 (build 2016.07.24)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _TCPUtils_H_
#define _TCPUtils_H_

// �� windows ����Ҫ���� lib �ļ�
#if defined(_WIN64)
#pragma comment(lib, "TCPUtils64.lib")
#elif defined(_WIN32)
#pragma comment(lib, "TCPUtils32.lib")
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* Linux ���� __stdcall/__cdecl ���η�, Ϊ�˼��� Windows �����η� */

#ifdef __linux

// __stdcall �궨��
#ifndef __stdcall
#define __stdcall
#endif

// __cdecl �궨��
#ifndef __cdecl
#define __cdecl
#endif

#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �������� */

// ����ֵ��������
enum TU_TTCPResult
        {TU_trSuccess         =  1,       // �ɹ�
         TU_trFailure         =  0,       // ʧ��
         TU_trUnknown         = -1,       // δ֪����
         TU_trNotExist        = -2,       // ������(��: ���Ӷ���)
         TU_trNotConnect      = -3,       // ����δ��
         TU_trNonsupport      = -4,       // ��֧��
         TU_trVersion         = -5,       // �汾������
         TU_trTimeout         = -6,       // ������ʱ
         TU_trPassword        = -7,       // �������
         TU_trIsExisted       = -8,       // �Ѵ���
         TU_trIsRefused       = -9,       // ���ܾ�
         TU_trIsIllegal       = -10,      // ���Ϸ�
         TU_trIsNullName      = -11,      // ����Ϊ��
         TU_trAttrInvalid     = -12,      // ������Ч
         TU_trStateInvalid    = -13,      // ״̬��Ч
         TU_trHandleInvalid   = -14,      // �����Ч
         TU_trAccessIllegal   = -15};     // ��ȡ�Ƿ�

// �ӿڵ�������
enum TU_TTCPAttrib
        {TU_taVersion         = 0,        // �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
         TU_taConnCount       = 1,        // ���Ӹ���
         TU_taServerCount     = 2,        // ����˸���
         TU_taSendThreads     = 3,        // �������ӵķ����߳�������, Ĭ��ֵ: 8
         TU_taSendCacheSize   = 4,        // �������ӵķ����̻߳���سߴ�, Ĭ��ֵ: 8
         TU_taRecvThreads     = 5,        // ���пͻ��˵Ľ����߳�������, Ĭ��ֵ: 8
         TU_taRecvCacheSize   = 6};       // ���пͻ��˵Ľ����̻߳���سߴ�, Ĭ��ֵ: 8

// �����״̬
enum TU_TTCPObjState
        {TU_tosInactive       = 0,        // δ��
         TU_tosClosing        = 1,        // ���ڹر�
         TU_tosOpening        = 2,        // ���ڴ�
         TU_tosOpened         = 3};       // �Ѵ�

// ���ӵ�����
enum TU_TTCPConnType
        {TU_tctUnknown        = 0,        // (δ֪)
         TU_tctClient         = 1,        // TCP �ͻ�������
         TU_tctSrvClient      = 2};       // TCP ����˵Ŀͻ�����

// ����˵�������
enum TU_TTCPServerAttrib
        {TU_tsaOwner          = -1,       // �������������(����ͨ�����Խӿ�����)
         TU_tsaError          = 0,        // �򿪷���ʧ�ܷ��صĴ�����Ϣ
         TU_tsaState          = 1,        // ����˵�״̬(TU_TTCPObjState)
         TU_tsaLinger         = 2,        // TCP ����رպ������ʱ��(��), Ĭ��ֵ: 0
         TU_tsaBindAddr       = 3,        // ����˵İ�IP��ַ, Ĭ��ֵ: ""
         TU_tsaBindPort       = 4,        // ����˵������˿ں�, Ĭ��ֵ: 0
         TU_tsaListenQueue    = 5,        // ����˵��������Ӷ��гߴ�, Ĭ��ֵ: 256
         TU_tsaRecvThreads    = 6,        // �����߳�������, Ĭ��ֵ: 8
         TU_tsaRecvCacheSize  = 7,        // �����̻߳���سߴ�, Ĭ��ֵ: 8
         TU_tsaMaxClientCount = 8,        // ��������ӵ����ͻ��˸���, Ĭ��ֵ: 0 ��ʾ������
         TU_tsaSrvClientCount = 9,        // ��ǰ���������ӵĿͻ��˸���
         TU_tsaSendQueue      = 10,       // ���ӵķ��Ͷ��гߴ�, Ĭ��ֵ: 64, ���� 0 ��ʾ�޶���
         TU_tsaKeepTimeout    = 11,       // ���ӵ�������ⳬʱʱ��(��λ: ����), Ĭ��ֵ: 30000
         TU_tsaKeepInterval   = 12,       // ���ӵ�����������Լ��(��λ: ����), Ĭ��ֵ: 10000
         TU_tsaKeepRetryTimes = 13,       // ���ӵ�����������Դ���, Ĭ��ֵ: 3

         // �ص��¼�����, ȡֵ��Χ: [32..63]
         TU_tsaOnDisconnect   = 32,       // ����� OnDisconnect     �¼�, �Ͽ�����(TU_TTCPOnEvent)
         TU_tsaOnAccept       = 33,       // ����� OnAccept         �¼�, ��������(TU_TTCPOnAccept), ����������������
         TU_tsaOnFreeClt      = 34,       // ����� OnFreeClt        �¼�, �ͷ�����(TU_TTCPOnEvent)
         TU_tsaOnCltDisconnect= 35,       // ���ӵ� OnCltDisconnect  �¼�, �Ͽ�����(TU_TTCPOnEvent)
         TU_tsaOnCltRecvEvent = 36};      // ���ӵ� OnCltRecvEvent   �¼�, �����¼�(TU_TTCPOnEvent)

// ���ӵ�������
enum TU_TTCPConnAttrib
        {TU_tcaOwner          = -1,       // ������������(����ͨ�����Խӿ�����)
         TU_tcaError          = 0,        // ������ʧ�ܷ��صĴ�����Ϣ(ע: Type == TU_tctClient)
         TU_tcaType           = 1,        // ���ӵ�����(TU_TTCPConnType)
         TU_tcaState          = 2,        // ���ӵ�״̬(TU_TTCPObjState)
         TU_tcaSrvObj         = 3,        // ���ӵķ���˶���(ע: Type == TU_tctSrvClient)
         TU_tcaLinger         = 4,        // ���ӵľ���رպ�����ʱ��(��), Ĭ��ֵ: 0
         TU_tcaTimeout        = 5,        // ���ӵĴ򿪳�ʱ(��λ: ����), Ĭ��ֵ: 30000
         TU_tcaPeerAddr       = 6,        // ���ӵĶԶ˵�ַ
         TU_tcaPeerPort       = 7,        // ���ӵĶԶ˶˿ں�
         TU_tcaCurrAddr       = 8,        // ���ӵı��ص�ַ
         TU_tcaCurrPort       = 9,        // ���ӵı��ض˿ں�
         TU_tcaLastTick       = 10,       // ���ӵ����һ���շ�����ʱ tick ֵ
         TU_tcaSendQueue      = 11,       // ���ӵķ��Ͷ��гߴ�, Ĭ��ֵ: 64, ���� 0 ��ʾ�޶���
         TU_tcaKeepTimeout    = 12,       // ���ӵ�������ⳬʱʱ��(��λ: ����), Ĭ��ֵ: 30000
         TU_tcaKeepInterval   = 13,       // ���ӵ�����������Լ��(��λ: ����), Ĭ��ֵ: 10000
         TU_tcaKeepRetryTimes = 14,       // ���ӵ�����������Դ���, Ĭ��ֵ: 3

         // �ص��¼�����, ȡֵ��Χ: [32..63]
         TU_tcaOnDisconnect   = 32,       // ���ӵ� OnDisconnect  �¼�, �Ͽ�����(TU_TTCPOnEvent)
         TU_tcaOnRecvEvent    = 33};      // ���ӵ� OnRecvEvent   �¼�, �����¼�(TU_TTCPOnEvent)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* �ӿڻص��������Ͷ��� */

// �����¼��Ļص���������(��: OnRecvEvent, OnDisconnect, OnFreeClt �ȵ�)
typedef void (__stdcall *TU_TTCPOnEvent)(void* AConnObj, void* AOwner);

// ����˽��������¼��Ļص���������(OnAccept)
// ע: 1. ����˵Ŀͻ����Ӷ��� AConnObj �� Server �������ͷ�;
//     2. ���¼�����ֵ > 0 ���ʾ��������, ����ܾ�����;
//     3. �������δ���� OnAccept �¼����޷��򿪷���˶���.
typedef long (__stdcall *TU_TTCPOnAccept)(void* ASrvObj, void* AConnObj);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef __cplusplus
extern "C"
{
#endif

/* �ӿ���غ��� */

// �ӿڳ�ʼ��/�ͷź���
void  __stdcall TCPInitialize();
void  __stdcall TCPUninitialize();

// �ӿڰ汾��Ϣ, ��ʽ: "x.x.x.x (build yyyy.mm.dd)"
// AVersion    ���ذ汾��, ��: 0x0708090A ��ʾ 7.8.9.10
char* __stdcall TCPGetVersion(long* AVersion);

// ȡ��ǰ��������������(ע: ���ڷ���ֵ�Ǵ�����ĺ���)
long  __stdcall TCPGetLastError();

// ȡ�ӿ�����
// ע: ����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�
long  __stdcall TCPGetInt(long Attrib, long* ARetCode/* = NULL*/);
void* __stdcall TCPGetObj(long Attrib, long* ARetCode/* = NULL*/);
char* __stdcall TCPGetStr(long Attrib, long* ARetCode/* = NULL*/);

// ���ýӿ�����
long  __stdcall TCPSetInt(long Attrib, long  AValue);
long  __stdcall TCPSetObj(long Attrib, void* AValue);
long  __stdcall TCPSetStr(long Attrib, const char* AValue, long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* д debug ��Ϣ�� KD ����غ��� */

// �ж� Debug �Ƿ��Ѿ���
long  __stdcall TCPDbgOpened();

// д Debug ��
void  __stdcall TCPDbgWrite(const char* AInfo);

// ��ʽ��д Debug
void  __cdecl   TCPDbgWriteFmt(const char* AFormat, ...);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* TCP ����˵���غ��� */

// ����/�ͷ� TCP �����
void* __stdcall TCPServerCreate();
void  __stdcall TCPServerFree(void* ASrvObj);

// ��/�ر� TCP �����
long  __stdcall TCPServerOpen(void* ASrvObj);
long  __stdcall TCPServerClose(void* ASrvObj);

// ȡ TCP ���������
// ע: ����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�
long  __stdcall TCPServerGetInt(void* ASrvObj, long Attrib, long* ARetCode/* = NULL*/);
void* __stdcall TCPServerGetObj(void* ASrvObj, long Attrib, long* ARetCode/* = NULL*/);
char* __stdcall TCPServerGetStr(void* ASrvObj, long Attrib, long* ARetCode/* = NULL*/);

// ���� TCP ���������
long  __stdcall TCPServerSetInt(void* ASrvObj, long Attrib, long  AValue);
long  __stdcall TCPServerSetObj(void* ASrvObj, long Attrib, void* AValue);
long  __stdcall TCPServerSetStr(void* ASrvObj, long Attrib,
                          const char* AValue,  long ASize);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* ���ӵ���غ��� */

// ����/�ͷ� TCP �ͻ���, �������Ӷ���
void* __stdcall TCPClientCreate();
void  __stdcall TCPClientFree(void* AConnObj);

// ���Ӷ������ü�����/�� 1
// ע: 1. �� TCPConnIncRef ����TU_trSuccessʱ, �������� TCPConnDecRef ������;
//     2. TCPConnDecRef ��������� AConnObj �����Ƿ�Ϸ�, �� AConnObj ���뱻��
//        �� TCPConnIncRef �ɹ�������.
long  __stdcall TCPConnIncRef(void* AConnObj);
void  __stdcall TCPConnDecRef(void* AConnObj);

// ��/�ر�����
long  __stdcall TCPConnOpen(void* AConnObj);
long  __stdcall TCPConnClose(void* AConnObj);

// ��������
// ע: 1. Ϊ�˺������ܲ���� AConnObj �����Ƿ�Ϸ�, �� AConnObj �������;
//     2. ����ֵΪ���ճɹ������ݳߴ�, ��С�� 0 ��Ϊ������;
//     3. �˺��������� OnRecvEvent �¼��е���.
long  __stdcall TCPConnRecv(void* AConnObj, void* AData, long ASize);

// ��������
// ע: 1. Ϊ�˺������ܲ���� AConnObj �����Ƿ�Ϸ�, �� AConnObj �������;
//     2. ����ֵΪ���ͳɹ������ݳߴ�, ��С�� 0 ��Ϊ������;
//     3. �� SendQueue == 0 ʱ, ���ʾ�Ѽ��뷢�ͻ������ߴ�;
//     4. �����������ȼ��뷢�ͻ�����, ������������ʱ����뷢�Ͷ���.
long  __stdcall TCPConnSend(void* AConnObj, const void* AData, long ASize);

// ȡ��������
// ע: ����ֵΪ����ֵ, ARetCode Ϊʧ�ܵķ�����, ����Ϊ��, ���ַ������ͳɹ�ʱΪֵ�ߴ�
long  __stdcall TCPConnGetInt(void* AConnObj, long Attrib, long* ARetCode/* = NULL*/);
void* __stdcall TCPConnGetObj(void* AConnObj, long Attrib, long* ARetCode/* = NULL*/);
char* __stdcall TCPConnGetStr(void* AConnObj, long Attrib, long* ARetCode/* = NULL*/);

// ������������
long  __stdcall TCPConnSetInt(void* AConnObj, long Attrib, long  AValue);
long  __stdcall TCPConnSetObj(void* AConnObj, long Attrib, void* AValue);
long  __stdcall TCPConnSetStr(void* AConnObj, long Attrib,
                        const char* AValue,   long ASize);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
