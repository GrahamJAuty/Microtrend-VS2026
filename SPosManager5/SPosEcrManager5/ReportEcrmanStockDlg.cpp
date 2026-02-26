/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "..\CommonEcrStock\DefEditDlg.h"
#include "FilterTypes.h"
#include "..\CommonEcrStock\ListDataPresetDlg.h"
#include "MaxLengths.h"
#include "Password.h"
#include "PluSalesHistoryFileArray.h"
#include "..\CommonEcrStock\PresetReportManager.h"
#include "..\CommonEcrTray\ReportPluDelivery.h"
#include "RepSpawn.h"
#include "..\CommonEcrStock\SelectPresetActionDlg.h"
#include "StatusProgress.h"
/**********************************************************************/
#include "ReportEcrmanStockDlg.h"
/**********************************************************************/

CReportEcrmanStockDlg::CReportEcrmanStockDlg( int nPresetNo, bool bAutoRunPreset, CWnd* pParent /*=NULL*/)
	: CSSDialog(CReportEcrmanStockDlg::IDD, pParent), 
	m_DepartmentSelector ( m_editDeptNo, m_comboDeptName ),
	m_EcrmanSupplierSelector( m_editSupplier, m_comboSupplier, TRUE ),
	m_DateSelectorRange( m_comboDates, m_DatePickerFrom, m_DatePickerTo ),
	m_FilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CReportEcrmanStockDlg)
	//}}AFX_DATA_INIT
	m_nPresetNo = nPresetNo;
	m_nPresetError = 0;
	m_bAutoRunPreset = bAutoRunPreset;

	m_nDepartmentFilter = DEPARTMENTSET_ALL;
	m_nSuppNo = ECRMANSUPPLIER_ALL;

	CDataManagerInfo info;
	DataManagerNonDb.OpenEcrmanSupplier( DB_READONLY, info );

	m_nLastDbIdx = -1;
	m_lastDateFrom = COleDateTime( 1998, 1, 1, 0, 0, 0 );
	m_lastTimeFrom = m_lastDateFrom;
}

/**********************************************************************/

CReportEcrmanStockDlg::~CReportEcrmanStockDlg()
{
	CDataManagerInfo info;
	DataManagerNonDb.CloseEcrmanSupplier( info );
}

/**********************************************************************/

void CReportEcrmanStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CReportEcrmanStockDlg)

	DDX_Control(pDX, IDC_COMBO_DATES, m_comboDates);
	DDX_Control(pDX, IDC_DATEPICKER_FROM, m_DatePickerFrom);
	DDX_Control(pDX, IDC_STATIC_TO, m_staticTo);
	DDX_Control(pDX, IDC_DATEPICKER_TO, m_DatePickerTo);
	DDX_Control(pDX, IDC_TIMEPICKER, m_TimePicker);
	
	DDX_Control(pDX, IDC_CHECK_SPECIFY, m_checkSpecify);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_TO, m_buttonBrowseTo);
	DDX_Control(pDX, IDC_BUTTON_BROWSE_FROM, m_buttonBrowseFrom);
	DDX_Control(pDX, IDC_CHECK_DEPTSORT, m_checkDeptSort);
	DDX_Control(pDX, IDC_EDIT_DEPTNO, m_editDeptNo);
	DDX_Control(pDX, IDC_COMBO_DEPTNAME, m_comboDeptName);
	DDX_Control(pDX, IDC_EDIT_SUPPLIER, m_editSupplier);
	DDX_Control(pDX, IDC_COMBO_SUPPLIER, m_comboSupplier);
	DDX_Control(pDX, IDC_CHECK_ZEROSKIP, m_checkZeroSkip);
	
	DDX_Control(pDX, IDC_COMBO_LOCATION, m_comboDatabase);
	
	DDX_Control(pDX, IDC_STATIC_PRESET, m_staticPreset);
	DDX_Control(pDX, IDC_BUTTON_PRESET, m_buttonPreset);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_COMBO_REPORT, m_comboReport);
	DDX_Control(pDX, IDC_CHECK_SUPPSORT, m_checkSupplierSort);
	DDX_Control(pDX, IDC_CHECK_ZEROSKIP, m_checkZeroSkip);
	
	DDX_Control(pDX, IDC_CHECK_DEPTSORT, m_checkDeptSort);
	DDX_Control(pDX, IDC_CHECK_SUPPSORT, m_checkSupplierSort);
	
	//}}AFX_DATA_MAP
	
	DDX_IntegerString(pDX, IDC_EDIT_PLU_FROM, m_strPluNoFrom);
	DDX_IntegerString(pDX, IDC_EDIT_PLU_TO, m_strPluNoTo);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CReportEcrmanStockDlg, CDialog)
	//{{AFX_MSG_MAP(CReportEcrmanStockDlg)
	
	ON_CBN_SELCHANGE(IDC_COMBO_DATES, OnSelectDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_FROM, OnChangeDateFrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATEPICKER_TO, OnChangeDateTo)
	
	ON_BN_CLICKED(IDC_CHECK_SPECIFY, OnTogglePluRange)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FROM, OnButtonBrowseFrom)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_TO, OnButtonBrowseTo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_FROM, OnKillFocusPluFrom)
	ON_EN_KILLFOCUS(IDC_EDIT_PLU_TO, OnKillFocusPluTo)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPTNO, OnKillFocusDeptNo)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTNAME, OnSelectDept)
	ON_EN_KILLFOCUS(IDC_EDIT_SUPPLIER, OnKillFocusSupplier)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPLIER, OnSelectSupplier)
	
	ON_CBN_SELCHANGE(IDC_COMBO_LOCATION, OnSelectLocation)
	
	ON_BN_CLICKED(IDC_BUTTON_PRESET, OnButtonPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelectPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_REPORT, OnSelectReport)
	
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CReportEcrmanStockDlg::GetEditPluFrom() { return GetEdit( IDC_EDIT_PLU_FROM ); }
CEdit* CReportEcrmanStockDlg::GetEditPluTo() { return GetEdit( IDC_EDIT_PLU_TO ); }
/**********************************************************************/

