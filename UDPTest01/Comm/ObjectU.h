//---------------------------------------------------------------------------

#ifndef ObjectUH
#define ObjectUH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <ADODB.hpp>
#include "stdio.h"
//---------------------------------------------------------------------------
#define WM_TERMINATED           (WM_USER + 0x01)
#define WM_TIME_SET  			(WM_USER + 0x02)
#define WM_PROC_KILL  			(WM_USER + 0x03)

#define WM_ICP_OPEN             (WM_USER + 0x11)
#define WM_ICP_IOSTATUS         (WM_USER + 0x12)
#define WM_ICP_SEND         	(WM_USER + 0x13)


#define WM_ACP_OPEN             (WM_USER + 0x21)
#define WM_ACP_IOSTATUS         (WM_USER + 0x22)
#define WM_ACP_SEND         	(WM_USER + 0x23)

#define WM_DB_CURR_JOB          (WM_USER + 0x31)
#define WM_DB_PROC              (WM_USER + 0x32)
#define WM_DB_IDLE              (WM_USER + 0x33)
#define WM_DB_CONNECT           (WM_USER + 0x34)
#define WM_DB_DISCONNECT        (WM_USER + 0x35)
#define WM_DB_CHECK             (WM_USER + 0x36)

#define WM_TRAY_NOTIFYCATION    (WM_USER + 0x99)
//---------------------------------------------------------------------------
#define IDC_MAIN_TRAY 1006
#define MAX_DB_SESSION 10
//---------------------------------------------------------------------------
enum TdcDBJobType     {djtSelect, djtInsert, djtUpdate, djtDelete, djtCancel, djtClear};
enum TdcDBQueryType   {dqtExecute, dqtOpen};
//---------------------------------------------------------------------------
template <class T> class TBaseList : public TList
{
private:
protected:
	T* __fastcall   GetItems(int Index);
public:
	__fastcall  	TBaseList();
	__fastcall 	   ~TBaseList();
public:
	int  __fastcall Add(T *Item);
	void __fastcall Delete(int Index, bool Flag = true);
	void __fastcall Clear();
	int  __fastcall IndexOf(T *Item);
public:
	__property T* 	Items[int iIndex] = { read = GetItems };
};
//---------------------------------------------------------------------------
template <class T> __fastcall TBaseList<T>::TBaseList()
	: TList()
{
}
//---------------------------------------------------------------------------
template <class T> __fastcall TBaseList<T>::~TBaseList()
{
	Clear();
}
//---------------------------------------------------------------------------
template <class T> int  __fastcall TBaseList<T>::Add(T *Item)
{
	return TList::Add( (void*)Item );
}
//---------------------------------------------------------------------------
template <class T> void __fastcall TBaseList<T>::Delete(int Index, bool Flag)
{
	T *Item;

	if(Index < 0 || Index >= Count) return;
	if(Flag){
		Item = (T*)TList::Items[Index];
		delete Item;
	}
	TList::Delete(Index);
}
//---------------------------------------------------------------------------
template <class T> void __fastcall TBaseList<T>::Clear()
{
	while(Count) Delete(0);
	TList::Clear();
}
//---------------------------------------------------------------------------
template <class T> int  __fastcall TBaseList<T>::IndexOf(T *Item)
{
	return TList::IndexOf((void*)Item);
}
//---------------------------------------------------------------------------
template <class T> T* __fastcall TBaseList<T>::GetItems(int Index)
{
	return (T*)TList::Items[Index];
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TSocketComm
{
private:
	BYTE 				    m_aOPCode;
	int  				    m_iPort;
	int  				    m_iFromProcID;
	int  				    m_iToProcID;
	AnsiString 			    m_sIPAddress;

	int  __fastcall         fnGetToNodeID();
	int  __fastcall         fnGetToTaskID();
	int  __fastcall         fnGetToSeqNo();
public:
	__fastcall TSocketComm();
public:
	__property BYTE       	OPCode     = { read = m_aOPCode    , write = m_aOPCode    };
	__property AnsiString 	IPAddress  = { read = m_sIPAddress , write = m_sIPAddress };
	__property int        	Port       = { read = m_iPort      , write = m_iPort      };
	__property int          FromProcID = { read = m_iFromProcID, write = m_iFromProcID};
	__property int          ToProcID   = { read = m_iToProcID  , write = m_iToProcID  };
	__property int          ToNodeID   = { read = fnGetToNodeID                       };
	__property int          ToTaskID   = { read = fnGetToTaskID                       };
	__property int          ToSeqNo    = { read = fnGetToSeqNo                        };
};
//---------------------------------------------------------------------------
class TSocketCommList : public TBaseList<TSocketComm>
{
public:
	__fastcall TSocketCommList();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TEventInfo
{
private:
	BYTE 		  m_byEventCode;
	int 		  m_iEmrgency;
	TDateTime	  m_dtEmrCollDate;
	TDateTime	  m_dtEmrUpdtDate;
	UnicodeString m_sEventId;
	int 		  m_iEmrRelease;

	int 		  m_iEmrSkip;
public:
	__fastcall  TEventInfo();
	__fastcall ~TEventInfo();

	__property BYTE 		 EventCode   = {read=m_byEventCode  , write=m_byEventCode  };
	__property int 			 Emrgency    = {read=m_iEmrgency    , write=m_iEmrgency	   };
	__property TDateTime     EmrCollDate = {read=m_dtEmrCollDate, write=m_dtEmrCollDate};
	__property TDateTime     EmrUpdtDate = {read=m_dtEmrUpdtDate, write=m_dtEmrUpdtDate};
	__property UnicodeString EventId	 = {read=m_sEventId     , write=m_sEventId	   };
	__property int 			 EmrRelease  = {read=m_iEmrRelease  , write=m_iEmrRelease  };
	__property int 			 EmrSkip     = {read=m_iEmrSkip     , write=m_iEmrSkip     };

};
//---------------------------------------------------------------------------
class TEventList : public TBaseList<TEventInfo>
{
public:
	__fastcall TEventList();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TSensorInfo
{
private:
	TEventList*   m_pEventList;
	int 		  m_iIndex;
	int 		  m_iUseFlag;
	__int64		  m_iSensorID;
	UnicodeString m_sSiteId;
	UnicodeString m_sBuildingId;
	UnicodeString m_sSensorName;
	UnicodeString m_sRemoteIP;
	int			  m_iRemotePort;
	int 		  m_iWaitTime;
	int 		  m_iRetryCnt;
	BYTE 		  m_iActiveCnt;
	int 		  m_iHumidity;
	bool  		  m_bHumidityTest;
	int 		  m_iHumidityMin;
	BYTE 		  m_byHumidityFlag;
	int 		  m_iTemper;
	int 		  m_iTemperMax;
	bool  		  m_bTemperTest;
	BYTE 		  m_byTemperFlag;
	int 		  m_iSmoke;
	int 		  m_iSmokeMax;
	bool  		  m_bSmokeTest;
	BYTE 		  m_bySmokeFlag;
	int 		  m_iCPUStatus;
	TDateTime	  m_dtRecvTime;

	int 		  m_iSendS;
	int 		  m_iRecvS;

	BYTE __fastcall fnGetActivecnt();
	UnicodeString __fastcall fnGetTemperStr();
	UnicodeString __fastcall fnGetSmokeStr();
	void 		  __fastcall fnSetHumidity(int a_iValue);
	void 		  __fastcall fnSetTemper(int a_iValue);
	void 		  __fastcall fnSetSmoke(int a_iValue);
public:
	__fastcall  TSensorInfo();
	__fastcall ~TSensorInfo();

	TEventInfo* __fastcall fnGetEventInfoOfCode(BYTE a_byCode);


	__property int 			 Index  	 = {read=m_iIndex,       	write=m_iIndex      };
	__property int 			 UseFlag  	 = {read=m_iUseFlag,     	write=m_iUseFlag    };
	__property __int64       SensorID    = {read=m_iSensorID,    	write=m_iSensorID   };
	__property UnicodeString SiteId  	 = {read=m_sSiteId,      	write=m_sSiteId 	};
	__property UnicodeString BuildingId  = {read=m_sBuildingId,     write=m_sBuildingId };
	__property UnicodeString SensorName  = {read=m_sSensorName,     write=m_sSensorName };
	__property UnicodeString RemoteIP 	 = {read=m_sRemoteIP,       write=m_sRemoteIP   };
	__property int 			 RemotePort  = {read=m_iRemotePort,     write=m_iRemotePort };
	__property int 			 WaitTime 	 = {read=m_iWaitTime,       write=m_iWaitTime   };
	__property int			 RetryCnt 	 = {read=m_iRetryCnt,       write=m_iRetryCnt   };
	__property BYTE 		 ActiveCnt 	 = {read=fnGetActivecnt					        };
	__property TDateTime     RecvTime 	 = {read=m_dtRecvTime,      write=m_dtRecvTime  };

	__property int 			 Humidity     = {read=m_iHumidity,      write=fnSetHumidity };
	__property int 			 HumidityMin  = {read=m_iHumidityMin,   write=m_iHumidityMin};
	__property BYTE 		 HumidityFlag = {read=m_byHumidityFlag, write=m_byHumidityFlag};
	__property bool 		 HumidityTest = {read=m_bHumidityTest,  write=m_bHumidityTest};
	__property int   		 Temper       = {read=m_iTemper,        write=fnSetTemper	 };
	__property UnicodeString TemperStr    = {read=fnGetTemperStr     		 		 	 };
	__property int   		 TemperMax    = {read=m_iTemperMax,     write=m_iTemperMax	 };
	__property BYTE   		 TemperFlag   = {read=m_byTemperFlag,   write=m_byTemperFlag };
	__property bool   		 TemperTest   = {read=m_bTemperTest,    write=m_bTemperTest	 };
	__property int			 Smoke 	      = {read=m_iSmoke,         write=fnSetSmoke     };
	__property int			 SmokeMax 	  = {read=m_iSmokeMax,      write=m_iSmokeMax    };
	__property UnicodeString SmokeStr 	  = {read=fnGetSmokeStr 				         };
	__property BYTE			 SmokeFlag 	  = {read=m_bySmokeFlag,    write=m_bySmokeFlag  };
	__property bool			 SmokeTest    = {read=m_bSmokeTest,     write=m_bSmokeTest   };
	__property int			 CPUStatus 	  = {read=m_iCPUStatus,     write=m_iCPUStatus   };

	__property int			 SendS 	     = {read=m_iSendS,          write=m_iSendS  	 };
	__property int			 RecvS 	     = {read=m_iRecvS,          write=m_iRecvS  	 };
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TSensorList : public TBaseList<TSensorInfo>
{
private:
public:
	__fastcall  TSensorList();
	__fastcall ~TSensorList();

	UnicodeString __fastcall fnGetSensorName(int a_iSensorID);
	int 		  __fastcall fnGetSensorError();
};
//---------------------------------------------------------------------------
class TApplicationInfo
{
private:
	TSensorList	    *m_pSensorList;
	TSocketCommList *m_pSockComList;
	int 			 m_iProcID;
	int 			 m_iIcpPort;
	DWORD            m_dwIcpThrd;
	DWORD            m_dwAcpThrd;
	int 			 m_iCommPort;
	int 		     m_iCommTimeout;
	int 		     m_iCommRetryCnt;
	int 			 m_iEventTime;
	int 			 m_iReleEventTime;

	DWORD        	m_dwDBMThrd;
	DWORD      	 	m_dwDBPThrd [MAX_DB_SESSION];
	bool         	m_aDBConnect[MAX_DB_SESSION];
	int          	m_aDBStatus [MAX_DB_SESSION];
	int          	m_iMaxDBSession;
	int    		 	m_iSessionNo;

	int             m_iCommLogLevel;
	int             m_iPosX;
	int             m_iPosY;
	int             m_iPosW;
	int             m_iPosH;
	UnicodeString 	m_sConnectStr;
	UnicodeString   m_sDBServerName;
	UnicodeString   m_sDBUserName;
	UnicodeString   m_sDBPassword;
	UnicodeString 	m_sVersion;

	DWORD __fastcall fnGetDBPM(int a_iIndex);
	void  __fastcall fnSetDBPM(int a_iIndex, DWORD a_dwValue);
	bool  __fastcall fnGetDBConnect(int a_iIndex);
	void  __fastcall fnSetDBConnect(int a_iIndex, bool a_bValue);
	int   __fastcall fnGetDBStatus(int a_iIndex);
	void  __fastcall fnSetDBStatus(int a_iIndex, int a_iValue);
	int   __fastcall fnGetDBConnCount();
	UnicodeString __fastcall fnVersionInfo(const UnicodeString &sQuery);

public:
	__fastcall  TApplicationInfo();
	__fastcall ~TApplicationInfo();
public:
//	void 	 __fastcall fnLoadSensorInfo();
	__int64       __fastcall fnGetSensorID(UnicodeString a_sIP, int a_sPort);
	UnicodeString __fastcall fnGetBuildingID(UnicodeString a_sIP, int a_sPort);
	TSensorInfo*  __fastcall fnGetSensorInfo(__int64 a_iSensorID);
	bool	      __fastcall fnGetAppStart();
	void __fastcall fnWriteStream(AnsiString sId, BYTE *a_pBuf, DWORD a_dwSize, int a_iFlag);
	void __fastcall fnWriteDebug(AnsiString a_sMsg, int a_iFlag);
	void __fastcall fnWriteDebug(AnsiString a_sMsg, AnsiString a_sSenssor);


	__property int 			 	 ProcID         ={read=m_iProcID	 	 , write=m_iProcID       };
	__property int               IcpPort 		={read=m_iIcpPort        , write=m_iIcpPort	     };
	__property TSensorList*  	 SensorList     ={read=m_pSensorList	 , write=m_pSensorList   };
	__property TSocketCommList*  SockComList    ={read=m_pSockComList	 , write=m_pSockComList  };
	__property DWORD  	         IcpThrd        ={read=m_dwIcpThrd	     , write=m_dwIcpThrd	 };
	__property DWORD  	         AcpThrd        ={read=m_dwAcpThrd	     , write=m_dwAcpThrd	 };
	__property int               CommPort       ={read=m_iCommPort       , write=m_iCommPort	 };
	__property int               CommTimeout    ={read=m_iCommTimeout    , write=m_iCommTimeout	 };
	__property int        	     CommRetryCnt   ={read=m_iCommRetryCnt   , write=m_iCommRetryCnt };

	__property int               EventTime      ={read=m_iEventTime      , write=m_iEventTime 	 };
	__property int               ReleEventTime  ={read=m_iReleEventTime  , write=m_iReleEventTime};

	__property DWORD         DBMThrd	    ={read = m_dwDBMThrd     , write = m_dwDBMThrd   };
	__property DWORD         DBPThrd  [int] ={read = fnGetDBPM       , write = fnSetDBPM     };
	__property bool          DBConnect[int] ={read = fnGetDBConnect  , write = fnSetDBConnect};
	__property int           DBStatus [int] ={read = fnGetDBStatus   , write = fnSetDBStatus };
	__property int           MaxDBSession   ={read = m_iMaxDBSession                         };
	__property int           DBConnCount    ={read = fnGetDBConnCount                        };
	__property UnicodeString DBConnectStr   ={read = m_sConnectStr							 };
	__property int           CommLogLevel   ={read = m_iCommLogLevel                         };
	__property UnicodeString Version        ={read = m_sVersion       , write = m_sVersion   };
	__property int           PosX           ={read = m_iPosX                                 };
	__property int           PosY           ={read = m_iPosY                                 };
	__property int           PosW           ={read = m_iPosW                                 };
	__property int           PosH           ={read = m_iPosH                                 };

};
//---------------------------------------------------------------------------
class TPacketInfo
{
private:
	__int64		  m_iSensorID;
	UnicodeString m_sBuildingId;
	UnicodeString m_sRemoteIP;
	int        	  m_iRemotePort;
	BYTE       	  m_aBuffer[1024];
	int           m_iSize;

	BYTE* __fastcall fnGetBuffer(void);
public:
	__fastcall  TPacketInfo(void);
	__fastcall ~TPacketInfo(void);
public:
	void __fastcall fnSetBuffer(BYTE *a_pBuf, int a_iSize);
public:
	__property __int64		 SensorID	= {read=m_iSensorID	 , write=m_iSensorID  };
	__property UnicodeString BuildingId = {read=m_sBuildingId, write=m_sBuildingId};
	__property UnicodeString RemoteIP   = {read=m_sRemoteIP  , write=m_sRemoteIP  };
	__property int           RemotePort = {read=m_iRemotePort, write=m_iRemotePort};
	__property BYTE*         Buffer     = {read=fnGetBuffer                       };
	__property int           Size       = {read=m_iSize      , write=m_iSize      };
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TdcReqQuery
{
private:
	bool		    m_bResponse;
	DWORD           m_dwJobID;
	TdcDBJobType    m_eJobType;
	TdcDBQueryType  m_eQueryType;
	UnicodeString   m_sSQL;
	UnicodeString   m_sSensorId;
	BYTE	 		m_byCode;

	HWND__*			m_hResHndle;
	DWORD			m_dwResThread;

	int 			m_iSessionNo;
public:
	__fastcall  TdcReqQuery();
	__fastcall ~TdcReqQuery(void);
public:
	__property bool            Response     = {read=m_bResponse   ,write=m_bResponse  };
	__property DWORD           JobID        = {read=m_dwJobID     ,write=m_dwJobID    };
	__property TdcDBJobType    JobType      = {read=m_eJobType    ,write=m_eJobType   };
	__property TdcDBQueryType  QueryType    = {read=m_eQueryType  ,write=m_eQueryType };
	__property UnicodeString   SQL			= {read=m_sSQL     	  ,write=m_sSQL       };
	__property UnicodeString   SensorId     = {read=m_sSensorId   ,write=m_sSensorId  };
	__property BYTE			   Code 	    = {read=m_byCode      ,write=m_byCode  	  };
	__property HWND__*         ResHndle 	= {read=m_hResHndle   ,write=m_hResHndle  };
	__property DWORD           ResThread 	= {read=m_dwResThread ,write=m_dwResThread};
	__property int 			   SessionNo 	= {read=m_iSessionNo  ,write=m_iSessionNo };
};
//---------------------------------------------------------------------------
class TdcReqQueryList : public TBaseList<TdcReqQuery>
{
private:
public:
	__fastcall  TdcReqQueryList(void);
	__fastcall ~TdcReqQueryList(void);
};
//---------------------------------------------------------------------------
class TdcResQuery
{
private:
	DWORD           m_dwJobID;
	TdcDBJobType    m_eJobType;
	TdcDBQueryType  m_eQueryType;
	UnicodeString   m_sSQL;
	UnicodeString   m_sSensorId;
	BYTE	 		m_byCode;

	HWND__*			m_hResHndle;
	DWORD			m_dwResThread;

	TADOQuery*      m_pQuery;
public:
	__fastcall  TdcResQuery();
	__fastcall ~TdcResQuery(void);
public:
	__property DWORD           JobID        = {read=m_dwJobID     ,write=m_dwJobID    };
	__property TdcDBJobType    JobType      = {read=m_eJobType    ,write=m_eJobType   };
	__property TdcDBQueryType  QueryType    = {read=m_eQueryType  ,write=m_eQueryType };
	__property UnicodeString   SQL			= {read=m_sSQL     	  ,write=m_sSQL       };
	__property UnicodeString   SensorId     = {read=m_sSensorId   ,write=m_sSensorId  };
	__property BYTE			   Code 	    = {read=m_byCode      ,write=m_byCode  	  };
	__property HWND__*         ResHndle 	= {read=m_hResHndle   ,write=m_hResHndle  };
	__property DWORD           ResThread 	= {read=m_dwResThread ,write=m_dwResThread};
	__property TADOQuery*      Query 	    = {read=m_pQuery 	  ,write=m_pQuery	  };

};

//---------------------------------------------------------------------------


#endif
