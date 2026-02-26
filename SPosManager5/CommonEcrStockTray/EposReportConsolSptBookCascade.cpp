/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
/**********************************************************************/
#include "EposReportConsolSptBookCascade.h"
/**********************************************************************/

CEposReportConsolSptBookCascade::CEposReportConsolSptBookCascade()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolSptBookCascade::Reset()
{
	m_nYear = 2015;
	m_nMonth = 0;
	m_nTaxBand = 0;
	m_nCategoryNo = 0;
	m_nDeptNo = 0;
	m_nTaxDateCode = 0;
	m_dValue = 0.0;
	m_dTax = 0.0;
}

/**********************************************************************/

void CEposReportConsolSptBookCascade::Add (CEposReportConsolSptBookCascade& source )
{
	m_dValue += source.m_dValue;
	m_dTax += source.m_dTax;
}

/**********************************************************************/

int CEposReportConsolSptBookCascade::Compare( CEposReportConsolSptBookCascade& source, int nHint )
{
	if ( m_nYear != source.m_nYear )
		return ( ( m_nYear > source.m_nYear ) ? 1 : -1 );

	if ( m_nMonth != source.m_nMonth )
		return ( ( m_nMonth > source.m_nMonth ) ? 1 : -1 );

	if ( m_nTaxBand != source.m_nTaxBand )
		return ( ( m_nTaxBand > source.m_nTaxBand ) ? 1 : -1 );

	if ( m_nCategoryNo != source.m_nCategoryNo )
		return ( ( m_nCategoryNo > source.m_nCategoryNo ) ? 1 : -1 );

	if ( m_nDeptNo != source.m_nDeptNo )
		return ( ( m_nDeptNo > source.m_nDeptNo ) ? 1 : -1 );

	if ( m_nTaxDateCode != source.m_nTaxDateCode )
		return ( ( m_nTaxDateCode > source.m_nTaxDateCode ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

int CEposReportConsolSptBookCascade::GetDifferenceLevel( CEposReportConsolSptBookCascade& source )
{	
	if ( m_nYear != source.m_nYear )				return 5;
	if ( m_nMonth != source.m_nMonth )				return 4;
	if ( m_nTaxBand != source.m_nTaxBand )			return 3;
	if ( m_nCategoryNo != source.m_nCategoryNo )	return 2;
	if ( m_nDeptNo != source.m_nDeptNo )			return 1;
	return 0;
}

/**********************************************************************/

const char* CEposReportConsolSptBookCascade::GetYearText( bool bTotal )
{
	if ( FALSE == bTotal )
		m_strYear.Format( "%d", m_nYear );
	else
		m_strYear.Format( "<..>%d Total", m_nYear );

	return m_strYear;
}

/**********************************************************************/

const char* CEposReportConsolSptBookCascade::GetMonthText( bool bTotal )
{
	switch( m_nMonth )
	{
	case 1:		m_strMonth = "January";		break;
	case 2:		m_strMonth = "February";	break;
	case 3:		m_strMonth = "March";		break;
	case 4:		m_strMonth = "April";		break;
	case 5:		m_strMonth = "May";			break;
	case 6:		m_strMonth = "June";		break;
	case 7:		m_strMonth = "July";		break;
	case 8:		m_strMonth = "August";		break;
	case 9:		m_strMonth = "September";	break;
	case 10:	m_strMonth = "October";		break;
	case 11:	m_strMonth = "November";	break;
	case 12:	m_strMonth = "December";	break;
	default:	m_strMonth = "Unknown";		break;
	}

	if ( TRUE == bTotal )
	{
		CString strTemp = "";
		strTemp += "<..>";
		strTemp += m_strMonth;
		strTemp += " Total";
		m_strMonth = strTemp;
	}

	return m_strMonth;
}

/**********************************************************************/

const char* CEposReportConsolSptBookCascade::GetTaxBandText( bool bTotal )
{
	CTaxRateInfo infoTaxRate;
	DataManager.TaxRates.GetTaxRateInfo( m_nTaxBand, infoTaxRate );
	m_strTaxBand = infoTaxRate.m_strReportText;

	if ( TRUE == bTotal )
	{
		CString strTemp = "";
		strTemp += "<..>";
		strTemp += m_strTaxBand;
		strTemp += " Total";
		m_strTaxBand = strTemp;
	}

	return m_strTaxBand;
}

/**********************************************************************/

const char* CEposReportConsolSptBookCascade::GetCategoryText( bool bTotal )
{
	m_strCategory = DataManager.AnalysisCategory.GetDisplayText( m_nCategoryNo );

	if ( TRUE == bTotal )
	{
		CString strTemp = "";
		strTemp += "<..>";
		strTemp += m_strCategory;
		strTemp += " Total";
		m_strCategory = strTemp;
	}

	return m_strCategory;
}

/**********************************************************************/

const char* CEposReportConsolSptBookCascade::GetDepartmentText()
{
	int nDeptIdx;
	if ( DataManager.Department.FindDeptByNumber ( m_nDeptNo, nDeptIdx  ) == TRUE )
	{
		CDepartmentCSVRecord Department;
		DataManager.Department.GetAt ( nDeptIdx, Department );
		m_strDepartment = Department.GetReportText();
	}
	else
		m_strDepartment.Format( "Unknown (%d)", m_nDeptNo );

	return m_strDepartment;
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

bool CEposReportSptBookCascadeBlock::GotData()
{
	if ( m_pSales -> GetSize() != 0 )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

