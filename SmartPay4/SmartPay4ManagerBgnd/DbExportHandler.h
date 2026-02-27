#pragma once
/**********************************************************************/
#include "..\SmartPay4Shared\Consolidation.h"
#include "..\SmartPay4Shared\ReportConsolidationArray.h"
#include "..\SmartPay4Shared\SQLDb.h"
#include "..\SmartPay4Shared\SQLTable_Account\SQLRowAccountFull.h"
/**********************************************************************/
#include "CardPicker.h"
/**********************************************************************/
#define	 nDBEXPORTFIELDS_DATA		0			// csv export to a file
#define	 nDBEXPORTFIELDS_DBASE		1			// custom database report
#define	 nDBEXPORTFIELDS_REPORT		2			// extra report fields
#define  nDBEXPORTFIELDS_NONE		3			// no field selection
/**********************************************************************/
static const char* szDONTCARE			= "Don't care";
static const char* szPURSE1				= "Purse1";
static const char* szPURSE2				= "Purse2";
static const char* szPURSE3				= "Purse3";
static const char* szPURSE1_BALANCE		= "Purse1 Balance";
static const char* szPURSE1_LIABILITY	= "Purse1 Liability";
static const char* szPURSE1_CREDIT		= "Purse1 Credit";
static const char* szPURSE2_BALANCE		= "Purse2 Balance";
static const char* szPURSE3_BALANCE		= "Purse3 Balance";
static const char* szPURSE3_LIABILITY	= "Purse3 Liability";
static const char* szPURSE3_CREDIT		= "Purse3 Credit";
static const char* szPOINTS				= "Points";
static const char* szCASHSPENDTD		= "CashSpendTD";
static const char* szPURSE1SPENDTD		= "Purse1SpendTD";
static const char* szPURSE2SPENDTD		= "Purse2SpendTD";
static const char* szPURSE3SPENDTD		= "Purse3SpendTD";
static const char* szTOTALSPENDTD		= "TotalSpendTD";
static const char* szPOINTSTD			= "PointsTD";
/**********************************************************************/
static const char* szUNKNOWN			= "unknown";
static const char* szMALE				= "Male";
static const char* szFEMALE				= "Female";
/**********************************************************************/
static const char* szOR					= "OR";
static const char* szAND				= "AND";
/**********************************************************************/
#define	nSORTONUSERID		0
#define	nSORTONUSERNAME		1
#define	nSORTONSURNAME		2				// new option
#define nSORTONMEMBERID		3
#define nSORTONEXTREF1		4
#define nSORTONEXTREF2		5
#define nSORTONINFO1		6
#define nSORTONINFO2		7
#define nSORTONINFO3		8
#define nSORTONINFO4		9
#define nSORTONINFO5		10
#define nSORTONINFO6		11
#define nSORTONINFO7		12
#define nSORTONINFO8		13
/**********************************************************************/
#define nFILTER_IGNORE		0
#define nFILTER_YES			1
#define nFILTER_NO			2
/**********************************************************************/
#define nFILTER_UNKNOWN		0
#define nFILTER_MALE		1
#define nFILTER_FEMALE		2
/**********************************************************************/
#define nMAX_SORTLABELENGTH		15
/**********************************************************************/

struct CDatabaseReportInfo
{
public:
	CDatabaseReportInfo();

public:
	CString m_strLabel;
	CString m_strName;
	CString m_strHeader;
	CString m_strFilter;
	CString m_strCondition;
	CString m_strOutputMethod;
};

/**********************************************************************/

class CDbExportHandler
{
public:
	CDbExportHandler();
	~CDbExportHandler();
	bool Read(const char* szLabel);
	bool Save(const char* szLabel);
	bool Delete(const char* szLabel);

	void GetCustomListArray(CArray<CDatabaseReportInfo, CDatabaseReportInfo>& arrayInfo);
	void CopyFromReportInfo(CDatabaseReportInfo& info);
	void CopyToReportInfo(CDatabaseReportInfo& info);
	void MakeDefaultReportInfo(CDatabaseReportInfo& info);
	bool MergeCustomReports(CArray<CDatabaseReportInfo, CDatabaseReportInfo>& arrayCustom);

