/**********************************************************************/
#include "PriceSetItemManager.h"
#include "SysInfo.h"
/**********************************************************************/
#include "PropPagePluProgram.h"
/**********************************************************************/

CPropPagePluProgram::CPropPagePluProgram() : 
	CSSPropertyPage(CPropPagePluProgram::IDD)
{
	//{{AFX_DATA_INIT(CPropPagePluProgram)
	m_nPriority = 0;
	//}}AFX_DATA_INIT
	
	m_IDPrinter[0] = IDC_CHECK_PRINTER1;
	m_IDPrinter[1] = IDC_CHECK_PRINTER2;
	m_IDPrinter[2] = IDC_CHECK_PRINTER3;
	m_IDPrinter[3] = IDC_CHECK_PRINTER4;
	m_IDPrinter[4] = IDC_CHECK_PRINTER5;
	m_IDPrinter[5] = IDC_CHECK_PRINTER6;
	m_IDPrinter[6] = IDC_CHECK_PRINTER7;
	m_IDPrinter[7] = IDC_CHECK_PRINTER8;
	m_IDPrinter[8] = IDC_CHECK_PRINTER9;
	m_IDPrinter[9] = IDC_CHECK_PRINTER10;
	m_IDPrinter[10] = IDC_CHECK_PRINTER11;
	m_IDPrinter[11] = IDC_CHECK_PRINTER12;
	m_IDPrinter[12] = IDC_CHECK_PRINTER13;
	m_IDPrinter[13] = IDC_CHECK_PRINTER14;
	m_IDPrinter[14] = IDC_CHECK_PRINTER15;
	m_IDPrinter[15] = IDC_CHECK_PRINTER16;
	
	m_pPluRecord = NULL;
	m_bReadOnly = TRUE;
	m_strInitialDir = ".\\";
}

/**********************************************************************/

CPropPagePluProgram::~CPropPagePluProgram()
{
}

/**********************************************************************/

void CPropPagePluProgram::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CPropPagePluProgram)

	DDX_Control( pDX, IDC_COMBO_OP_GROUP, m_comboOPGroup );

	for ( int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++ )
	{
		DDX_Control(pDX, m_IDPrinter[n], m_checkPrinter[n]);
	}

	/*****/
	
	DDX_Control(pDX, IDC_COMBO_PRICE_TYPE, m_comboPriceType);
	DDX_Control(pDX, IDC_COMBO_AGE_TYPE, m_comboAgeType);
	DDX_Control(pDX, IDC_CHECK_KPNOTALONE, m_checkKPNotAlone);
	DDX_Control(pDX, IDC_CHECK_KPTOPITEM, m_checkKPTopItem);
	DDX_Control(pDX, IDC_CHECK_AUTOCOVERSQTY, m_checkAutoCoversQty);
	DDX_Control(pDX, IDC_CHECK_RECEIPT, m_checkEANOnReceipt);
	DDX_Control(pDX, IDC_CHECK_SALES, m_checkSalesTracking);
	DDX_Control(pDX, IDC_CHECK_SOLDOUT, m_checkSoldOut);
	DDX_Control(pDX, IDC_CHECK_DISCOUNTABLE, m_checkDiscountable);
	DDX_Control(pDX, IDC_CHECK_STAFFDISCOUNT, m_checkStaffDiscount);
	DDX_Control(pDX, IDC_CHECK_FREETEXT, m_checkFreeText);
	DDX_Control(pDX, IDC_CHECK_FREETEXT_DESCRIPTION, m_checkFreeTextDescription);
	DDX_Control(pDX, IDC_CHECK_ACCOUNT_REQUIRED, m_checkAccountRequired);
	
	/*****/
	
	DDX_Control(pDX, IDC_TAB_IMAGE, m_TabImage);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_editHeight);
	DDX_Control(pDX, IDC_EDIT_IMAGENAME, m_editImageName);
	DDX_Control(pDX, IDC_BUTTON_IMAGE, m_buttonImage);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_CHECK_PRINT, m_checkPrint);
	DDX_Control(pDX, IDC_CHECK_MODIFIER, m_checkModifier);
	
	//}}AFX_DATA_MAP
	
	DDX_Text(pDX, IDC_EDIT_PRIORITY, m_nPriority);
	DDV_MinMaxInt(pDX, m_nPriority, Plu::Priority.Min, Plu::Priority.Max);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPagePluProgram, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPagePluProgram)
	ON_CBN_SELCHANGE(IDC_COMBO_OP_GROUP, OnSelectPrinterGroup)
	ON_BN_CLICKED(IDC_CHECK_FREETEXT, OnSelectFreeText)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMAGE, OnSelectTabImage)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE, OnButtonImage)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CPropPagePluProgram::GetEditPriority() { return GetEdit( IDC_EDIT_PRIORITY ); }
