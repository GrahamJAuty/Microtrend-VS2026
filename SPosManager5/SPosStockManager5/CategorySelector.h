#pragma once
/**********************************************************************/
#ifdef STOCKMAN_SYSTEM
/**********************************************************************/
#include "CategorySetCSVArray.h"
//include "MyComboBox.h"
/**********************************************************************/

class CCategorySelector  
{
public:
	CCategorySelector();
	~CCategorySelector();
	
	void SelectCategorySet( int nFilter );
	void IncludeCategory ( int nCategory, bool bInclude );
	void IncludeCategorySets() { m_bIncludeCategorySets = TRUE; }
	void IncludeReportEnabled() { m_bIncludeReportEnabled = TRUE; }
	void IncludeAll() { m_bIncludeAll = TRUE; }
	void IncludeCustom() { m_bIncludeCustom = TRUE; }

	void FillCategoryCombo( CSSComboBox* pCategoryCombo );
	void ForceSelection ( int nFilter );

	bool GotCombo(){ return m_bGotCombo; }
	
	int GetSelection();
	const char* GetCategoryText();

private:
	CSSComboBox* m_pCategoryCombo;
	CCategorySetCSVRecord m_CategorySet;
	CString m_strCategoryText;
	int m_nFilter;
	bool m_bIncludeCategorySets;
	bool m_bIncludeReportEnabled;
	bool m_bIncludeAll;
	bool m_bIncludeCustom;
	bool m_bGotCombo;
};

/**********************************************************************/
#endif
/**********************************************************************/


