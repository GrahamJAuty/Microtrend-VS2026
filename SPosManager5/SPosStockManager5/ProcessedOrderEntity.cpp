/**********************************************************************/
 
/**********************************************************************/
#include "ProcessedOrderEntity.h"
/**********************************************************************/

CProcessedOrderEntity::CProcessedOrderEntity()
{
	Reset();
}

/**********************************************************************/

void CProcessedOrderEntity::Reset()
{
	m_nOrderNo = 0;
	m_strDate = "";
	m_strTime = "";
	m_nSearchType = 1;
	m_strDetail = "";
}

/**********************************************************************/

void CProcessedOrderEntity::Add (CProcessedOrderEntity& source )
{
}

/**********************************************************************/

int CProcessedOrderEntity::Compare( CProcessedOrderEntity& source, int nHint )
{
	if ( m_nOrderNo != source.m_nOrderNo )
		return ( ( m_nOrderNo > source.m_nOrderNo ) ? 1 : -1 );

	if ( m_strDate != source.m_strDate )
		return ( ( m_strDate > source.m_strDate ) ? 1 : -1 );

	if ( m_strTime != source.m_strTime )
		return ( ( m_strTime > source.m_strTime ) ? 1 : -1 );

	if ( m_nSearchType != source.m_nSearchType )
		return ( ( m_nSearchType > source.m_nSearchType ) ? 1 : -1 );
	else
		return 0;
}

/**********************************************************************/

const char* CProcessedOrderEntity::GetComboText()
{
	if ( 0 == m_nOrderNo )
		m_strComboText = "All";
	else
	{
		m_strComboText.Format( "%6.6d  %s/%s/%s at %s:%s",
			m_nOrderNo,
			(const char*) m_strDate.Right(2),
			(const char*) m_strDate.Mid(4,2),
			(const char*) m_strDate.Left(4),
			(const char*) m_strTime.Left(2),
			(const char*) m_strTime.Mid(2,2) );
	}
		
	return m_strComboText;
}

/**********************************************************************/
