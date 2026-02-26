/**********************************************************************/
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "BarcodePluNoTable.h"
#include "PriceHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluRangeProfitDlg.h"
/**********************************************************************/

CPluRangeProfitDlg::CPluRangeProfitDlg( CPluFilterArray& FilterArray, CWnd* pParent )
	: CSSDialog(CPluRangeProfitDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangeProfitDlg)
	//}}AFX_DATA_INIT
	m_nMethod = 0;
	m_dGPFixed = 0.0;
	m_dGPPercent = 0.0;
}

/**********************************************************************/

void CPluRangeProfitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeProfitDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Radio(pDX, IDC_RADIO_GP_FIXED, m_nMethod);
	//}}AFX_DATA_MAP
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
	DDX_Text(pDX, IDC_EDIT_GP_FIXED, m_dGPFixed );
	DDX_Text(pDX, IDC_EDIT_GP_PERCENT, m_dGPPercent );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeProfitDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeProfitDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_BN_CLICKED(IDC_RADIO_GP_PERCENT, OnRadioGpPercent)
	ON_BN_CLICKED(IDC_RADIO_GP_FIXED, OnRadioGpFixed)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangeProfitDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangeProfitDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_RANGE_END ); }
CEdit* CPluRangeProfitDlg::GetEditGPPercent() { return GetEdit( IDC_EDIT_GP_PERCENT ); }
CEdit* CPluRangeProfitDlg::GetEditGPFixed() { return GetEdit( IDC_EDIT_GP_FIXED ); }
/**********************************************************************/

BOOL CPluRangeProfitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_GP_FIXED, SS_NUM_DP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
	SubclassEdit ( IDC_EDIT_GP_PERCENT, SS_NUM_DP, 6, "%.2f" );
	
	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );
	
	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
	{
		m_comboDeptName.EnableWindow ( FALSE );
		m_editDeptNo.EnableWindow ( FALSE );
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);

	GetEditGPFixed() -> EnableWindow ( TRUE );
	GetEditGPPercent() -> EnableWindow ( FALSE );
		
	return TRUE;  
}

/**********************************************************************/

void CPluRangeProfitDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangeProfitDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluRangeProfitDlg::OnKillFocusPluNo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangeProfitDlg::OnRadioGpPercent() 
{
	GetEditGPFixed() -> EnableWindow ( FALSE );
	GetEditGPPercent() -> EnableWindow ( TRUE );
}

/**********************************************************************/

void CPluRangeProfitDlg::OnRadioGpFixed() 
{
	GetEditGPFixed() -> EnableWindow ( TRUE );
	GetEditGPPercent() -> EnableWindow ( FALSE );
}

/**********************************************************************/

int CPluRangeProfitDlg::UpdateProfit()
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

		__int64 nPluNo = NewRecord.GetPluNoInt(); 
		if ( ( nPluNo < nPluFrom ) || ( nPluNo > nPluTo ) )
			continue;

		//skip plus with fixed prices
		if ( NewRecord.GetGPType() == PLU_GPTYPE_VARIABLE )	
			continue;

		//skip plus with non matching departments
		if ( DepartmentSet.GetDepartmentStatus ( NewRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ((nCategory != 9999) && (nCategory != NewRecord.GetBaseAnalysisCategory()))
			continue;

		CTaxRateInfo TaxRateInfo;
		DataManager.TaxRates.GetTaxRateInfo( NewRecord.GetBaseTaxBandInt(), TaxRateInfo );
			
		double dCost = DataManager.Plu.GetPluPurchaseCost( NewRecord );
				
		if ( m_nMethod == 0 )
		{
			NewRecord.SetPrice ( 0, CPriceHelpers::GetPrice1GPFixed ( TaxRateInfo.m_dTaxRate, dCost, m_dGPFixed )  );
			NewRecord.SetGPType ( PLU_GPTYPE_FIXED );
			NewRecord.SetProfitMargin ( m_dGPFixed );
		}
		else
		{
			NewRecord.SetPrice ( 0, CPriceHelpers::GetPrice1GPPercent ( TaxRateInfo.m_dTaxRate, dCost, m_dGPPercent ) );
			NewRecord.SetGPType ( PLU_GPTYPE_PERCENT );
			NewRecord.SetProfitMargin ( m_dGPPercent );	
		}
			
		if ( DataManager.Plu.SavePluRecord ( nPluIdx, NewRecord ) != 0 )
			nCount++;
	}
	return nCount;
}

/**********************************************************************/

void CPluRangeProfitDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluRangeProfitDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusDeptNumber();
		OnKillFocusPluNo();

		if ( m_nMethod == 1 && m_dGPPercent >= 100.0 )
		{
			Prompter.Error ( "You cannot set a profit\nof 100% or greater." );
			return;
		}

		StatusProgress.Lock( TRUE, "Updating PLU profit" );
		int nCount = UpdateProfit();
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

		CDWordArray dw;
		dw.Add( nCount );
		MessageLogger.LogSystem( 2, 10, FALSE, dw );
		
		EndDialog ( IDOK );
	}
}

/**********************************************************************/








