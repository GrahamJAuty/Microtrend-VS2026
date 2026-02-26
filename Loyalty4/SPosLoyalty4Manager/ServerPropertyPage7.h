#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************

class CServerPropertyPage7 : public CSSPropertyPage
{
public:
	CServerPropertyPage7();
	virtual ~CServerPropertyPage7();

	enum { IDD = IDD_PROPPAGE_SERVER7 };
	CButton m_checkExcludeHotlist;
	CButton m_checkExcludeInactive;
	CButton m_checkSurname;
	CButton m_checkSnakes;
	CStatic m_staticSnakes1;
	CStatic m_staticSnakes2;
	CStatic m_staticSnakes3;
	CStatic m_staticSnakes4;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnToggleSnakes();
	DECLARE_MESSAGE_MAP()
};

//$$******************************************************************