BOOL CReportEcrmanStockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DeptAndPluComboHelpers.SetControlPointers( NULL, &m_LocationSelector, &m_DepartmentSelector );

	m_DateSelectorRange.InitialiseDateControls();
	m_DateSelectorRange.AddStandardReportRanges();
	m_DateSelectorRange.SetComboSelection( 0, szRANGE_TODAY );

	m_TimePicker.SetFormat ( "HH:mm" );
	m_TimePicker.SetTime( COleDateTime( 2018, 1, 1, 0, 0, 0 ) );
	MoveControl( &m_TimePicker, 67, 62 );

	{
		int nResult = m_comboDates.FindStringExact( 0, EcrmanOptions.GetReportsDefaultDateRange() );
			
		if ( nResult != CB_ERR )
			m_comboDates.SetCurSel( nResult );
		else
			m_comboDates.SetCurSel( 0 );

		OnSelectDate();
	}

	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_DATABASE );

	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboDatabase );

	CDataManagerInfo info;
	DataManager.OpenDatabaseReadOnly( 0, info, FALSE );
	
	m_editDeptNo.LimitText( MAX_LENGTH_DEPTNO );
	m_DeptAndPluComboHelpers.UpdateCombos();

	m_editSupplier.LimitText(2);
	m_EcrmanSupplierSelector.FillEcrmanSupplierCombo( m_nSuppNo );

	bool bPluApparent = PasswordArray.GetEnable( PasswordTicks::ReportsPluApparent );
	bool bPluDelivery = PasswordArray.GetEnable( PasswordTicks::ReportsPluDelivery ) & EcrmanOptions.AllowPluDeliveryAudit();

	if ( bPluApparent == FALSE && bPluDelivery == FALSE )
		bPluDelivery = TRUE;

	if ( bPluApparent == TRUE )
	{
		AddToReportCombo ( "Apparent Stock", REPORT_PLUSTOCK_APPARENT );
		AddToReportCombo ( "Apparent Valuation", REPORT_PLUSTOCK_VALUATION );

		if ( ( bPluDelivery == TRUE ) && ( DealerFlags.GetApparentStockDateFlag() == TRUE ) )
		{
			AddToReportCombo ( "Previous Stock", REPORT_PLUSTOCK_APPARENT_DATE );
			AddToReportCombo ( "Previous Valuation", REPORT_PLUSTOCK_VALUATION_DATE );
			AddToReportCombo ( "Previous Replenishment", REPORT_PLUSTOCK_REPLENISH_DATE );
		}

		AddToReportCombo ( "Reorder Report", REPORT_PLUSTOCK_REORDER );
	}

	if ( bPluDelivery == TRUE )
	{
		AddToReportCombo ( "Delivery Audit", REPORT_PLUSTOCK_DEL_AUDIT );
		AddToReportCombo ( "Delivery Totals", REPORT_PLUSTOCK_DEL_TOTAL );
		AddToReportCombo ( "Delivery Valuation", REPORT_PLUSTOCK_DEL_VALUE );
	}

	m_comboReport.SetCurSel (0);
	OnSelectReport();

	SubclassEdit ( IDC_EDIT_PLU_FROM, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_PLU_TO, SS_NUM, Super.MaxPluLen(), "%s" );

	GetEditPluFrom() -> EnableWindow ( FALSE );
	GetEditPluTo() -> EnableWindow ( FALSE );
	m_buttonBrowseFrom.EnableWindow ( FALSE );
	m_buttonBrowseTo.EnableWindow ( FALSE );

	PresetReportManager.EcrStock.SetPasswordAccess();
	FillPresetCombo(-1);

	if ( m_nPresetNo != 0 )
	{
		m_nPresetError = PresetReportManager.SelectFromCombo( NODE_PRESET_ECR_STOCK, m_comboPreset, m_nPresetNo );

		if ( 0 == m_nPresetError )
		{
			OnSelectPreset();

			if ( TRUE == m_bAutoRunPreset )
				OnOK();
		}

		if ( TRUE == m_bAutoRunPreset )
		{
			EndDialog( IDCANCEL );
			return FALSE;
		}			
	}

	bool bPreset = PasswordArray.CheckLocationAccessSystem();
	ShowAndEnableWindow( &m_buttonPreset, bPreset );
	ShowAndEnableWindow( &m_staticPreset, FALSE == bPreset );
	
	return TRUE;  
}

