//$$******************************************************************
#include "ServerPropertyPage7.h"
//$$******************************************************************

CServerPropertyPage7::CServerPropertyPage7()
	: CSSPropertyPage(CServerPropertyPage7::IDD)
{
}

//$$******************************************************************

CServerPropertyPage7::~CServerPropertyPage7()
{
}

//$$******************************************************************

void CServerPropertyPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_EXCLUDE_HOTLIST, m_checkExcludeHotlist);
	DDX_Control(pDX, IDC_CHECK_EXCLUDE_INACTIVE, m_checkExcludeInactive);
	DDX_Control(pDX, IDC_CHECK_SURNAME, m_checkSurname);
	DDX_Control(pDX, IDC_CHECK_SNAKES, m_checkSnakes);
	DDX_Control(pDX, IDC_STATIC_SNAKES1, m_staticSnakes1);
	DDX_Control(pDX, IDC_STATIC_SNAKES2, m_staticSnakes2);
	DDX_Control(pDX, IDC_STATIC_SNAKES3, m_staticSnakes3);
	DDX_Control(pDX, IDC_STATIC_SNAKES4, m_staticSnakes4);
}

//$$******************************************************************

BEGIN_MESSAGE_MAP(CServerPropertyPage7, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_SNAKES,OnToggleSnakes)
END_MESSAGE_MAP()

//$$******************************************************************

BOOL CServerPropertyPage7::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_checkExcludeHotlist.SetCheck(Server.GetSearchExcludeHotlistFlag());
	m_checkExcludeInactive.SetCheck(Server.GetSearchExcludeInactiveFlag());
	m_checkSurname.SetCheck(Server.GetSearchSurnameMatchFullnameFlag());
	m_checkSnakes.SetCheck(Server.GetSearchSnakesAndLaddersFlag());
	OnToggleSnakes();
	return TRUE;
}

//$$******************************************************************

BOOL CServerPropertyPage7::OnKillActive()
{
	Server.SetSearchExcludeHotlistFlag(IsTicked(m_checkExcludeHotlist));
	Server.SetSearchExcludeInactiveFlag(IsTicked(m_checkExcludeInactive));
	Server.SetSearchSurnameMatchFullnameFlag(IsTicked(m_checkSurname));
	Server.SetSearchSnakesAndLaddersFlag(IsTicked(m_checkSnakes));
	return CPropertyPage::OnKillActive();
}

//$$******************************************************************

void CServerPropertyPage7::OnToggleSnakes()
{
	bool bShowWarning = IsTicked(m_checkSnakes);
	ShowAndEnableWindow(&m_staticSnakes1, bShowWarning);
	ShowAndEnableWindow(&m_staticSnakes2, bShowWarning);
	ShowAndEnableWindow(&m_staticSnakes3, bShowWarning);
	ShowAndEnableWindow(&m_staticSnakes4, bShowWarning);

	m_checkSurname.EnableWindow(FALSE == bShowWarning);
	m_checkExcludeHotlist.EnableWindow(FALSE == bShowWarning);
	m_checkExcludeInactive.EnableWindow(FALSE == bShowWarning);

	if (TRUE == bShowWarning)
	{
		m_checkSurname.SetCheck(FALSE);
		m_checkExcludeHotlist.SetCheck(FALSE);
		m_checkExcludeInactive.SetCheck(FALSE);
	}
}

//$$******************************************************************
