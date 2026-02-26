/**********************************************************************/
 
/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "DataNodeInfoKeyboard.h"
/**********************************************************************/

CDataNodeInfoKeyboard::CDataNodeInfoKeyboard()
{
	Clear();
}

/**********************************************************************/

void CDataNodeInfoKeyboard::Clear()
{
	m_nNodeType = NODE_PLACEHOLDER;
	m_nSPOSVersion = 0;
	m_nDbIdx = 0;
	m_nLocIdx = 0;
	m_nKbsIdx = 0;
	m_nKbNo = 0;
}

/**********************************************************************/

bool CDataNodeInfoKeyboard::Compare( CDataNodeInfoKeyboard& test )
{
	if ( m_nNodeType != test.m_nNodeType ) return FALSE;
	if ( m_nSPOSVersion != test.m_nSPOSVersion ) return FALSE;
	if ( m_nDbIdx != test.m_nDbIdx ) return FALSE;
	if ( m_nLocIdx != test.m_nLocIdx ) return FALSE;
	if ( m_nKbsIdx != test.m_nKbsIdx ) return FALSE;
	if ( m_nKbNo != test.m_nKbNo ) return FALSE;
	return TRUE;
}

/**********************************************************************/
