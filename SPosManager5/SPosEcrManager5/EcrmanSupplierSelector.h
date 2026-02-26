#pragma once
/**********************************************************************/
#ifndef STOCKMAN_SYSTEM
/**********************************************************************/
#define ECRMANSUPPLIER_UNKNOWN 9999
#define ECRMANSUPPLIER_ALL 9998
/**********************************************************************/
//include "MyComboBox.h"
/**********************************************************************/

class CEcrmanSupplierSelector  
{
public:
	CEcrmanSupplierSelector( CEdit& edit, CSSComboBox& combo, bool bAll );
	void FillEcrmanSupplierCombo( int nListNo );
	void SelectEcrmanSupplierFromCombo();
	void SelectEcrmanSupplierFromEditBox();
	void UpdateEcrmanSupplierCombo( int nNewSuppNo );
	void UpdateEcrmanSupplierEditBox();
	int GetEcrmanSupplierNo() { return m_nSuppNo; }
	
private:
	CEdit& m_EditBox;
	CSSComboBox& m_ComboBox;
	int m_nSuppNo;
	bool m_bAll;
};

/**********************************************************************/
#endif
/**********************************************************************/
