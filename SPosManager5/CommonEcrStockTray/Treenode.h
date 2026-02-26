#pragma once
/**********************************************************************/

class CTreeNode
{
public:
	CTreeNode( int nNodeType );
		
	int GetNodeType() { return m_nNodeType; }
	int GetNetworkNo() { return m_nNetworkNo; }
	int GetDbNo() { return m_nDbNo; }
	int GetLocNo() { return m_nLocNo; }
	int GetTerminalIdx() { return m_nTerminalIdx; }
	int GetRpNo() { return m_nRpNo; }
	int GetLsNo() { return m_nLsNo; }
	int GetSpNo() { return m_nSpNo; }
	int GetSiteNo() { return m_nSiteNo; }
	int GetKbMapIndex(){ return m_nKbMapIndex; }
	int GetKbNo(){ return m_nKbNo; }
	bool GetClickableFlag(){ return m_bIsClickable; }
	
	void SetDbNo( int nDbNo ) { m_nDbNo = nDbNo; }
	void SetNetworkNo( int nNetworkNo ) { m_nNetworkNo = nNetworkNo; }
	void SetLocNo( int nLocNo ) { m_nLocNo = nLocNo; }
	void SetTerminalIdx( int nTerminalIdx ) { m_nTerminalIdx = nTerminalIdx; }
	void SetRpNo ( int nRpNo ) { m_nRpNo = nRpNo; }
	void SetLsNo ( int nLsNo ) { m_nLsNo = nLsNo; }
	void SetSpNo( int nSpNo ) { m_nSpNo = nSpNo; }
	void SetSiteNo ( int nSiteNo ) { m_nSiteNo = nSiteNo; }
	void SetKbMapIndex( int nIdx ){ m_nKbMapIndex = nIdx; }
	void SetKbNo( int nKbNo ){ m_nKbNo = nKbNo; }
	void SetClickableFlag( bool b ){ m_bIsClickable = b; }

private:
	int m_nNodeType;
	int m_nNetworkNo;
	int m_nDbNo;
	int m_nLocNo;
	int m_nTerminalIdx;
	int m_nRpNo;
	int m_nLsNo;
	int m_nSpNo;
	int m_nSiteNo;
	int m_nKbMapIndex;
	int m_nKbNo;
	int m_nSPOSVersion;
	bool m_bIsClickable;
};

/**********************************************************************/

class COfferGroupNode
{
public:
	COfferGroupNode( int nNodeType );
	
	int GetNodeType() { return m_nNodeType; }
	__int64 GetPluNo() { return m_nPluNo; }
	int GetModifierFlags() { return m_nModifierFlags; }
	int GetModifierNo() { return m_nModifierNo; }
	int GetDeptNo(){ return m_nDeptNo; }
	int GetGroupNo(){ return m_nGroupNo; }

	void SetPluNo( __int64 nPluNo ) { m_nPluNo = nPluNo; }
	void SetModifierFlags( int nFlags ) { m_nModifierFlags = nFlags; }
	void SetModifierNo( int nNo ) { m_nModifierNo = nNo; }
	void SetDeptNo( int nNo ) { m_nDeptNo = nNo; }
	void SetGroupNo( int nNo ) { m_nGroupNo = nNo; }

private:
	int m_nNodeType;
	__int64 m_nPluNo;
	int m_nModifierFlags;
	int m_nModifierNo;
	int m_nDeptNo;
	int m_nGroupNo;
};

/**********************************************************************/

