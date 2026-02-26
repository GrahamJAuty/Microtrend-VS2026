/**********************************************************************/
#include <process.h>
/**********************************************************************/
#include "BarcodePluNoTable.h"
#include "BarcodeRangeDlg.h"
#include "DatabaseCSVArray.h"
#include "FilterTypes.h"
#include "ListLabelsItemAddDlg.h" 
#include "ListLabelsItemEditDlg.h" 
#include "PCOptions.h"
#include "PriceHelpers.h"
#include "PriceSetItemManager.h"
#include "ReportHelpers.h"
#include "StatusProgress.h"
#include "SysInfo.h"
/**********************************************************************/
#include "ListLabelsDlg.h"
/**********************************************************************/

CListLabelsDlg::CListLabelsDlg( bool bAdhoc, bool bReadOnly, bool bAutoPrint, CLabelListCSVArray& LabelList, const char* szStockpointName, CWnd* pParent)
	: CSSDialog(CListLabelsDlg::IDD, pParent), m_LabelList ( LabelList ), m_LabelListCreator( LabelList ), m_BarcodeFilterArray ( FILTERTYPE_DATABASE ), m_PluFilterArray ( FILTERTYPE_BROWSE_PLU )
{
	//{{AFX_DATA_INIT(CListLabelsDlg)
	//}}AFX_DATA_INIT
	m_nFirstLabel = 1;
	m_bAdhoc = bAdhoc;
	m_bAutoPrint = bAutoPrint;
	m_bReadOnly = bReadOnly;
	m_bPrinted = FALSE;
	m_strStockpointName = szStockpointName;

	if ( m_LabelList.IsProductLabelList() == FALSE )
		m_bBarcode = FALSE;
	else
	{
		m_bBarcode = TRUE;
		m_BarcodeFilterArray.SetEanFlag( TRUE );
		m_BarcodeFilterArray.BuildRecordList();
		m_PluFilterArray.SetEANOnlyFilter( TRUE );
	}

	m_PluFilterArray.SetLiteralOnlyFilter( TRUE );
	m_PluFilterArray.BuildRecordList();
}

/**********************************************************************/

CListLabelsDlg::~CListLabelsDlg()
{
}

/**********************************************************************/

void CListLabelsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListLabelsDlg)
	DDX_Control(pDX, IDC_STATIC_PAGE, m_staticPage);
	DDX_Control(pDX, IDC_COMBO_SHELF_NAME, m_comboLabelName);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_buttonAdd);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_buttonEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_buttonDelete);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_buttonDisplay);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_buttonPrint);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
	DDX_Control(pDX, IDOK, m_buttonOK);
	DDX_Control(pDX, IDCANCEL, m_buttonCancel);
	DDX_Text(pDX, IDC_EDIT_LABEL, m_nFirstLabel);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CListLabelsDlg, CDialog)
	//{{AFX_MSG_MAP(CListLabelsDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDoubleClickList)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST, OnGetDispInfoList)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, OnButtonDisplay)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillFocusList)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST, OnSetFocusList)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CListLabelsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nColumn = 0;

	SubclassEdit ( IDC_EDIT_LABEL, SS_NUM, 3, "%d" );

	m_list.SubclassDlgItem ( IDC_LIST, this );

	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
	{
		ResizeDialog( 518, 350 );
		MoveControl( &m_buttonAdd, 461, 42 );
		MoveControl( &m_buttonEdit, 461, 77 );
		MoveControl( &m_buttonDelete, 461, 112 );
		MoveControl( &m_buttonDisplay, 461, 167 );
		MoveControl( &m_buttonPrint, 461, 202 );
		MoveControl( &m_buttonSave, 461, 251 );
		MoveControl( &m_buttonOK, 461, 313 );
		MoveControl( &m_buttonCancel, 461, 329 );
		ResizeControl( &m_staticPage, 444, 31 );
		ResizeControl( &m_list, 444, 301 );
	}

	m_list.InsertColumn ( nColumn++, "Barcode No", LVCFMT_LEFT, 110 );
	m_list.InsertColumn ( nColumn++, "Plu No", LVCFMT_LEFT, 110 );

	if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
		m_list.InsertColumn ( nColumn++, "Modifier", LVCFMT_LEFT, 100 );

	if ( m_LabelListCreator.GetPriceLevel2() == 0 )
	{
		m_list.InsertColumn ( nColumn++, "Description", LVCFMT_LEFT, 215 );
		m_list.InsertColumn ( nColumn++, "Price", LVCFMT_LEFT, 60 );
	}
	else
	{
		m_list.InsertColumn ( nColumn++, "Description", LVCFMT_LEFT, 155 );
		m_list.InsertColumn ( nColumn++, "Price1", LVCFMT_LEFT, 60 );
		m_list.InsertColumn ( nColumn++, "Price2", LVCFMT_LEFT, 60 );
	}

	m_list.InsertColumn ( nColumn++, "Qty", LVCFMT_RIGHT, 35 );
	m_list.SetItemCountEx ( m_LabelList.GetSize() );

	if ( m_bAdhoc == TRUE )
	{
		m_buttonOK.ShowWindow ( SW_HIDE );
		m_buttonCancel.SetWindowText ( "Exit" );
	}
	
	if ( ( m_bAdhoc == FALSE ) || ( m_bReadOnly == TRUE ) )
		m_buttonSave.ShowWindow ( SW_HIDE );

	CString strName;
	if ( m_bBarcode )
		strName = EcrmanOptions.GetLabelsProductNameBatch();
	else
		strName = EcrmanOptions.GetLabelsShelfNameBatch();

	m_LabelNameHandler.Read();
	m_LabelNameHandler.FillComboBox( m_comboLabelName, strName, TRUE );

	m_nLabelCount = m_LabelList.GetTotalLabelCount();
	DisplayScreen ( 0 );
	return TRUE;
}

