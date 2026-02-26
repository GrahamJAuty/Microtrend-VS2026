/**********************************************************************/
#include "DefEditTableCopyDlg.h"
/**********************************************************************/

CDefEditTableCopyDlg::CDefEditTableCopyDlg( int nIndex, CWnd* pParent )
	: CSSDialog(CDefEditTableCopyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefEditTableCopyDlg)
	//}}AFX_DATA_INIT
	m_nIndex = nIndex;
	m_nTablesCreated = 0;
	m_nTablesModified = 0;
}

/**********************************************************************/

void CDefEditTableCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefEditTableCopyDlg)
	DDX_Control(pDX, IDC_CHECK_COPY_SHARPPOS, m_checkCopySharpPos);
	DDX_Control(pDX, IDC_CHECK_ACCOUNT, m_checkAccount);
	DDX_Control(pDX, IDC_CHECK_LIMIT, m_checkLimit);
	DDX_Control(pDX, IDC_CHECK_AUTOSTATUS1, m_checkAutoStatus1);
	DDX_Control(pDX, IDC_CHECK_AUTOSTATUS2, m_checkAutoStatus2);
	/*****/
	DDX_Control(pDX, IDC_CHECK_COPY_REPORT, m_checkCopyReport);
	DDX_Control(pDX, IDC_CHECK_FILTER, m_checkFilter);
	DDX_Control(pDX, IDC_COMBO_GROUP1, m_comboGroup[0]);
	DDX_Control(pDX, IDC_COMBO_GROUP2, m_comboGroup[1]);
	DDX_Control(pDX, IDC_COMBO_GROUP3, m_comboGroup[2]);
	/*****/
	DDX_Control(pDX, IDC_CHECK_CREATE, m_checkCreate);
	DDX_Control(pDX, IDC_CHECK_MODIFY, m_checkModify);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDefEditTableCopyDlg, CDialog)
	//{{AFX_MSG_MAP(CDefEditTableCopyDlg)
	ON_BN_CLICKED(IDC_CHECK_COPY_SHARPPOS, OnToggleCopySharpPos)
	ON_BN_CLICKED(IDC_CHECK_COPY_REPORT, OnToggleCopyReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDefEditTableCopyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit( IDC_EDIT_LIMIT, SS_NUM, 4, "%d" );
	SubclassEdit(IDC_EDIT_TABLE_START, SS_NUM, 3, "%d");
	SubclassEdit(IDC_EDIT_TABLE_END, SS_NUM, 3, "%d");

	CArray<int, int> arrayStops;
	arrayStops.Add(5);
	arrayStops.Add(40);
	
	for ( int n = 0; n < 3; n++ )	
	{
		m_comboGroup[n].SetTabStops( arrayStops );
	}

	for ( int g = 1; g <= MAX_TABLEGROUP; g++ )
	{
		CTableGroupCSVRecord Group;
		DataManagerNonDb.TableGroups.GetAt( g - 1, Group );

		for ( int n = 1; n <= 3; n++ )	
		{
			m_comboGroup[n-1].AddItem( Group.GetTabbedComboText(g,n) );	
		}
	}

	GetData();

	m_checkCreate.SetCheck(TRUE);
	m_checkModify.SetCheck(FALSE);
	m_checkCopySharpPos.SetCheck(TRUE);
	m_checkCopyReport.SetCheck(FALSE);
	OnToggleCopyReport();

	return TRUE;  
}

/**********************************************************************/
CEdit* CDefEditTableCopyDlg::GetEditLimit(){ return GetEdit( IDC_EDIT_LIMIT ); }
CEdit* CDefEditTableCopyDlg::GetEditTableStart() { return GetEdit(IDC_EDIT_TABLE_START); }
CEdit* CDefEditTableCopyDlg::GetEditTableEnd() { return GetEdit(IDC_EDIT_TABLE_END); }
/**********************************************************************/

void CDefEditTableCopyDlg::OnToggleCopySharpPos()
{
	bool bEnable = IsTicked(m_checkCopySharpPos);
	m_checkAccount.EnableWindow(bEnable);
	m_checkLimit.EnableWindow(bEnable);
	GetEditLimit()->EnableWindow(bEnable);
	m_checkAutoStatus1.EnableWindow(bEnable);
	m_checkAutoStatus2.EnableWindow(bEnable);
}

/**********************************************************************/

void CDefEditTableCopyDlg::OnToggleCopyReport()
{
	bool bEnable = IsTicked(m_checkCopyReport);
	m_checkFilter.EnableWindow(bEnable);
	m_comboGroup[0].EnableWindow(bEnable);
	m_comboGroup[1].EnableWindow(bEnable);
	m_comboGroup[2].EnableWindow(bEnable);
}

/**********************************************************************/

