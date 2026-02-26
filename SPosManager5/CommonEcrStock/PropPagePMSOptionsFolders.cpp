/**********************************************************************/
#ifdef COMPILE_PMS 
/**********************************************************************/
#include "PMSOptions.h"
/**********************************************************************/
#include "PropPagePMSOptionsFolders.h"
/**********************************************************************/

CPropPagePMSOptionsFolders::CPropPagePMSOptionsFolders() : CPropertyPage(CPropPagePMSOptionsFolders::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePMSOptionsFolders)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPagePMSOptionsFolders::~CPropPagePMSOptionsFolders()
{
}

/**********************************************************************/

void CPropPagePMSOptionsFolders::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPagePMSOptionsFolders)
	DDX_Control(pDX, IDC_BUTTON_SET_ROOM, m_buttonSetRoom);
	DDX_Control(pDX, IDC_BUTTON_SET_SALES, m_buttonSetSales);
	DDX_Control(pDX, IDC_EDIT_FOLDER_ROOM, m_editFolderRoom);
	DDX_Control(pDX, IDC_EDIT_FOLDER_SALES, m_editFolderSales);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePMSOptionsFolders, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePMSOptionsFolders)
	ON_BN_CLICKED(IDC_BUTTON_SET_ROOM, OnButtonSetRoom)
	ON_BN_CLICKED(IDC_BUTTON_SET_SALES, OnButtonSetSales)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPagePMSOptionsFolders::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_strFolderRoom = PMSOptions.GetEposLinkRoomPath();
	m_editFolderRoom.SetWindowText( m_strFolderRoom );

	m_strFolderSales = PMSOptions.GetEposLinkPostPath();
	m_editFolderSales.SetWindowText( m_strFolderSales );

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPagePMSOptionsFolders::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePMSOptionsFolders::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPagePMSOptionsFolders::SaveRecord()
{
	PMSOptions.SetEposLinkRoomPath( m_strFolderRoom );
	PMSOptions.SetEposLinkPostPath( m_strFolderSales );
}

/**********************************************************************/

void CPropPagePMSOptionsFolders::OnButtonSetRoom()
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select folder for Guest Account Room Data", NULL, this ) == TRUE )
	{
		m_strFolderRoom = strPathname;
		m_editFolderRoom.SetWindowText( m_strFolderRoom );
	}
}

/**********************************************************************/

void CPropPagePMSOptionsFolders::OnButtonSetSales()
{
	CString strPathname = "";
	if ( BrowseFolder ( strPathname, "Select folder for Guest Account Post Data", NULL, this ) == TRUE )
	{
		m_strFolderSales = strPathname;
		m_editFolderSales.SetWindowText( m_strFolderSales );
	}
}

/**********************************************************************/
#endif
/**********************************************************************/
