/**********************************************************************/
#include "..\SPosEcrManager5\PluFilterDlgEcrman.h"
#include "..\SPosEcrManager5\PropertySheetPluMixMatchEcrman.h"
/**********************************************************************/
#include "..\SPosStockManager5\PluFilterDlgStockman.h"
#include "..\SPosStockManager5\PropertySheetPluMixMatchStockman.h"
/**********************************************************************/
#include "NewRecordNumberDlg.h"
#include "FilterTypes.h"
#include "NodeTypes.h"
#include "BarcodePluNoTable.h"
#include "PromotionsCSVArray.h"
#include "PropertySheetPromotion.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPageMixMatchGeneral.h"
/**********************************************************************/

CPropPageMixMatchGeneral::CPropPageMixMatchGeneral() : CSSPropertyPage(CPropPageMixMatchGeneral::IDD), m_FilterArray( FILTERTYPE_DATABASE )
{
	//{{AFX_DATA_INIT(CPropPageMixMatchGeneral)
	m_strDescription = _T("");
	m_bEnable = FALSE;
	m_strPluNo = _T("");
	m_strCost = _T("");
	//}}AFX_DATA_INIT
	m_nLastPromoNo = 0;
	m_nLastBucketNoFreeItem = 0;
	m_nLastBucketNoFreeBucket = 1;
	m_nLastBucketNoFixedBucket = 1;
	m_strLastPluNo = "";
	m_strLastCost = "0.00";
	m_nUseGate = 0;
	m_bAutoLock = FALSE;
	m_pMixMatchRecord = NULL;
}

/**********************************************************************/

CPropPageMixMatchGeneral::~CPropPageMixMatchGeneral()
{
}

/**********************************************************************/

void CPropPageMixMatchGeneral::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageMixMatchGeneral)
	DDX_Control(pDX, IDC_BUTTON_EDIT_PROMO, m_buttonEditPromo);
	DDX_Control(pDX, IDC_BUTTON_EDIT_PLU, m_buttonEditPlu);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_COMBO_PROMO, m_comboPromo);
	DDX_Control(pDX, IDC_COMBO_BUCKET, m_comboBucket);
	DDX_Control(pDX, IDC_EDIT_PLUDESC, m_editPluDesc);
	DDX_Control(pDX, IDC_EDIT_PLUPRICE, m_editPluPrice);
	DDX_Control(pDX, IDC_STATIC_PLUNO3, m_staticPluNo3);
	DDX_Control(pDX, IDC_STATIC_PLUNO2, m_staticPluNo2);
	DDX_Control(pDX, IDC_STATIC_PLUNO, m_staticPluNo);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_comboAction);
	DDX_Control(pDX, IDC_DATEPICKER_START, m_DatePickerStart);
	DDX_Control(pDX, IDC_DATEPICKER_END, m_DatePickerEnd);
	DDX_Control(pDX, IDC_COMBO_ACTIVATION, m_comboActivation);
	DDX_Control(pDX, IDC_COMBO_ORDERMODE, m_comboOrderMode);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnable);
	DDX_Text(pDX, IDC_EDIT_PLUNO, m_strPluNo);
	DDX_Text(pDX, IDC_EDIT_COST, m_strCost);
	DDX_Check(pDX, IDC_CHECK_AUTOLOCK, m_bAutoLock);
	DDX_Control(pDX, IDC_STATIC_PRICEBAND, m_staticPriceBand);
	DDX_Control(pDX, IDC_COMBO_PRICEBAND, m_comboPriceBand);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, MixMatch::Description.Max);
	DDX_Text(pDX, IDC_EDIT_USEGATE, m_nUseGate);
	DDV_MinMaxInt(pDX, m_nUseGate, MixMatch::UseGate.Min,MixMatch::UseGate.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageMixMatchGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageMixMatchGeneral)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION, OnSelectAction)
	ON_CBN_SELCHANGE(IDC_COMBO_BUCKET, OnSelectBucket)
	ON_CBN_SELCHANGE(IDC_COMBO_PROMO, OnSelectPromo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO, OnKillFocusPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_COST, OnKillFocusCost)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PLU, OnButtonEditPlu)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_PROMO, OnButtonEditPromo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPageMixMatchGeneral::GetEditPluNo() { return GetEdit( IDC_EDIT_PLUNO ); }
