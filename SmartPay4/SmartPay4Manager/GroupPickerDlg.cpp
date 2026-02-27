//*********************************************************************************
#include "..\SmartPay4Shared\SQLTable_Group\SQLRepositoryGroup.h"
#include "..\SmartPay4Shared\SQLDb.h"
//*********************************************************************************
#include "SmartPay4Manager.h"
#include "ColumnHandler.h"
//*********************************************************************************
#include "GroupPickerDlg.h"
//*********************************************************************************
static int nColumnCount		= 3;
//*********************************************************************************

CGroupPickerDlg::CGroupPickerDlg( const char* szTitle, const char* szListFilename, CWnd* pParent )
	: CSSAutoShutdownDialog(AutoShutdownHelper, CGroupPickerDlg::IDD, pParent)
{
	m_strTitle = szTitle;
	m_strListFilename = szListFilename;
}

//*********************************************************************************

CGroupPickerDlg::~CGroupPickerDlg()
{
}

//*********************************************************************************

void CGroupPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CARDPICKER, m_listctrlPicker);
}

//*********************************************************************************

BEGIN_MESSAGE_MAP(CGroupPickerDlg, CSSAutoShutdownDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnBnClickedButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, OnBnClickedButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()

//*******************************************************************
// CGroupPickerDlg message handlers

BOOL CGroupPickerDlg::CSSAutoShutdownPostInitDialog()
{
	CColumnHandler column;
	if ( column.Read (  "GLIST", nColumnCount ) == FALSE )
	{
		column.SetWidth ( 0, 40 );			// group No
		column.SetWidth ( 1, 120 );			// name
		column.SetWidth ( 2, 40 );			// Set
	}

	m_listctrlPicker.InsertColumn ( 0, "Grp", LVCFMT_LEFT, column.GetWidth(0) );
	m_listctrlPicker.InsertColumn ( 1, "Name", LVCFMT_LEFT, column.GetWidth(1) );
	m_listctrlPicker.InsertColumn ( 2, "Set", LVCFMT_LEFT, column.GetWidth(2) );

	m_listctrlPicker.SetExtendedStyle ( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

	FillList();

	m_listctrlPicker.SetCurSel(0);
	m_listctrlPicker.SetFocus();

	return FALSE;
}

void CGroupPickerDlg::OnOK()
{
	int nCount = 0;
	for ( int nIndex = 0 ; nIndex < m_listctrlPicker.GetItemCount() ; nIndex++ )
	{
		if ( m_listctrlPicker.GetCheck(nIndex) == TRUE )
			++nCount;
	}

	if ( nCount == 0 )
	{
		Prompter.Error( "No groups have been selected", m_strTitle );
		return;
	}

	CSSFile file;
	if ( file.Open ( m_strListFilename, "wb" ) == TRUE )
	{
		for ( int nIndex = 0 ; nIndex < m_listctrlPicker.GetItemCount() ; nIndex++ )
		{
			if ( m_listctrlPicker.GetCheck(nIndex) == TRUE )
			{
				CString strGroupNo = m_listctrlPicker.GetItemText ( nIndex, 0 );
				file.WriteLine ( strGroupNo );
			}
			}
		file.Close();
	}

	CColumnHandler column;
	column.Save ( "GLIST", 0, nColumnCount, m_listctrlPicker );

	EndDialog(IDOK);
}

//********************************************************************

void CGroupPickerDlg::FillList()
{
	CSQLRecordSetGroupFull RecordSet(NULL, RSParams_Group_NormalNoParams{});

	CSQLRowGroupFull RowGroup;
	while (RecordSet.StepSelectAll(RowGroup) == TRUE)
	{
		CCSV csv ( '\t' );
		csv.Add ( RowGroup.GetGroupNo() );
		csv.Add ( RowGroup.GetGroupName() );
		csv.Add ( RowGroup.GetSetNo() );

		m_listctrlPicker.AddString ( csv.GetLine() );
	}

	CSSFile file;
	if ( file.Open ( m_strListFilename, "rb" ) == TRUE )
	{
		CString strLine;
		while ( file.ReadString ( strLine ) == TRUE )
		{
			int nGroupNo = atoi ( strLine );

			for ( int nIndex = 0 ; nIndex < m_listctrlPicker.GetItemCount() ; nIndex++ )
			{
				int n = atoi ( m_listctrlPicker.GetItemText ( nIndex, 0 ) );
				if ( nGroupNo == n )
				{
					m_listctrlPicker.SetCheck(nIndex, TRUE);
					break;
				}
			}
		}
	}
}

//*******************************************************************

void CGroupPickerDlg::OnBnClickedButtonInvert()
{
	for ( int nIndex = 0 ; nIndex < m_listctrlPicker.GetItemCount() ; nIndex++ )
	{
		BOOL bReqd = m_listctrlPicker.GetCheck(nIndex);
		m_listctrlPicker.SetCheck(nIndex, ( FALSE == bReqd ));
	}
}

//*******************************************************************

void CGroupPickerDlg::OnBnClickedButtonAll()	{ SetAll ( TRUE );	}
void CGroupPickerDlg::OnBnClickedButtonClear()	{ SetAll ( FALSE );	}

void CGroupPickerDlg::SetAll ( bool bReqd )
{
	for ( int nIndex = 0 ; nIndex < m_listctrlPicker.GetItemCount() ; nIndex++ )
		m_listctrlPicker.SetCheck(nIndex, bReqd );
}


