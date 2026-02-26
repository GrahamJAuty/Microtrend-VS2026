/**********************************************************************/
#include "MaxLengths.h"
#include "MessageLogger.h"
#include "BarcodePluNoTable.h"
#include "PriceSetItemManager.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PluRangePriceDlg.h"
/**********************************************************************/
static const int FIXED_ADJUST = 0;
static const int PERCENT_ADJUST = 1;
static const int SET_AT = 2;
static const int COPY_PRICE = 3;
/**********************************************************************/

CPluRangePriceDlg::CPluRangePriceDlg( CPluFilterArray& FilterArray, CWnd* pParent /*=NULL*/)
	: CSSDialog(CPluRangePriceDlg::IDD, pParent), m_FilterArray ( FilterArray ),
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName )
{
	//{{AFX_DATA_INIT(CPluRangePriceDlg)
	m_nMethod = 0;
	//}}AFX_DATA_INIT

	for ( int nIndex = 0; nIndex < 10; nIndex++ )
	{
		m_bPrice [ nIndex ] = FALSE;
		m_dPrice [ nIndex ] = 0.0;
	}

	m_bPoints = FALSE;
	m_nPoints = 0;

	m_checkPriceID [ 0 ] = IDC_CHECK_PRICE1;
	m_checkPriceID [ 1 ] = IDC_CHECK_PRICE2;
	m_checkPriceID [ 2 ] = IDC_CHECK_PRICE3;
	m_checkPriceID [ 3 ] = IDC_CHECK_PRICE4;
	m_checkPriceID [ 4 ] = IDC_CHECK_PRICE5;
	m_checkPriceID [ 5 ] = IDC_CHECK_PRICE6;
	m_checkPriceID [ 6 ] = IDC_CHECK_PRICE7;
	m_checkPriceID [ 7 ] = IDC_CHECK_PRICE8;
	m_checkPriceID [ 8 ] = IDC_CHECK_PRICE9;
	m_checkPriceID [ 9 ] = IDC_CHECK_PRICE10;

	m_editPriceID [ 0 ] = IDC_EDIT_PRICE1;
	m_editPriceID [ 1 ] = IDC_EDIT_PRICE2;
	m_editPriceID [ 2 ] = IDC_EDIT_PRICE3;
	m_editPriceID [ 3 ] = IDC_EDIT_PRICE4;
	m_editPriceID [ 4 ] = IDC_EDIT_PRICE5;
	m_editPriceID [ 5 ] = IDC_EDIT_PRICE6;
	m_editPriceID [ 6 ] = IDC_EDIT_PRICE7;
	m_editPriceID [ 7 ] = IDC_EDIT_PRICE8;
	m_editPriceID [ 8 ] = IDC_EDIT_PRICE9;
	m_editPriceID [ 9 ] = IDC_EDIT_PRICE10;

	m_nCopyLevel = 1;
	m_nMethod = FIXED_ADJUST;
}

/**********************************************************************/

void CPluRangePriceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangePriceDlg)
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Radio(pDX, IDC_RADIO_FIXED, m_nMethod);
	//}}AFX_DATA_MAP

	for ( int nIndex = 0; nIndex < 10; nIndex++ )
	{
		DDX_Control(pDX, m_checkPriceID[nIndex], m_checkPrice[nIndex]);
		DDX_Check(pDX, m_checkPriceID[nIndex], m_bPrice[nIndex]);
		DDX_Text(pDX, m_editPriceID[nIndex], m_dPrice[nIndex] );	
	}

	DDX_Control(pDX, IDC_CHECK_POINTS, m_checkPoints);
	DDX_Check(pDX, IDC_CHECK_POINTS, m_bPoints);

	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_START, m_strPluFrom);
	DDV_IntegerString(pDX, m_strPluFrom, "1", Super.PluEnd() );
	DDX_IntegerString(pDX, IDC_EDIT_PLU_RANGE_END, m_strPluTo);
	DDV_IntegerString(pDX, m_strPluTo, "1", Super.PluEnd() );
	DDX_Text(pDX, IDC_EDIT_LEVEL, m_nCopyLevel );
	DDV_MinMaxInt( pDX, m_nCopyLevel, 1, MAX_PLU_PRICES );
	DDX_Text(pDX, IDC_EDIT_POINTS, m_nPoints );
	DDV_MinMaxInt( pDX, m_nPoints, -Plu::Points.Max, Plu::Points.Max );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangePriceDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangePriceDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelchangeComboDeptname)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNumber)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_END, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_LEVEL, OnKillFocusLevel)
	ON_BN_CLICKED(IDC_RADIO_COPY, OnRadioCopy)
	ON_BN_CLICKED(IDC_RADIO_FIXED, OnRadioFixed)
	ON_BN_CLICKED(IDC_RADIO_PERCENT, OnRadioPercent)
	ON_BN_CLICKED(IDC_RADIO_SETAT, OnRadioSetAt)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_RANGE_START, OnKillFocusPluNo)
	ON_BN_CLICKED(IDC_STATIC_RANGE, OnStaticRange)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_PRICE1, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE2, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE3, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE4, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE5, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE6, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE7, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE8, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE9, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_PRICE10, OnCheckPrice)
	ON_BN_CLICKED(IDC_CHECK_POINTS, OnCheckPrice)
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPluRangePriceDlg::GetEditPluFrom() { return GetEdit ( IDC_EDIT_PLU_RANGE_START ); }
CEdit* CPluRangePriceDlg::GetEditPluTo() { return GetEdit ( IDC_EDIT_PLU_RANGE_END ); }
CEdit* CPluRangePriceDlg::GetEditPrice( int n ) { return GetEdit ( m_editPriceID[n] ); }
CEdit* CPluRangePriceDlg::GetEditCopyLevel() { return GetEdit ( IDC_EDIT_LEVEL ); }
CEdit* CPluRangePriceDlg::GetEditPoints() { return GetEdit ( IDC_EDIT_POINTS ); }
/**********************************************************************/

BOOL CPluRangePriceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLU_RANGE_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_RANGE_END, SS_NUM, Super.MaxPluLen(), "%s" );
	
	for ( int nIndex = 0; nIndex < 10; nIndex++ )
		SubclassEdit ( m_editPriceID[nIndex], SS_NUM_SDP, Super.MaxPriceLen(), "%.*f", SysInfo.GetDPValue() );
	
	SubclassEdit( IDC_EDIT_POINTS, SS_NUM_S, 5, "%d" );
	SubclassEdit ( IDC_EDIT_LEVEL, SS_NUM, 1, "%d" );

	m_editDeptNo.LimitText ( MAX_LENGTH_DEPTNO );
	m_DepartmentSelector.FillDepartmentComboDatabase( m_FilterArray.GetDepartmentFilter(), DEPARTMENT_SELECTOR_NORMAL );

	if ( m_FilterArray.GetDepartmentFilter() != DEPARTMENTSET_ALL )
	{
		m_comboDeptName.EnableWindow ( FALSE );
		m_editDeptNo.EnableWindow ( FALSE );
	}

	CAnalysisCategoryCSVArray::PrepareFilterCombo(m_comboCategory);
	
	if ( PriceSetItemManager.IsPluMaster() == TRUE )
	{
		for ( int nIndex = 0; nIndex < 10; nIndex++ )
			m_bAllowPrice [ nIndex ] = TRUE;

		m_bAllowPoints = TRUE;
	}
	else
	{
		for ( int nIndex = 0; nIndex < 10; nIndex++ )
		{
			CString strField;
			CPluCSVRecord::GetPriceLabel( nIndex + 1, strField );
			m_bAllowPrice [ nIndex ] = PriceSetItemManager.CheckField ( strField );
		}

		m_bAllowPoints = PriceSetItemManager.CheckField( Plu::Points.Label );
	}

	SetPriceEnables();
	return TRUE; 
}

