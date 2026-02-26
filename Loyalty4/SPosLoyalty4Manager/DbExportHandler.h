#pragma once
//*******************************************************************
#include "resource.h"
//*******************************************************************
#include "..\SPosLoyalty4Shared\Consolidation.h"
#include "..\SPosLoyalty4Shared\SQLHelpers.h"
#include "..\SPosLoyalty4Shared\SQLTable_Account\SQLRowAccountFull.h"
//*******************************************************************
#include "CardPicker.h"
//*******************************************************************

//m_nexportflag settings **********************************************

#define	 nDBEXPORTFIELDS_DATA		0			// csv export to a file
#define	 nDBEXPORTFIELDS_DBASE		1			// custom database report
#define	 nDBEXPORTFIELDS_REPORT		2			// extra report fields
#define  nDBEXPORTFIELDS_NONE		3			// no field selection

//*********************************************************************
// defines for conditional reporting 

static const char* szDONTCARE		= "Don't care";
static const char* szPURSE1			= "Purse1";
static const char* szPURSE2			= "Purse2";
static const char* szPOINTS			= "Points";
static const char* szPURSE1SPENDTD	= "Purse1SpendTD";
static const char* szPURSE2SPENDTD	= "Purse2SpendTD";
static const char* szCASHSPENDTD	= "CashSpendTD";
static const char* szTOTALSPENDTD	= "TotalSpendTD";
static const char* szPOINTSTD		= "PointsTD";

static const char* szOR				= "OR";
static const char* szAND			= "AND";

static const char* szUNKNOWN = "unknown";
static const char* szMALE = "Male";
static const char* szFEMALE = "Female";
/**********************************************************************/

//**********************************************************************

#define	nSORTONCARDNO			0
#define	nSORTONCARDNAME			1
#define	nSORTONSURNAME			2
#define nSORTONEXTREF			3
#define nSORTONINFO1			4
#define nSORTONINFO2			5
#define nSORTONINFO3			6
#define nSORTONINFO4			7
#define nSORTONADDRESS1			8
#define nSORTONADDRESS2			9
#define nSORTONADDRESS3			10
#define nSORTONADDRESS4			11
#define nSORTONADDRESS5			12

/**********************************************************************/

class CDbExportHandler
{
public:
	CDbExportHandler();
	bool Read(const char* szLabel);
	bool Save(const char* szLabel);
	bool Delete(const char* szLabel);
	void GetCustomList(CCSV& csvLine);					// get list of custom reports
	bool GetNextCustomLabel(CString& strNextLabel);

	bool IsValidOrderByField(CSQLRowAccountFull& RowAccount);
	bool IsCustomReport();

	void SetTitle(const char* szTitle) { m_strTitle = szTitle; }

	bool IsCardInRange(__int64 nUserID);
	bool IsCardInRange(__int64 nUserID, int nGroupNo);

	int GetCardRangeIndex() { return m_csvFilters.GetInt(0); }	//0=range, 1=list
	__int64 GetStartCardNoInt64() { return m_csvFilters.GetInt64(1); }
	const char* GetStartCardNoString() { return m_csvFilters.GetString(1); }
	__int64 GetEndCardNoInt64() { return m_csvFilters.GetInt64(2); }
	const char* GetEndCardNoString() { return m_csvFilters.GetString(2); }
	int GetGroupNo() { return m_csvFilters.GetInt(3); }
	bool GetGroupSummary() { return m_csvFilters.GetBool(4); }
	bool GetValidReqd() { return m_csvFilters.GetBool(5); }
	bool GetHotlistedReqd() { return m_csvFilters.GetBool(6); }
	bool GetExpiredReqd() { return m_csvFilters.GetBool(7); }
	bool GetInactiveReqd() { return m_csvFilters.GetBool(8); }
	bool GetSortByGroup() { return m_csvFilters.GetBool(9); }
	CString GetSortOnLabel();
	bool GetUnknownReqd() { return m_csvFilters.GetBool(11); }
	bool GetSkipBlanks() { return m_csvFilters.GetBool(12); }
	int GetGenderOption() { return m_csvFilters.GetInt(15); }
	int GetGenderCode() { return m_csvFilters.GetInt(16); }
	CString GetAlertFilter() { return m_csvFilters.GetString(17); }

	int GetSortOn();

