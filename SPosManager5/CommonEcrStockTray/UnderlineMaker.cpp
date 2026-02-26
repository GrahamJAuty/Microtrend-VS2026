/**********************************************************************/
#include "UnderlineMaker.h"
/**********************************************************************/

CUnderlineMaker::CUnderlineMaker()
{
	Reset();
}

/**********************************************************************/

void CUnderlineMaker::Reset()
{
	m_arraySections.RemoveAll();
	m_nPendingCount = 0;
	m_bTotalsSection = FALSE;
}

/**********************************************************************/
void CUnderlineMaker::AddBlanks( int n ){ AddColumns( FALSE, n ); }
void CUnderlineMaker::AddTotals( int n ){ AddColumns( TRUE, n ); }
/**********************************************************************/

void CUnderlineMaker::AddColumns( bool bTotals, int n )
{
	if ( m_arraySections.GetSize() == 0 )
	{
		m_arraySections.Add( bTotals );
		m_nPendingCount = n;
	}
	else if ( bTotals == m_bTotalsSection )
	{
		m_nPendingCount += n;
	}
	else
	{
		m_arraySections.Add( m_nPendingCount );
		m_nPendingCount = n;
	}

	m_bTotalsSection = bTotals;
}

/**********************************************************************/

const char* CUnderlineMaker::GetLine()
{
	m_strLine = "";
	int nSize = m_arraySections.GetSize();

	if ( nSize != 0 )
	{
		m_arraySections.Add( m_nPendingCount );

		CString strColumn = ( m_arraySections.GetAt(0) == 1 ) ? "<LI>\t" : "\t";

		for ( int n = 1; n < m_arraySections.GetSize(); n++ )
		{
			int nCount = m_arraySections.GetAt(n);
			for ( int x = 0; x < nCount; x++ )
				m_strLine += strColumn;

			strColumn = ( strColumn == "\t" ) ? "<LI>\t" : "\t";
		}

		m_arraySections.RemoveAt( nSize );
	}

	return m_strLine;
}

/**********************************************************************/