void CDefEditTableCopyDlg::GetData()
{
	DataManagerNonDb.TableNames.GetAt( m_nIndex, m_TableRecord );

	CString strTitle;
	strTitle.Format( "Copy Table (%d, %s)",
		m_TableRecord.GetTableNo(),
		m_TableRecord.GetTableName() );
	SetWindowText( strTitle );

	{
		CString strTableNo = "";
		strTableNo.Format("%d", m_TableRecord.GetTableNo());
		GetEditTableStart()->SetWindowText(strTableNo);
		GetEditTableEnd()->SetWindowText(strTableNo);
	}

	m_checkFilter.SetCheck(m_TableRecord.GetShowAsFilterFlag() );
	m_checkAccount.SetCheck(m_TableRecord.GetAccountTableFlag() );
	m_checkLimit.SetCheck(m_TableRecord.GetEnableLimitFlag() );
	m_checkAutoStatus1.SetCheck(m_TableRecord.GetAutoStatusEnableFlag() );
	m_checkAutoStatus2.SetCheck(m_TableRecord.GetAutoStatusUnusedAfterPaymentFlag() );

	SetEditBoxInt( *GetEditLimit(), m_TableRecord.GetLimitPounds() );

	for ( int n = 1; n <= 3; n++ )
	{
		int nGroup = m_TableRecord.GetReportGroup(n);

		if ((nGroup >= 1) && (nGroup <= m_comboGroup[n - 1].GetCount()))
		{
			m_comboGroup[n - 1].SetCurSel(nGroup - 1);
		}
		else
		{
			m_comboGroup[n - 1].SetCurSel(0);
		}
	}
}

/**********************************************************************/

void CDefEditTableCopyDlg::MergeTableSettings(CTableNameCSVRecord& Table, bool bCopySharpPos, bool bCopyReport)
{
	if (TRUE == bCopySharpPos)
	{
		Table.SetAccountTableFlag(m_TableRecord.GetAccountTableFlag());
		Table.SetEnableLimitFlag(m_TableRecord.GetEnableLimitFlag());
		Table.SetLimitPounds(m_TableRecord.GetLimitPounds());
		Table.SetAutoStatusEnableFlag(m_TableRecord.GetAutoStatusEnableFlag());
		Table.SetAutoStatusUnusedAfterPaymentFlag(m_TableRecord.GetAutoStatusUnusedAfterPaymentFlag());
	}

	if (TRUE == bCopyReport)
	{
		Table.SetShowAsFilterFlag(m_TableRecord.GetShowAsFilterFlag());
		Table.SetReportGroup(1, m_TableRecord.GetReportGroup(1));
		Table.SetReportGroup(2, m_TableRecord.GetReportGroup(2));
		Table.SetReportGroup(3, m_TableRecord.GetReportGroup(3));
	}
}

/**********************************************************************/

bool CDefEditTableCopyDlg::CopyTable()
{
	bool bResult = FALSE;
	m_nTablesCreated = 0;
	m_nTablesModified = 0;

	int nTableStart = GetEditBoxInt(*GetEditTableStart());
	int nTableEnd = GetEditBoxInt(*GetEditTableEnd());
	bool bCreate = IsTicked(m_checkCreate);
	bool bModify = IsTicked(m_checkModify);
	bool bCopySharpPos = IsTicked(m_checkCopySharpPos);
	bool bCopyReport = IsTicked(m_checkCopyReport);

	if ((nTableEnd < nTableStart) || (nTableStart < 1) || (nTableEnd > 999))
	{
		Prompter.Error("Please select a valid table range.");
	}
	else if ((FALSE == bCreate) && (FALSE == bModify))
	{
		Prompter.Error("Please select one or both of the create or modify options.");
	}
	else
	{
		bResult = TRUE;

		m_TableRecord.SetShowAsFilterFlag(IsTicked(m_checkFilter));
		m_TableRecord.SetAccountTableFlag(IsTicked(m_checkAccount));
		m_TableRecord.SetEnableLimitFlag(IsTicked(m_checkLimit));
		m_TableRecord.SetAutoStatusEnableFlag(IsTicked(m_checkAutoStatus1));
		m_TableRecord.SetAutoStatusUnusedAfterPaymentFlag(IsTicked(m_checkAutoStatus2));
		m_TableRecord.SetLimitPounds(GetEditBoxInt(*GetEditLimit()));

		for (int n = 1; n <= 3; n++)
		{
			m_TableRecord.SetReportGroup(n, m_comboGroup[n - 1].GetCurSel() + 1);
		}

		for (int nTableNo = nTableStart; nTableNo <= nTableEnd; nTableNo++)
		{
			int nTableIdx = 0;
			if (DataManagerNonDb.TableNames.FindTableByNumber(nTableNo, nTableIdx) == FALSE)
			{
				if (TRUE == bCreate)
				{
					CTableNameCSVRecord NewTableRecord;
					NewTableRecord.SetTableNo(nTableNo);

					{
						CString strName = "";
						strName.Format("Table %d", nTableNo);
						NewTableRecord.SetTableName(strName);
					}

					MergeTableSettings(NewTableRecord, bCopySharpPos, bCopyReport);
					DataManagerNonDb.TableNames.InsertAt(nTableIdx, NewTableRecord);
					m_nTablesCreated++;
				}
			}
			else
			{
				if (TRUE == bModify)
				{
					CTableNameCSVRecord TableRecord;
					DataManagerNonDb.TableNames.GetAt(nTableIdx, TableRecord);
					MergeTableSettings(TableRecord, bCopySharpPos, bCopyReport);
					DataManagerNonDb.TableNames.SetAt(nTableIdx, TableRecord);
					m_nTablesModified++;
				}
			}
		}
	}

	return bResult;
}

/**********************************************************************/

void CDefEditTableCopyDlg::OnOK()
{
	if (CopyTable() == TRUE)
	{
		EndDialog(IDOK);
	}
}

/**********************************************************************/
