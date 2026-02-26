#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/

class CSubUnitFractionEdit : public CEdit
{
public:
	CSubUnitFractionEdit();
	BOOL SubclassDlgItem ( UINT nID, CWnd* pParent, int nWidth, int nSUYield, bool bSigned );
	void SetMode( int nWidth, int nSUYield, bool bSigned );

	void GetSubUnitString( double dQty, CString& strSubUnits );
	bool GetSubUnitQuantity( const char* szSubUnits, double& dQty );

protected:
	afx_msg void OnChar ( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKillFocus ( CWnd* pNewWnd );

public:
	enum CharTypes { control, minus, number, slash, dot, invalid_type }; 
	enum States { empty, want_stock, want_sub, want_fraction, invalid_state };

private:
	int GetCharType ( UINT c );

private:
	bool m_bSigned;
	int m_nSUYield;

	DECLARE_MESSAGE_MAP()
};

/**********************************************************************/
#endif
/**********************************************************************/