/**********************************************************************/

void CListLabelsDlg::DisplayScreen ( int nIndex )
{
	bool bWantSpace = FALSE;
	CCSV csvDbName;

	if ( m_strStockpointName != "" )
	{
		CString strText = "Apparent Stock at ";
		strText += m_strStockpointName;
		csvDbName.Add( strText );
		bWantSpace = TRUE;
	}
	else if ( SysInfo.GetMaxDatabases() > 1 )
	{
		int nDbIdx = DataManager.GetActiveDbIdx();
		csvDbName.Add ( dbDatabase.GetName( nDbIdx ) );
		bWantSpace = TRUE;
	}

	if ( PriceSetItemManager.IsPluMaster() == FALSE )
	{
		CString strName = ( bWantSpace == TRUE ) ? " " : "";
		csvDbName.Add( strName + PriceSetItemManager.GetPriceSetNameAndNumber() );
		bWantSpace = TRUE;
	}
		
	CString strTitle;
	if ( m_bBarcode == FALSE )
		strTitle = "Shelf Edge Labels";
	else
		strTitle = "Product Labels";

	CString strExtra;
	CString strPlural = ( m_nLabelCount == 1 ) ? "" : "s";
		
	if ( bWantSpace == TRUE )
	{
		strExtra.Format ( " (%s, %d label%s to print)", 
			csvDbName.GetLine(),
			m_nLabelCount,
			(const char*) strPlural );
	}
	else
	{
		strExtra.Format ( " (%d label%s to print)", 
			m_nLabelCount,
			(const char*) strPlural );
	}

	SetWindowText ( strTitle + strExtra );
	int nSize = m_list.GetItemCount();

	if ( ( nIndex >= 0 ) && ( nIndex < nSize ) )
		m_list.SetCurSel ( nIndex );
	else
		m_list.SetCurSel ( nSize - 1 );

	m_list.Invalidate();
}

/**********************************************************************/