/**********************************************************************/

BOOL CPropPagePluProgram::OnInitDialog() 
{
	m_staticImage.SubclassDlgItem(IDC_STATIC_IMAGE,this);
	
	GetRecordData();
	CPropertyPage::OnInitDialog();
	SubclassEdit ( IDC_EDIT_PRIORITY, SS_NUM, 2, "%d" );
	
	m_TabImage.InsertItem( 0, "List Button" );
	m_TabImage.InsertItem( 1, "Kitchen Printer" );
	m_TabImage.SetCurSel(0);
	m_nTabImage = 0;
	
	SetRecordControls();

	if ( SysInfo.GetMaxBasePluLen() == 0 )
	{
		m_checkModifier.SetCheck( FALSE );
		m_checkModifier.EnableWindow( FALSE );
		m_checkModifier.ShowWindow( SW_HIDE );
	}

	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(40);
		m_comboOPGroup.SetTabStops( arrayStops );
	}

	for (int n = 0; n < 16; n++)
	{
		COPNameMiniInfo infoMini;
		infoMini.m_bDefer = TRUE;

		if (PriceSetItemManager.IsPluMaster() == FALSE )
		{
			DataManagerNonDb.OPNameLocation.GetOPName(n, infoMini);
		}

		if (TRUE == infoMini.m_bDefer)
		{
			DataManagerNonDb.OPNameDatabase.GetOPName(n, infoMini);
		}

		if (TRUE == infoMini.m_bDefer)
		{
			DataManagerNonDb.OPNameSystem.GetOPName(n, infoMini);
		}

		CString strName = infoMini.m_strName;
		if (strName == "")
		{
			strName.Format("Printer %d", infoMini.m_nPrinterNo);
		}

		m_checkPrinter[n].SetWindowText(strName);
	}

	FillOPGroupCombo();
	FillPriceTypeCombo();
	FillAgeTypeCombo();
	return TRUE;  
}

/**********************************************************************/

void CPropPagePluProgram::OnSelectTabImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	m_nTabImage = m_TabImage.GetCurSel();
	if ( ( m_nTabImage < 0 ) || ( m_nTabImage > 1 ) )
	{
		m_nTabImage = 0;
	}

	SelectTabImage();
}

/**********************************************************************/