	CString GetSQLOrderBy();
	CString GetSQLConditionalField(int n);
	bool NonSQLFilter(CSQLRowAccountFull& AccountRow, bool bExist = TRUE);

	CString GetSortFieldText(CSQLRowAccountFull& AccountRow);
	static double GetValueByFieldName(CString strField, CSQLRowAccountFull& RowAccount);

	CString CreateSortLabel(CSQLRowAccountFull& RowAccount);
	void GetSortItem(CSQLRowAccountFull& AccountRow, CSortedDbReportItem& item, bool bGotRow);

	bool IsCustomReport();

	void SetTitle(const char* szTitle) { m_strTitle = szTitle; }

	bool IsCardInRange(const char* szCardNo);

	int GetCardRangeIndex() { return m_csvFilters.GetInt(0); }	//0=range, 1=list
	__int64 GetStartCardNoInt() { return m_csvFilters.GetInt64(1); }
	const char* GetStartCardNoString() { return m_csvFilters.GetString(1); }
	__int64 GetEndCardNoInt() { return m_csvFilters.GetInt64(2); }
	const char* GetEndCardNoString() { return m_csvFilters.GetString(2); }

	int GetGroupNo() { return m_csvFilters.GetInt(3); }
	bool GetGroupSummary() { return m_csvFilters.GetBool(4); }
	bool GetValidReqd() { return m_csvFilters.GetBool(5); }
	bool GetExpiredReqd() { return m_csvFilters.GetBool(6); }
	bool GetInactiveReqd() { return m_csvFilters.GetBool(7); }
	bool GetSortByGroup() { return m_csvFilters.GetBool(8); }
	CString GetSortOnLabel();														// { return m_csvFilters.GetInt(9);	}	// cardno, cardname
	bool GetAlertReqd() { return m_csvFilters.GetBool(10); }
	bool GetUnknownReqd() { return m_csvFilters.GetBool(11); }
	bool GetSkipBlanks() { return m_csvFilters.GetBool(12); }
	int GetTick1() { return m_csvFilters.GetInt(13); }
	int GetTick2() { return m_csvFilters.GetInt(14); }
	int GetTick3() { return m_csvFilters.GetInt(15); }
	int GetTick4() { return m_csvFilters.GetInt(16); }
	bool GetUnissuedReqd() { return m_csvFilters.GetBool(17); }
	bool GetTotalise() { return m_csvFilters.GetBool(18); }
	bool GetGroupShiftNoneReqd() { return m_csvFilters.GetBool(19) == FALSE; }
	bool GetGroupShiftCurrentReqd() { return m_csvFilters.GetBool(20) == FALSE; }
	bool GetGroupShiftFutureReqd() { return m_csvFilters.GetBool(21) == FALSE; }

	int GetSortOn();

	// see nFILTERFIELDS
	void SetCardRangeIndex(int n) { m_csvFilters.SetAt(0, n); }
	void SetStartCardNo(__int64 nCardNo) { m_csvFilters.SetAt(1, nCardNo); }
	void SetStartCardNo(const char* szCardNo) { m_csvFilters.SetAt(1, szCardNo); }
	void SetEndCardNo(__int64 nCardNo) { m_csvFilters.SetAt(2, nCardNo); }
	void SetEndCardNo(const char* szCardNo) { m_csvFilters.SetAt(2, szCardNo); }
	void SetGroupNo(int nGroupNo) { m_csvFilters.SetAt(3, nGroupNo); }
	void SetGroupSummary(bool bReqd) { m_csvFilters.SetAt(4, bReqd); }
	void SetValidReqd(bool bReqd) { m_csvFilters.SetAt(5, bReqd); }
	void SetExpiredReqd(bool bReqd) { m_csvFilters.SetAt(6, bReqd); }
	void SetInactiveReqd(bool bReqd) { m_csvFilters.SetAt(7, bReqd); }
	void SetSortByGroup(bool bReqd) { m_csvFilters.SetAt(8, bReqd); }
	void SetSortOn(const char* szLabel) { m_csvFilters.SetAt(9, szLabel); }
	void SetAlertReqd(bool bReqd) { m_csvFilters.SetAt(10, bReqd); }
	void SetUnknownReqd(bool bReqd) { m_csvFilters.SetAt(11, bReqd); }
	void SetSkipBlanks(bool bReqd) { m_csvFilters.SetAt(12, bReqd); }
	void SetTick1(int n) { m_csvFilters.SetAt(13, n); }
	void SetTick2(int n) { m_csvFilters.SetAt(14, n); }
	void SetTick3(int n) { m_csvFilters.SetAt(15, n); }
	void SetTick4(int n) { m_csvFilters.SetAt(16, n); }
	void SetUnissuedReqd(bool bReqd) { m_csvFilters.SetAt(17, bReqd); }
	void SetTotalise(bool bReqd) { m_csvFilters.SetAt(18, bReqd); }
	void SetGroupShiftNoneReqd(bool bReqd) { m_csvFilters.SetAt(19, bReqd == FALSE); }
	void SetGroupShiftCurrentReqd(bool bReqd) { m_csvFilters.SetAt(20, bReqd == FALSE); }
	void SetGroupShiftFutureReqd(bool bReqd) { m_csvFilters.SetAt(21, bReqd == FALSE); }