void CListLabelsDlg::OnGetDispInfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (( pDispInfo->item.mask & LVIF_TEXT ) != 0 )
	{
		int nIndex = pDispInfo->item.iItem;
		
		CString strCSVBarcodeNo = "";
		CString strCSVPluNo = "";
		CString strCSVLabelText = "Unknown";
		CString strCSVPrice1 = "";
		CString strCSVPrice2 = "";
		int nCSVQty = 0;
		int nMod = 0;

		CString strRecord ="";
		if ( nIndex < m_LabelList.GetSize() )
		{
			CLabelListCSVRecord LabelListRecord;
			m_LabelList.GetAt ( nIndex, LabelListRecord );

			if ( ( LabelListRecord.GetDirectPluFlag() == FALSE ) && ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE ) )
			{
				strCSVBarcodeNo = LabelListRecord.GetBarcodeNoString();
				nMod = LabelListRecord.GetModifier();
			}
			
			strCSVPluNo = LabelListRecord.GetPluNoString();
			
			int nPluIdx;
			__int64 nPluNo = LabelListRecord.GetPluNoInt();
			if ( DataManager.Plu.FindPluByNumber ( nPluNo, nPluIdx ) == TRUE )
			{
				CPluCSVRecord PluRecord;
				DataManager.Plu.GetAt ( nPluIdx, PluRecord );

				if ( DealerFlags.GetEcrTextOnLabelsFlag() == FALSE )
					strCSVLabelText = PluRecord.GetReportText();
				else
					strCSVLabelText = PluRecord.GetEposText();

				CLabelPriceInfo info;
				m_LabelListCreator.GetPriceString ( PluRecord, nMod, info, 3 );
				strCSVPrice1 = info.m_strPrice1;
				strCSVPrice2 = info.m_strPrice2;
			}
			
			nCSVQty = LabelListRecord.GetQtyToPrint();
		}	

		CCSV csv ( '\t', '"', TRUE, FALSE, FALSE );
		csv.Add( strCSVBarcodeNo );
		csv.Add( strCSVPluNo );
		
		if ( EcrmanOptions.AllowBarcodeModifierLinks() == TRUE )
			csv.Add( ( 0 == nMod ) ? "" : DataManager.Modifier.GetDisplayName( nMod ) );

		csv.Add( strCSVLabelText );
		csv.Add( strCSVPrice1 );
		if ( m_LabelListCreator.GetPriceLevel2() != 0 ) csv.Add( strCSVPrice2 );
		csv.Add( nCSVQty );

		lstrcpy ( pDispInfo->item.pszText, csv.GetString ( pDispInfo->item.iSubItem ) );
	}
	*pResult = 0;
}

/**********************************************************************/

void CListLabelsDlg::OnButtonAdd() 
{
	CLabelListCSVRecord LabelListRecord;
	CListLabelsItemAddDlg dlg ( m_bBarcode, LabelListRecord, m_BarcodeFilterArray, m_PluFilterArray, this );
	
	if ( dlg.DoModal() != IDOK )
		return;

	CString strType;
	__int64 nNumberToCheck;
	CString strNumberToCheck;

	if ( LabelListRecord.GetDirectPluFlag() == TRUE )
	{
		strType = "Plu";
		nNumberToCheck = LabelListRecord.GetPluNoInt();
		::FormatPluNo( nNumberToCheck, strNumberToCheck );
	}
	else
	{
		strType = "Barcode";
		nNumberToCheck = LabelListRecord.GetBarcodeNoInt();
		::FormatBarcodeNo( nNumberToCheck, strNumberToCheck );
	}

	int nIndex;
	if ( m_LabelList.FindBarcodeByNumber ( nNumberToCheck, nIndex ) == TRUE )
	{
		CString strMsg;
		strMsg.Format ( "%s %s is\nalready listed for printing",
			( const char* ) strType,
			( const char* ) strNumberToCheck );
		
		Prompter.Error ( strMsg );
		DisplayScreen ( nIndex );
	}
	else
	{
		m_LabelList.InsertRecord ( LabelListRecord, nIndex );
		m_list.SetItemCountEx ( m_LabelList.GetSize() );
		m_nLabelCount += LabelListRecord.GetQtyToPrint();
		DisplayScreen ( nIndex );
	}
}

/**********************************************************************/

void CListLabelsDlg::OnDoubleClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEdit();
	*pResult = 0;
}

/**********************************************************************/

void CListLabelsDlg::OnButtonEdit() 
{
	int nIndex = m_list.GetCurSel();
	if ( nIndex < 0 || nIndex >= m_LabelList.GetSize() )
		return;

	CLabelListCSVRecord LabelListRecord;
	m_LabelList.GetAt ( nIndex, LabelListRecord );
	CListLabelsItemEditDlg dlg ( LabelListRecord, this );		
	
	int nOldBarcodeQty = LabelListRecord.GetQtyToPrint();

	if ( dlg.DoModal() == IDOK )
	{
		m_LabelList.SetAt ( nIndex, LabelListRecord );
		
		m_nLabelCount += LabelListRecord.GetQtyToPrint();
		m_nLabelCount -= nOldBarcodeQty;
		DisplayScreen ( nIndex );
	}
}

/**********************************************************************/

