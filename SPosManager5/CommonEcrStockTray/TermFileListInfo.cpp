/**********************************************************************/
#include "TermFileListInfo.h"
/**********************************************************************/

CTermFileListInfo::CTermFileListInfo()
{
	Reset();
}

/**********************************************************************/

void CTermFileListInfo::Reset()
{
	m_nSelectArrayIdx = 0;
	m_nCashRSPMode = 0;
	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_nTermIdx = 0;
	m_nRange = 0;
	m_bIsActivePMSFile = FALSE;
	m_strFilename = "";
	m_strDateFolder = "";
	m_strDateTran = "";
	m_strDateSale = "";
}

/**********************************************************************/

bool CTermFileListInfo::IsLegacySalesFolder()
{
	return ( m_strDateFolder.GetLength() != 17 );
}

/**********************************************************************/

int CTermFileListInfo::Compare ( CTermFileListInfo& source, int nHint )
{
	if ( m_nSelectArrayIdx != source.m_nSelectArrayIdx )
		return ( ( m_nSelectArrayIdx > source.m_nSelectArrayIdx ) ? 1 : -1 );

	if ( m_nDbIdx != source.m_nDbIdx )
		return ( ( m_nDbIdx > source.m_nDbIdx ) ? 1 : -1 );

	if ( m_nLocIdx != source.m_nLocIdx )
		return ( ( m_nLocIdx > source.m_nLocIdx ) ? 1 : -1 );

	if ( m_nTermIdx != source.m_nTermIdx )
		return ( ( m_nTermIdx > source.m_nTermIdx ) ? 1 : -1 );

	if ( m_nCashRSPMode != source.m_nCashRSPMode )
		return ( ( m_nCashRSPMode > source.m_nCashRSPMode ) ? 1 : -1 );

	if ( m_nRange != source.m_nRange )
		return ( ( m_nRange > source.m_nRange ) ? 1 : -1 );

	if ( m_bIsActivePMSFile != source.m_bIsActivePMSFile )
		return ( ( TRUE == m_bIsActivePMSFile ) ? 1 : -1 );

	if ( m_strDateTran != source.m_strDateTran )
		return ( ( m_strDateTran > source.m_strDateTran ) ? 1 : -1 );

	if ( m_strDateSale != source.m_strDateSale )
		return ( ( m_strDateSale > source.m_strDateSale ) ? 1 : -1 );

	if ( m_strDateFolder != source.m_strDateFolder )
		return ( ( m_strDateFolder > source.m_strDateFolder ) ? 1 : -1 );

	if ( m_strFilename != source.m_strFilename )
		return ( ( m_strFilename > source.m_strFilename ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

void CTermFileListInfo::Add ( CTermFileListInfo& source )
{
}

/**********************************************************************/
