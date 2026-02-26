/**********************************************************************/
#include "TableReportHelpers.h"
/**********************************************************************/
#include "PresetReportPluHist.h"
/**********************************************************************/

CPresetReportInfoPluHist::CPresetReportInfoPluHist()
{
	Reset();	
}

/**********************************************************************/

void CPresetReportInfoPluHist::Reset()
{
	m_strDateType = "";
	m_strDateFrom = "";
	m_strDateTo = "";
	m_nRoundToType = 0;
	m_bSecondDateBlock = FALSE;
	m_nSecondDateBlockAutoType = 0;
	m_strDateType2 = "";
	m_strDateFrom2 = "";
	m_strDateTo2 = "";
	m_nRoundToType2 = 0;
	m_strSessionName = "";
	m_strTimeSliceName = "";
	
	m_nTableFilterType = TABLEFILTER_ALL;
	m_nTableFilterLevel = 1;
	m_nTableFilterEntityNo = 1;
	m_bEposSales = FALSE;
	m_bManualSales = FALSE;
	m_bImportSales = FALSE;

	m_bPluRange = FALSE;
	m_strPluFrom = "";
	m_strPluTo = "";
	m_nDepartmentFilter = 0;
	m_nDepartmentFilterType = 0;
	m_nPluFilterNo = 0;
	m_bSaleTime = FALSE;
	m_nTaxFilter = 0;
	m_nPriceLevelFilter = 0;
	m_nModifierFilter = 0;
	m_nTaxCustomFilter = 0;
	m_nPriceLevelCustomFilter = 0;
	m_nModifierCustomFilter = 0;

	m_strReportName = "";
	m_bByMonth = FALSE;
	m_bDeptSort = FALSE;
	m_bPercentSales = FALSE;
	m_bEstProfit = FALSE;
	m_bCustomer = FALSE;
	m_bVIPSales = FALSE;
	m_bGuest = FALSE;

	m_nTermDateType = 0;
}

/**********************************************************************/

void CPresetReportInfoPluHist::ToString(CString& str)
{
	CCSV csv;

	csv.Add(m_strDateType);
	csv.Add(m_strDateFrom);
	csv.Add(m_strDateTo);
	csv.Add(m_nRoundToType);
	csv.Add(m_bSecondDateBlock);
	csv.Add(m_nSecondDateBlockAutoType);
	csv.Add(m_strDateType2);
	csv.Add(m_strDateFrom2);
	csv.Add(m_strDateTo2);
	csv.Add(m_nRoundToType2);
	csv.Add(m_strSessionName);
	csv.Add(m_strTimeSliceName);

	csv.Add(m_nTableFilterType);
	csv.Add(m_nTableFilterLevel);
	csv.Add(m_nTableFilterEntityNo);
	csv.Add(m_bEposSales);
	csv.Add(m_bManualSales);
	csv.Add(m_bImportSales);

	csv.Add(m_bPluRange);
	csv.Add(m_strPluFrom);
	csv.Add(m_strPluTo);
	csv.Add(m_nDepartmentFilter);
	csv.Add(m_nDepartmentFilterType);
	csv.Add(m_nPluFilterNo);
	csv.Add(m_bSaleTime);
	csv.Add(m_nTaxFilter);
	csv.Add(m_nPriceLevelFilter);
	csv.Add(m_nModifierFilter);
	csv.Add(m_nTaxCustomFilter);
	csv.Add(m_nPriceLevelCustomFilter);
	csv.Add(m_nModifierCustomFilter);

	csv.Add(m_strReportName);
	csv.Add(m_bByMonth);
	csv.Add(m_bDeptSort);
	csv.Add(m_bPercentSales);
	csv.Add(m_bEstProfit);
	csv.Add(m_bCustomer);
	csv.Add(m_bVIPSales);
	csv.Add(m_bGuest);

	csv.Add(m_nTermDateType);

	str = csv.GetLine();
}

/**********************************************************************/

