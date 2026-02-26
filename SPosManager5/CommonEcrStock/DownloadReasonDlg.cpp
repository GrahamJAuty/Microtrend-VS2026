/**********************************************************************/
#include "..\CommonEcrStock\CascadingReasonCSVArray.h"
/**********************************************************************/
#include "DownloadReasonDlg.h"
/**********************************************************************/

CDownloadReasonDlg::CDownloadReasonDlg(CWnd* pParent)
	: CSSDialog(CDownloadReasonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownloadReasonDlg)
	//}}AFX_DATA_INIT
	m_nParam1 = 0;
}

/**********************************************************************/

void CDownloadReasonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadReasonDlg)
	DDX_Control(pDX, IDC_CHECK_CLEAR, m_checkClear);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDownloadReasonDlg, CSSDialog)
	//{{AFX_MSG_MAP(CDownloadReasonDlg)
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_NONE, OnButtonNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDownloadReasonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	m_listReasonTypes.SubclassDlgItem ( IDC_LIST, this );
	m_listReasonTypes.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	m_listReasonTypes.InsertColumn ( 0, "", LVCFMT_LEFT, 210 );
	m_listReasonTypes.SetLockItemZeroFlag( FALSE );

	AddList();
	return TRUE;  
}

/**********************************************************************/

void CDownloadReasonDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
   if ( nIDCtl == IDC_LIST )   
   {
	   m_listReasonTypes.AdjustRowHeight( lpMeasureItemStruct, 10 ); 
   }
}

/**********************************************************************/

void CDownloadReasonDlg::OnButtonAll()
{	
	m_listReasonTypes.SelectAll();
}

/**********************************************************************/

void CDownloadReasonDlg::OnButtonNone()
{	
	m_listReasonTypes.SelectNone();
}

/**********************************************************************/

void CDownloadReasonDlg::AddList()
{
	for ( int n = 0; n < REASONTEXT_TYPES; n++ )
	{
		CSSListTaggedSelectItem item;
		item.m_strText = CCascadingReasonCSVArray::GetReasonTypeText(n);
		item.m_nData = 0;
		item.SetTagState(0);
		m_listReasonTypes.AddItemToList( item );
	}
}

/**********************************************************************/

void CDownloadReasonDlg::OnOK()
{
	m_nParam1 = 0;
	int nMask = 1;

	for (int n = 0; n < REASONTEXT_TYPES; n++)
	{
		if (m_listReasonTypes.GetItemTagState(n) == 1)
		{
			m_nParam1 += nMask;
		}

		nMask *= 2;
	}

	if (0 == m_nParam1)
	{
		Prompter.Error("Please select one or more text types to download.");
	}
	else
	{
		if (IsTicked(m_checkClear))
		{
			m_nParam1 += 0x10000;
		}

		EndDialog(IDOK);
	}
}

/**********************************************************************/