void CPropPagePluProgram::SelectTabImage()
{
	bool bCanEnableDelete = ( ( PriceSetItemManager.IsPluMaster() == TRUE ) && ( m_bReadOnly == FALSE ) );
		
	bool bSavedAsBlank = FALSE;
	if ( 1 == m_nTabImage )
		bSavedAsBlank = DataManager.ResourceWrapperPlu.SavedAsBlank( PLUINFO_TYPE_KPIMAGE );
	else
		bSavedAsBlank = DataManager.ListKeyImageWrapperPlu.SavedAsBlank();

	if ( TRUE == bSavedAsBlank )
	{
		m_staticImage.SetBlankFlag( TRUE );
		m_staticImage.SetFailedFlag( FALSE );
		m_buttonDelete.EnableWindow( FALSE );
		m_staticImage.Invalidate();
		m_editWidth.SetWindowText( "" );
		m_editHeight.SetWindowText( "" );
	}
	else
	{
		bool bFailed = FALSE;
		bool bGotFile = FALSE;

		{
			CString strImageFilename = m_pPluRecord -> GetImageFilename();
			
			if ( ( 1 == m_nTabImage ) || ( strImageFilename != "" ) )
			{
				CString strFile = "";
				if ( 1 == m_nTabImage )
					strFile = DataManager.ResourceWrapperPlu.GetFilePathVisible( PLUINFO_TYPE_KPIMAGE );
				else
					strFile = DataManager.ListKeyImageWrapperPlu.GetFilePathVisible();
			
				if ( ::FileExists( strFile )== TRUE )
				{
					bGotFile = ( m_staticImage.LoadImage( strFile ) == TRUE );
					bFailed = ( FALSE == bGotFile );
				}
				else
				{
					bFailed = ( m_nTabImage != 1 );
				}
			}
		}

		if ( TRUE == bGotFile )
		{
			m_staticImage.SetBlankFlag( FALSE );
			m_staticImage.SetFailedFlag( FALSE );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( m_staticImage.GetWidthString() );
			m_editHeight.SetWindowText( m_staticImage.GetHeightString() );
		}
		else
		{
			m_staticImage.SetBlankFlag( TRUE );
			m_staticImage.SetFailedFlag( bFailed );
			m_staticImage.Invalidate();
			m_editWidth.SetWindowText( "" );
			m_editHeight.SetWindowText( "" );
		}

		m_buttonDelete.EnableWindow( ( bGotFile | bFailed ) & bCanEnableDelete );
	}

	if ( 1 == m_nTabImage )
	{
		m_buttonDelete.SetWindowText( "Delete\nImage" );
		m_checkPrint.ShowWindow( SW_SHOW );
		m_checkModifier.ShowWindow( SW_SHOW );
		m_editImageName.SetWindowText( "" );
	}
	else
	{
		m_buttonDelete.SetWindowText( "Remove\nImage" );
		m_checkPrint.ShowWindow( SW_HIDE );
		m_checkModifier.ShowWindow( SW_HIDE );
		m_editImageName.SetWindowText( m_pPluRecord -> GetImageFilename() );
	}
}

/**********************************************************************/

void CPropPagePluProgram::FillOPGroupCombo()
{
	if ( m_comboOPGroup.GetCount() == 0 )
	{
		m_comboOPGroup.AddItem( ",None" );

		for ( int n = 0; n < DataManagerNonDb.OPGroupSystem.GetOPGroupCount(); n++ )
		{
			COPGroupMiniInfo info;
			DataManagerNonDb.OPGroupSystem.GetOPGroup( n, info );

			CString strInfo;
			strInfo.Format( "G%2.2d,%s", 
				info.m_nGroupNo,
				(const char*) info.m_strName );

			m_comboOPGroup.AddItem( strInfo );
		}
	}

	int nSel = m_pPluRecord -> GetOrderPrinterGroup();
	if ( ( nSel >= 0 ) && ( nSel < m_comboOPGroup.GetCount() ) )
		m_comboOPGroup.SetCurSel( nSel );
	else
		m_comboOPGroup.SetCurSel( 0 );

	OnSelectPrinterGroup();
}

/**********************************************************************/

void CPropPagePluProgram::FillPriceTypeCombo()
{
	m_comboPriceType.ResetContent();
	m_comboPriceType.AddString ( "Preset" );
	m_comboPriceType.AddString ( "Non-Priced" );
	m_comboPriceType.AddString ( "Price Check" );
	m_comboPriceType.SetItemData ( 0, 2 );
	m_comboPriceType.SetItemData ( 1, 0 );
	m_comboPriceType.SetItemData ( 2, 1 );

	int nPriceType = m_pPluRecord -> GetPriceType();

	for ( int nIndex = 0; nIndex < m_comboPriceType.GetCount(); nIndex++ )
	{
		if ( m_comboPriceType.GetItemData ( nIndex ) == (WORD) nPriceType )
		{
			m_comboPriceType.SetCurSel ( nIndex );
			return;
		}
	}

	m_comboPriceType.InsertString ( 0, "??" );
	m_comboPriceType.SetItemData ( 0, nPriceType );
	m_comboPriceType.SetCurSel(0);
}

/**********************************************************************/

void CPropPagePluProgram::SavePriceType()
{
	int nSelect = m_comboPriceType.GetCurSel();
	if ( nSelect >= 0 && nSelect < m_comboPriceType.GetCount() )
		m_pPluRecord -> SetPriceType ( m_comboPriceType.GetItemData ( nSelect ) );
}

/**********************************************************************/

