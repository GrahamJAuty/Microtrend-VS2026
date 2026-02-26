#pragma once
/**********************************************************************/

class CKbdListSelector  
{
public:
	CKbdListSelector( CEdit& edit, CSSComboBox& combo, bool bModifier );
	void FillKbdListCombo( int nListNo );
	void SelectKbdListFromCombo();
	void SelectKbdListFromEditBox();
	void UpdateKbdListCombo( int nNewListNo );
	void UpdateKbdListEditBox();
	int GetKbdListNo() { return m_nListNo; }
	
private:
	CEdit& m_EditBox;
	CSSComboBox& m_ComboBox;

private:
	bool m_bModifier;
	int m_nUnknownPos;
	int m_nListNo;

private:
	CReportConsolidationArray<CSortedStringByInt> m_arrayHidden;
};

/**********************************************************************/
