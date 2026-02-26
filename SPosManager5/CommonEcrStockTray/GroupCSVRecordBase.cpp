/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "GroupCSVArrayBase.h"
/**********************************************************************/

CGroupCSVRecordBase::CGroupCSVRecordBase()
{
}

/**********************************************************************/

CString CGroupCSVRecordBase::StripSpaces ( const char* szText )
{
	CString strText = szText;
	::TrimSpaces ( strText, FALSE );
	return strText;
}

/**********************************************************************/

void CGroupCSVRecordBase::SetGroupNo ( int nGroupNo )
{
	Set ( m_nGroupNo, nGroupNo, GetGroupNoRange() );
}

/**********************************************************************/

const char* CGroupCSVRecordBase::GetDefaultText()
{
	m_strDefaultText.Format ( "Group %d", GetGroupNo() );
	return m_strDefaultText;
}

/**********************************************************************/

const char* CGroupCSVRecordBase::GetReportText()
{
	m_strReportText = GetRepText();
	::TrimSpaces ( m_strReportText, FALSE );
	return m_strReportText;
}

/**********************************************************************/