	void SetCardRangeIndex(int n) { m_csvFilters.SetAt(0, n); }
	void SetStartCardNo(int nCardNo) { m_csvFilters.SetAt(1, nCardNo); }
	void SetStartCardNo(const char* szCardNo) { m_csvFilters.SetAt(1, szCardNo); }
	void SetEndCardNo(int nCardNo) { m_csvFilters.SetAt(2, nCardNo); }
	void SetEndCardNo(const char* szCardNo) { m_csvFilters.SetAt(2, szCardNo); }
	void SetGroupNo(int nGroupNo) { m_csvFilters.SetAt(3, nGroupNo); }
	void SetGroupSummary(bool bReqd) { m_csvFilters.SetAt(4, bReqd); }
	void SetValidReqd(bool bReqd) { m_csvFilters.SetAt(5, bReqd); }
	void SetHotlistedReqd(bool bReqd) { m_csvFilters.SetAt(6, bReqd); }
	void SetExpiredReqd(bool bReqd) { m_csvFilters.SetAt(7, bReqd); }
	void SetInactiveReqd(bool bReqd) { m_csvFilters.SetAt(8, bReqd); }
	void SetSortByGroup(bool bReqd) { m_csvFilters.SetAt(9, bReqd); }
	void SetSortOn(const char* szLabel) { m_csvFilters.SetAt(10, szLabel); }
	void SetUnknownReqd(bool bReqd) { m_csvFilters.SetAt(11, bReqd); }
	void SetSkipBlanks(bool bReqd) { m_csvFilters.SetAt(12, bReqd); }
	void SetGenderOption(int nOption) { m_csvFilters.SetAt(15, nOption); }
	void SetGenderCode(int nGenderCode) { m_csvFilters.SetAt(16, nGenderCode); }
	void SetAlertFilter(CString str) { m_csvFilters.SetAt(17, str); }

	bool NonSQLFilter(CSQLRowAccountFull& RowAccount, bool bExist = TRUE);
	CString GetSQLConditionalField(int n);
	CString GetSQLOrderBy();
	void GetSortItem(CSQLRowAccountFull& AccountRow, CSortedDbReportItem& item, bool bGotRow);

	const char* GetField1() { return m_csvConditions.GetString(0); }
	const char* GetCondition1() { return m_csvConditions.GetString(1); }
	double GetValue1() { return m_csvConditions.GetDouble(2); }
	const char* GetAndOr() { return m_csvConditions.GetString(3); }
	const char* GetField2() { return m_csvConditions.GetString(4); }
	const char* GetCondition2() { return m_csvConditions.GetString(5); }
	double GetValue2() { return m_csvConditions.GetDouble(6); }

	int GetSaveAsType() { return m_csvOutputMethod.GetInt(0); }
	const char* GetSaveAsFile() { return m_csvOutputMethod.GetString(1); }

	void SetOutputMethod(int n) { m_csvOutputMethod.SetAt(0, n); }
	void SetOutputFile(const char* szFilename) { m_csvOutputMethod.SetAt(1, szFilename); }

	void SetDefaultConditions();
	void SetField1(const char* szText) { m_csvConditions.SetAt(0, szText); }
	void SetCondition1(const char* szText) { m_csvConditions.SetAt(1, szText); }
	void SetValue1(double dValue) { m_csvConditions.SetAt(2, dValue, 2); }
	void SetAndOr(const char* szText) { m_csvConditions.SetAt(3, szText); }
	void SetField2(const char* szText) { m_csvConditions.SetAt(4, szText); }
	void SetCondition2(const char* szText) { m_csvConditions.SetAt(5, szText); }
	void SetValue2(double dValue) { m_csvConditions.SetAt(6, dValue, 2); }

private:
	void Reset();
	void SetTitles();
	void SetDefaultFilters();
	void SetDefaultOutputMethod();

public:
	int m_nExportFlag;
	bool m_bSecondaryOutput;
	bool m_bOrderByAllowed;
	bool m_bGroupSummaryAllowed;
	bool m_bGroupSortAllowed;
	bool m_bCardListAllowed;
	bool m_bUnknownAllowed;

	CString m_strLabel;
	CString m_strTitle;
	CString m_strFilename;
	CString m_strHeader;
	CString m_strFileFilters;
	CString m_strDefExtension;

protected:
	CCSV m_csvFilters;						// report filters used for report key

private:
	CString m_strDefFilename;
	CCSV m_csvConditions;
	CCSV m_csvOutputMethod;
	CString m_strDefaultConditions;
	CCardPicker m_cardPicker;
};

/**********************************************************************/