void CListLabelsDlg::OnButtonDelete() 
{
	int nIndex = m_list.GetCurSel();
	if ( nIndex < 0 || nIndex >= m_LabelList.GetSize() )
		return;

	CLabelListCSVRecord Record;
	m_LabelList.GetAt ( nIndex, Record );

	CBarcodeRangeDlg dlg ( "Remove Print Requests", this );
	dlg.SetBarcodeFrom ( Record.GetBarcodeNoInt() );
	dlg.SetBarcodeTo ( Record.GetBarcodeNoInt() );
	
	if ( dlg.DoModal() != IDOK )
		return;
	
	int nStart;
	int nEnd;

	m_LabelList.FindBarcodeByNumber ( dlg.GetBarcodeFrom(), nStart );
	bool bEndFound = m_LabelList.FindBarcodeByNumber ( dlg.GetBarcodeTo(), nEnd );

	if ( bEndFound == FALSE )
		nEnd--;

	for ( int nToGo = nEnd; nToGo >= nStart; nToGo-- )
	{
		m_LabelList.RemoveAt ( nToGo );
		m_list.SetItemCountEx ( m_LabelList.GetSize() );		
	}

	m_nLabelCount = m_LabelList.GetTotalLabelCount();
	DisplayScreen ( nIndex );
}

/**********************************************************************/

void CListLabelsDlg::OnButtonPrint() 
{
	if ( UpdateData ( TRUE ) == TRUE )
		PrintLabels ( TRUE );
}

/**********************************************************************/

void CListLabelsDlg::OnButtonDisplay() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		PrintLabels ( FALSE );

		CString strName;
		m_comboLabelName.GetWindowText( strName );
		
		m_LabelNameHandler.Read();
		m_LabelNameHandler.FillComboBox( m_comboLabelName, strName, TRUE );
	}
}

/**********************************************************************/

void CListLabelsDlg::PrintLabels ( bool bPrint )
{
	if ( ::FileExists ( "SPosBarcodeManager.pgm" ) == FALSE )
	{
		CString strMsg = "You must install the file SPosBarcodeManager.pgm\n";
		strMsg += "before you can use this option.";
		Prompter.Error ( strMsg );
		return;
	}

	StatusProgress.Lock( TRUE, "Creating label list" );
	int nResult = m_LabelListCreator.CreateLabelList();
	StatusProgress.Unlock();

	switch ( nResult )
	{
	case	1:	
		Prompter.Error ( "Unable to create temporary\nfile for Barcode Manager" );	
		break;

	case 2:
		Prompter.Error ( "No valid records were found" );
		break;
	}

	if ( nResult != 0 )
		return;

	CString strDataPath = "Labels.dat";
	GetDataProgramPath( strDataPath );
	
	CString strName;
	m_comboLabelName.GetWindowText( strName );

	CString strLabel;
	strLabel.Format ( "%d", m_nFirstLabel );
	
	CString strPrint = ( bPrint == TRUE ) ? "/p" : "";

	CString strProgramPath = "";
	GetDataProgramPath( strProgramPath );

	CString strDevmodePath = "PRINTA_";
	GetPCPathClient( strDevmodePath );

	CProtect1 protect;
	SSspawnl ( GetSafeOwner(this), P_WAIT, "SPOSBARCODEMANAGER.PGM", "SPOSBARCODEMANAGER.PGM", "1", protect.GetCode(), strDataPath, strName, strLabel, strProgramPath, strPrint, PCOptionsClient.GetDefaultLabelPrinter(), strDevmodePath, NULL );

	m_bPrinted = TRUE;
}

/**********************************************************************/

void CListLabelsDlg::OnButtonSave() 
{
	CString strMsg = "Are you sure you wish to save this list of labels and\n";
	strMsg += "overwrite any previous list for this database.";

	if ( Prompter.YesNo ( strMsg ) != IDYES )
		return;

	if ( m_LabelList.SaveList() == TRUE )
		Prompter.Info ( "The list has been saved." );
	else
		Prompter.Error ( "Unable to save list." );
}

/**********************************************************************/

void CListLabelsDlg::OnKillFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDOK );	
	*pResult = 0;
}

/**********************************************************************/

void CListLabelsDlg::OnSetFocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDefID ( IDC_BUTTON_EDIT );
	*pResult = 0;
}

/**********************************************************************/

void CListLabelsDlg::OnOK() 
{
	if ( m_bAutoPrint && m_bPrinted && m_LabelList.GetSize() > 0 )
	{
		CString strMsg = "You have printed or displayed a list of labels that is\n";         
#ifdef STOCKMAN_SYSTEM
		strMsg += "updated automatically by Stock Manager.\n\n";	        
#else
		strMsg += "updated automatically by Ecr Manager.\n\n";
#endif
		strMsg += "Would you like to clear this list now so that future\n";
		strMsg += "print requests will be added to an empty list.";
		
		if (Prompter.YesNo ( strMsg, FALSE ) == IDYES )
		{
			m_LabelList.RemoveAll();
			m_list.SetItemCountEx( m_LabelList.GetSize() );
		}
	}
		
	EndDialog ( IDOK );	
}

/**********************************************************************/