void CPresetReportInfoPluHist::FromString( CString& str )
{
	Reset();
	CCSV csv( str );

	int nPos = 0;
	m_strDateType = csv.GetString(nPos++);
	m_strDateFrom = csv.GetString(nPos++);
	m_strDateTo = csv.GetString(nPos++);
	m_nRoundToType = csv.GetInt(nPos++);
	m_bSecondDateBlock = csv.GetBool(nPos++);
	m_nSecondDateBlockAutoType = csv.GetInt(nPos++);
	m_strDateType2 = csv.GetString(nPos++);
	m_strDateFrom2 = csv.GetString(nPos++);
	m_strDateTo2 = csv.GetString(nPos++);
	m_nRoundToType2 = csv.GetInt(nPos++);
	m_strSessionName = csv.GetString(nPos++);
	m_strTimeSliceName = csv.GetString(nPos++);
	
	m_nTableFilterType = csv.GetInt(nPos++);
	m_nTableFilterLevel = csv.GetInt(nPos++);
	m_nTableFilterEntityNo = csv.GetInt(nPos++);
	m_bEposSales = csv.GetBool(nPos++);
	m_bManualSales = csv.GetBool(nPos++);
	m_bImportSales = csv.GetBool(nPos++);

	m_bPluRange = csv.GetBool(nPos++);
	m_strPluFrom = csv.GetString(nPos++);
	m_strPluTo = csv.GetString(nPos++);
	m_nDepartmentFilter = csv.GetInt(nPos++);
	m_nDepartmentFilterType = csv.GetInt(nPos++);
	m_nPluFilterNo = csv.GetInt(nPos++);
	m_bSaleTime = csv.GetBool(nPos++);
	m_nTaxFilter = csv.GetInt(nPos++);
	m_nPriceLevelFilter = csv.GetInt(nPos++);
	m_nModifierFilter = csv.GetInt(nPos++);
	m_nTaxCustomFilter = csv.GetInt(nPos++);
	m_nPriceLevelCustomFilter = csv.GetInt(nPos++);;
	m_nModifierCustomFilter = csv.GetInt(nPos++);

	m_strReportName = csv.GetString(nPos++);
	m_bByMonth = csv.GetBool(nPos++);
	m_bDeptSort = csv.GetBool(nPos++);
	m_bPercentSales = csv.GetBool(nPos++);
	m_bEstProfit = csv.GetBool(nPos++);
	m_bCustomer = csv.GetBool(nPos++);
	m_bVIPSales = csv.GetBool(nPos++);
	m_bGuest = csv.GetBool(nPos++);

	m_nTermDateType = csv.GetInt(nPos++);
}

/**********************************************************************/

void CPresetReportInfoPluHist::ConvertFromCSV(CCSV& csv, CPresetReportInfo& infoBase)
{
	Reset();
	infoBase.Reset();
	infoBase.SetName(csv.GetString(0));

	switch (csv.GetInt(1))
	{
	case 1:	V1To8ConvertFromCSV(csv, infoBase, 1);	break;
	case 2:	V1To8ConvertFromCSV(csv, infoBase, 2);	break;
	case 3:	V1To8ConvertFromCSV(csv, infoBase, 3);	break;
	case 4:	V1To8ConvertFromCSV(csv, infoBase, 4);	break;
	case 5:	V1To8ConvertFromCSV(csv, infoBase, 5);	break;
	case 6:	V1To8ConvertFromCSV(csv, infoBase, 6);	break;
	case 7:	V1To8ConvertFromCSV(csv, infoBase, 7);	break;
	case 8:	V1To8ConvertFromCSV(csv, infoBase, 8);	break;
	}
}

/**********************************************************************/

