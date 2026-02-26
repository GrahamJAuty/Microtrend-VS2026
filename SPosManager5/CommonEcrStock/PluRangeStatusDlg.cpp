/**********************************************************************/
#include "MaxLengths.h"
#include "BarcodePluNoTable.h"
#include "PriceSetItemManager.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "PluRangeStatusDlg.h"
/**********************************************************************/

CPluRangeStatusDlg::CPluRangeStatusDlg( CPluFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CPluRangeStatusDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangeStatusDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CPluRangeStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeStatusDlg)
	DDX_Control(pDX, IDC_COMBO_STATUS_OLD, m_comboStatusOld);
	DDX_Control(pDX, IDC_COMBO_STATUS_NEW, m_comboStatusNew);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeStatusDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeStatusDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangeStatusDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
/**********************************************************************/

BOOL CPluRangeStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );
	
	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
	{
		m_comboDeptName.EnableWindow ( FALSE );
		m_editDeptNo.EnableWindow ( FALSE );
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);

	m_comboStatusOld.AddString ( "All" );
	m_comboStatusOld.AddString ( "Location" );
	m_comboStatusOld.AddString ( "Master" );
	m_comboStatusOld.AddString ( "Excluded" );

	m_comboStatusNew.AddString ( "Location" );
	m_comboStatusNew.AddString ( "Master" );
	m_comboStatusNew.AddString ( "Excluded" );

	m_comboStatusOld.SetCurSel ( 0 );
	m_comboStatusNew.SetCurSel ( 0 );

	return TRUE;  
}

/**********************************************************************/

void CPluRangeStatusDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangeStatusDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluRangeStatusDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

int CPluRangeStatusDlg::UpdateStatus( int nOldComboStatus, int nNewComboStatus )
{
	int nStartIdx, nRange;
	__int64 nPluFrom = _atoi64( m_strPluFrom );
	__int64 nPluTo = _atoi64( m_strPluTo );
	m_FilterArray.GetArrayIdxRange( nPluFrom, nPluTo, nStartIdx, nRange );
	
	CDepartmentSetCSVRecord DepartmentSet;
	DepartmentSet.CreateDepartmentSet ( m_DepartmentSelector.GetDepartmentFilter() );

	int nCategory = CAnalysisCategoryCSVArray::GetFilterFromCombo(m_comboCategory);

	int nCount = 0;
	for ( int nOffset = 0; nOffset < nRange; nOffset++ )
	{
		StatusProgress.SetPos ( nOffset, nRange ); 

		CPluCSVRecord NewRecord;
		int nPluIdx = m_FilterArray.GetPluItemIndexAt ( nStartIdx + nOffset );	
		DataManager.Plu.GetAt ( nPluIdx, NewRecord ); 

		if ( ( NewRecord.GetPluNoInt() < nPluFrom ) || ( NewRecord.GetPluNoInt() > nPluTo ) )
			continue;

		//skip plus with non matching departments
		if ( DepartmentSet.GetDepartmentStatus ( NewRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ((nCategory != 9999) && (nCategory != NewRecord.GetBaseAnalysisCategory()))
			continue;

		int nOldRecordStatus = PriceSetItemManager.GetPluStatus ( nPluIdx );

		bool bMatch = FALSE;		
		switch( nOldComboStatus )
		{
		case 0:	bMatch = ( nOldRecordStatus == LOCALPLU_LOCAL );	break;
		case 1:	bMatch = ( nOldRecordStatus == LOCALPLU_MASTER );	break;
		case 2:	bMatch = ( nOldRecordStatus == LOCALPLU_EXCLUDE );	break;
		}
		
		if ( ( bMatch == TRUE ) || ( nOldComboStatus == -1 ) )
		{
			int nNewRecordStatus = -1;
			switch( nNewComboStatus )
			{
			case 0:	nNewRecordStatus = LOCALPLU_LOCAL;		break;
			case 1:	nNewRecordStatus = LOCALPLU_MASTER;		break;
			case 2:	nNewRecordStatus = LOCALPLU_EXCLUDE;	break;
			}

			if ( ( nNewRecordStatus != nOldRecordStatus ) && ( nNewRecordStatus != -1 ) )
			{
				switch ( nNewRecordStatus )
				{
				case LOCALPLU_LOCAL:	
					PriceSetItemManager.RememberMasterRecord( nPluIdx );
					PriceSetItemManager.SetPluStatus ( nPluIdx, LOCALPLU_LOCAL );	
					break;

				case LOCALPLU_MASTER:
				case LOCALPLU_EXCLUDE:
					PriceSetItemManager.RestoreMasterRecord( nPluIdx );
					PriceSetItemManager.SetPluStatus ( nPluIdx, nNewRecordStatus );	
					break;
				}
				nCount++;
			}
		}
	}
	return nCount;
}

/**********************************************************************/

void CPluRangeStatusDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluRangeStatusDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusDeptNumber();
		OnKillFocusPluNo();

		int nOldStatus = m_comboStatusOld.GetCurSel() - 1;
		int nNewStatus = m_comboStatusNew.GetCurSel();

		if ( nOldStatus == nNewStatus )
		{
			Prompter.Info ( "No records were changed" );
			EndDialog ( IDOK );
		}
		else
		{
			StatusProgress.Lock( TRUE, "Updating PLU status" );
			int nCount = UpdateStatus( nOldStatus, nNewStatus );
			StatusProgress.Unlock();
			
			switch ( nCount )
			{
			case 0:
				Prompter.Info ( "No records were changed" );
				break;

			case 1:
				Prompter.Info ( "1 record was changed" );
				break;

			default:
				{
					CString strBuf;
					strBuf.Format ( "%d records were changed", nCount );
					Prompter.Info ( strBuf );
				}
				break;
			}
			EndDialog ( IDOK );
		}
	}
}

/**********************************************************************/
