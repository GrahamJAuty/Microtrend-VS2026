/**********************************************************************/

#include "math.h"

/**********************************************************************/
/*														*/
/*	CSubTab												*/
/*														*/
/**********************************************************************/

CSubTab::CSubTab ( const char* szText, int nAlign, double dPercentWidth )
	: m_strText(szText), m_nAlign(nAlign), m_dPercentWidth(dPercentWidth)
{
}

/**********************************************************************/
/*														*/
/*	CSubTabArray											*/
/*														*/
/**********************************************************************/

CSubTabArray::~CSubTabArray()
{
	RemoveAll();
}

void CSubTabArray::Add ( const char* szText, int nAlign, double dPercentWidth )
{
	CSubTab* pSubTab = new CSubTab ( szText, nAlign, dPercentWidth );

	if ( pSubTab != NULL )
		CObArray::Add ( pSubTab );
}

void CSubTabArray::RemoveAll()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
		delete GetAt(i);

	CObArray::RemoveAll();
}

/**********************************************************************/
/*														*/
/*	CTabStop												*/
/*														*/
/**********************************************************************/

CTabStop::CTabStop ( const char* szText, int nAlign, int nWidth, int nOrder )
	: m_strText(szText), m_nAlign(nAlign), m_nWidth(nWidth), m_nOrder(nOrder)
{
}

/**********************************************************************/

CTabStop::~CTabStop()
{
	m_SubTabArray.RemoveAll();
}

/**********************************************************************/

void CTabStop::AddSubTab ( const char* szText, int nAlign, double dPercentWidth )
{
	m_SubTabArray.Add ( szText, nAlign, dPercentWidth );
}

void CTabStop::SetWidth ( int nWidth )
{
	m_nWidth = nWidth;
}

void CTabStop::SetAlign ( int nAlign )
{
	m_nAlign = nAlign;
}

void CTabStop::SetOrder ( int nOrder )
{
	m_nOrder = nOrder;
}

/**********************************************************************/

int CTabStop::GetSubTabSize()
{
	return m_SubTabArray.GetSize();
}

/**********************************************************************/

const char* CTabStop::GetSubTabText ( int nIndex )
{
	if ( nIndex < m_SubTabArray.GetSize() )
		return m_SubTabArray.GetAt(nIndex)->GetText();

	return "";
}

/**********************************************************************/

int CTabStop::GetSubTabAlign ( int nIndex )
{
	if ( nIndex < m_SubTabArray.GetSize() )
		return m_SubTabArray.GetAt(nIndex)->GetAlign();

	return TA_LEFT;
}

/**********************************************************************/

double CTabStop::GetSubTabPercentWidth ( int nIndex )
{
	if ( nIndex < m_SubTabArray.GetSize() )
		return m_SubTabArray.GetAt(nIndex)->GetPercentWidth();

	return 0.0;
}

/**********************************************************************/

int CTabStop::GetSubTabWidth ( int nIndex )
{
	if ( nIndex < m_SubTabArray.GetSize() )
	{
		double dPercentWidth = m_SubTabArray.GetAt(nIndex)->GetPercentWidth();
		double dDoubleWidth = ( ((double)m_nWidth) * dPercentWidth ) / 100.0;
		return int(floor(dDoubleWidth));
	}

	return 0;
}

/**********************************************************************/
/*														*/
/*	CTabStopArray											*/
/*														*/
/**********************************************************************/

CTabStopArray::~CTabStopArray()
{
	RemoveAll();
}

void CTabStopArray::Add ( const char* szText, int nAlign, int nWidth, int nOrder )
{
	CTabStop* pTabStop = new CTabStop ( szText, nAlign, nWidth, nOrder );

	if ( pTabStop != NULL )
		CObArray::Add ( pTabStop );
}

void CTabStopArray::RemoveAll()
{
	for ( int i = 0 ; i < GetSize() ; i++ )
		delete GetAt(i);

	CObArray::RemoveAll();
}

void CTabStopArray::AddSubTab ( int nIndex, const char* szText, int nAlign, double dPercentWidth )
{
	GetAt(nIndex)->AddSubTab ( szText, nAlign, dPercentWidth );
}

const char* CTabStopArray::GetText ( int nIndex )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetText();

	return "";
}

