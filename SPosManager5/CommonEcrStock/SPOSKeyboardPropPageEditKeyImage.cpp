/**********************************************************************/
#include "SPOSKeyboardImageImportDlg.h"
#include "SPOSKeyboardImageSelectDlg.h"
#include "SPOSKeyboardPropertySheetEditKey.h"
/**********************************************************************/
#include "SPOSKeyboardPropPageEditKeyImage.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditKeyImage::CSPOSKeyboardPropPageEditKeyImage() : CSSPropertyPage(CSPOSKeyboardPropPageEditKeyImage::IDD)
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditKeyImage)
	//}}AFX_DATA_INIT
	m_pParentSheet = NULL;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::SetKeyRecord( CSPOSKeyboardKeyRecord* pRecord )
{
	m_pKeyRecord = pRecord;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditKeyImage::~CSPOSKeyboardPropPageEditKeyImage()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditKeyImage)
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_editHeight);
	DDX_Control(pDX, IDC_COMBO_FILL, m_comboFill);
	DDX_Control(pDX, IDC_BUTTON_PLU, m_buttonPlu);
	DDX_Control(pDX, IDC_BUTTON_DEPT, m_buttonDept);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_buttonRemove);
	DDX_Control(pDX, IDC_EDIT_IMAGE, m_editImage);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditKeyImage, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditKeyImage)
	ON_BN_CLICKED(IDC_BUTTON_PLU, OnButtonPlu)
	ON_BN_CLICKED(IDC_BUTTON_DEPT, OnButtonDept)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_FILL, RedrawKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
	