/**********************************************************************/

void CReportEcrmanStockDlg::AddToReportCombo( const char* szLabel, int nType )
{
	CString strLabel = szLabel;
	int nPos = m_comboReport.AddString( strLabel );
	m_comboReport.SetItemData( nPos, nType );
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnSelectLocation()
{
	m_DeptAndPluComboHelpers.UpdateCombos();
}

/**********************************************************************/

void CReportEcrmanStockDlg::BuildZeroDateMap( int nDbIdx, COleDateTime& dateFrom, COleDateTime& timeFrom )
{
	if ( ( nDbIdx != m_nLastDbIdx ) || ( dateFrom != m_lastDateFrom ) || ( timeFrom != m_lastTimeFrom ) )
	{
		m_nLastDbIdx = nDbIdx;
		m_lastDateFrom = dateFrom;
		m_lastTimeFrom = timeFrom;

		m_StockLevelBuffer.RemoveAll();

		CPluStockLevelBuffer buffer;
		buffer.m_bWorkBackFromCurrent = TRUE;
		buffer.m_strZeroDateTime = "";
		for ( int n = 0; n < DataManager.Plu.GetSize(); n++ )
		{
			CPluCSVRecord PluRecord;
			DataManager.Plu.GetAt( n, PluRecord );
			buffer.m_dApparentQty = PluRecord.GetApparentLevel();
			m_StockLevelBuffer.Add( buffer );
		}

		CString strReportDateTime;
		strReportDateTime.Format( "%4.4d%2.2d%2.2d%2.2d%2.2d",
			dateFrom.GetYear(),
			dateFrom.GetMonth(),
			dateFrom.GetDay(),
			timeFrom.GetHour(),
			timeFrom.GetMinute() );

		{
			CSSFile fileAudit;
			CFilenameUpdater FnUp ( SysFiles::PluStockAudit, nDbIdx );
			if ( fileAudit.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
			{
				StatusProgress.Lock( TRUE, "Checking for cleared stock" );

				CString strBuffer;
				while ( fileAudit.ReadString( strBuffer ) == TRUE )
				{
					StatusProgress.SetPos( fileAudit );

					CCSVPluDelivery csvIn ( strBuffer );

					if ( csvIn.GetClearFlag() == TRUE )
					{
						int nPluIdx;
						if ( DataManager.Plu.FindPluByNumber( csvIn.GetPluNo(), nPluIdx ) == TRUE )
						{
							COleDateTime oleLineDate;
							CString strZeroDateTime = csvIn.GetDate();

							if ( strZeroDateTime.GetLength() == 8 )
							{
								::MakeYearFirstDate( strZeroDateTime );
								oleLineDate = COleDateTime ( atoi(strZeroDateTime.Left(4)), atoi(strZeroDateTime.Mid(4,2)), atoi(strZeroDateTime.Right(2)), 0, 0, 0 ); 	

								if ( oleLineDate.m_status == COleDateTime::valid )
								{
									strZeroDateTime += csvIn.GetTime();

									if ( strZeroDateTime == strReportDateTime )
									{
										m_StockLevelBuffer[nPluIdx].m_strZeroDateTime = strReportDateTime;
										m_StockLevelBuffer[nPluIdx].m_bWorkBackFromCurrent = FALSE;
										m_StockLevelBuffer[nPluIdx].m_dApparentQty = 0.0;
									}
									else if ( strZeroDateTime > strReportDateTime )
									{
										m_StockLevelBuffer[nPluIdx].m_bWorkBackFromCurrent = FALSE;
										m_StockLevelBuffer[nPluIdx].m_dApparentQty = 0.0;
									}
									else if ( strZeroDateTime > m_StockLevelBuffer[nPluIdx].m_strZeroDateTime )
										m_StockLevelBuffer[nPluIdx].m_strZeroDateTime = strZeroDateTime;
								}
							}
						}
					}
				}

				StatusProgress.Unlock();
			}
		}

		{
			CSSFile fileAudit;
			CFilenameUpdater FnUp ( SysFiles::PluStockAudit, nDbIdx );
			if ( fileAudit.Open( FnUp.GetFilenameToUse(), "rb" ) == TRUE )
			{
				StatusProgress.Lock( TRUE, "Adjusting for deliveries" );

				CString strBuffer;
				while ( fileAudit.ReadString( strBuffer ) == TRUE )
				{
					StatusProgress.SetPos( fileAudit );

					CCSVPluDelivery csvIn ( strBuffer );

					if ( csvIn.GetClearFlag() == FALSE )
					{
						int nPluIdx;
						if ( DataManager.Plu.FindPluByNumber( csvIn.GetPluNo(), nPluIdx ) == TRUE )
						{
							COleDateTime oleLineDate;
							CString strDeliveryDateTime = csvIn.GetDate();

							if ( strDeliveryDateTime.GetLength() == 8 )
							{
								::MakeYearFirstDate( strDeliveryDateTime );
								oleLineDate = COleDateTime ( atoi(strDeliveryDateTime.Left(4)), atoi(strDeliveryDateTime.Mid(4,2)), atoi(strDeliveryDateTime.Right(2)), 0, 0, 0 ); 	

								if ( oleLineDate.m_status == COleDateTime::valid )
								{
									strDeliveryDateTime += csvIn.GetTime();

									if ( m_StockLevelBuffer[nPluIdx].m_bWorkBackFromCurrent == FALSE )
									{
										if ( ( strDeliveryDateTime > m_StockLevelBuffer[nPluIdx].m_strZeroDateTime ) && ( strDeliveryDateTime <= strReportDateTime ) )
											m_StockLevelBuffer[nPluIdx].m_dApparentQty += csvIn.GetQty();
									}
									else
									{
										if  ( strDeliveryDateTime > strReportDateTime )
											m_StockLevelBuffer[nPluIdx].m_dApparentQty -= csvIn.GetQty();
									}

									if  ( strDeliveryDateTime > strReportDateTime )
										m_StockLevelBuffer[nPluIdx].m_dDeliveryQty += csvIn.GetQty();
								}
							}
						}
					}
				}

				StatusProgress.Unlock();
			}
		}

		CString strSalesCheckStart = strReportDateTime;

		for ( int n = 0; n < m_StockLevelBuffer.GetSize(); n++ )
		{
			if ( m_StockLevelBuffer[n].m_bWorkBackFromCurrent == FALSE )
			{
				CString str = m_StockLevelBuffer[n].m_strZeroDateTime;

				if ( str == "" )
				{
					strSalesCheckStart = "199801010000";
					break;
				}

				if ( str < strSalesCheckStart )
					strSalesCheckStart = str;
			}
		}

		{
			CSessionCSVRecord ReportSession;
			ReportSession.SetSessionStartHour( 0 );
			ReportSession.SetSessionStartMinute( 0 );
			ReportSession.SetSessionStartHour( 23 );
			ReportSession.SetSessionStartMinute( 59 );
			ReportSession.SetContinuousFlag( FALSE );
			ReportSession.SetEODFlag( FALSE );

			COleDateTime dateStart = COleDateTime( atoi( strSalesCheckStart.Left(4) ), atoi( strSalesCheckStart.Mid(4,2) ), atoi( strSalesCheckStart.Mid(6,2) ), 0, 0, 0 );   

			if ( dateStart.m_status != COleDateTime::valid )
				dateStart = COleDateTime ( 1998, 1, 1, 0, 0, 0 );

			COleDateTime dateNow = COleDateTime::GetCurrentTime();

			DataManagerNonDb.SessionDateTimeFilter.Reset();
			DataManagerNonDb.SessionDateTimeFilter.SetDates( dateStart, dateNow, ReportSession );
		}

		CEposSelectArray SelectArray;
		SelectArray.SelectDatabase( nDbIdx );
		SelectArray.MakeList();

		CPluSalesHistoryFileArray arrayFileInfo( SelectArray );
		arrayFileInfo.SetEposSalesFlag( TRUE );
		arrayFileInfo.SetImportSalesFlag( FALSE );
		arrayFileInfo.SetManualSalesFlag( FALSE );
		arrayFileInfo.SetEODModeFlag( FALSE );
		arrayFileInfo.SetGuestToTerminalFlag( TRUE );
		arrayFileInfo.SetSaleTimeFlag( FALSE );
		arrayFileInfo.BuildFileList();

		__int64 nTarget = arrayFileInfo.GetTotalFileSize();
		__int64 nProgress = 0;

		StatusProgress.Lock( TRUE, "Adjusting for sales" );

		for ( int nFileIdx = 0; nFileIdx < arrayFileInfo.GetFileCount(); nFileIdx++ )
		{
			CTermFileListInfo infoFile;
			arrayFileInfo.GetFileInfo( nFileIdx, infoFile );

			CSSFile fileSales;
			if ( fileSales.Open ( infoFile.m_strFilename, "rb", _SH_DENYNO ) == FALSE )
				continue;

			CPluSalesLine csvSales( infoFile.IsLegacySalesFolder() );
			
			int nLinesToRead;
			CString strBuffer;
			if ( ::FindFirstPluLine( fileSales, strBuffer, nLinesToRead ) == TRUE )
			{
				do
				{
					StatusProgress.SetPos( nProgress + fileSales.Tell(), nTarget );

					csvSales.ParseLine( strBuffer );

					//CHECK FOR RELEVANT LINE TYPES
					int nLineType = csvSales.GetLineType();
					if ( nLineType != CSVPLUDATA_PLU_SALE )
						continue;

					CSQLPluNoInfo infoPluNo;
					infoPluNo.m_nEposPluNo = csvSales.GetPluNoNew();
					::ProcessPluNo( infoPluNo, FALSE, TRUE );
							
					if ( FALSE == infoPluNo.m_bValid )
						continue;

					int nPluIdx;
					if ( DataManager.Plu.FindPluByNumber( infoPluNo.m_nBasePluNo, nPluIdx ) == TRUE )
					{
						CString strSaleDateTime = infoFile.m_strDateTran + csvSales.GetTranTime();
						strSaleDateTime = strSaleDateTime.Left(12);

						if ( m_StockLevelBuffer[nPluIdx].m_bWorkBackFromCurrent == FALSE )
						{
							if ( ( strSaleDateTime > m_StockLevelBuffer[nPluIdx].m_strZeroDateTime ) && ( strSaleDateTime <= strReportDateTime ) )
							{
								double dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );
								m_StockLevelBuffer[nPluIdx].m_dApparentQty -= ( csvSales.GetSaleQty() * dMultiplier );
							}
						}
						else
						{
							if  ( strSaleDateTime > strReportDateTime )
							{
								double dMultiplier = DataManager.Modifier.GetMultiplierForQty( infoPluNo.m_nModifier );
								m_StockLevelBuffer[nPluIdx].m_dApparentQty += ( csvSales.GetSaleQty() * dMultiplier );
							}
						}
					}
				}
				while ( fileSales.ReadString( strBuffer ) == TRUE );
			}
			nProgress += fileSales.GetLength();
			fileSales.Close();
		}

		StatusProgress.Unlock();
	}
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnSelectDate() 
{
	m_DateSelectorRange.UpdateDateControls();	
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnChangeDateFrom(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.OnChangeFromDate();
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnChangeDateTo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_DateSelectorRange.CheckForChangedDate();
	*pResult = 0;
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnTogglePluRange() 
{
	bool bEnable = ( m_checkSpecify.GetCheck() != 0 );
	GetEditPluFrom() -> EnableWindow ( bEnable );
	GetEditPluTo() -> EnableWindow ( bEnable );
	m_buttonBrowseFrom.EnableWindow ( bEnable );
	m_buttonBrowseTo.EnableWindow ( bEnable );	
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnButtonBrowseFrom() 
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
		{
			m_strPluNoFrom = DataManager.Plu.GetPluNoString ( nPluIdx );
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnButtonBrowseTo() 
{
	UpdateData( TRUE, FALSE );

	int nDlgResult, nPluIdx;
	::ShowPluFilterDlg( m_FilterArray, this, nDlgResult, nPluIdx );

	if ( IDOK == nDlgResult )
	{
		if ( nPluIdx >= 0 && nPluIdx < DataManager.Plu.GetSize() )
		{
			m_strPluNoTo = DataManager.Plu.GetPluNoString ( nPluIdx );
			UpdateData ( FALSE );
		}
	}
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnKillFocusPluFrom() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoFrom = BarcodePluNoTable.ConvertPlu ( m_strPluNoFrom );
	UpdateData ( FALSE );	
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnKillFocusPluTo() 
{
	UpdateData( TRUE, FALSE );
	m_strPluNoTo = BarcodePluNoTable.ConvertPlu ( m_strPluNoTo );
	UpdateData ( FALSE );
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnKillFocusDeptNo() 
{
	m_DepartmentSelector.SelectDepartmentFromEditBox();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnSelectDept() 
{
	m_DepartmentSelector.SelectDepartmentFromCombo();
	m_nDepartmentFilter = m_DepartmentSelector.GetDepartmentFilter();
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnKillFocusSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromEditBox();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnSelectSupplier() 
{
	m_EcrmanSupplierSelector.SelectEcrmanSupplierFromCombo();
	m_nSuppNo = m_EcrmanSupplierSelector.GetEcrmanSupplierNo();	
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnButtonPreset() 
{
	CPresetReportInfo Preset;
	CreatePreset( Preset );
	PresetReportManager.PresetButton( NODE_PRESET_ECR_STOCK, m_comboPreset, Preset, m_pParentWnd );
}

/**********************************************************************/

void CReportEcrmanStockDlg::CreatePreset( CPresetReportInfo& Base ) 
{
	CPresetReportInfoEcrStock Preset;

	int nSel = m_comboReport.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_comboReport.GetCount() ) )
		Preset.SetReportType( m_comboReport.GetItemData( nSel ) );

	CString strDateType;
	m_comboDates.GetWindowText( strDateType );
	Preset.SetDateType( strDateType );

	CString strDateFrom;
	COleDateTime dateFrom;
	m_DatePickerFrom.GetTime( dateFrom );
	::GetStringFromOleDate( dateFrom, strDateFrom );
	Preset.SetDateFrom( strDateFrom );
	
	CString strDateTo;
	COleDateTime dateTo;
	m_DatePickerTo.GetTime( dateTo );
	::GetStringFromOleDate( dateTo, strDateTo );
	Preset.SetDateTo( strDateTo );

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	Base.SetPresetConLevel( LocSelEntity.GetConType() );

	int nEntityNo1, nEntityNo2;
	CPresetReportArray::GetLocSelEntityNo( LocSelEntity, nEntityNo1, nEntityNo2 );
	Base.SetPresetEntityNo1( nEntityNo1 );
	Base.SetPresetEntityNo2( nEntityNo2 );

	Preset.SetPluRangeFlag( m_checkSpecify.GetCheck() != 0 );

	CString strPluFrom;
	GetEditPluFrom() -> GetWindowText( strPluFrom );
	Preset.SetPluFrom( strPluFrom );

	CString strPluTo;
	GetEditPluTo() -> GetWindowText( strPluTo );
	Preset.SetPluTo( strPluTo );

	Preset.SetDepartmentFilter( m_DepartmentSelector.GetDepartmentFilter() );
	Preset.SetSupplierFilter( m_EcrmanSupplierSelector.GetEcrmanSupplierNo() );
	Preset.SetZeroSkipFlag( m_checkZeroSkip.GetCheck() != 0 );

	Preset.SetDepartmentSortFlag( m_checkDeptSort.GetCheck() != 0 );
	Preset.SetSupplierSortFlag( m_checkSupplierSort.GetCheck() != 0 );

	CString str;
	Preset.ToString( str );
	Base.SetOtherFields( str );
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnSelectPreset() 
{
	int nSel = m_comboPreset.GetCurSel() - 1;
	if ( nSel < 0 || nSel >= PresetReportManager.EcrStock.GetSize() )
		return;

	CPresetReportInfo Base;
	PresetReportManager.EcrStock.GetAt( nSel, Base );

	CPresetReportInfoEcrStock Preset;
	{
		CString str = Base.GetOtherFields();
		Preset.FromString( str );
	}

	int nReportType = Preset.GetReportType();
	for ( int n = 0; n < m_comboReport.GetCount(); n++ )
	{
		if ( m_comboReport.GetItemData(n) == nReportType )
		{
			m_comboReport.SetCurSel(n);
			break;
		}
	}

	//DATE GROUP
	int nResult = m_comboDates.FindStringExact( 0, Preset.GetDateType() );
		
	if ( nResult != CB_ERR )
		m_comboDates.SetCurSel( nResult );
	
	CString strDateFrom = Preset.GetDateFrom();
	CString strDateTo = Preset.GetDateTo();

	COleDateTime dateFrom, dateTo;
	
	if ( ::GetOleDateFromString( strDateFrom, dateFrom ) == TRUE )
		m_DatePickerFrom.SetTime( dateFrom );

	if ( ::GetOleDateFromString( strDateTo, dateTo ) == TRUE )
		m_DatePickerTo.SetTime( dateTo );

	OnSelectDate();
	OnSelectReport(); //MUST DO THIS AFTER SELECT DATE

	int nConLevel = Base.GetPasswordConLevel();
	int nEntity1 = Base.GetPasswordEntityNo1();
	int nEntity2 = Base.GetPasswordEntityNo2();
	
	int nEntityIdx = 0;
	CPresetReportArray::GetEntityIdx( nConLevel, nEntity1, nEntity2, nEntityIdx );	
	m_LocationSelector.ForceSelection( nConLevel, nEntityIdx );
	OnSelectLocation();

	m_checkSpecify.SetCheck( Preset.GetPluRangeFlag() );
	OnTogglePluRange();

	CString strPluFrom = Preset.GetPluFrom();
	if ( ::TestNumeric( strPluFrom ) == TRUE )
		if ( strPluFrom.GetLength() <= Super.MaxPluLen() )
			GetEditPluFrom() -> SetWindowText( strPluFrom );

	CString strPluTo = Preset.GetPluTo();
	if ( ::TestNumeric( strPluTo ) == TRUE )
		if ( strPluTo.GetLength() <= Super.MaxPluLen() )
			GetEditPluTo() -> SetWindowText( strPluTo );

	m_DepartmentSelector.UpdateDepartmentCombo( Preset.GetDepartmentFilter() );
	m_DepartmentSelector.UpdateDepartmentEditBox();

	m_EcrmanSupplierSelector.UpdateEcrmanSupplierCombo( Preset.GetSupplierFilter() );
	m_EcrmanSupplierSelector.UpdateEcrmanSupplierEditBox();
	
	m_checkZeroSkip.SetCheck( Preset.GetZeroSkipFlag() );
	m_checkDeptSort.SetCheck( Preset.GetDepartmentSortFlag() );
	m_checkSupplierSort.SetCheck( Preset.GetSupplierSortFlag() );
}

/**********************************************************************/

void CReportEcrmanStockDlg::FillPresetCombo( int nPresetIdxSel )
{
	PresetReportManager.FillPresetCombo( NODE_PRESET_ECR_STOCK, m_comboPreset, nPresetIdxSel );
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnSelectReport() 
{
	int nSel = m_comboReport.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboReport.GetCount() )
		return;

	int nReportType = m_comboReport.GetItemData( nSel );

	switch ( nReportType )
	{
	case REPORT_PLUSTOCK_APPARENT_DATE:
	case REPORT_PLUSTOCK_VALUATION_DATE:
	case REPORT_PLUSTOCK_REPLENISH_DATE:
		ShowAndEnableWindow( &m_TimePicker, TRUE );
		ShowAndEnableWindow( &m_staticTo, FALSE );
		ShowAndEnableWindow( &m_DatePickerTo, FALSE );
		m_comboDates.EnableWindow( FALSE );
		m_DatePickerFrom.EnableWindow( TRUE );
		m_DateSelectorRange.SetComboSelection( 0, szRANGE_CUSTOM_DATE );
		m_checkZeroSkip.EnableWindow( TRUE );
		m_checkDeptSort.EnableWindow( TRUE );
		m_checkSupplierSort.EnableWindow( TRUE );
		break;

	case REPORT_PLUSTOCK_APPARENT:
	case REPORT_PLUSTOCK_VALUATION:
		ShowAndEnableWindow( &m_TimePicker, FALSE );
		ShowAndEnableWindow( &m_staticTo, TRUE );	
		ShowAndEnableWindow( &m_DatePickerTo, TRUE, FALSE );
		m_comboDates.EnableWindow( FALSE );
		m_DatePickerFrom.EnableWindow( FALSE );
		m_checkZeroSkip.EnableWindow( TRUE );
		m_checkDeptSort.EnableWindow( TRUE );
		m_checkSupplierSort.EnableWindow( TRUE );
		break;

	case REPORT_PLUSTOCK_REORDER:
		ShowAndEnableWindow( &m_TimePicker, FALSE );
		ShowAndEnableWindow( &m_staticTo, TRUE );	
		ShowAndEnableWindow( &m_DatePickerTo, TRUE, FALSE );
		m_comboDates.EnableWindow( FALSE );
		m_DatePickerFrom.EnableWindow( FALSE );
		m_checkZeroSkip.EnableWindow( TRUE );
		m_checkDeptSort.EnableWindow( TRUE );
		m_checkSupplierSort.EnableWindow( TRUE );
		break;

	case REPORT_PLUSTOCK_DEL_AUDIT:
		ShowAndEnableWindow( &m_TimePicker, FALSE );
		ShowAndEnableWindow( &m_staticTo, TRUE );	
		ShowAndEnableWindow( &m_DatePickerTo, TRUE );
		m_comboDates.EnableWindow( TRUE );
		m_DatePickerFrom.EnableWindow( TRUE );
		m_checkZeroSkip.EnableWindow( FALSE );
		m_checkDeptSort.EnableWindow( FALSE );
		m_checkSupplierSort.EnableWindow( FALSE );
		break;
	
	case REPORT_PLUSTOCK_DEL_TOTAL:
	case REPORT_PLUSTOCK_DEL_VALUE:
		ShowAndEnableWindow( &m_TimePicker, FALSE );
		ShowAndEnableWindow( &m_staticTo, TRUE );	
		ShowAndEnableWindow( &m_DatePickerTo, TRUE );
		m_comboDates.EnableWindow( TRUE );
		m_DatePickerFrom.EnableWindow( TRUE );
		m_checkZeroSkip.EnableWindow( FALSE );
		m_checkDeptSort.EnableWindow( TRUE );
		m_checkSupplierSort.EnableWindow( TRUE );
		break;
	}		
}

/**********************************************************************/

void CReportEcrmanStockDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		OnKillFocusDeptNo();
		OnKillFocusSupplier();
		OnKillFocusPluFrom();
		OnKillFocusPluTo();

		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		CDataManagerInfo info;
		DataManager.OpenDatabaseReadOnly( LocSelEntity.GetDbIdx(), info, FALSE );

		COleDateTime dateFrom, dateTo;
		m_DatePickerFrom.GetTime( dateFrom );
		m_DatePickerTo.GetTime( dateTo );

		COleDateTime timeFrom;
		m_TimePicker.GetTime( timeFrom );

		int nSel = m_comboReport.GetCurSel();
		if ( nSel < 0 || nSel >= m_comboReport.GetCount() )
			return;

		int nReportType = m_comboReport.GetItemData( nSel );

		if ( nReportType != REPORT_PLUSTOCK_DEL_AUDIT )
		{
			CReportPluStockLevel Report;
			Report.SetReportType( nReportType );
			Report.SetDbIdx( LocSelEntity.GetDbIdx() );

			Report.SetPluRangeFlag ( m_checkSpecify.GetCheck() != 0 );
			Report.SetPluFrom ( _atoi64(m_strPluNoFrom) );
			Report.SetPluTo ( _atoi64(m_strPluNoTo) );
		
			Report.SetDepartmentFilter( m_nDepartmentFilter );
			Report.SetDepartmentSortFlag( m_checkDeptSort.GetCheck() != 0 );
		
			Report.SetSupplierFilter( m_nSuppNo );
			Report.SetSupplierSortFlag( m_checkSupplierSort.GetCheck() != 0 );

			Report.SetZeroSkipFlag( m_checkZeroSkip.GetCheck() != 0 );

			switch ( nReportType )
			{
			case REPORT_PLUSTOCK_APPARENT_DATE:
			case REPORT_PLUSTOCK_VALUATION_DATE:
			case REPORT_PLUSTOCK_REPLENISH_DATE:
				{
					COleDateTime timeNow = COleDateTime::GetCurrentTime();
					COleDateTime dateNow = COleDateTime( timeNow.GetYear(), timeNow.GetMonth(), timeNow.GetDay(), 0, 0, 0 );

					if ( dateFrom > dateNow )
						dateFrom = dateNow;

					BuildZeroDateMap( LocSelEntity.GetDbIdx(), dateFrom, timeFrom );
					Report.SetStockLevelArray( &m_StockLevelBuffer );
				}
				break;
			}

			switch ( nReportType )
			{
			case REPORT_PLUSTOCK_APPARENT:
				Report.SetReportName ( "Plu Apparent Stock" );
				Report.ConsolidateStockLevels();
				break;

			case REPORT_PLUSTOCK_REORDER:
				Report.SetReportName ( "Plu Stock Reorder" );
				Report.ConsolidateStockLevels();
				break;

			case REPORT_PLUSTOCK_VALUATION:
				Report.SetReportName ( "Plu Stock Valuation" );
				Report.ConsolidateStockLevels();
				break;

			case REPORT_PLUSTOCK_DEL_TOTAL:
				Report.SetDateFrom( dateFrom );
				Report.SetDateTo( dateTo );
				Report.SetReportName ( "Plu Delivery Totals" );
				Report.ConsolidateDeliveryTotals();
				break;

			case REPORT_PLUSTOCK_DEL_VALUE:
				Report.SetDateFrom( dateFrom );
				Report.SetDateTo( dateTo );
				Report.SetReportName ( "Plu Delivery Valuation" );
				Report.ConsolidateDeliveryTotals();
				break;

			case REPORT_PLUSTOCK_APPARENT_DATE:
				{
					CString strTitle;
					strTitle.Format( "Plu Previous Stock (%2.2d:%2.2d on %2.2d/%2.2d/%4.4d)",
						timeFrom.GetHour(), timeFrom.GetMinute(), dateFrom.GetDay(), dateFrom.GetMonth(), dateFrom.GetYear() );
					Report.SetReportName( strTitle );
				}
				Report.ConsolidateStockLevels();
				break;

			case REPORT_PLUSTOCK_VALUATION_DATE:
				{
					CString strTitle;
					strTitle.Format( "Plu Previous Valuation (%2.2d:%2.2d on %2.2d/%2.2d/%4.4d)",
						timeFrom.GetHour(), timeFrom.GetMinute(), dateFrom.GetDay(), dateFrom.GetMonth(), dateFrom.GetYear() );
					Report.SetReportName( strTitle );
				}
				Report.ConsolidateStockLevels();
				break;

			case REPORT_PLUSTOCK_REPLENISH_DATE:
				{
					CString strTitle;
					strTitle.Format( "Plu Previous Stock and Replenishment (%2.2d:%2.2d on %2.2d/%2.2d/%4.4d)",
						timeFrom.GetHour(), timeFrom.GetMinute(), dateFrom.GetDay(), dateFrom.GetMonth(), dateFrom.GetYear() );
					Report.SetReportName( strTitle );
				}
				Report.ConsolidateStockLevels();
				break;
			}

			CString strKey;
			strKey.Format ( "%d", nReportType );
			strKey += Report.GetReportKey();

			CFilenameUpdater FnUp( SysFiles::PluApparentPrm );

			if ( Report.CreateReport() == TRUE )
			{
				CReportManager ReportManager;
				ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), strKey, this );
			}
		}
		else
		{
			CReportPluDelivery Report;
			Report.SetDbIdx( LocSelEntity.GetDbIdx() );
			
			Report.SetDateFrom( dateFrom );
			Report.SetDateTo( dateTo );

			Report.SetPluRangeFlag ( m_checkSpecify.GetCheck() != 0 );
			if ( m_strPluNoFrom != "" ) ::AddLeading ( m_strPluNoFrom, Super.MaxPluLen(), '0' );
			if ( m_strPluNoTo != "" ) ::AddLeading ( m_strPluNoTo, Super.MaxPluLen(), '0' );
			
			Report.SetPluFrom ( _atoi64(m_strPluNoFrom) );
			Report.SetPluTo ( _atoi64(m_strPluNoTo) );
			Report.SetDepartmentFilter ( m_nDepartmentFilter );
			Report.SetSupplierFilter( m_nSuppNo );

			CFilenameUpdater FnUp ( SysFiles::PluDelAuditPrm );
			
			if ( Report.CreateDeliveryAuditReport() == TRUE )
			{
				CReportManager ReportManager;
				ReportManager.DisplayReport ( Super.ReportFilename(), FnUp.GetFilenameToUse(), "", this );
			}
		}
		
		UpdateData ( FALSE );
	}
}

/**********************************************************************/