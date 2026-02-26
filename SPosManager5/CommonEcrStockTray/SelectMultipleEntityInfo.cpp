/**********************************************************************/
#include "SelectMultipleEntityInfo.h"
/**********************************************************************/

CSelectMultipleEntityInfo::CSelectMultipleEntityInfo()
{
	Reset();
}

/**********************************************************************/

void CSelectMultipleEntityInfo::Reset()
{
	m_nNodeType = -1;
	m_nDbIdx = -1;
	m_nNwkIdx = -1;
	m_nLocIdx = -1;
	m_nTermIdx = -1;
	m_nTNo = -1;
	m_nSpIdx = -1;
	m_nKbsIdx = -1;
	m_nKbNo = -1;
	m_nPasswordIdx = -1;
	m_nPluNo = -1;
	m_nPromoNo = -1;
	m_bSelected = FALSE;
	m_strText[0] = "";
	m_strText[1] = "";
	m_strText[2] = "";
	m_strText[3] = "";
	m_strText[4] = "";
}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

CSelectMultipleEntityFeeder::CSelectMultipleEntityFeeder()
{
	Reset();
}

/**********************************************************************/

void CSelectMultipleEntityFeeder::Reset()
{
	m_nDbNo = -1;
	m_nNwkNo = -1;
	m_nLocNo = -1;
	m_nSpNo = -1;
	m_nTermIdx = -1;
}

/**********************************************************************/

