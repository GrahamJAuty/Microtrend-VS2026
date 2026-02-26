/**********************************************************************/
#include "DepartmentCSVArray.h"
/**********************************************************************/

CDepartmentCSVRecord::CDepartmentCSVRecord()
{
	ClearRecord();
}

/**********************************************************************/

void CDepartmentCSVRecord::ClearRecord()
{
	m_nDeptNo = 1;
	m_bReportEnable = TRUE;
	m_nEposGroup = 1;
	m_nReportGroup = 0;
	m_strRepText = "";
	m_strEposText = "";
	m_nTicketsMode = 0;
	m_nTickets = 0;
	m_bDPQty = FALSE;
	m_bNegative = FALSE;
	m_bNonAdd = FALSE;
	m_bIDraughtExport = FALSE;
	m_nSpecimenLearnPlu = 0;
	m_bStockTemplate = FALSE;
	m_bStockDescription = FALSE;
	m_bStockCategory = FALSE;
	m_bStockQty = FALSE;
	m_bSportsBooker = FALSE;
	m_strImageFilename = "";
	m_nLoyaltyBonusType = 0;
	m_dLoyaltyBonusTrip = 0.0;
	m_nLoyaltyBonusPoints = 0;
	m_dLoyaltyBonusValue = 0.0;
	m_nLoyaltyBonusPercent = 0;
	m_bLoyaltyGeneralSpendBonus = TRUE;
}

/**********************************************************************/
/*	convert to csv											*/
/**********************************************************************/

void CDepartmentCSVRecord::ConvertToCSV(CCSV& csv)
{
	csv.Add(m_nDeptNo);
	csv.Add(DEPTDB_VERSION);
	csv.Add(m_bReportEnable);
	csv.Add(m_nEposGroup);
	csv.Add(m_nReportGroup);
	csv.Add(m_strRepText);
	csv.Add(m_strEposText);
	csv.Add(m_nTicketsMode);
	csv.Add(m_nTickets);
	csv.Add(m_bDPQty);
	csv.Add(m_bNegative);
	csv.Add(m_bNonAdd);
	csv.Add(m_bIDraughtExport);
	csv.Add(m_nSpecimenLearnPlu);
	csv.Add(m_bStockTemplate);
	csv.Add(m_bStockDescription);
	csv.Add(m_bStockCategory);
	csv.Add(m_bStockQty);
	csv.Add(m_bSportsBooker);
	csv.Add(m_strImageFilename);
	csv.Add(m_nLoyaltyBonusType);
	csv.Add(m_dLoyaltyBonusTrip, 2);
	csv.Add(m_nLoyaltyBonusPoints);
	csv.Add(m_dLoyaltyBonusValue, 2);
	csv.Add(m_nLoyaltyBonusPercent);
	csv.Add(m_bLoyaltyGeneralSpendBonus);
}

/**********************************************************************/
/*	convert from csv												*/
/*																	*/
/*	For future compatibility, if count not stored in file:			*/
/*																	*/
/*	1) Cannot use defines											*/
/*	2) Cannot make arrays smaller than previous versions			*/
/*																	*/
/**********************************************************************/

void CDepartmentCSVRecord::ConvertFromCSV(CCSV& csv)
{
	ClearRecord();

	SetDeptNo(csv.GetInt(0));
	switch (csv.GetInt(1))			/* version number */
	{
	case 1:	V1ConvertFromCSV(csv);			break;
	case 2:	V2To7ConvertFromCSV(csv, 2);	break;
	case 3:	V2To7ConvertFromCSV(csv, 3);	break;
	case 4:	V2To7ConvertFromCSV(csv, 4);	break;
	case 5:	V2To7ConvertFromCSV(csv, 5);	break;
	case 6:	V2To7ConvertFromCSV(csv, 6);	break;
	case 7:	V2To7ConvertFromCSV(csv, 7);	break;
	}
}

/**********************************************************************/

void CDepartmentCSVRecord::V1ConvertFromCSV(CCSV& csv)
{
	SetReportEnable(csv.GetBool(2));
	SetEposGroup(csv.GetInt(3));
	SetRepText(csv.GetString(4));
	SetEposText(csv.GetString(5));
}
	
/**********************************************************************/

