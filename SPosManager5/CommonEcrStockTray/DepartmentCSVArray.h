#pragma once
/**********************************************************************/
#include "NamespaceDept.h"
/**********************************************************************/
#define DEPTDB_VERSION 7
/**********************************************************************/

class CDepartmentCSVRecord : public CCSVRecord
{
public:
	CDepartmentCSVRecord();
	virtual ~CDepartmentCSVRecord() {}
	virtual void ConvertToCSV ( CCSV& csv );
	virtual void ConvertFromCSV ( CCSV& csv );

	CString StripSpaces ( const char* szText );

public:
	void SetDeptNo ( int nDeptNo );
	void SetReportEnable ( bool bEnable );
	void SetEposGroup ( int n );
	void SetReportGroup ( int n );
	void SetRepText ( const char* szRepText );
	void SetEposText ( const char* szEposText );
	void SetTicketsMode( int n );
	void SetTickets( int n );
	void SetDPQtyFlag( bool b );
	void SetNegativeFlag( bool b );
	void SetNonAddFlag( bool b );
	void SetIDraughtExportFlag( bool b );
	void SetSpecimenLearnPlu( __int64 n );
	void SetStockTemplateFlag( bool b );
	void SetStockDescriptionFlag( bool b );
	void SetStockCategoryFlag( bool b );
	void SetStockQtyFlag( bool b );
	void SetSportsBookerFlag( bool b );
	void SetImageFilename( const char* sz );
	void SetLoyaltyBonusType( int n );
	void SetLoyaltyBonusTrip(double d);
	void SetLoyaltyBonusValue(double d);
	void SetLoyaltyBonusPoints(int n);
	void SetLoyaltyBonusPercent(int n);
	void SetLoyaltyGeneralSpendBonus(bool b);

public:
	int GetDeptNo() { return m_nDeptNo; }
	bool GetReportEnable() { return m_bReportEnable; }
	int GetEposGroup() { return m_nEposGroup; }
	int GetReportGroup() { return m_nReportGroup; }
	const char* GetRepText() { return m_strRepText; }
	const char* GetEposText() { return m_strEposText; }
	const char* GetDefaultText();
	const char* GetReportText();
	int GetTickets() { return m_nTickets; }
	int GetTicketsMode() { return m_nTicketsMode; }
	bool GetDPQtyFlag() { return m_bDPQty; }
	bool GetNegativeFlag() { return m_bNegative; }
	bool GetNonAddFlag() { return m_bNonAdd; }
	bool GetIDraughtExportFlag() { return m_bIDraughtExport; }
	__int64 GetSpecimenLearnPlu() { return m_nSpecimenLearnPlu; }
	bool GetStockTemplateFlag() { return m_bStockTemplate; }
	bool GetStockDescriptionFlag() { return m_bStockDescription; }
	bool GetStockCategoryFlag() { return m_bStockCategory; }
	bool GetStockQtyFlag() { return m_bStockQty; }
	bool GetSportsBookerFlag() { return m_bSportsBooker; }
	const char* GetImageFilename(){ return m_strImageFilename; }
	int GetLoyaltyBonusType(){ return m_nLoyaltyBonusType; }
	double GetLoyaltyBonusTrip(){ return m_dLoyaltyBonusTrip; }
	double GetLoyaltyBonusValue(){ return m_dLoyaltyBonusValue; }
	int GetLoyaltyBonusPoints(){ return m_nLoyaltyBonusPoints; }
	int GetLoyaltyBonusPercent(){ return m_nLoyaltyBonusPercent; }
	bool GetLoyaltyGeneralSpendBonus() { return m_bLoyaltyGeneralSpendBonus; }

private:
	void ClearRecord();
	void V1ConvertFromCSV ( CCSV& csv );
	void V2To7ConvertFromCSV ( CCSV& csv, int nVer );
	
private:
	int m_nDeptNo;
	bool m_bReportEnable;
	int m_nEposGroup;
	int m_nReportGroup;
	CString m_strRepText;
	CString m_strEposText;
	CString m_strDefaultText;
	CString m_strReportText;
	int m_nTickets;
	int m_nTicketsMode;
	bool m_bDPQty;
	bool m_bNegative;
	bool m_bNonAdd;
	bool m_bIDraughtExport;
	__int64 m_nSpecimenLearnPlu;
	bool m_bStockTemplate;
	bool m_bStockDescription;
	bool m_bStockCategory;
	bool m_bStockQty;
	bool m_bSportsBooker;
	CString m_strImageFilename;
	int m_nLoyaltyBonusType;
	double m_dLoyaltyBonusTrip;
	double m_dLoyaltyBonusValue;
	int m_nLoyaltyBonusPoints;
	int m_nLoyaltyBonusPercent;
	bool m_bLoyaltyGeneralSpendBonus;
};

/**********************************************************************/

class CDepartmentCSVArray : public CSharedCSVArray
{
public:
	CDepartmentCSVArray();

public:
	int Open( const char* szFilename, int nMode );

	const char* GetReportTextByDeptNo( int nDeptNo );
	bool GetReportEnableByDeptNo( int nDeptNo );
	const char* GetDefaultTextByDeptNo( int nDeptNo );

	bool FindDeptByNumber ( int nDeptNo, int& nIndex );

	void SetDeptZero( const char* sz );
	void AddPMSDepartment( int nDeptNo, const char* szText );

public:
	int GetSportsBookerDeptNo();
	void SetSportsBookerDeptNo( int nDeptNo );
	void FixSportsBookerDeptNo();

private:
	CString m_strReportText;
	CString m_strDefaultText;

private:
	bool m_bCheckedSportsBookerDeptNo;
	int m_nSportsBookerDeptNo;
};

/**********************************************************************/