void CPresetReportInfoPluHist::V1To8ConvertFromCSV(CCSV& csv, CPresetReportInfo& infoBase, int nVer)
{
	int nPos = 2;

	if (nVer >= 3)
	{
		infoBase.SetEntityNo(csv.GetInt(nPos++));
	}

	SetDateType(csv.GetString(nPos++));
	SetDateFrom(csv.GetString(nPos++));
	SetDateTo(csv.GetString(nPos++));
	SetRoundToType(csv.GetInt(nPos++));

	if (nVer >= 6)
	{
		SetSecondDateBlockFlag(csv.GetBool(nPos++));
		SetSecondDateBlockAutoType(csv.GetInt(nPos++));
		SetDateType2(csv.GetString(nPos++));
		SetDateFrom2(csv.GetString(nPos++));
		SetDateTo2(csv.GetString(nPos++));
		SetRoundToType2(csv.GetInt(nPos++));
	}

	SetSessionName(csv.GetString(nPos++));

	if (nVer >= 2)
	{
		SetTimeSliceName(csv.GetString(nPos++));
	}

	if (nVer >= 4)
	{
		infoBase.SetPresetConLevel(csv.GetInt(nPos++));
		infoBase.SetPresetEntityNo1(csv.GetInt(nPos++));
		infoBase.SetPresetEntityNo2(csv.GetInt(nPos++));
		infoBase.SetTerminalIdx(csv.GetInt(nPos++));
		infoBase.SetPasswordEnableFlag(csv.GetBool(nPos++));
		infoBase.SetPasswordConLevel(csv.GetInt(nPos++));
		infoBase.SetPasswordEntityNo1(csv.GetInt(nPos++));
		infoBase.SetPasswordEntityNo2(csv.GetInt(nPos++));
	}
	else
	{
		infoBase.SetPresetConLevel(csv.GetInt(nPos++));
		int nLegacyIdx = csv.GetInt(nPos++);
		infoBase.SetTerminalIdx(csv.GetInt(nPos++));

		int nEntityNo1, nEntityNo2;
		CPresetReportArray::GetLegacyConLevelInfo(infoBase.GetPresetConLevel(), nLegacyIdx, nEntityNo1, nEntityNo2);
		infoBase.SetPasswordEntityNo1(nEntityNo1);
		infoBase.SetPasswordEntityNo2(nEntityNo2);
	}

	if (nVer >= 7)
	{
		SetTableFilterType(csv.GetInt(nPos++));
		SetTableFilterLevel(csv.GetInt(nPos++));
		SetTableFilterEntityNo(csv.GetInt(nPos++));
	}

	SetEposSalesFlag(csv.GetBool(nPos++));
	SetManualSalesFlag(csv.GetBool(nPos++));
	SetImportSalesFlag(csv.GetBool(nPos++));
	SetPluRangeFlag(csv.GetBool(nPos++));
	SetPluFrom(csv.GetString(nPos++));
	SetPluTo(csv.GetString(nPos++));
	SetDepartmentFilter(csv.GetInt(nPos++));
	SetDepartmentFilterType(csv.GetInt(nPos++));
	SetTaxFilter(csv.GetInt(nPos++));
	SetReportName(csv.GetString(nPos++));
	SetByMonthFlag(csv.GetBool(nPos++));
	SetDeptSortFlag(csv.GetBool(nPos++));
	SetPercentSalesFlag(csv.GetBool(nPos++));
	SetEstProfitFlag(csv.GetBool(nPos++));

	if (nVer >= 2)
	{
		SetPriceLevelFilter(csv.GetInt(nPos++));
		SetVIPSalesFlag(csv.GetBool(nPos++));
		SetGuestFlag(csv.GetBool(nPos++));
		SetCustomerFlag(csv.GetBool(nPos++));
		SetPluFilterNo(csv.GetInt(nPos++));
		SetSaleTimeFlag(csv.GetBool(nPos++));
	}

	if (nVer >= 5)
	{
		SetModifierFilter(csv.GetInt(nPos++));
		SetTaxCustomFilter(csv.GetInt(nPos++));
		SetPriceLevelCustomFilter(csv.GetInt(nPos++));
		SetModifierCustomFilter(csv.GetInt(nPos++));
	}

	if (nVer >= 8)
	{
		SetTermDateType(csv.GetInt(nPos++));
	}
}

/**********************************************************************/