void CDepartmentCSVRecord::V2To7ConvertFromCSV(CCSV& csv, int nVer)
{
	SetReportEnable(csv.GetBool(2));
	SetEposGroup(csv.GetInt(3));

	int nOffset = 4;

	if (nVer >= 3)
	{
		SetReportGroup(csv.GetInt(nOffset++));
	}

	SetRepText(csv.GetString(nOffset++));
	SetEposText(csv.GetString(nOffset++));

	if (nVer >= 4)
	{
		SetTicketsMode(csv.GetInt(nOffset++));
	}

	SetTickets(csv.GetInt(nOffset++));
	SetDPQtyFlag(csv.GetBool(nOffset++));
	SetNegativeFlag(csv.GetBool(nOffset++));
	SetNonAddFlag(csv.GetBool(nOffset++));
	SetIDraughtExportFlag(csv.GetBool(nOffset++));
	SetSpecimenLearnPlu(csv.GetInt64(nOffset++));
	SetStockTemplateFlag(csv.GetBool(nOffset++));
	SetStockDescriptionFlag(csv.GetBool(nOffset++));
	SetStockCategoryFlag(csv.GetBool(nOffset++));
	SetStockQtyFlag(csv.GetBool(nOffset++));
	SetSportsBookerFlag(csv.GetBool(nOffset++));

	if (nVer >= 5)
	{
		SetImageFilename(csv.GetString(nOffset++));
	}

	if (nVer >= 6)
	{
		SetLoyaltyBonusType(csv.GetInt(nOffset++));
		SetLoyaltyBonusTrip(csv.GetDouble(nOffset++));
		SetLoyaltyBonusPoints(csv.GetInt(nOffset++));
		SetLoyaltyBonusValue(csv.GetDouble(nOffset++));
		SetLoyaltyBonusPercent(csv.GetInt(nOffset++));
	}

	if (nVer >= 7)
	{
		SetLoyaltyGeneralSpendBonus(csv.GetBool(nOffset++));
	}
}
	
/**********************************************************************/

CString CDepartmentCSVRecord::StripSpaces(const char* szText)
{
	CString strText = szText;
	::TrimSpaces(strText, FALSE);
	return strText;
}

/**********************************************************************/

const char* CDepartmentCSVRecord::GetDefaultText()
{
	m_strDefaultText.Format("Department %d", m_nDeptNo);
	return m_strDefaultText;
}

/**********************************************************************/

const char* CDepartmentCSVRecord::GetReportText()
{
	m_strReportText = m_strRepText;
	::TrimSpaces(m_strReportText, FALSE);

	if (m_strReportText == "")
	{
		m_strReportText = m_strEposText;
	}

	return m_strReportText;
}

/**********************************************************************/
/*	Set Routines											*/
/**********************************************************************/

void CDepartmentCSVRecord::SetDeptNo(int nDeptNo)
{
	Set(m_nDeptNo, nDeptNo, Dept::DeptNo);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetReportEnable(bool bEnable)
{
	Set(m_bReportEnable, bEnable);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetEposGroup(int n)
{
	Set(m_nEposGroup, n, Dept::EposGroup);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetReportGroup(int n)
{
	Set(m_nReportGroup, n, Dept::ReportGroup);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetRepText(const char* szRepText)
{
	Set(m_strRepText, StripSpaces(szRepText), Dept::RepText);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetEposText(const char* szEposText)
{
	Set(m_strEposText, FilterEposText(szEposText), Dept::EposText);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetTicketsMode(int n)
{
	Set(m_nTicketsMode, n, Dept::TicketsMode);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetTickets(int n)
{
	Set(m_nTickets, n, Dept::Tickets);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetDPQtyFlag(bool b)
{
	Set(m_bDPQty, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetNegativeFlag(bool b)
{
	Set(m_bNegative, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetNonAddFlag(bool b)
{
	Set(m_bNonAdd, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetIDraughtExportFlag(bool b)
{
	Set(m_bIDraughtExport, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetSpecimenLearnPlu(__int64 n)
{
	Set(m_nSpecimenLearnPlu, n, Dept::SpecimenLearnPlu);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetStockTemplateFlag(bool b)
{
	Set(m_bStockTemplate, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetStockDescriptionFlag(bool b)
{
	Set(m_bStockDescription, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetStockCategoryFlag(bool b)
{
	Set(m_bStockCategory, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetStockQtyFlag(bool b)
{
	Set(m_bStockQty, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetSportsBookerFlag(bool b)
{
	Set(m_bSportsBooker, b);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetImageFilename(const char* sz)
{
	Set(m_strImageFilename, StripSpaces(sz), Dept::ImageFilename);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetLoyaltyBonusType(int n)
{
	Set(m_nLoyaltyBonusType, n, Dept::LoyaltyBonusType);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetLoyaltyBonusTrip(double d)
{
	Set(m_dLoyaltyBonusTrip, d, Dept::LoyaltyBonusTrip);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetLoyaltyBonusPoints(int n)
{
	Set(m_nLoyaltyBonusPoints, n, Dept::LoyaltyBonusPoints);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetLoyaltyBonusValue(double d)
{
	Set(m_dLoyaltyBonusValue, d, Dept::LoyaltyBonusValue);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetLoyaltyBonusPercent(int n)
{
	Set(m_nLoyaltyBonusPercent, n, Dept::LoyaltyBonusPercent);
}

/**********************************************************************/

void CDepartmentCSVRecord::SetLoyaltyGeneralSpendBonus(bool b)
{
	m_bLoyaltyGeneralSpendBonus = b;
}

/**********************************************************************/
