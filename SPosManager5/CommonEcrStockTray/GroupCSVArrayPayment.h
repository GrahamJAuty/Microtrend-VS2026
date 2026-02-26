#pragma once
/**********************************************************************/
 
/**********************************************************************/
#include "GroupCSVArrayBase.h"
/**********************************************************************/
/*	DEPARTMENT GROUP DATABASE										  */
/**********************************************************************/
#define	PAYMENTGROUPDB_VERSION_NO	2	
/**********************************************************************/

namespace PaymentGroup
{
const recInt GroupNo			= { "GroupNo", 1, 99 };
const recString	RepText			= { "RepText", 0, 32 };
};

/**********************************************************************/

class CGroupCSVRecordPayment : public CGroupCSVRecordBase
{
public:
	CGroupCSVRecordPayment();
	virtual ~CGroupCSVRecordPayment(){}

public:
	const recInt GetGroupNoRange(){ return PaymentGroup::GroupNo; }
	void SetDefaultGroup( int nGroupNo );

public:
	void ClearRecord();
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );
	void V1ConvertFromCSV ( CCSV& csv );
	void V2ConvertFromCSV ( CCSV& csv );
	
public:
	const char* GetEposText(){ return GetRepText(); }
	const char* GetRepText(){ return m_strRepText; }

public:
	void SetEposText( const char* sz ){ SetRepText( sz ); }
	void SetRepText( const char* sz );

private:
	CString m_strRepText;
};

/**********************************************************************/

class CGroupCSVArrayPayment : public CGroupCSVArrayBase
{
public:
	CGroupCSVArrayPayment();

public:
	int Open( const char* szFilename, int nMode );
	void GetGroupInfo( int nIndex, CBaseGroupInfo& info );
	bool GetListDataLine( int nIndex, CString& strLine );
	const recInt GetGroupNoRange(){ return PaymentGroup::GroupNo; }
	
public:
	void RememberSettings();
	void RestoreSettings();
};

/**********************************************************************/