BOOL CSPOSKeyboardPropPageEditKeyImage::OnInitDialog() 
{	
	m_staticImage.SubclassDlgItem(IDC_STATIC_IMAGE,this);
	CPropertyPage::OnInitDialog();
	
	if ( NULL == m_pParentSheet )
	{
		FillScaleCombo( m_pKeyRecord -> GetImageScale() );
		m_editImage.SetWindowText( m_pKeyRecord -> GetImageFilename() );

		bool bIsPluKey = ( m_pKeyRecord -> GetType() == KEY_TYPE_PLUNUMBER ); 
		ShowAndEnableWindow( &m_buttonPlu, bIsPluKey );
		ShowAndEnableWindow( &m_buttonDept, bIsPluKey );
		
		RedrawImage();
	}

	return TRUE;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::FillScaleCombo( int nScale )
{
	m_comboFill.ResetContent();

	int nSel = -1;
	for ( int n = 1; n <= 10; n++ )
	{
		CString str;
		str.Format( "%d", n * 10 );
		m_comboFill.AddString( str );
		m_comboFill.SetItemData( n - 1, n * 10 );

		if ( nScale == n * 10 )
			nSel = n - 1;
	}

	if ( nSel == -1 )
	{
		CString str;
		str.Format( "%d",  nScale );
		m_comboFill.InsertString( 0, str );
		m_comboFill.SetItemData( 0, nScale );
		nSel = 0;
	}

	m_comboFill.SetCurSel( nSel );
}

/**********************************************************************/
	
BOOL CSPOSKeyboardPropPageEditKeyImage::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyImage::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );

	if ( NULL != m_pParentSheet )
	{
		FillScaleCombo( m_pKeyRecord -> GetImageScale() );
		m_editImage.SetWindowText( m_pKeyRecord -> GetImageFilename() );

		bool bIsPluKey = ( m_pKeyRecord -> GetType() == KEY_TYPE_PLUNUMBER ); 
		ShowAndEnableWindow( &m_buttonPlu, bIsPluKey );
		ShowAndEnableWindow( &m_buttonDept, bIsPluKey );
		
		RedrawImage();
	}
	
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::SaveRecord()
{
	CString str;
	m_editImage.GetWindowText( str );
	m_pKeyRecord -> SetImageFilename( str );
	
	int nSel = m_comboFill.GetCurSel();

	if ( nSel >= 0 && nSel <= m_comboFill.GetCount() )
	{
		int nFill = m_comboFill.GetItemData(nSel);
		m_pKeyRecord -> SetImageScale( nFill );
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::OnButtonImport() 
{
	CSPOSKeyboardImageImportDlg dlg( this );

	if ( dlg.DoModal() == IDOK )
	{
		SPOSKeyboardManager.GetImageLibrary() -> Refresh( TRUE );
	
		CString strImageFilename = dlg.GetNewSelectedImage();
		if ( strImageFilename != "" )
		{
			m_editImage.SetWindowText( strImageFilename );
			RedrawImage();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::OnButtonPlu()
{
	__int64 nPluNo = m_pKeyRecord -> GetValue();

	int nPluIdx;
	if ( SPOSKeyboardManager.GetPluDatabase() -> FindPluItem( nPluNo, nPluIdx ) == FALSE )
	{
		Prompter.Error( "Unable to find PLU item" );
	}
	else
	{
		CSPOSKeyboardImportPluCSVRecord PluRecord;
		SPOSKeyboardManager.GetPluDatabase() -> GetAt( nPluIdx, PluRecord );
		CString strImageFilename = PluRecord.GetImageFilename();

		if ( strImageFilename == "" )
			Prompter.Error( "No image is linked to the selected PLU item" );
		else
		{
			m_editImage.SetWindowText( strImageFilename );
			RedrawImage();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::OnButtonDept()
{
	__int64 nPluNo = m_pKeyRecord -> GetValue();

	int nPluIdx;
	if ( SPOSKeyboardManager.GetPluDatabase() -> FindPluItem( nPluNo, nPluIdx ) == FALSE )
	{
		Prompter.Error( "Unable to find PLU item" );
	}
	else
	{
		CSPOSKeyboardImportPluCSVRecord PluRecord;
		SPOSKeyboardManager.GetPluDatabase() -> GetAt( nPluIdx, PluRecord );
		
		int nDeptIdx;
		int nDeptNo = PluRecord.GetDeptNo();
		if ( DataManager.Department.FindDeptByNumber( nDeptNo, nDeptIdx ) == FALSE )
		{
			Prompter.Error( "Unable to find department" );
		}

		CDepartmentCSVRecord DeptRecord;
		DataManager.Department.GetAt( nDeptIdx, DeptRecord );
		CString strImageFilename = DeptRecord.GetImageFilename();
	
		if ( strImageFilename == "" )
			Prompter.Error( "No department image is linked to the selected item" );
		else
		{
			m_editImage.SetWindowText( strImageFilename );
			RedrawImage();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::OnButtonRemove()
{
	if ( Prompter.YesNo( "Are you sure you wish to remove this image" ) == IDYES )
	{
		m_editImage.SetWindowText( "" );
		m_buttonRemove.EnableWindow( FALSE );
		m_staticImage.SetBlankFlag ( TRUE );
		m_staticImage.SetFailedFlag ( FALSE );
		m_staticImage.Invalidate();
		m_editWidth.SetWindowText( "" );
		m_editHeight.SetWindowText( "" );
		RedrawKey();
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::OnButtonSelect()
{
	CString strText;
	m_editImage.GetWindowText( strText );

	CSPOSKeyboardImageSelectDlg dlgSelect( strText, this );
	if ( dlgSelect.DoModal() == IDOK )
	{
		CString strFilename = dlgSelect.GetNewSelectedImage();
		if ( strFilename != "" )
		{
			m_editImage.SetWindowText( strFilename );
			RedrawImage();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::RedrawImage() 
{
	CString strImage;
	m_editImage.GetWindowText( strImage );

	m_buttonRemove.EnableWindow( strImage != "" );

	if ( strImage == "" )
	{
		m_staticImage.SetBlankFlag ( TRUE );
		m_staticImage.SetFailedFlag ( FALSE );
		m_staticImage.Invalidate();
		m_editWidth.SetWindowText( "" );
		m_editHeight.SetWindowText( "" );
		RedrawKey();
	}
	else
	{
		CString strFilename = SPOSKeyboardManager.GetLayoutOptions() -> GetImageFolder();
		strFilename += "\\";
		strFilename += strImage;

		BOOL bResult = m_staticImage.LoadImage( strFilename );
		m_staticImage.SetBlankFlag ( FALSE == bResult );
		m_staticImage.SetFailedFlag ( FALSE == bResult );
		m_staticImage.Invalidate();

		m_editWidth.SetWindowText( m_staticImage.GetWidthString() );
		m_editHeight.SetWindowText( m_staticImage.GetHeightString() );
		RedrawKey();
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyImage::RedrawKey()
{
	if ( m_pParentSheet != NULL )
	{
		SaveRecord();
		( (CSPOSKeyboardPropertySheetEditKey*) m_pParentSheet ) -> RedrawKey();
	}
}

/**********************************************************************/
