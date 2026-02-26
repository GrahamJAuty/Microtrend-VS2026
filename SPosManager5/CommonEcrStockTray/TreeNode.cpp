/**********************************************************************/
 
/**********************************************************************/
#include "TreeNode.h"
/**********************************************************************/

CTreeNode::CTreeNode( int nNodeType )
{
	m_nNodeType = nNodeType;
	m_nNetworkNo = 0;
	m_nDbNo = 0;
	m_nLocNo = 0;
	m_nTerminalIdx = 0;
	m_nRpNo = 0;
	m_nLsNo = 0;
	m_nSpNo = 0;
	m_nSiteNo = 0;
	m_nKbMapIndex = 0;
	m_nKbNo = 0;
	m_bIsClickable = FALSE;
}

/**********************************************************************/

COfferGroupNode::COfferGroupNode( int nNodeType )
{
	m_nNodeType = nNodeType;
	m_nPluNo = 0;
	m_nModifierFlags = 0;
	m_nModifierNo = 0;
	m_nDeptNo = 0;
	m_nGroupNo = 0;
}

/**********************************************************************/