	// see nCONDITIONFIELDS
	const char* GetField1() { return m_csvConditions.GetString(0); }
	const char* GetCondition1() { return m_csvConditions.GetString(1); }
	double		GetValue1() { return m_csvConditions.GetDouble(2); }
	const char* GetAndOr() { return m_csvConditions.GetString(3); }
	const char* GetField2() { return m_csvConditions.GetString(4); }
	const char* GetCondition2() { return m_csvConditions.GetString(5); }
	double		GetValue2() { return m_csvConditions.GetDouble(6); }
	bool		GetCheckAllergies() { return m_csvConditions.GetBool(7); }
	int 		GetAllergyCodes() { return m_csvConditions.GetInt(8); }
	bool		GetCheckAlertCode() { return m_csvConditions.GetBool(9); }
	int 		GetAlertCode() { return m_csvConditions.GetInt(10); }
	const char* GetCondition3() { return m_csvConditions.GetString(11); }		// gender
	int			GetInfoFilterIndex() { return m_csvConditions.GetInt(12); }
	const char* GetInfoFilterText() { return m_csvConditions.GetString(13); }
	int 		GetBioRegisterCheck() { return m_csvConditions.GetInt(14); }
	bool		GetDietaryCheck() { return m_csvConditions.GetBool(15); }
	CString 	GetDietaryText() { return m_csvConditions.GetString(16); }

	//Addition output files
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
	void SetCheckAllergies(bool bReqd) { m_csvConditions.SetAt(7, bReqd); }
	void SetAllergyCodes(int n) { m_csvConditions.SetAt(8, n); }
	void SetCheckAlertCode(bool bReqd) { m_csvConditions.SetAt(9, bReqd); }
	void SetAlertCode(int n) { m_csvConditions.SetAt(10, n); }
	void SetCondition3(const char* szText) { m_csvConditions.SetAt(11, szText); }
	void SetInfoFilterIndex(int nIndex) { m_csvConditions.SetAt(12, nIndex); }
	void SetInfoFilterText(const char* szText) { m_csvConditions.SetAt(13, szText); }
	void SetBioRegisterCheck(int n) { m_csvConditions.SetAt(14, n); }
	void SetDietaryCheck(bool bReqd) { m_csvConditions.SetAt(15, bReqd); }
	void SetDietaryText(const char* szText) { m_csvConditions.SetAt(16, szText); }

private:
	void Reset();
	void SetTitles();
	void SetDefaultFilters();
	void SetDefaultOutputMethod();

	CString MakeSortLabel(const char* szText);

public:
	int m_nExportFlag;
	bool m_bSecondaryOutput;
	bool m_bSortOnAllowed;
	bool m_bTotaliseAllowed;
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
	CCSV m_csvFilters;										// report filters used for report key

private:
	CString m_strDefFilename;
	CCSV m_csvConditions;
	CCSV m_csvOutputMethod;
	CString m_strDefaultConditions;
	CCardPicker m_cardPicker;

private:
	CMap <CString, LPCTSTR, CString, LPCTSTR> m_ExportConditionFieldMap;
};

/**********************************************************************/