void CPropPagePluProgram::FillAgeTypeCombo()
{
	m_comboAgeType.ResetContent();
	m_comboAgeType.AddString ( "None" );
	m_comboAgeType.AddString ( "16" );
	m_comboAgeType.AddString ( "18" );
	m_comboAgeType.AddString ( "21" );
	m_comboAgeType.AddString ( "25" );
	m_comboAgeType.SetItemData ( 0, 0 );
	m_comboAgeType.SetItemData ( 1, 1 );
	m_comboAgeType.SetItemData ( 2, 2 );
	m_comboAgeType.SetItemData ( 3, 3 );
	m_comboAgeType.SetItemData ( 4, 4 );

	int nAgeType = m_pPluRecord -> GetAgeVerificationType();

	for ( int nIndex = 0; nIndex < m_comboAgeType.GetCount(); nIndex++ )
	{
		if ( m_comboAgeType.GetItemData ( nIndex ) == (WORD) nAgeType )
		{
			m_comboAgeType.SetCurSel ( nIndex );
			return;
		}
	}

	m_comboAgeType.InsertString ( 0, "??" );
	m_comboAgeType.SetItemData ( 0, nAgeType );
	m_comboAgeType.SetCurSel(0);
}

/**********************************************************************/

void CPropPagePluProgram::SaveAgeType()
{
	int nSelect = m_comboAgeType.GetCurSel();
	if ( nSelect >= 0 && nSelect < m_comboAgeType.GetCount() )
		m_pPluRecord -> SetAgeVerificationType ( m_comboAgeType.GetItemData ( nSelect ) );
}

/**********************************************************************/

void CPropPagePluProgram::OnSelectPrinterGroup()
{
	int nSel = m_comboOPGroup.GetCurSel();
	
	if ( nSel == 0 )
	{	
		bool bCatZero = ( m_pPluRecord -> GetBaseAnalysisCategory() == 0 );
	
		if ( ( PriceSetItemManager.IsPluMaster() == FALSE ) || ( m_bReadOnly == TRUE ) )
		{
			int nPluStatus = PriceSetItemManager.GetPluStatus();

			if ( ( nPluStatus != LOCALPLU_LOCAL ) || ( m_bReadOnly == TRUE ) )
			{
				for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
				{
					m_checkPrinter[n].EnableWindow(FALSE);
				}
			}
			else
			{
				for ( int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++ )
				{
					CString strLabel;
					CPluCSVRecord::GetOrderPrinterLabel( n + 1, strLabel );
					m_checkPrinter[n].EnableWindow ( PriceSetItemManager.CheckField ( strLabel ) && ( FALSE == bCatZero ) );
				}
			}
		}
		else
		{
			for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
			{
				m_checkPrinter[n].EnableWindow(FALSE == bCatZero);
			}
		
		}
	}
	else
	{
		COPGroupMiniInfo infoMini;
		infoMini.m_bDefer = TRUE;
	
		if ( PriceSetItemManager.IsPluMaster() == FALSE )
			DataManagerNonDb.OPGroupLocation.GetOPGroup( nSel - 1, infoMini );
			
		if ( TRUE == infoMini.m_bDefer )
			DataManagerNonDb.OPGroupDatabase.GetOPGroup( nSel - 1, infoMini );

		if ( TRUE == infoMini.m_bDefer )
			DataManagerNonDb.OPGroupSystem.GetOPGroup( nSel - 1, infoMini );

		COPGroupInfo infoGroup;
		infoGroup.SetEnableFlagsSystem( infoMini.m_nFlags );
		
		for ( int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++ )
		{
			m_checkPrinter[n].EnableWindow( FALSE );
			m_checkPrinter[n].SetCheck( infoGroup.GetEnableSystem(n) );
		}
	}
}

/**********************************************************************/

void CPropPagePluProgram::OnSelectFreeText()
{	
	bool bEnable = TRUE;

	if ( ( PriceSetItemManager.IsPluMaster() == FALSE ) || ( m_bReadOnly == TRUE ) )
	{
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		if ( ( nPluStatus != LOCALPLU_LOCAL ) || ( m_bReadOnly == TRUE ) )
			bEnable = FALSE;
		else
			bEnable = ( PriceSetItemManager.CheckField( Plu::FreeTextDescription.Label ) );
	}

	if ( m_checkFreeText.GetCheck() == 0 )
	{
		bEnable = FALSE;
		m_checkFreeTextDescription.SetCheck( FALSE );
	}

	m_checkFreeTextDescription.EnableWindow( bEnable );
}