/**********************************************************************/

void CPluRangePriceDlg::OnKillFocusDeptNumber() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
}

/**********************************************************************/

void CPluRangePriceDlg::OnSelchangeComboDeptname() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
}

/**********************************************************************/

void CPluRangePriceDlg::OnKillFocusPluNo() 
{
	UpdateData ( TRUE, FALSE );
	m_strPluFrom = BarcodePluNoTable.ConvertPlu ( m_strPluFrom );
	m_strPluTo = BarcodePluNoTable.ConvertPlu ( m_strPluTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangePriceDlg::OnKillFocusLevel() 
{
	UpdateData( TRUE, FALSE );
	m_nMethod = COPY_PRICE;
	UpdateData ( FALSE );
}

/**********************************************************************/

void CPluRangePriceDlg::OnCheckPrice() 
{
	UpdateData( TRUE, FALSE );
	SetPriceEnables();
}

/**********************************************************************/

void CPluRangePriceDlg::SetPriceEnables()
{
	for ( int nIndex = 0; nIndex < 10; nIndex++ )
	{
		m_checkPrice[nIndex].EnableWindow ( m_bAllowPrice [ nIndex ] );
		GetEditPrice(nIndex) -> EnableWindow ( m_bPrice[nIndex] && m_bAllowPrice[nIndex] && m_nMethod != COPY_PRICE );
	}

	if ( COPY_PRICE == m_nMethod )
	{
		GetEditCopyLevel() -> EnableWindow ( TRUE  );
		m_checkPoints.EnableWindow( FALSE );
		GetEditPoints() -> EnableWindow( FALSE );
	}
	else
	{
		GetEditCopyLevel() -> EnableWindow ( FALSE  );
		m_checkPoints.EnableWindow( m_bAllowPoints );
		GetEditPoints() -> EnableWindow( m_bPoints );
	}

	if ( SysInfo.GotLoyaltyOrSmartPayLink() == FALSE )
	{
		m_checkPoints.ShowWindow( SW_HIDE );
		GetEditPoints() -> ShowWindow( SW_HIDE );
		m_checkPoints.EnableWindow( FALSE );
		GetEditPoints() -> EnableWindow( FALSE );
	}
}

/**********************************************************************/

void CPluRangePriceDlg::OnRadioCopy() 
{
	m_nMethod = COPY_PRICE;
	SetPriceEnables();	
}

/**********************************************************************/

void CPluRangePriceDlg::OnRadioFixed() 
{
	m_nMethod = FIXED_ADJUST;
	SetPriceEnables();
}

/**********************************************************************/

void CPluRangePriceDlg::OnRadioPercent() 
{
	m_nMethod = PERCENT_ADJUST;
	SetPriceEnables();	
}

/**********************************************************************/

void CPluRangePriceDlg::OnRadioSetAt() 
{
	m_nMethod = SET_AT;
	SetPriceEnables();
}

/**********************************************************************/

bool CPluRangePriceDlg::IsRelevant()
{
	if ( PriceSetItemManager.IsPluMaster() == TRUE ) 
		return TRUE;

	for ( int nIndex = 0; nIndex < MAX_PLU_PRICES; nIndex++ )
	{
		CString strField;
		CPluCSVRecord::GetPriceLabel( nIndex + 1, strField );

		if ( PriceSetItemManager.CheckField ( strField ) == TRUE ) 
			return TRUE;
	}

	if ( PriceSetItemManager.CheckField( Plu::Points.Label ) == TRUE )
		return TRUE;

	return FALSE;
}

/**********************************************************************/

int CPluRangePriceDlg::UpdatePrices()
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

		//skip plus with fixed profit
		if ( NewRecord.GetGPType() != PLU_GPTYPE_VARIABLE )	
			continue;

		if ( DepartmentSet.GetDepartmentStatus ( NewRecord.GetBaseDeptNo() ) == FALSE )
			continue;

		if ((nCategory != 9999) && (nCategory != NewRecord.GetBaseAnalysisCategory()))
			continue;

		//cannot change prices for database record in price set
		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			if ( PriceSetItemManager.GetPluStatus( nPluIdx ) != LOCALPLU_LOCAL )
				continue;

		for ( int nPriceLevel = 0; nPriceLevel < 10; nPriceLevel++ )
		{
			if ( m_bPrice [ nPriceLevel ] == TRUE && m_bAllowPrice [ nPriceLevel ] == TRUE )
			{
				switch ( m_nMethod )
				{
				case SET_AT:
					if ( m_dPrice [ nPriceLevel ] >= 0.0 ) NewRecord.SetPrice ( nPriceLevel, m_dPrice [ nPriceLevel ] );
					break;

				case FIXED_ADJUST:
					{
						double dNewPrice = NewRecord.GetPrice ( nPriceLevel ) + m_dPrice [ nPriceLevel ];
						if ( dNewPrice >= 0.0 ) NewRecord.SetPrice ( nPriceLevel, dNewPrice );
					}
					break;
						
				case PERCENT_ADJUST:
					{
						double dChange = ( NewRecord.GetPrice ( nPriceLevel ) * m_dPrice [ nPriceLevel ] ) / 100.0;
						double dNewPrice = NewRecord.GetPrice ( nPriceLevel ) + dChange;
						if ( dNewPrice >= 0.0 ) NewRecord.SetPrice ( nPriceLevel, dNewPrice );
					}
					break;

				case COPY_PRICE:
					NewRecord.SetPrice ( nPriceLevel, NewRecord.GetPrice ( m_nCopyLevel - 1 ) );
					break;
				}
			}		
		}

		if ( SysInfo.GotLoyaltyOrSmartPayLink() && ( TRUE == m_bPoints ) )
		{
			switch ( m_nMethod )
			{
				case SET_AT:
					NewRecord.SetPoints ( m_nPoints );
					break;

				case FIXED_ADJUST:
					{
						int nNewPoints = NewRecord.GetPoints() + m_nPoints;
						NewRecord.SetPoints ( nNewPoints );
					}
					break;
						
				case PERCENT_ADJUST:
					{
						int nChange = ( NewRecord.GetPoints() * m_nPoints ) / 100;
						int nNewPoints = NewRecord.GetPoints() + nChange;
						NewRecord.SetPoints ( nNewPoints );
					}
					break;
				}
			}
		if ( DataManager.Plu.SavePluRecord ( nPluIdx, NewRecord ) != 0 )
			nCount++;
	}
	return nCount;
}

/**********************************************************************/

void CPluRangePriceDlg::OnStaticRange() 
{
	CString strStart = "1";
	::AddLeading ( strStart, Super.MaxPluLen(), '0' );
	GetEditPluFrom() -> SetWindowText ( strStart );
	
	CString strEnd = "9";
	::AddLeading ( strEnd, Super.MaxPluLen(), '9' );
	GetEditPluTo() -> SetWindowText( strEnd );
}

/**********************************************************************/

void CPluRangePriceDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	OnKillFocusDeptNumber();
	OnKillFocusPluNo();

	StatusProgress.Lock( TRUE, "Updating PLU prices" );
	int nCount = UpdatePrices();
	StatusProgress.Unlock();
	
	CString strBuf;
	switch ( nCount )
	{
	case 0:
		Prompter.Info ( "No records were changed" );
		break;

	case 1:
		Prompter.Info ( "1 record was changed" );
		break;

	default:
		strBuf.Format ( "%d records were changed", nCount );
		Prompter.Info ( strBuf );
		break;
	}

	CDWordArray dw;
	dw.Add( nCount );
	MessageLogger.LogSystem( 2, 4, FALSE, dw );
	
	EndDialog ( IDOK );
}

/**********************************************************************/
