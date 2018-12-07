//---------------------------------------------------------------------------

#pragma hdrstop

#include "ObjectU.h"
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
// TSocketComm
//---------------------------------------------------------------------------
__fastcall TSocketComm::TSocketComm()
{
}
//---------------------------------------------------------------------------
int __fastcall TSocketComm::fnGetToNodeID()
{
	return m_iToProcID / 10000;
}
//---------------------------------------------------------------------------
int __fastcall TSocketComm::fnGetToTaskID()
{
	return (m_iToProcID / 100) % 100;
}
//---------------------------------------------------------------------------
int __fastcall TSocketComm::fnGetToSeqNo()
{
	return m_iToProcID % 100;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// TSocketCommList
//---------------------------------------------------------------------------
__fastcall TSocketCommList::TSocketCommList()
	: TBaseList<TSocketComm>()
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TEventInfo::TEventInfo()
{
	m_byEventCode	= 0x00;
	m_iEmrgency     = 0;
	m_dtEmrCollDate = StrToTime(L"00:00:00");
	m_dtEmrUpdtDate = StrToTime(L"00:00:00");
	m_sEventId		= "";
	m_iEmrRelease   = 0;
	m_iEmrSkip      = 0;
}
//---------------------------------------------------------------------------
__fastcall TEventInfo::~TEventInfo()
{

}
//---------------------------------------------------------------------------
__fastcall TEventList::TEventList()
{

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TSensorInfo::TSensorInfo()
{
	m_iIndex	   = 0;
	m_iUseFlag     = 0;
	m_iSensorID    = 0;
	m_sSiteId	   = "";
	m_sBuildingId  = "";
	m_sSensorName  = "";
	m_sRemoteIP    = "";
	m_iRemotePort  = 0;
	m_iWaitTime    = 0;
	m_iRetryCnt    = 0;
	m_iActiveCnt   = 0;

	m_iHumidity      = 0;
	m_iHumidityMin   = 0;
	m_byHumidityFlag = 0;
	m_iTemper	     = 0;
	m_iTemperMax     = 0;
	m_byTemperFlag   = 0;
	m_iSmoke	     = 0;
	m_iSmokeMax	     = 0;
	m_bySmokeFlag    = 0;
	m_iCPUStatus     = 0;
	m_dtRecvTime     = Now() + StrToDateTime("00:00:05");



	m_bHumidityTest  = 0;;
	m_bTemperTest    = 0;
	m_bSmokeTest     = 0;

	m_iSendS	     = 0;
	m_iRecvS         = 0;

	m_pEventList   = new TEventList();

	BYTE byTemp = 0x20;
	TEventInfo *pEventInfo;
	for(int i=0; i<10; i++){
        pEventInfo = new TEventInfo();
		pEventInfo->EventCode = byTemp+i;
		m_pEventList->Add(pEventInfo);
	}

}
//---------------------------------------------------------------------------
__fastcall TSensorInfo::~TSensorInfo()
{

}
//---------------------------------------------------------------------------
BYTE __fastcall TSensorInfo::fnGetActivecnt()
{
	m_iActiveCnt++;
	if(m_iActiveCnt == 0) m_iActiveCnt = 0;
	return m_iActiveCnt;
}
//---------------------------------------------------------------------------
TEventInfo* __fastcall TSensorInfo::fnGetEventInfoOfCode(BYTE a_byCode)
{
	TEventInfo *pEventInfo;
	for(int i=0; i<m_pEventList->Count; i++){
		pEventInfo = m_pEventList->Items[i];
		if(pEventInfo->EventCode == a_byCode){
			return pEventInfo;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TSensorInfo::fnGetSmokeStr()
{
	UnicodeString sTemp;
	sTemp.sprintf(L"%.01f",((float)m_iSmoke/3000)*100);

	return sTemp;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TSensorInfo::fnGetTemperStr()
{
	UnicodeString sTemp;
	sTemp.sprintf(L"%.01f", m_iTemper*0.1);

	return sTemp;
}
//---------------------------------------------------------------------------
void __fastcall TSensorInfo::fnSetHumidity(int a_iValue)
{
	m_iHumidity = a_iValue;

	if(m_iHumidity <= m_iHumidityMin){
     	m_byHumidityFlag = true;
	}else{
        m_byHumidityFlag = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TSensorInfo::fnSetTemper(int a_iValue)
{
	m_iTemper = a_iValue;
	if(StrToFloat(TemperStr) >= m_iTemperMax){
		m_byTemperFlag = true;
	}else{
		m_byTemperFlag = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSensorInfo::fnSetSmoke(int a_iValue)
{
	m_iSmoke = a_iValue;
	if(StrToFloat(SmokeStr) >= m_iSmokeMax){
		m_bySmokeFlag = true;
	}else{
		m_bySmokeFlag = false;
	}
}
//---------------------------------------------------------------------------
__fastcall TSensorList::TSensorList()
{

}
//---------------------------------------------------------------------------
__fastcall TSensorList::~TSensorList()
{

}
//---------------------------------------------------------------------------
UnicodeString __fastcall TSensorList::fnGetSensorName(int a_iSensorID)
{
	for(int i=0; i<Count; i++){
		if(Items[i]->SensorID == a_iSensorID){
			return Items[i]->SensorName;
		}
	}
	return "";
}
//---------------------------------------------------------------------------
int __fastcall TSensorList::fnGetSensorError()
{
	int iResult = 0;
	for(int i=0; i<Count; i++){
		if(Items[i]->CPUStatus != 1){
			iResult++;
		}
	}
	return iResult;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TApplicationInfo::TApplicationInfo()
{
	m_pSensorList  = new TSensorList();
	m_pSockComList = new TSocketCommList();

	m_iIcpPort     = 0;
	AnsiString sPath;
	sPath 			= ExtractFilePath(Application->ExeName) + "CompSensorSvr.ini";
	TIniFile* pIni  = new TIniFile(sPath);
	m_iProcID	    = pIni->ReadInteger("APP", "ProcessID"    ,  0);
	m_iCommLogLevel = pIni->ReadInteger("APP", "CommLogLevel" ,  0);
	m_iPosX         = pIni->ReadInteger("APP", "PosX"         ,  0);
	m_iPosY         = pIni->ReadInteger("APP", "PosY"         ,  0);
	m_iPosW         = pIni->ReadInteger("APP", "PosW"         ,  0);
	m_iPosH         = pIni->ReadInteger("APP", "PosH"         ,  0);

	m_iEventTime    = pIni->ReadInteger("APP", "EventTime"    ,  0);
	m_iReleEventTime= pIni->ReadInteger("APP", "ReleEventTime",  0);

	m_iMaxDBSession = pIni->ReadInteger("DATABASE", "MaxSession",  1);
	m_sDBServerName = pIni->ReadString ("DATABASE", "ServerName", "");
	m_sDBUserName   = pIni->ReadString ("DATABASE", "UserName"  , "");
	m_sDBPassword   = pIni->ReadString ("DATABASE", "Password"  , "");
	m_sVersion      = fnVersionInfo("FileVersion");

	m_sConnectStr.printf(L"Provider=MSDASQL.1;Password=%s;Persist Security Info=True;User ID=%s;Data Source=%s",
						  m_sDBPassword, m_sDBUserName, m_sDBServerName);

	m_iCommPort     = pIni->ReadInteger("COMM", "Port"   , 0);
	m_iCommTimeout  = pIni->ReadInteger("COMM", "Timeout", 0);
	m_iCommRetryCnt = pIni->ReadInteger("COMM", "Retry"  , 0);

	if(m_iMaxDBSession > MAX_DB_SESSION) m_iMaxDBSession = MAX_DB_SESSION;
	m_dwAcpThrd = 0;
	m_dwIcpThrd = 0;
	m_dwDBMThrd = 0;


	ZeroMemory(m_dwDBPThrd, sizeof(m_dwDBPThrd));
	ZeroMemory(m_aDBConnect, sizeof(m_aDBConnect));
	ZeroMemory(m_aDBStatus , sizeof(m_aDBStatus ));

	delete pIni;
}
//---------------------------------------------------------------------------
__fastcall TApplicationInfo::~TApplicationInfo()
{
	delete m_pSensorList;
	delete m_pSockComList;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TApplicationInfo::fnVersionInfo(const UnicodeString &sQuery)
{
	DWORD dwHandle = 0, dwVersionInfoSize;
	UINT uLength;
	LPVOID pFileInfo, ptr;
	UnicodeString sOut;

	UnicodeString filename = Application->ExeName;

	dwVersionInfoSize = GetFileVersionInfoSize(filename.c_str(), &dwHandle);

	pFileInfo = (LPVOID) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwVersionInfoSize);

	GetFileVersionInfo(filename.c_str(), dwHandle, dwVersionInfoSize, pFileInfo);


	VerQueryValue(pFileInfo, TEXT("\\VarFileInfo\\Translation"), &ptr, &uLength);

	WORD *id = (WORD *) ptr;
	UnicodeString szString = "\\StringFileInfo\\" + IntToHex(id[0], 4) + IntToHex(id[1], 4) + "\\" + sQuery;

	VerQueryValue(pFileInfo, szString.c_str(), &ptr, &uLength);
	sOut = UnicodeString((wchar_t *) ptr);
	HeapFree(GetProcessHeap(), 0, pFileInfo );

	return sOut;
}
//---------------------------------------------------------------------------
//void __fastcall TApplicationInfo::fnLoadSensorInfo()
//{
//	TStringList  *pList, *pDataList;
//	TSensorInfo *pSensorInfo;
//	UnicodeString sFileName;
//	UnicodeString sProcID = "";
//	try{
//		try{
//
//			sFileName     = ExtractFilePath(Application->ExeName) + L"\\Config\\SensorInfo.csv";
//			pList         = new TStringList();
//			pDataList     = new TStringList();
//
//			if(FileExists(sFileName)){
//				pList->LoadFromFile(sFileName);
//				for(int i=0; i<pList->Count; i++){
//					pDataList->Clear();
//					if(pList->Strings[i] == "") continue;
//
//					pDataList->CommaText = pList->Strings[i];
//					if(pDataList->Strings[0].ToIntDef(0) == 0) continue;
//					pSensorInfo = new TSensorInfo();
//					pSensorInfo->UseFlag    = pDataList->Strings[0].ToIntDef(0);
//					pSensorInfo->SensorID   = pDataList->Strings[1].ToIntDef(0);
//					pSensorInfo->SensorName = pDataList->Strings[2];
//					pSensorInfo->RemoteIP   = pDataList->Strings[3];
//					pSensorInfo->RemotePort = pDataList->Strings[4].ToIntDef(0);
//					m_pSensorList->Add(pSensorInfo);
//				}
//			}
//		}catch(Exception &e){
//
//		}
//	}__finally{
//		delete pList;
//		delete pDataList;
//	}
//
//
//
//}
//---------------------------------------------------------------------------
__int64 __fastcall TApplicationInfo::fnGetSensorID(UnicodeString a_sIP, int a_sPort)
{
	TSensorInfo *pSensorInfo;
	for(int i=0; i<m_pSensorList->Count; i++){
		pSensorInfo = m_pSensorList->Items[i];
		if(pSensorInfo->RemoteIP == a_sIP && pSensorInfo->RemotePort == a_sPort){
			return pSensorInfo->SensorID;
        }
	}
	return 0;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TApplicationInfo::fnGetBuildingID(UnicodeString a_sIP, int a_sPort)
{
	TSensorInfo *pSensorInfo;
	for(int i=0; i<m_pSensorList->Count; i++){
		pSensorInfo = m_pSensorList->Items[i];
		if(pSensorInfo->RemoteIP == a_sIP && pSensorInfo->RemotePort == a_sPort){
			return pSensorInfo->BuildingId;
		}
	}
	return "";
}
//---------------------------------------------------------------------------
TSensorInfo* __fastcall TApplicationInfo::fnGetSensorInfo(__int64 a_iSensorID)
{
	TSensorInfo *pSensorInfo;
	for(int i=0; i<m_pSensorList->Count; i++){
		pSensorInfo = m_pSensorList->Items[i];
		if(pSensorInfo->SensorID == a_iSensorID){
			return pSensorInfo;
        }
	}
	return NULL;
}
//---------------------------------------------------------------------------
DWORD __fastcall TApplicationInfo::fnGetDBPM(int a_iIndex)
{
	if ( a_iIndex < 0 || a_iIndex >= m_iMaxDBSession ) return 0;
	return m_dwDBPThrd[a_iIndex];
}
//---------------------------------------------------------------------------
void __fastcall TApplicationInfo::fnSetDBPM(int a_iIndex, DWORD a_dwValue)
{
	if ( a_iIndex < 0 || a_iIndex >= m_iMaxDBSession ) return;
	m_dwDBPThrd[a_iIndex] = a_dwValue;
}
//---------------------------------------------------------------------------
bool __fastcall TApplicationInfo::fnGetDBConnect(int a_iIndex)
{
	if ( a_iIndex < 0 || a_iIndex >= m_iMaxDBSession ) return false;
	return m_aDBConnect[a_iIndex];
}
//---------------------------------------------------------------------------
void __fastcall TApplicationInfo::fnSetDBConnect(int a_iIndex, bool a_bValue)
{
	if ( a_iIndex < 0 || a_iIndex >= m_iMaxDBSession ) return;
	m_aDBConnect[a_iIndex] = a_bValue;
}
//---------------------------------------------------------------------------
int __fastcall TApplicationInfo::fnGetDBStatus(int a_iIndex)
{
	if ( a_iIndex < 0 || a_iIndex >= m_iMaxDBSession ) return -1;
	return m_aDBStatus[a_iIndex];
}
//---------------------------------------------------------------------------
void __fastcall TApplicationInfo::fnSetDBStatus(int a_iIndex, int a_iValue)
{
	if ( a_iIndex < 0 || a_iIndex >= m_iMaxDBSession ) return;
	m_aDBStatus[a_iIndex] = a_iValue;
}
//---------------------------------------------------------------------------
int __fastcall TApplicationInfo::fnGetDBConnCount(void)
{
	int iCount = 0;

	for ( int i = 0; i < m_iMaxDBSession; i++ )
	{
		if(m_aDBConnect[i]) iCount++;
	}
	return iCount;
}
//---------------------------------------------------------------------------
 bool __fastcall TApplicationInfo::fnGetAppStart()
 {
	 if(m_iIcpPort&& m_pSensorList->Count /*&& m_pSockComList->Count*/){
        return true;
	 }
	 return false;
 }
//---------------------------------------------------------------------------
void __fastcall TApplicationInfo::fnWriteStream(AnsiString sId, BYTE *a_pBuf, DWORD a_dwSize, int a_iFlag)
{
	HANDLE     hFile;
	DWORD      dwWriteSize;
	char       aBuf[4];
	UnicodeString sFName;
	AnsiString sMsg;

	if(CommLogLevel > 0){
		sFName  = ExtractFilePath(Application->ExeName) + "log\\" + Now().FormatString("yyyy-mm-dd");
//        sFName  = GetCurrentDir() + "\\log\\" + Now().FormatString("yyyy-mm-dd");
		sFName += "\\" + sId + ".log";
		if((hFile = CreateFile(sFName.w_str(),
							   GENERIC_READ|GENERIC_WRITE,
							   FILE_SHARE_READ,
							   NULL,
							   OPEN_ALWAYS,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL)) == INVALID_HANDLE_VALUE) return;
		SetFilePointer(hFile, 0, NULL, FILE_END);
		sMsg = "[" + Now().FormatString("hh:nn:ss") + "] ";
		if     (a_iFlag == 1) sMsg += " Send";
		else if(a_iFlag == 2) sMsg += " Recv";
		sMsg += IntToStr((int)a_dwSize) + " bytes\r\n";
		WriteFile(hFile, sMsg.c_str(), sMsg.Length(), &dwWriteSize, NULL);
		sMsg = "";
		for(DWORD i=0; i<a_dwSize; i++){
			if(i > 0 && i%30 == 0){
				sMsg += "\r\n";
				WriteFile(hFile, sMsg.c_str(), sMsg.Length(), &dwWriteSize, NULL);
				sMsg = "";
			}
			sprintf(aBuf, "%02X ", a_pBuf[i]&0xFF);
			sMsg += AnsiString(aBuf);
		}
		if(sMsg.Length() > 0){
			sMsg += "\r\n\r\n";
			WriteFile(hFile, sMsg.c_str(), sMsg.Length(), &dwWriteSize, NULL);
		}
		CloseHandle(hFile);
	}
}
//---------------------------------------------------------------------------
void __fastcall TApplicationInfo::fnWriteDebug(AnsiString a_sMsg, int a_iFlag)
{
    HANDLE     hFile;
    DWORD      dwWriteSize;
	UnicodeString sFName;

	if(CommLogLevel > 0){
		sFName  = ExtractFilePath(Application->ExeName) + "log\\" + Now().FormatString("yyyy-mm-dd") + "\\Debug_";
//        sFName = GetCurrentDir() + "\\log\\" + Now().FormatString("yyyy-mm-dd") + "\\Debug_";
		if(a_iFlag != -1) sFName += IntToStr(a_iFlag);
		sFName += ".log";
		if((hFile = CreateFile(sFName.w_str(),
							   GENERIC_READ|GENERIC_WRITE,
							   FILE_SHARE_READ,
							   NULL,
							   OPEN_ALWAYS,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL)) == INVALID_HANDLE_VALUE) return;
		SetFilePointer(hFile, 0, NULL, FILE_END);
		a_sMsg = "[" + Now().FormatString("hh:nn:ss") + "] " + a_sMsg + "\r\n";
		WriteFile(hFile, a_sMsg.c_str(), a_sMsg.Length(), &dwWriteSize, NULL);
		CloseHandle(hFile);
	}
}
//---------------------------------------------------------------------------
void __fastcall TApplicationInfo::fnWriteDebug(AnsiString a_sMsg, AnsiString a_sSenssor)
{
    HANDLE     hFile;
    DWORD      dwWriteSize;
	UnicodeString sFName;

	if(CommLogLevel > 0){
		sFName  = ExtractFilePath(Application->ExeName) + "log\\" + Now().FormatString("yyyy-mm-dd") + "\\Debug_";
//        sFName = GetCurrentDir() + "\\log\\" + Now().FormatString("yyyy-mm-dd") + "\\Debug_";
		 sFName += a_sSenssor;
		sFName += ".log";
		if((hFile = CreateFile(sFName.w_str(),
							   GENERIC_READ|GENERIC_WRITE,
							   FILE_SHARE_READ,
							   NULL,
							   OPEN_ALWAYS,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL)) == INVALID_HANDLE_VALUE) return;
		SetFilePointer(hFile, 0, NULL, FILE_END);
		a_sMsg = "[" + Now().FormatString("hh:nn:ss") + "] " + a_sMsg + "\r\n";
		WriteFile(hFile, a_sMsg.c_str(), a_sMsg.Length(), &dwWriteSize, NULL);
		CloseHandle(hFile);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TPacketInfo::TPacketInfo(void)
{
	ZeroMemory(m_aBuffer, sizeof(m_aBuffer));
	m_sBuildingId = "";
	m_sRemoteIP   = "";
	m_iRemotePort = 0;
	m_iSensorID	  = 0;
	m_iSize       = 0;
}
//---------------------------------------------------------------------------
__fastcall TPacketInfo::~TPacketInfo(void)
{
}
//---------------------------------------------------------------------------
BYTE* __fastcall TPacketInfo::fnGetBuffer(void)
{
	return (BYTE*)m_aBuffer;
}
//---------------------------------------------------------------------------
void __fastcall TPacketInfo::fnSetBuffer(BYTE *a_pBuf, int a_iSize)
{
	if(a_iSize > (int)sizeof(m_aBuffer)) return;
	CopyMemory(m_aBuffer, a_pBuf, a_iSize);
	m_iSize = a_iSize;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
__fastcall  TdcReqQuery::TdcReqQuery()
{
	m_bResponse   = false;
	m_dwJobID     = 0;
	m_sSQL		  = "";
	m_sSensorId	  = "";
	m_byCode	  = 0;
	m_hResHndle   = NULL;
	m_dwResThread = 0;
	m_iSessionNo  = -1;
}
//---------------------------------------------------------------------------
__fastcall TdcReqQuery::~TdcReqQuery(void)
{

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TdcReqQueryList::TdcReqQueryList(void)
{

}
//---------------------------------------------------------------------------
__fastcall TdcReqQueryList::~TdcReqQueryList(void)
{

}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall  TdcResQuery::TdcResQuery()
{
	m_dwJobID     = 0;
	m_sSQL		  = "";
	m_sSensorId	  = "";
	m_byCode	  = 0;
	m_hResHndle   = NULL;
	m_dwResThread = 0;

    m_pQuery      = new TADOQuery(NULL);
}
//---------------------------------------------------------------------------
__fastcall TdcResQuery::~TdcResQuery(void)
{
	delete m_pQuery;
}//---------------------------------------------------------------------------

