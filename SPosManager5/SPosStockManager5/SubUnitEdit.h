#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CSubUnitEdit : public CEdit
{
public:
	CSubUnitEdit();
	BOOL SubclassDlgItem ( UINT nID, CWnd* pParent, int nWidth, int nSUYield, bool bSigned );
	void SetMode( int nWidth, int nSUYield, bool bSigned );

	void GetSubUnitString( __int64 n, CString& strSubUnits );
	void GetSubUnitQuantity( const char* szSubUnits, __int64& n );

protected:
	afx_msg void OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKillFocus ( CWnd* pNewWnd );

private:
	bool IsValidChar ( UINT nChar );

private:
	bool m_bSigned;
	int m_nSUYield;

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/


