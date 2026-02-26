#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#define SB_READDATES_OK 0
#define SB_READDATES_NOFILE 1
#define SB_READDATES_ERROR 2
/**********************************************************************/
#define SB_WEBDATE_NONE 0
#define SB_WEBDATE_FIRST 1
#define SB_WEBDATE_NEXT 2
/**********************************************************************/
 
/**********************************************************************/

class CSportsBookerWebAPISalesDateTime
{
public:
	CSportsBookerWebAPISalesDateTime(void);
	
public:
	const char* GetWebAPIDateTimeString();
	void ProcessXMLDateTime( const char* szDate );

public:
	bool CreateFile( int nNetworkIdx );
	int Read( int nNetworkIdx );
	bool Write();

public:
	int GetDateTimeStatus(){ return m_nDateTimeStatus; }
	const char* GetDisplayDateTimeNextRead();

public:
	void GetNextReadDateTime( COleDateTime& datetime );
	void SetFirstReadDateTime( COleDateTime& datetime );

private:
	void ResetDateTime();
	bool ReadV1Line( CCSV& csv );
	bool StringToOleDateTime( const char* sz, COleDateTime& datetime );
	void OleDateTimeToString( COleDateTime& date, CString& str );

private:
	int m_nNetworkIdx;
	int m_nDateTimeStatus;
	COleDateTime m_oleDateTimeNextRead;
	
private:
	CString m_strWebAPIDateTimeString;
	CString m_strDisplayDateTimeNextRead;
};

/**********************************************************************/
#endif
/**********************************************************************/