/**********************************************************************/

BOOL CPropPagePluProgram::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPagePluProgram::OnSetActive() 
{
	GetRecordData();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	SetRecordControls();
	FillPriceTypeCombo();
	FillAgeTypeCombo();
	FillOPGroupCombo();
	return bResult;
}

/**********************************************************************/

void CPropPagePluProgram::Refresh()
{
	GetRecordData();
	UpdateData ( FALSE );
	SetRecordControls();
	FillPriceTypeCombo();
	FillAgeTypeCombo();
	FillOPGroupCombo();
}

/**********************************************************************/

bool CPropPagePluProgram::UpdateRecord()
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		SaveRecord();
		return TRUE;
	}
	return FALSE;
}

/**********************************************************************/

void CPropPagePluProgram::GetRecordData()
{
	m_nPriority = m_pPluRecord -> GetPriority();
}

/**********************************************************************/

void CPropPagePluProgram::SaveRecord()
{
	if (m_bReadOnly == TRUE)
		return;

	m_pPluRecord->SetOrderPrinterGroup(m_comboOPGroup.GetCurSel());

	for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
	{
		m_pPluRecord->SetOrderPrinterFlag(n, IsTicked(m_checkPrinter[n]));
	}

	m_pPluRecord->SetPriority(m_nPriority);

	/*****/

	SavePriceType();
	SaveAgeType();

	m_pPluRecord->SetKPNotAloneFlag(IsTicked(m_checkKPNotAlone));
	m_pPluRecord->SetKPConsolidateTopItemFlag(IsTicked(m_checkKPTopItem));
	m_pPluRecord->SetAutoCoversQtyFlag(IsTicked(m_checkAutoCoversQty));
	m_pPluRecord->SetEANOnReceiptFlag(IsTicked(m_checkEANOnReceipt));
	m_pPluRecord->SetSalesTrackingFlag(IsTicked(m_checkSalesTracking));
	m_pPluRecord->SetSoldOutFlag(IsTicked(m_checkSoldOut));
	m_pPluRecord->SetDiscountableFlag(IsTicked(m_checkDiscountable));
	m_pPluRecord->SetStaffDiscountFlag(IsTicked(m_checkStaffDiscount));
	m_pPluRecord->SetFreeTextFlag(IsTicked(m_checkFreeText));
	m_pPluRecord->SetFreeTextDescriptionFlag(IsTicked(m_checkFreeTextDescription));
	m_pPluRecord->SetAccountRequiredFlag(IsTicked(m_checkAccountRequired));

	/*****/

	m_pPluRecord->SetKPImagePrintFlag(IsTicked(m_checkPrint));
	m_pPluRecord->SetKPImageModifierFlag(IsTicked(m_checkModifier));
}

/**********************************************************************/