int CTabStopArray::GetAlign ( int nIndex )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetAlign();

	return TA_LEFT;
}

int CTabStopArray::GetWidth ( int nIndex )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetWidth();

	return 0;
}

int CTabStopArray::GetOrder ( int nIndex )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetOrder();

	return nIndex;
}

void CTabStopArray::SetWidth ( int nIndex, int nWidth )
{
	if ( nIndex < GetSize() )
		GetAt(nIndex)->SetWidth ( nWidth );
}

void CTabStopArray::SetAlign ( int nIndex, int nAlign )
{
	if ( nIndex < GetSize() )
		GetAt(nIndex)->SetAlign ( nAlign );
}

void CTabStopArray::SetOrder ( int nIndex, int nOrder )
{
	if ( nIndex < GetSize() )
		GetAt(nIndex)->SetOrder ( nOrder );
}

int CTabStopArray::GetSubTabSize ( int nIndex )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetSubTabSize();

	return 0;
}

const char* CTabStopArray::GetSubTabText ( int nIndex, int n )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetSubTabText ( n );

	return "";
}

int CTabStopArray::GetSubTabAlign ( int nIndex, int n )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetSubTabAlign ( n );

	return TA_LEFT;
}

int CTabStopArray::GetSubTabWidth ( int nIndex, int n )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetSubTabWidth ( n );

	return 0;
}

double CTabStopArray::GetSubTabPercentWidth ( int nIndex, int n )
{
	if ( nIndex < GetSize() )
		return GetAt(nIndex)->GetSubTabPercentWidth ( n );

	return 0.0;
}

/**********************************************************************/
/*														*/
/*	CTabRuler												*/
/*														*/
/**********************************************************************/

CTabRuler::CTabRuler()
	: m_nTotalWidth(0), m_nOrder(0), m_bOrdered(FALSE)
{
}

/**********************************************************************/

CTabRuler::~CTabRuler()
{
	RemoveAll();
}

/**********************************************************************/

void CTabRuler::RemoveAll()
{
	m_TabStopArray.RemoveAll();
	m_OrderedArray.RemoveAll();

	m_nTotalWidth = 0;
	m_nOrder = 0;
	m_bOrdered = FALSE;
}

/**********************************************************************/

CTabRuler& CTabRuler::operator= ( CTabRuler& r )
{
	RemoveAll();

	for ( int i = 0 ; i < r.GetSize() ; i++ )
	{
		AddColumn ( r.GetText(i), r.GetAlign(i), r.GetWidth(i), r.GetOrder(i) );

		for ( int j = 0 ; j < r.GetSubTabSize(i) ; j++ )
			AddSubColumn ( i, r.GetSubTabText(i,j), r.GetSubTabAlign(i,j), r.GetSubTabPercentWidth(i,j) );
	}

	m_nTotalWidth = r.m_nTotalWidth;
	m_nOrder = r.m_nOrder;
	m_bOrdered = FALSE;

	return *this;
}

/**********************************************************************/

void CTabRuler::AddColumn ( const char* szText, int nAlign, int nWidth )
{
	m_TabStopArray.Add ( szText, nAlign, nWidth, m_nOrder++ );

	m_nTotalWidth += nWidth;

	m_bOrdered = FALSE;			// need to create ordered array
}

/**********************************************************************/

void CTabRuler::AddColumn ( const char* szText, int nAlign, int nWidth, int nOrder )
{
	m_TabStopArray.Add ( szText, nAlign, nWidth, nOrder );

	m_nTotalWidth += nWidth;

	m_bOrdered = FALSE;			// need to create ordered array
}

/**********************************************************************/

void CTabRuler::AddSubColumn ( int nIndex, const char* szText, int nAlign, double dPercentWidth )
{
	m_TabStopArray.AddSubTab ( nIndex, szText, nAlign, dPercentWidth );

	m_bOrdered = FALSE;			// need to create ordered array
}

/**********************************************************************/

int CTabRuler::GetSubTabSize ( int nIndex )
{
	return m_TabStopArray.GetSubTabSize ( nIndex );
}

const char* CTabRuler::GetSubTabText ( int nIndex, int n )
{
	return m_TabStopArray.GetSubTabText ( nIndex, n );
}

