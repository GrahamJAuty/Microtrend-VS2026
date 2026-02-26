/**********************************************************************/
#include "EposReportConsolLoyaltyRecon.h"
/**********************************************************************/

CEposReportConsolLoyaltyRecon::CEposReportConsolLoyaltyRecon()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolLoyaltyRecon::Reset()
{
	m_strDate = "";
	m_strTime = "";
	m_nSequenceNo = 0;
	m_nValue = 0;
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolLoyaltyRecon::Add (CEposReportConsolLoyaltyRecon& source )
{
	m_nValue += source.m_nValue;
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolLoyaltyRecon::Compare(CEposReportConsolLoyaltyRecon& source, int nHint)
{
	if (m_strDate != source.m_strDate)
	{
		return ((m_strDate > source.m_strDate) ? 1 : -1);
	}
	else if (m_strTime != source.m_strTime)
	{
		return ((m_strTime > source.m_strTime) ? 1 : -1);
	}
	if (m_nSequenceNo != source.m_nSequenceNo)
	{
		return ((m_nSequenceNo > source.m_nSequenceNo) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportConsolLoyaltyReconLog::CEposReportConsolLoyaltyReconLog()
{
	Reset();
}

/**********************************************************************/

void CEposReportConsolLoyaltyReconLog::Reset()
{
	m_nDbNo = 0;
	m_nFolderSet = 0;
	m_nTerminalNo = 0;
	m_nSequenceNo = 0;
	m_strDate = "";
	m_strTime = "";
	BaseReset();
}

/**********************************************************************/

void CEposReportConsolLoyaltyReconLog::Add(CEposReportConsolLoyaltyReconLog& source)
{
	BaseAdd(source);
}

/**********************************************************************/

int CEposReportConsolLoyaltyReconLog::Compare(CEposReportConsolLoyaltyReconLog& source, int nHint)
{
	if (m_nDbNo != source.m_nDbNo)
	{
		return ((m_nDbNo > source.m_nDbNo) ? 1 : -1);
	}
	else if (m_nFolderSet != source.m_nFolderSet)
	{
		return ((m_nFolderSet > source.m_nFolderSet) ? 1 : -1);
	}
	else if (m_nTerminalNo != source.m_nTerminalNo)
	{
		return ((m_nTerminalNo > source.m_nTerminalNo) ? 1 : -1);
	}
	else if (m_nSequenceNo != source.m_nSequenceNo)
	{
		return ((m_nSequenceNo > source.m_nSequenceNo) ? 1 : -1);
	}
	else
	{
		return 0;
	}
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CEposReportLoyaltyReconBlock::CEposReportLoyaltyReconBlock()
{
	m_pSales = NULL;
}

/**********************************************************************/

bool CEposReportLoyaltyReconBlock::GotData()
{
	if (m_pSales->GetSize() != 0)
	{
		return TRUE;
	}

	return FALSE;
}

/**********************************************************************/

