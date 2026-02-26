//$$******************************************************************
#pragma once
//$$******************************************************************
#include "resource.h"
//$$******************************************************************
#include "ServerPropertyPage1.h"
#include "ServerPropertyPage2.h"
#include "ServerPropertyPage3.h"
#include "ServerPropertyPage4.h"
#include "ServerPropertyPage5.h"
#include "ServerPropertyPage6.h"
#include "ServerPropertyPage7.h"
#include "ServerPropertyPage8.h"
#include "ServerPropertyPage9.h"
//$$******************************************************************

class CServerPropertySheet : public CSSPropertySheet
{
public:
	CServerPropertySheet( const char* szFilename, CWnd* pParentWnd );
	virtual ~CServerPropertySheet();
	bool Save();

public:
	CServerPropertyPage1 m_Page1;
	CServerPropertyPage2 m_Page2;
	CServerPropertyPage3 m_Page3;
	CServerPropertyPage4 m_Page4;
	CServerPropertyPage5 m_Page5;
	CServerPropertyPage6 m_Page6;
	CServerPropertyPage7 m_Page7;
	CServerPropertyPage8 m_Page8;
	CServerPropertyPage9 m_Page9;

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CString m_strServerDatafile;
};

//$$******************************************************************