void CPropPagePluProgram::SetRecordControls()
{
	bool bCatZero = (m_pPluRecord->GetBaseAnalysisCategory() == 0);
	bool bOPGroup = (m_comboOPGroup.GetCurSel() != 0);

	if ((PriceSetItemManager.IsPluMaster() == FALSE) || (m_bReadOnly == TRUE))
	{
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		if ((nPluStatus != LOCALPLU_LOCAL) || (m_bReadOnly == TRUE))
		{
			m_comboOPGroup.EnableWindow(FALSE);

			for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
			{
				m_checkPrinter[n].EnableWindow(FALSE);
			}

			GetEditPriority()->EnableWindow(FALSE);

			/*****/

			m_comboPriceType.EnableWindow(FALSE);
			m_comboAgeType.EnableWindow(FALSE);
			m_checkKPNotAlone.EnableWindow(FALSE);
			m_checkKPTopItem.EnableWindow(FALSE);
			m_checkAutoCoversQty.EnableWindow(FALSE);
			m_checkEANOnReceipt.EnableWindow(FALSE);
			m_checkSalesTracking.EnableWindow(FALSE);
			m_checkSoldOut.EnableWindow(FALSE);
			m_checkDiscountable.EnableWindow(FALSE);
			m_checkStaffDiscount.EnableWindow(FALSE);
			m_checkFreeText.EnableWindow(FALSE);
			m_checkFreeTextDescription.EnableWindow(FALSE);
			m_checkAccountRequired.EnableWindow(FALSE);
		}
		else
		{
			m_comboOPGroup.EnableWindow(PriceSetItemManager.CheckField(Plu::OrderPrinterGroup.Label));

			for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
			{
				CString strLabel;
				CPluCSVRecord::GetOrderPrinterLabel(n + 1, strLabel);
				m_checkPrinter[n].EnableWindow(PriceSetItemManager.CheckField(strLabel) && (FALSE == bCatZero) && (FALSE == bOPGroup));
			}

			GetEditPriority()->EnableWindow(PriceSetItemManager.CheckField(Plu::Priority.Label) && (FALSE == bCatZero));

			/*****/

			m_comboPriceType.EnableWindow(PriceSetItemManager.CheckField(Plu::PriceType.Label));
			m_comboAgeType.EnableWindow(PriceSetItemManager.CheckField(Plu::AgeVerificationType.Label));
			m_checkKPNotAlone.EnableWindow(PriceSetItemManager.CheckField(Plu::KPNotAlone.Label));
			m_checkKPTopItem.EnableWindow(PriceSetItemManager.CheckField(Plu::KPConsolidateTopItem.Label));
			m_checkAutoCoversQty.EnableWindow(PriceSetItemManager.CheckField(Plu::AutoCoversQty.Label));
			m_checkEANOnReceipt.EnableWindow(PriceSetItemManager.CheckField(Plu::EANOnReceipt.Label));
			m_checkSalesTracking.EnableWindow(PriceSetItemManager.CheckField(Plu::SalesTracking.Label));
			m_checkSoldOut.EnableWindow(PriceSetItemManager.CheckField(Plu::SoldOut.Label));
			m_checkDiscountable.EnableWindow(PriceSetItemManager.CheckField(Plu::Discountable.Label));
			m_checkStaffDiscount.EnableWindow(PriceSetItemManager.CheckField(Plu::StaffDiscount.Label));
			m_checkFreeText.EnableWindow(PriceSetItemManager.CheckField(Plu::FreeText.Label));
			m_checkFreeTextDescription.EnableWindow(PriceSetItemManager.CheckField(Plu::FreeTextDescription.Label));
			m_checkAccountRequired.EnableWindow(PriceSetItemManager.CheckField(Plu::AccountRequired.Label));
		}
	}
	else
	{
		GetEditPriority()->EnableWindow(FALSE == bCatZero);

		for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
		{
			m_checkPrinter[n].EnableWindow(FALSE == bCatZero);
		}
	}

	if (m_checkFreeText.GetCheck() == 0)
	{
		m_checkFreeTextDescription.EnableWindow(FALSE);
		m_checkFreeTextDescription.SetCheck(FALSE);
	}

	//KP AND KEY IMAGE
	if ((PriceSetItemManager.IsPluMaster() == FALSE) || (m_bReadOnly == TRUE))
	{
		int nPluStatus = PriceSetItemManager.GetPluStatus();

		if ((nPluStatus != LOCALPLU_LOCAL) || (m_bReadOnly == TRUE))
		{
			m_checkPrint.EnableWindow(FALSE);
			m_checkModifier.EnableWindow(FALSE);
			m_buttonImage.EnableWindow(FALSE);
			m_buttonImage.ShowWindow(SW_HIDE);
			m_buttonDelete.EnableWindow(FALSE);
			m_buttonDelete.ShowWindow(SW_HIDE);
		}
		else
		{
			m_checkPrint.EnableWindow(PriceSetItemManager.CheckField(Plu::KPImagePrint.Label));
			m_checkModifier.EnableWindow(PriceSetItemManager.CheckField(Plu::KPImageModifier.Label));
			m_buttonImage.EnableWindow(FALSE);
			m_buttonImage.ShowWindow(SW_HIDE);
			m_buttonDelete.EnableWindow(FALSE);
			m_buttonDelete.ShowWindow(SW_HIDE);
		}
	}

	for (int n = 0; n < PLU_ORDER_PRINTERS_NOW; n++)
	{
		m_checkPrinter[n].SetCheck(m_pPluRecord->GetOrderPrinterFlag(n));
	}
	/*****/
	m_checkKPNotAlone.SetCheck(m_pPluRecord->GetKPNotAloneFlag());
	m_checkKPTopItem.SetCheck(m_pPluRecord->GetKPConsolidateTopItemFlag());
	m_checkAutoCoversQty.SetCheck(m_pPluRecord->GetAutoCoversQtyFlag());
	m_checkEANOnReceipt.SetCheck(m_pPluRecord->GetEANOnReceiptFlag());
	m_checkSalesTracking.SetCheck(m_pPluRecord->GetSalesTrackingFlag());
	m_checkSoldOut.SetCheck(m_pPluRecord->GetSoldOutFlag());
	m_checkDiscountable.SetCheck(m_pPluRecord->GetDiscountableFlag());
	m_checkStaffDiscount.SetCheck(m_pPluRecord->GetStaffDiscountFlag());
	m_checkFreeText.SetCheck(m_pPluRecord->GetFreeTextFlag());
	m_checkFreeTextDescription.SetCheck(m_pPluRecord->GetFreeTextDescriptionFlag());
	m_checkAccountRequired.SetCheck(m_pPluRecord->GetAccountRequiredFlag());
	/*****/
	m_checkPrint.SetCheck(m_pPluRecord->GetKPImagePrintFlag());
	m_checkModifier.SetCheck(m_pPluRecord->GetKPImageModifierFlag());

	SelectTabImage();
}