int CTabRuler::GetSubTabAlign ( int nIndex, int n )
{
	return m_TabStopArray.GetSubTabAlign ( nIndex, n );
}

int CTabRuler::GetSubTabWidth ( int nIndex, int n )
{
	return m_TabStopArray.GetSubTabWidth ( nIndex, n );
}

double CTabRuler::GetSubTabPercentWidth ( int nIndex, int n )
{
	return m_TabStopArray.GetSubTabPercentWidth ( nIndex, n );
}

/**********************************************************************/

void CTabRuler::CreateOrderedArray()
{
	if ( m_bOrdered == FALSE )
	{
		m_OrderedArray.RemoveAll();

		for ( int i = 0 ; i < m_TabStopArray.GetSize() ; i++ )
		{
			int nOrder = m_TabStopArray.GetOrder(i);

			if ( m_TabStopArray.GetWidth(nOrder) > 0 )	// columns of width 0 are not included in the ordered array
			{
				int nIndex = 0;

				for ( int n = 0 ; n < nOrder ; n++ )
				{
					if ( m_TabStopArray.GetSubTabSize(n) > 0 )
						nIndex += m_TabStopArray.GetSubTabSize(n);
					else
						nIndex++;
				}

				if ( m_TabStopArray.GetSubTabSize(nOrder) > 0 )
				{
					for ( int j = 0 ; j < m_TabStopArray.GetSubTabSize(nOrder) ; j++ )
						m_OrderedArray.Add ( m_TabStopArray.GetSubTabText(nOrder,j), m_TabStopArray.GetSubTabAlign(nOrder,j), m_TabStopArray.GetSubTabWidth(nOrder,j), nIndex+j );
				}
				else
					m_OrderedArray.Add ( "", m_TabStopArray.GetAlign(nOrder), m_TabStopArray.GetWidth(nOrder), nIndex );
			}
		}

		m_bOrdered = TRUE;
	}
}

/**********************************************************************/

int CTabRuler::GetOrderedSize()
{
	CreateOrderedArray();			// create ordered array if required

	return m_OrderedArray.GetSize();
}

int CTabRuler::GetOrderedWidth ( int nIndex )
{
	CreateOrderedArray();			// create ordered array if required

	return m_OrderedArray.GetWidth(nIndex);
}

int CTabRuler::GetOrderedAlign ( int nIndex )
{
	CreateOrderedArray();			// create ordered array if required

	return m_OrderedArray.GetAlign(nIndex);
}

int CTabRuler::GetOrderedIndex ( int nIndex )
{
	CreateOrderedArray();			// create ordered array if required

	return m_OrderedArray.GetOrder(nIndex);
}

const char* CTabRuler::GetOrderedSubTabText ( int nIndex )
{
	CreateOrderedArray();			// create ordered array if required

	return m_OrderedArray.GetText(nIndex);
}

/**********************************************************************/

const char* CTabRuler::GetText ( int nIndex )
{
	return m_TabStopArray.GetText ( nIndex );
}

/**********************************************************************/

int CTabRuler::GetAlign ( int nIndex )
{
	return m_TabStopArray.GetAlign ( nIndex );
}

void CTabRuler::SetAlign ( int nIndex, int nAlign )
{
	m_TabStopArray.SetAlign ( nIndex, nAlign );

	m_bOrdered = FALSE;			// need to create ordered array
}

/**********************************************************************/

int CTabRuler::GetWidth ( int nIndex )
{
	return m_TabStopArray.GetWidth ( nIndex );
}

void CTabRuler::SetWidth ( int nIndex, int nWidth )
{
	m_nTotalWidth += ( nWidth - m_TabStopArray.GetWidth ( nIndex ) );

	m_TabStopArray.SetWidth ( nIndex, nWidth );

	m_bOrdered = FALSE;			// need to create ordered array
}

/**********************************************************************/

int CTabRuler::GetOrder ( int nIndex )
{
	return m_TabStopArray.GetOrder ( nIndex );
}

void CTabRuler::SetOrder ( int nIndex, int nOrder )
{
	m_TabStopArray.SetOrder ( nIndex, nOrder );

	m_bOrdered = FALSE;			// need to create ordered array
}

/**********************************************************************/
