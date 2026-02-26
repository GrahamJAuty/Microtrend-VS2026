#pragma once
/**********************************************************************/
#ifndef POSTRAY_UTILITY
/**********************************************************************/
#include "CustomerCSVArray.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/

struct CCustomerAutoDeleteInfo
{
public:
	CCustomerAutoDeleteInfo();
	void Reset();
	
public:
	int Compare ( CCustomerAutoDeleteInfo& source, int nHint = 0 );
	void Add	( CCustomerAutoDeleteInfo& source );
	
public:
	int m_nNwkNo;
	int m_nLocNo;
	int m_nCheckDay;
	CTime m_FileTime;
	COleDateTime m_LastCheckTime;
};

/**********************************************************************/

class CCustomerAutoDeleter  
{
public:
	CCustomerAutoDeleter();
	int GetLocIdx(){ return m_nLocIdx; }
	
public:
	void DeleteCustomerIdleTime();
	void DeleteCustomerExternal( int nLocIdx );
	void SetReadyToGoFlag( bool b ){ m_bReadyToGo = b; }
	void GotoNextLoc();

public:
	virtual bool IsOneOff() = 0;
	virtual bool CheckEnabled() = 0;
	virtual bool OpenCustomerFile( CDataManagerInfo& info ) = 0;
	virtual void CloseCustomerFile( CDataManagerInfo& info ) = 0;
	virtual void WriteCustomerFile( CDataManagerInfo& info ) = 0;
	virtual CCustomerCSVArray* GetCustomerArray() = 0;
	virtual const char* GetCustomerFilename() = 0;
	virtual const char* GetCustomerFileTypeName() = 0;
	
private:
	void CheckLocIdx();
	bool NeedUpdate();
	void RememberUpdate();
	void DeleteCustomerInternal();
		
protected:
	int m_nLocIdx;
	bool m_bReadyToGo;
	bool m_bUpdated;
	CString m_strCustomerFilename;
	CString m_strCustomerFileTypeName;

private:
	CReportConsolidationArray<CCustomerAutoDeleteInfo> m_arrayAutoDeleteInfo;
};

/**********************************************************************/

class CCustomerAutoDeleterAccount : public CCustomerAutoDeleter
{
public:
	bool IsOneOff(){ return FALSE; }
	bool CheckEnabled();
	bool OpenCustomerFile( CDataManagerInfo& info );
	void CloseCustomerFile( CDataManagerInfo& info );
	void WriteCustomerFile( CDataManagerInfo& info );
	CCustomerCSVArray* GetCustomerArray();
	const char* GetCustomerFilename();
	const char* GetCustomerFileTypeName();
};

/**********************************************************************/

class CCustomerAutoDeleterOneOff : public CCustomerAutoDeleter
{
public:
	bool IsOneOff(){ return TRUE; }
	bool CheckEnabled();
	bool OpenCustomerFile( CDataManagerInfo& info );
	void CloseCustomerFile( CDataManagerInfo& info );
	void WriteCustomerFile( CDataManagerInfo& info );
	CCustomerCSVArray* GetCustomerArray();
	const char* GetCustomerFilename();
	const char* GetCustomerFileTypeName();
};

/**********************************************************************/
extern CCustomerAutoDeleterAccount CustomerAutoDeleterAccount;
extern CCustomerAutoDeleterOneOff CustomerAutoDeleterOneOff;
/**********************************************************************/
#endif
/**********************************************************************/
