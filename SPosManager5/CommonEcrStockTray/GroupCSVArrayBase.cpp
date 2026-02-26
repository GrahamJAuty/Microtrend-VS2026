 /**********************************************************************/
#include "GroupCSVArrayBase.h"
/**********************************************************************/

CBaseGroupInfo::CBaseGroupInfo()
{
	m_nGroupNo = 0;
	m_strReportText = "";
	m_strDefText = "";
}

/**********************************************************************/

CGroupCSVArrayBase::CGroupCSVArrayBase() : CSharedCSVArray ( ',', '"', FALSE, TRUE, TRUE )
{
	m_strPrefix = "G";
}

/**********************************************************************/

bool CGroupCSVArrayBase::FindGroupByNumber ( int nGroupNo, int& nIndex )
{
	int nStart = 0;
	int nEnd = GetSize() - 1;

	while (nStart <= nEnd)
	{
		nIndex = (nStart + nEnd) / 2;

		CBaseGroupInfo infoGroup;
		GetGroupInfo(nIndex, infoGroup);

		if (infoGroup.m_nGroupNo < nGroupNo)
		{
			nStart = nIndex + 1;
		}

		if (infoGroup.m_nGroupNo > nGroupNo)
		{
			nEnd = nIndex - 1;
		}

		if (infoGroup.m_nGroupNo == nGroupNo)
		{
			return TRUE;
		}
	}

	nIndex = nStart;
	return FALSE;
}

/**********************************************************************/

const char* CGroupCSVArrayBase::GetGroupDisplayText( int nGroupNo )
{
	m_strDisplayText.Format( "%s%2.2d: %s",
		(const char*) m_strPrefix,
		nGroupNo,
		GetGroupReportText( nGroupNo ) );

	return m_strDisplayText;
}
	
/**********************************************************************/

const char* CGroupCSVArrayBase::GetGroupReportText( int nGroupNo )
{
	int nGroupIdx;
	if ( FindGroupByNumber( nGroupNo, nGroupIdx ) == TRUE )
	{
		CBaseGroupInfo infoGroup;
		GetGroupInfo( nGroupIdx, infoGroup );
		m_strReportText = infoGroup.m_strReportText;
	}
	else
	{
		m_strReportText = "Unknown";
	}

	return m_strReportText;
}
	
/**********************************************************************/

bool CGroupCSVArrayBase::FindNextFreeGroupNumber(int& nInsertPos, int& nGroupNo)
{
	bool bResult = TRUE;
	nGroupNo = CanEditGroupZero() ? 0 : 1;
	nInsertPos = 0;

	if (GetSize() != 0)
	{
		if (FALSE == CanEditGroupZero())
		{
			CBaseGroupInfo GroupInfo;
			GetGroupInfo(0, GroupInfo);

			if (0 == GroupInfo.m_nGroupNo)
			{
				nInsertPos = 1;
			}
		}

		int nLastGroupNo = nGroupNo - 1;
		recInt GroupRange = GetGroupNoRange();
		bool bGotResult = FALSE;

		while ((nInsertPos < GetSize()) && (nGroupNo <= GroupRange.Max))
		{
			CBaseGroupInfo GroupInfo;
			GetGroupInfo(nInsertPos, GroupInfo);

			if (GroupInfo.m_nGroupNo <= nLastGroupNo)
			{
				bGotResult = TRUE;
				bResult = FALSE;
				break;
			}

			nLastGroupNo = GroupInfo.m_nGroupNo;

			if (nGroupNo < GroupInfo.m_nGroupNo)
			{
				bGotResult = TRUE;
				bResult = TRUE;
				break;
			}

			nInsertPos++;
			nGroupNo++;
		}

		if (FALSE == bGotResult)
		{
			bResult = (nGroupNo <= GroupRange.Max);
		}
	}

	return bResult;
}

/**********************************************************************/