CEdit* CPropPageMixMatchGeneral::GetEditCost() { return GetEdit( IDC_EDIT_COST ); }
/**********************************************************************/

BOOL CPropPageMixMatchGeneral::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	SubclassEdit ( IDC_EDIT_PLUNO, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_COST, SS_NUM_DP, 9, "%.2f" );
	SubclassEdit ( IDC_EDIT_USEGATE, SS_NUM, 3, "%d" );

	COleDateTime dateMin = COleDateTime( 2000, 1, 1, 0, 0, 0 );
	COleDateTime dateMax = COleDateTime( 9999, 12, 31, 0, 0, 0 );

	m_DatePickerStart.SetRange ( &dateMin, &dateMax );
	m_DatePickerEnd.SetRange ( &dateMin, &dateMax );
	m_DatePickerStart.SetFormat( "ddd dd MMM yyy" );
	m_DatePickerEnd.SetFormat( "ddd dd MMM yyy" );

	m_comboActivation.AddString( "Always On" );
	m_comboActivation.AddString( "Time Plan & Manual" );
	m_comboActivation.AddString( "Manual Only" );

	m_comboOrderMode.AddString( "Whole Order" );
	m_comboOrderMode.AddString( "Seat Only" );

	m_comboAction.AddString( "None" );
	m_comboAction.AddString( "Deduct cost of one product" );
	m_comboAction.AddString( "Apply promotion" );
	m_comboAction.AddString( "Cheapest one free" );
	m_comboAction.AddString( "Selling price by cost" );
	m_comboAction.AddString( "Selling price by PLU" );
	m_comboAction.AddString( "Selling price by PLU (2)" );
	m_comboAction.AddString( "One bucket free" );
	m_comboAction.AddString( "Set bucket price" );
	m_comboAction.SetItemData( 0 , MIXMATCH_ACTION_NONE );
	m_comboAction.SetItemData( 1 , MIXMATCH_ACTION_DISCOUNT_PLU );
	m_comboAction.SetItemData( 2 , MIXMATCH_ACTION_PROMO );
	m_comboAction.SetItemData( 3 , MIXMATCH_ACTION_FREE_ITEM );
	m_comboAction.SetItemData( 4 , MIXMATCH_ACTION_FIXCOST_AMOUNT );
	m_comboAction.SetItemData( 5 , MIXMATCH_ACTION_FIXCOST_PLU );
	m_comboAction.SetItemData( 6 , MIXMATCH_ACTION_FIXCOST_PLU_2 );
	m_comboAction.SetItemData( 7 , MIXMATCH_ACTION_FREE_BUCKET );
	m_comboAction.SetItemData( 8 , MIXMATCH_ACTION_FIXCOST_BUCKET );

	m_comboPriceBand.AddString("Always 1");
	m_comboPriceBand.AddString("Use Current");

	SetRecordControls();
	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageMixMatchGeneral::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageMixMatchGeneral::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	return bResult;
}

/**********************************************************************/

void CPropPageMixMatchGeneral::GetRecordData()
{
	m_strDescription = m_pMixMatchRecord -> GetDisplayName();
	m_bEnable = m_pMixMatchRecord -> GetEnableFlag();
	m_nUseGate = m_pMixMatchRecord -> GetUseGate();
	m_bAutoLock = m_pMixMatchRecord->GetAutoLockFlag();
}

/**********************************************************************/