void CPresetReportInfoPluHist::ConvertToCSV(CCSV& csv, CPresetReportInfo& infoBase)
{
	csv.Add(infoBase.GetName());
	csv.Add(PRESET_PLUHIST_VERSION);
	csv.Add(infoBase.GetEntityNo());
	csv.Add(GetDateType());
	csv.Add(GetDateFrom());
	csv.Add(GetDateTo());
	csv.Add(GetRoundToType());
	csv.Add(GetSecondDateBlockFlag());
	csv.Add(GetSecondDateBlockAutoType());
	csv.Add(GetDateType2());
	csv.Add(GetDateFrom2());
	csv.Add(GetDateTo2());
	csv.Add(GetRoundToType2());
	csv.Add(GetSessionName());
	csv.Add(GetTimeSliceName());
	csv.Add(infoBase.GetPresetConLevel());
	csv.Add(infoBase.GetPresetEntityNo1());
	csv.Add(infoBase.GetPresetEntityNo2());
	csv.Add(infoBase.GetTerminalIdx());
	csv.Add(infoBase.GetPasswordEnableFlag());
	csv.Add(infoBase.GetPasswordConLevel());
	csv.Add(infoBase.GetPasswordEntityNo1());
	csv.Add(infoBase.GetPasswordEntityNo2());
	csv.Add(GetTableFilterType());
	csv.Add(GetTableFilterLevel());
	csv.Add(GetTableFilterEntityNo());
	csv.Add(GetEposSalesFlag());
	csv.Add(GetManualSalesFlag());
	csv.Add(GetImportSalesFlag());
	csv.Add(GetPluRangeFlag());
	csv.Add(GetPluFrom());
	csv.Add(GetPluTo());
	csv.Add(GetDepartmentFilter());
	csv.Add(GetDepartmentFilterType());
	csv.Add(GetTaxFilter());
	csv.Add(GetReportName());
	csv.Add(GetByMonthFlag());
	csv.Add(GetDeptSortFlag());
	csv.Add(GetPercentSalesFlag());
	csv.Add(GetEstProfitFlag());
	csv.Add(GetPriceLevelFilter());
	csv.Add(GetVIPSalesFlag());
	csv.Add(GetGuestFlag());
	csv.Add(GetCustomerFlag());
	csv.Add(GetPluFilterNo());
	csv.Add(GetSaleTimeFlag());
	csv.Add(GetModifierFilter());
	csv.Add(GetTaxCustomFilter());
	csv.Add(GetPriceLevelCustomFilter());
	csv.Add(GetModifierCustomFilter());
	csv.Add(GetTermDateType());
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CPresetReportArrayPluHist::CPresetReportArrayPluHist(void) : CPresetReportArray()
{
}

/**********************************************************************/

CPresetReportArrayPluHist::~CPresetReportArrayPluHist(void)
{
}

/**********************************************************************/

const char* CPresetReportArrayPluHist::GetFilename()
{
	CFilenameUpdater FnUp ( SysFiles::PresetRepPluHist );
	m_strFilename = FnUp.GetFilenameToUse();
	return m_strFilename;
}

/**********************************************************************/

void CPresetReportArrayPluHist::PrepareForEditingInternal()
{
	m_arrayPresets.RemoveAll();

	for ( int n = 0; n < m_arrayFileStrings.GetSize(); n++ )
	{
		CCSV csv( m_arrayFileStrings.GetAt(n) );

		CPresetReportInfo infoBase;
		CPresetReportInfoPluHist infoPreset;
		infoPreset.ConvertFromCSV( csv, infoBase );
		
		CString strPreset;
		infoPreset.ToString( strPreset );
		infoBase.SetOtherFields( strPreset );
		m_arrayPresets.Add( infoBase );
	}

	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
}

/**********************************************************************/

void CPresetReportArrayPluHist::PrepareForWrite()
{
	m_arrayFileStrings.RemoveAt(0, m_arrayFileStrings.GetSize() );
	
	for ( int n = 0; n < m_arrayPresets.GetSize(); n++ )
	{
		CPresetReportInfo infoBase = m_arrayPresets.GetAt(n);
		
		CPresetReportInfoPluHist infoPreset;
		CString strFields = infoBase.GetOtherFields();
		infoPreset.FromString( strFields  );

		CCSV csv( ',', '"', TRUE, TRUE, TRUE );
		infoPreset.ConvertToCSV( csv, infoBase );
		m_arrayFileStrings.Add( csv.GetLine() );
	}
}

/**********************************************************************/
