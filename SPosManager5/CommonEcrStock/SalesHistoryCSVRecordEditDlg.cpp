/**********************************************************************/
#include "Consolidation.h"
#include "MaxLengths.h"
#include "ReportConsolidationArray.h"
/**********************************************************************/
#include "SalesHistoryCSVRecordEditDlg.h"
/**********************************************************************/

CSalesHistoryCSVRecordEditDlg::CSalesHistoryCSVRecordEditDlg( CWnd* pParent)
	: CDialog(CSalesHistoryCSVRecordEditDlg::IDD, pParent)
{
	m_bAdhoc = FALSE;
	PrepareControlArrays();
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::PrepareControlArrays()
{
	//{{AFX_DATA_INIT(CSalesHistoryCSVRecordEditDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT	
	m_comboFieldID[0] = IDC_COMBO_FIELD1;
	m_comboFieldID[1] = IDC_COMBO_FIELD2;
	m_comboFieldID[2] = IDC_COMBO_FIELD3;
	m_comboFieldID[3] = IDC_COMBO_FIELD4;
	m_comboFieldID[4] = IDC_COMBO_FIELD5;
	m_comboFieldID[5] = IDC_COMBO_FIELD6;
	m_comboFieldID[6] = IDC_COMBO_FIELD7;
	m_comboFieldID[7] = IDC_COMBO_FIELD8;
	m_checkTotalID[0] = IDC_CHECK_TOTAL1;
	m_checkTotalID[1] = IDC_CHECK_TOTAL2;
	m_checkTotalID[2] = IDC_CHECK_TOTAL3;
	m_checkTotalID[3] = IDC_CHECK_TOTAL4;
	m_checkTotalID[4] = IDC_CHECK_TOTAL5;
	m_checkTotalID[5] = IDC_CHECK_TOTAL6;
	m_checkTotalID[6] = IDC_CHECK_TOTAL7;
	m_checkTotalID[7] = IDC_CHECK_TOTAL8;
	m_checkHeaderID[0] = IDC_CHECK_HEADER1;
	m_checkHeaderID[1] = IDC_CHECK_HEADER2;
	m_checkHeaderID[2] = IDC_CHECK_HEADER3;
	m_checkHeaderID[3] = IDC_CHECK_HEADER4;
	m_checkHeaderID[4] = IDC_CHECK_HEADER5;
	m_checkHeaderID[5] = IDC_CHECK_HEADER6;
	m_checkHeaderID[6] = IDC_CHECK_HEADER7;
	m_checkHeaderID[7] = IDC_CHECK_HEADER8;
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSalesHistoryCSVRecordEditDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, MAX_LENGTH_GENERAL_NAME);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);

	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		DDX_Control(pDX, m_checkTotalID[nIndex], m_checkTotal[nIndex]);
		DDX_Control(pDX, m_checkHeaderID[nIndex], m_checkHeader[nIndex]);
		DDX_Control(pDX, m_comboFieldID[nIndex], m_comboField[nIndex]);
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSalesHistoryCSVRecordEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSalesHistoryCSVRecordEditDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD1, OnSelectField)
	ON_BN_CLICKED(IDC_STATIC_TIDY, OnButtonTidy)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD2, OnSelectField)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD3, OnSelectField)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD4, OnSelectField)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD5, OnSelectField)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD6, OnSelectField)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD7, OnSelectField)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD8, OnSelectField)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnToggleTopBottom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSalesHistoryCSVRecordEditDlg::OnInitDialog() 
{
	m_strName = GetName();
	CDialog::OnInitDialog();
	SetWindowText ( GetTitle() );
	FillCombos();
	return TRUE;  
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::FillCombos()
{	
	CWordArray arrayValidFields, arrayReportFields;
	PrepareCombos( arrayValidFields, arrayReportFields );

	int nCombo = 0;
	CReportConsolidationArray<CSortedStringItem> FieldsSoFar;
	for ( int nArrayPos = 0; ( nArrayPos < arrayReportFields.GetSize() ) && ( nCombo < 8 ); nArrayPos++ )
	{
		int nField = arrayReportFields.GetAt( nArrayPos );
		int nFieldType = nField % 100;
		
		CString strField;
		strField.Format ( "%3.3d", nFieldType );

		CSortedStringItem item;
		item.m_strItem = strField;
			
		int nFieldPos;
		if ( FieldsSoFar.Find( item, nFieldPos ) == TRUE )
			continue;
		
		FieldsSoFar.Consolidate( item );
		
		for ( int nSel = 1; nSel < m_comboField[ nCombo ].GetCount(); nSel++ )
		{
			if ( (WORD) nFieldType == m_comboField[ nCombo ].GetItemData( nSel ) )
			{
				m_comboField[nCombo].SetCurSel( nSel );
				m_checkTotal[nCombo].EnableWindow( m_comboType.GetCurSel() != 1 );
				m_checkHeader[nCombo].EnableWindow( TRUE );

				int x = nField / 100;
				m_checkTotal[nCombo].SetCheck ( ( x == 1 || x == 3 ) && ( m_comboType.GetCurSel() != 1 ) );
				m_checkHeader[nCombo].SetCheck ( x == 2 || x == 3 );
				nCombo++;
				break;
			}
		}
	}

	for ( ;nCombo < 8; nCombo++ )
	{
		m_checkTotal[ nCombo ].SetCheck( FALSE );
		m_checkHeader[ nCombo ].SetCheck( FALSE );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::OnSelectField() 
{
	SelectField();
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::SelectField()
{
	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		bool bField = ( m_comboField[nIndex].GetCurSel() != 0 );
		m_checkTotal[nIndex].EnableWindow ( bField && ( m_comboType.GetCurSel() != 1 ) );
		m_checkHeader[nIndex].EnableWindow ( bField );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::GetFieldSettings( CWordArray& arrayTemp )
{
	arrayTemp.RemoveAll();
	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		int nSel = m_comboField[nIndex].GetCurSel();
		if ( nSel < 1 || nSel >= m_comboField[nIndex].GetCount() )
			continue;

		int nField = m_comboField[nIndex].GetItemData( nSel );
		if ( m_checkTotal[nIndex].GetCheck() && ( m_comboType.GetCurSel() != 1 ) ) nField += 100;
		if ( m_checkHeader[nIndex].GetCheck() ) nField += 200;
		arrayTemp.Add( nField );
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::OnButtonTidy() 
{
	TidyFields();
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::OnToggleTopBottom() 
{
	for ( int nIndex = 0; nIndex < 8; nIndex++ )
	{
		bool bEnable = ( m_comboField[ nIndex ].GetCurSel() != 0 );
		m_checkHeader[ nIndex ].EnableWindow( bEnable );
		m_checkTotal[ nIndex ].EnableWindow( bEnable && ( m_comboType.GetCurSel() != 1 ) );	
	}
}

/**********************************************************************/

void CSalesHistoryCSVRecordEditDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		if ( SpecialValidate() == TRUE )
		{
			ProcessChanges();
			EndDialog ( IDOK );
		}
	}
}

/**********************************************************************/