void CPropPageMixMatchGeneral::SetRecordControls()
{
	m_DatePickerStart.SetTime(m_pMixMatchRecord->GetStartDate());
	m_DatePickerEnd.SetTime(m_pMixMatchRecord->GetEndDate());

	int nAction = m_pMixMatchRecord->GetAction();

	m_comboAction.SetCurSel(0);
	for (int n = 1; n < m_comboAction.GetCount(); n++)
	{
		if (m_comboAction.GetItemData(n) == nAction)
		{
			m_comboAction.SetCurSel(n);
			break;
		}
	}

	m_comboActivation.SetCurSel(m_pMixMatchRecord->GetActivationType() % 3);
	m_comboOrderMode.SetCurSel(m_pMixMatchRecord->GetOrderMode() % 2);
	m_comboPriceBand.SetCurSel(m_pMixMatchRecord->GetUseCurrentPriceBandFlag() ? 1 : 0);

	switch (nAction)
	{
	case MIXMATCH_ACTION_DISCOUNT_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU_2:
	{
		::FormatPluNo(m_pMixMatchRecord->GetActionPluNo(), m_strPluNo);
		m_strPluNo = BarcodePluNoTable.ConvertPlu(m_strPluNo);
		m_strCost = "";
		m_nLastPromoNo = 0;
		m_nLastBucketNoFreeItem = 0;
		m_nLastBucketNoFreeBucket = 1;
		m_nLastBucketNoFixedBucket = 1;
		m_strLastPluNo = m_strPluNo;
		m_strLastCost = "0.00";
	}
	break;

	case MIXMATCH_ACTION_PROMO:
		m_strPluNo = "";
		m_strCost = "";
		m_nLastPromoNo = m_pMixMatchRecord->GetActionPromo();
		m_nLastBucketNoFreeItem = 0;
		m_nLastBucketNoFreeBucket = 1;
		m_nLastBucketNoFixedBucket = 1;
		m_strLastPluNo = "";
		m_strLastCost = "0.00";
		break;

	case MIXMATCH_ACTION_FREE_ITEM:
		m_strPluNo = "";
		m_strCost = "";
		m_nLastPromoNo = 0;
		m_nLastBucketNoFreeItem = m_pMixMatchRecord->GetActionBucketFreeItem();
		m_nLastBucketNoFreeBucket = 1;
		m_nLastBucketNoFixedBucket = 1;
		m_strLastPluNo = "";
		m_strLastCost = "0.00";
		break;

	case MIXMATCH_ACTION_FREE_BUCKET:
		m_strPluNo = "";
		m_strCost = "";
		m_nLastPromoNo = 0;
		m_nLastBucketNoFreeItem = 0;
		m_nLastBucketNoFreeBucket = m_pMixMatchRecord->GetActionBucketFreeBucket();
		m_nLastBucketNoFixedBucket = 1;
		m_strLastPluNo = "";
		m_strLastCost = "0.00";
		break;

	case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		m_strPluNo = "";
		m_strCost.Format("%.2f", (double)m_pMixMatchRecord->GetActionCost() / 100.0);
		m_nLastPromoNo = 0;
		m_nLastBucketNoFreeItem = 0;
		m_nLastBucketNoFreeBucket = 1;
		m_nLastBucketNoFixedBucket = 1;
		m_strLastPluNo = "";
		m_strLastCost = m_strCost;
		break;

	case MIXMATCH_ACTION_FIXCOST_BUCKET:
		m_strPluNo = "";
		m_strCost.Format("%.2f", (double)m_pMixMatchRecord->GetActionCost() / 100.0);
		m_nLastPromoNo = 0;
		m_nLastBucketNoFreeItem = 0;
		m_nLastBucketNoFreeBucket = 1;
		m_nLastBucketNoFixedBucket = m_pMixMatchRecord->GetActionBucketFixedBucket();
		m_strLastPluNo = "";
		m_strLastCost = m_strCost;
		break;

	case MIXMATCH_ACTION_NONE:
	default:
		m_strPluNo = "";
		m_strCost = "";
		m_nLastPromoNo = 0;
		m_nLastBucketNoFreeItem = 0;
		m_nLastBucketNoFreeBucket = 1;
		m_nLastBucketNoFixedBucket = 1;
		m_strLastPluNo = "";
		m_strLastCost = "0.00";
		break;
	}

	GetEditCost()->SetWindowText(m_strCost);

	OnSelectAction();
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnSelectAction()
{
	int nSel = m_comboAction.GetCurSel();
	if (nSel < 0 || nSel >= m_comboAction.GetCount())
	{
		return;
	}

	bool bCost = FALSE;
	bool bBucket = FALSE;
	bool bPromo = FALSE;
	bool bPlu = FALSE;
	bool bPriceBand = FALSE;

	switch (m_comboAction.GetItemData(nSel))
	{
	case MIXMATCH_ACTION_NONE:
		m_staticPluNo.ShowWindow(SW_HIDE);
		break;

	case MIXMATCH_ACTION_FIXCOST_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU_2:
		bPriceBand = TRUE;
		//intentional fall through, do not break here

	case MIXMATCH_ACTION_DISCOUNT_PLU:
		bPlu = TRUE;
		m_staticPluNo.ShowWindow(SW_SHOW);
		m_staticPluNo.SetWindowText("Plu No");
		UpdatePluDetails(m_strLastPluNo);
		break;

	case MIXMATCH_ACTION_PROMO:
		bPromo = TRUE;
		m_staticPluNo.ShowWindow(SW_SHOW);
		m_staticPluNo.SetWindowText("Promotion");
		FillPromoCombo(m_nLastPromoNo);
		ShowAndEnableWindow(&m_comboPromo, TRUE, m_comboPromo.GetCount() > 1);
		ShowAndEnableWindow(&m_buttonEditPromo, EcrmanOptions.GetFeaturesPromotionFlag());
		break;

	case MIXMATCH_ACTION_FREE_ITEM:
		bBucket = TRUE;
		m_staticPluNo.ShowWindow(SW_SHOW);
		m_staticPluNo.SetWindowText("Bucket");
		FillBucketComboFreeItem(m_nLastBucketNoFreeItem);
		MoveControl(&m_comboBucket, 69, 172);
		break;

	case MIXMATCH_ACTION_FREE_BUCKET:
		bBucket = TRUE;
		m_staticPluNo.ShowWindow(SW_SHOW);
		m_staticPluNo.SetWindowText("Bucket");
		FillBucketComboFreeBucket(m_nLastBucketNoFreeBucket);
		MoveControl(&m_comboBucket, 69, 172);
		break;

	case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		bCost = TRUE;
		m_staticPluNo.ShowWindow(SW_SHOW);
		m_staticPluNo.SetWindowText("Cost");
		GetEditCost()->SetWindowText(m_strLastCost);
		break;

	case MIXMATCH_ACTION_FIXCOST_BUCKET:
		bCost = TRUE;
		m_staticPluNo.ShowWindow(SW_SHOW);
		m_staticPluNo.SetWindowText("Cost");
		GetEditCost()->SetWindowText(m_strLastCost);
		bBucket = TRUE;
		FillBucketComboFreeBucket(m_nLastBucketNoFixedBucket);
		MoveControl(&m_comboBucket, 159, 172);
	}

	ShowAndEnableWindow(GetEditPluNo(), bPlu);
	ShowAndEnableWindow(&m_buttonBrowse, bPlu);
	ShowAndEnableWindow(&m_buttonEditPlu, bPlu);
	ShowAndEnableWindow(&m_staticPluNo2, bPlu);
	ShowAndEnableWindow(&m_editPluDesc, bPlu);
	ShowAndEnableWindow(&m_staticPluNo3, bPlu);
	ShowAndEnableWindow(&m_editPluPrice, bPlu);
	ShowAndEnableWindow(&m_staticPriceBand, bPriceBand);
	ShowAndEnableWindow(&m_comboPriceBand, bPriceBand);

	ShowAndEnableWindow(GetEditCost(), bCost);
	ShowAndEnableWindow(&m_comboBucket, bBucket);

	if (FALSE == bPromo)
	{
		ShowAndEnableWindow(&m_comboPromo, FALSE);
		ShowAndEnableWindow(&m_buttonEditPromo, FALSE);
	}
}

/**********************************************************************/

void CPropPageMixMatchGeneral::FillPromoCombo( int nPromoNo )
{
	m_comboPromo.ResetContent();

	int nIdx = -1;

	m_comboPromo.AddString( "None" );
	m_comboPromo.SetItemData( 0, 0 );

	if ( nPromoNo == 0 )
		nIdx = 0;
	
	for ( int n = 0; n < DataManager.Promotion.GetSize(); n++ )
	{
		CPromotionsCSVRecord Promo;
		DataManager.Promotion.GetAt( n, Promo );

		int nTableNo = Promo.GetPromoNo();

		CString strPromo;
		strPromo.Format( "P%2.2d %s",
			nTableNo,
			Promo.GetDisplayName() );

		m_comboPromo.AddString( strPromo );
		m_comboPromo.SetItemData( n + 1, nTableNo );

		if ( nTableNo == nPromoNo )
			nIdx = n + 1;
	}

	if ( nIdx == -1 )
	{
		CString strPromo;
		strPromo.Format ( "P%2.2d Promo %d",
			nPromoNo,
			nPromoNo );

		m_comboPromo.InsertString( 1, strPromo );
		m_comboPromo.SetItemData( 1, nPromoNo );
		nIdx = 1;
	}

	m_comboPromo.SetCurSel( nIdx );
}

/**********************************************************************/

void CPropPageMixMatchGeneral::FillBucketComboFreeItem(int nBucketNo)
{
	m_comboBucket.ResetContent();

	m_comboBucket.AddString("Any");
	m_comboBucket.AddString("Bucket A");
	m_comboBucket.AddString("Bucket B");
	m_comboBucket.AddString("Bucket C");
	m_comboBucket.AddString("Bucket D");
	m_comboBucket.AddString("Bucket E");

	if ((nBucketNo >= 0) && (nBucketNo < m_comboBucket.GetCount()))
	{
		m_comboBucket.SetCurSel(nBucketNo);
	}
	else
	{
		m_comboBucket.SetCurSel(0);
	}
}

/**********************************************************************/

void CPropPageMixMatchGeneral::FillBucketComboFreeBucket(int nBucketNo)
{
	m_comboBucket.ResetContent();

	m_comboBucket.AddString("Bucket A");
	m_comboBucket.AddString("Bucket B");
	m_comboBucket.AddString("Bucket C");
	m_comboBucket.AddString("Bucket D");
	m_comboBucket.AddString("Bucket E");

	if ((nBucketNo >= 1) && (nBucketNo <= m_comboBucket.GetCount()))
	{
		m_comboBucket.SetCurSel(nBucketNo - 1);
	}
	else
	{
		m_comboBucket.SetCurSel(0);
	}
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnKillFocusCost()
{
	GetEditCost() -> GetWindowText( m_strLastCost );
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnButtonEditPlu() 
{
	KillFocusPluNo( TRUE );
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnKillFocusPluNo() 
{
	KillFocusPluNo( FALSE );
}

/**********************************************************************/

void CPropPageMixMatchGeneral::KillFocusPluNo( bool bEdit )
{
	CString strPluNo;
	GetEditPluNo() -> GetWindowText( strPluNo );
	strPluNo = BarcodePluNoTable.ConvertPlu( strPluNo );
	m_strLastPluNo = strPluNo;

	__int64 nPluNo = _atoi64(strPluNo);

	int nPluIdx;
	if ( DataManager.Plu.FindPluByNumber( nPluNo, nPluIdx ) == FALSE )
	{
		if ( SysInfo.IsValidSSPluNo( nPluNo ) == TRUE )
		{
			CString strMsg;
			strMsg.Format ( "Plu number %s does not exist in your database.\n\nWould you like to create this Plu now", 
				(const char*) strPluNo );

			if ( Prompter.YesNo( strMsg, TRUE ) == IDYES )
			{
				CPluCSVRecord PluRecord;
				PluRecord.SetPluNo( nPluNo );
				PluRecord.SetDefaultText();
				PluRecord.SetModifiedDateTimePlu();

				#ifndef STOCKMAN_SYSTEM
					DataManager.Plu.InsertRecord( PluRecord, TRUE );
					CPropertySheetPluMixMatchEcrman propSheet ( nPluIdx, this );
				#else
					DataManager.Plu.InsertRecord( PluRecord, "", TRUE );
					CPropertySheetPluMixMatchStockman propSheet ( nPluIdx, this );
				#endif

				if ( propSheet.DoModal() == IDOK )
					propSheet.SaveRecord();
			}
		}
		else
		{
			int nMaxBaseLen = SysInfo.GetMaxBasePluLen();

			CString strPluNo;
			strPluNo.Format( "%I64d", nPluNo );

			if ( strPluNo.GetLength() == nMaxBaseLen + 1 )
			{
				CString strMessage;
				strMessage.Format( "%s is not configured to allow %d digit PLU items.", 
					AfxGetAppName(),
					nMaxBaseLen + 1 );
					
				Prompter.ContactDealer( strMessage );
			}
		}
	}
	else if ( TRUE == bEdit )
	{
#ifndef STOCKMAN_SYSTEM
		CPropertySheetPluMixMatchEcrman propSheet ( nPluIdx, this );
#else
		CPropertySheetPluMixMatchStockman propSheet ( nPluIdx, this );
#endif

		if ( propSheet.DoModal() == IDOK )
			propSheet.SaveRecord();
	}

	UpdatePluDetails( strPluNo );
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnButtonBrowse() 
{
	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if (IDOK != nDlgResult)
	{
		return;
	}

	if (nPluIdx < 0)
	{
		return;
	}

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt( nPluIdx, PluRecord );
	CString strPluNo = PluRecord.GetPluNoString();
	UpdatePluDetails( strPluNo );
	m_strLastPluNo = strPluNo;
}

/**********************************************************************/

void CPropPageMixMatchGeneral::UpdatePluDetails( CString& strPluNo )
{	
	GetEditPluNo() -> SetWindowText( strPluNo );

	int nPluIdx;
	CPluCSVRecord PluRecord;
	if ( DataManager.Plu.FindPluByNumber( _atoi64(strPluNo), nPluIdx ) )
	{
		DataManager.Plu.GetAt( nPluIdx, PluRecord );
		m_editPluDesc.SetWindowText ( PluRecord.GetReportText() );

		CString strPrice;
		strPrice.Format ( "%.2f", PluRecord.GetPrice(0) );
		m_editPluPrice.SetWindowText( strPrice );
	}
	else
	{
		m_editPluDesc.SetWindowText ( "Unknown" );
		m_editPluPrice.SetWindowText ( "0.0" );
	}
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnSelectPromo() 
{
	int nSel = m_comboPromo.GetCurSel();
	if ( nSel >= 0 && nSel < m_comboPromo.GetCount() )
		m_nLastPromoNo = m_comboPromo.GetItemData( nSel );
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnButtonEditPromo()
{
	int nSel = m_comboPromo.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboPromo.GetCount() )
		return;

	int nOldPromoNo = m_comboPromo.GetItemData( nSel );
	
	if ( nOldPromoNo == 0 )
	{
		int nNewPromoNo = DataManager.Promotion.FindFirstFreeNumber();

		if ( nNewPromoNo < 1 )
		{
			Prompter.Error ( "All available promotions are already in use." );
			return;
		}

		CNewRecordNumberDlg dlg ( NODE_PROMOTION, this );
		dlg.m_nNumber = nNewPromoNo;
		if ( dlg.DoModal() != IDOK )
			return;

		int nPromoIdx;
		nNewPromoNo = dlg.m_nNumber;
		if ( DataManager.Promotion.FindTableByNumber ( nNewPromoNo, nPromoIdx ) )
		{
			Prompter.Error ( "The selected promotion is already in use." );
			return;
		}

		CPromotionsCSVRecord Promo;
		Promo.SetPromoNo( nNewPromoNo );
		Promo.SetDescription ( "" );
		DataManager.Promotion.InsertAt ( nPromoIdx, Promo );

		DataManager.Department.SetDeptZero( "None" );

		CPropertySheetPromotion propSheet( IDC_SPIN, FALSE, nPromoIdx, this );
		if ( propSheet.DoModal() == IDOK )
			propSheet.SaveRecord();

		DataManager.Department.SetDeptZero( "PROMO" );
	
		FillPromoCombo( nNewPromoNo );
		m_nLastPromoNo = nNewPromoNo;
		m_comboPromo.EnableWindow( m_comboPromo.GetCount() > 1 );
	}
	else
	{
		int nPromoIdx;
		if ( DataManager.Promotion.FindTableByNumber( nOldPromoNo, nPromoIdx ) == TRUE )
		{
			DataManager.Department.SetDeptZero( "None" );

			CPropertySheetPromotion propSheet( IDC_SPIN, FALSE, nPromoIdx, this );
			if ( propSheet.DoModal() == IDOK )
			{
				propSheet.SaveRecord();
				FillPromoCombo( nOldPromoNo );
			}

			DataManager.Department.SetDeptZero( "PROMO" );
		}
		else
		{
			if ( nOldPromoNo >= 1 && nOldPromoNo <= Promotions::PromoNo.Max )
			{
				CPromotionsCSVRecord Promo;
				Promo.SetPromoNo( nOldPromoNo );
				Promo.SetDescription ( "" );
				DataManager.Promotion.InsertAt ( nPromoIdx, Promo );

				DataManager.Department.SetDeptZero( "None" );

				CPropertySheetPromotion propSheet( IDC_SPIN, FALSE, nPromoIdx, this );
				if ( propSheet.DoModal() == IDOK )
					propSheet.SaveRecord();

				DataManager.Department.SetDeptZero( "PROMO" );
	
				FillPromoCombo( nOldPromoNo );
			}
		}
	}
}

/**********************************************************************/

void CPropPageMixMatchGeneral::OnSelectBucket() 
{
	int nAction = 0;
	int nSel = m_comboAction.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_comboAction.GetCount() ) )
		nAction = m_comboAction.GetItemData( nSel );

	switch( nAction )
	{
	case MIXMATCH_ACTION_FREE_ITEM:
		m_nLastBucketNoFreeItem = nSel;
		break;

	case MIXMATCH_ACTION_FREE_BUCKET:
		m_nLastBucketNoFreeBucket = nSel + 1;
		break;

	case MIXMATCH_ACTION_FIXCOST_BUCKET:
		m_nLastBucketNoFixedBucket = nSel + 1;
		break;
	}
}

/**********************************************************************/

bool CPropPageMixMatchGeneral::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPageMixMatchGeneral::SaveRecord()
{
	m_pMixMatchRecord -> SetDescription( m_strDescription );
	m_pMixMatchRecord -> SetEnableFlag ( m_bEnable != 0 );

	COleDateTime timeStart, timeEnd;
	m_DatePickerStart.GetTime( timeStart );
	m_DatePickerEnd.GetTime( timeEnd );
	
	if ( timeStart > timeEnd )
	{
		COleDateTime timeDummy = timeStart;
		timeStart = timeEnd;
		timeEnd = timeDummy;
	}
		
	m_pMixMatchRecord -> SetStartDate( timeStart );
	m_pMixMatchRecord -> SetEndDate( timeEnd );

	m_pMixMatchRecord -> SetActivationType( m_comboActivation.GetCurSel() );
	m_pMixMatchRecord -> SetOrderMode( m_comboOrderMode.GetCurSel() );
	m_pMixMatchRecord -> SetUseGate( m_nUseGate );

	m_pMixMatchRecord->SetAutoLockFlag(m_bAutoLock != 0);
	m_pMixMatchRecord->SetUseCurrentPriceBandFlag(m_comboPriceBand.GetCurSel() == 1);

	int nAction = 0;
	int nSel = m_comboAction.GetCurSel();
	if ((nSel >= 0) && (nSel < m_comboAction.GetCount()))
	{
		nAction = m_comboAction.GetItemData(nSel);
	}
	
	__int64 nParam1 = 1;

	switch( nAction )
	{
	case MIXMATCH_ACTION_DISCOUNT_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU:
	case MIXMATCH_ACTION_FIXCOST_PLU_2:
		m_pMixMatchRecord -> SetActionPluNo( nAction, _atoi64( m_strPluNo ) );
		break;

	case MIXMATCH_ACTION_PROMO:
		{
			int nSel = m_comboPromo.GetCurSel();

			if ( nSel >= 0 && nSel < m_comboPromo.GetCount() )
				m_pMixMatchRecord -> SetActionPromo( m_comboPromo.GetItemData(nSel) );
			else
				m_pMixMatchRecord -> SetActionPromo( 0 );
		}
		break;

	case MIXMATCH_ACTION_FREE_ITEM:
		m_pMixMatchRecord -> SetActionBucketFreeItem( m_comboBucket.GetCurSel() );
		break;

	case MIXMATCH_ACTION_FREE_BUCKET:
		m_pMixMatchRecord -> SetActionBucketFreeBucket( m_comboBucket.GetCurSel() + 1 );
		break;

	case MIXMATCH_ACTION_FIXCOST_AMOUNT:
		{
			double dCost = atof ( m_strCost );
			dCost *= 100.0;
			
			CString strCost;
			strCost.Format( "%.0f", dCost );
			m_pMixMatchRecord -> SetActionCost( atoi( strCost ) );
		}
		break;

	case MIXMATCH_ACTION_FIXCOST_BUCKET:
		{
			double dCost = atof ( m_strCost );
			dCost *= 100.0;
			
			CString strCost;
			strCost.Format( "%.0f", dCost );

			int nBucket = m_comboBucket.GetCurSel() + 1;

			m_pMixMatchRecord -> SetActionBucketFixedBucket( nBucket, atoi( strCost ) );
		}
		break;

	case MIXMATCH_ACTION_NONE:
	default:
		m_pMixMatchRecord -> SetActionNone();
		break;
	}
}

/**********************************************************************/

void CPropPageMixMatchGeneral::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
}

/**********************************************************************/