/**********************************************************************/

void CPropPagePluProgram::OnButtonImage() 
{
	CFileDialog dlgFile ( TRUE, "", "", OFN_NOCHANGEDIR|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, "Image Files (*.bmp;*.gif;*.jpg)|*.bmp;*.gif;*.jpg||", NULL );
	dlgFile.m_ofn.lpstrTitle = ( 1 == m_nTabImage ) ? "Select KP Image File" : "Select Key Image File";
	dlgFile.m_ofn.lpstrInitialDir = m_strInitialDir;
	
	if ( dlgFile.DoModal() == IDOK )
	{
		CString strFilepath = dlgFile.GetPathName();
		CString strFilename = dlgFile.GetFileName();
		CString strExt = dlgFile.GetFileExt();

		int nPathLen = strFilepath.GetLength();
		int nFileLen = strFilename.GetLength();

		if ( nPathLen >= nFileLen )
			m_strInitialDir = strFilepath.Left ( nPathLen - nFileLen );
			
		strExt.MakeUpper();
		if ( strExt == "BMP" || strExt == "GIF" || strExt == "JPG" )
		{
			if ( 1 == m_nTabImage )
			{
				DataManager.ResourceWrapperPlu.UpdateFileEdit002( PLUINFO_TYPE_KPIMAGE, strFilepath );
			}
			else
			{
				DataManager.ListKeyImageWrapperPlu.UpdateFileEdit002( strFilepath, nFileLen );
				m_pPluRecord -> SetImageFilename( DataManager.ListKeyImageWrapperPlu.GetNewKeyImageFileName() );
			}
			
			SetRecordControls();
		}
	}
}

/**********************************************************************/

void CPropPagePluProgram::OnButtonDelete() 
{
	if ( 1 == m_nTabImage )
	{
		if ( Prompter.YesNo( "Are you sure you wish to delete this image file", FALSE ) == IDYES )
		{
			DataManager.ResourceWrapperPlu.DeleteFileEdit002( PLUINFO_TYPE_KPIMAGE );
			DataManager.ResourceWrapperPlu.SetSavedAsBlankFlag( PLUINFO_TYPE_KPIMAGE, TRUE );
			SetRecordControls();
		}
	}
	else
	{
		if ( Prompter.YesNo( "Are you sure you wish to remove this image", FALSE ) == IDYES )
		{
			DataManager.ListKeyImageWrapperPlu.DeleteFileEdit002();
			DataManager.ListKeyImageWrapperPlu.SetSavedAsBlankFlag(TRUE );
			m_pPluRecord -> SetImageFilename( "" );
			SetRecordControls();
		}
	}
}

/**********************************************************************/

