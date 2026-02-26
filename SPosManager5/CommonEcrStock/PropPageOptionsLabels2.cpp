/**********************************************************************/
#include "MaxLengths.h"
#include "PropPageOptionsLabels2.h"
/**********************************************************************/

CPropPageOptionsLabels2::CPropPageOptionsLabels2() : CPropertyPage(CPropPageOptionsLabels2::IDD)
{
	//{{AFX_DATA_INIT(CPropPageOptionsLabels2)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

CPropPageOptionsLabels2::~CPropPageOptionsLabels2()
{
}

/**********************************************************************/

void CPropPageOptionsLabels2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageOptionsLabels2)
	
	DDX_Control(pDX, IDC_COMBO_SHELF_NAME, m_comboShelfEdgeName);
	DDX_Control(pDX, IDC_CHECK_SHELF, m_checkShelfEdge);
	DDX_Control(pDX, IDC_CHECK_SHELF_PROMPT, m_checkShelfEdgePrompt);
	
	DDX_Control(pDX, IDC_CHECK_SHELF_SINGLE, m_checkShelfEdgeSingle);
	DDX_Control(pDX, IDC_COMBO_SHELF_NAME_SINGLE, m_comboShelfEdgeNameSingle);
	DDX_Control(pDX, IDC_CHECK_SHELF_SINGLE_PROMPT, m_checkShelfEdgeSinglePrompt);
	
	DDX_Control(pDX, IDC_COMBO_PRODUCT_NAME, m_comboProductName);
	DDX_Control(pDX, IDC_CHECK_PRODUCT, m_checkProduct);

	DDX_Control(pDX, IDC_CHECK_PRODUCT_SINGLE, m_checkProductSingle);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_NAME_SINGLE, m_comboProductNameSingle);
	DDX_Control(pDX, IDC_CHECK_PRODUCT_SINGLE_PROMPT, m_checkProductSinglePrompt);
	
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPropPageOptionsLabels2, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageOptionsLabels2)
	ON_BN_CLICKED(IDC_CHECK_SHELF, OnToggleAutoShelf)
	ON_BN_CLICKED(IDC_CHECK_SHELF_SINGLE, OnToggleSingle)
	ON_BN_CLICKED(IDC_CHECK_PRODUCT_SINGLE, OnToggleSingle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPropPageOptionsLabels2::OnInitDialog() 
{
	GetRecordData();
	CPropertyPage::OnInitDialog();

	m_LabelNameHandler.Read();	
	SetRecordControls();
	OnToggleAutoShelf();
	OnToggleSingle();

#ifndef STOCKMAN_SYSTEM
	m_checkProduct.ShowWindow( SW_HIDE );
#endif

	return TRUE;  
}

/**********************************************************************/

BOOL CPropPageOptionsLabels2::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CPropPageOptionsLabels2::OnSetActive() 
{
	GetRecordData();
	SetRecordControls();
	OnToggleSingle();
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CPropPageOptionsLabels2::GetRecordData()
{
}

/**********************************************************************/

void CPropPageOptionsLabels2::SaveRecord()
{
	//SHELF EDGE BATCH PRINT
	CString strText;
	m_comboShelfEdgeName.GetWindowText( strText );
	EcrmanOptions.SetLabelsShelfNameBatch( strText );

	bool bShelfEdge = ( m_checkShelfEdge.GetCheck() != 0 );
	bool bShelfEdgePrompt = ( m_checkShelfEdgePrompt.GetCheck() != 0 );
	EcrmanOptions.SetLabelsShelfAutoBatchFlag ( bShelfEdge );
	EcrmanOptions.SetLabelsShelfPromptBatchFlag( bShelfEdge && bShelfEdgePrompt );
	
	//SHELF EDGE SINGLE ITEM
	if ( m_checkShelfEdgeSingle.GetCheck() != 0 )
	{
		m_comboShelfEdgeNameSingle.GetWindowText( strText );
		EcrmanOptions.SetLabelsShelfPrintSingleFlag( TRUE );
		EcrmanOptions.SetLabelsShelfNameSingle( strText );
		EcrmanOptions.SetLabelsShelfPromptSingleFlag( m_checkShelfEdgeSinglePrompt.GetCheck() != 0 );
	}
	else
		EcrmanOptions.SetLabelsShelfPrintSingleFlag( FALSE );

	//PRODUCT BATCH PRINT
	m_comboProductName.GetWindowText( strText );
	EcrmanOptions.SetLabelsProductNameBatch( strText );

#ifdef STOCKMAN_SYSTEM
	EcrmanOptions.SetLabelsProductAutoBatchFlag ( m_checkProduct.GetCheck() != 0 );
#endif

	//PRODUCT SINGLE ITEM
	if ( m_checkProductSingle.GetCheck() != 0 )
	{
		m_comboProductNameSingle.GetWindowText( strText );
		EcrmanOptions.SetLabelsProductPrintSingleFlag( TRUE );
		EcrmanOptions.SetLabelsProductNameSingle( strText );
		EcrmanOptions.SetLabelsProductPromptSingleFlag( m_checkProductSinglePrompt.GetCheck() != 0 );
	}
	else
		EcrmanOptions.SetLabelsProductPrintSingleFlag( FALSE );
}

/**********************************************************************/

void CPropPageOptionsLabels2::SetRecordControls()
{
	//SHELF EDGE BATCH PRINT
	m_LabelNameHandler.FillComboBox( m_comboShelfEdgeName, EcrmanOptions.GetLabelsShelfNameBatch(), TRUE );
	m_checkShelfEdge.SetCheck( EcrmanOptions.GetLabelsShelfAutoBatchFlag() );
	m_checkShelfEdgePrompt.SetCheck( EcrmanOptions.GetLabelsShelfPromptBatchFlag() );

	//SHELF EDGE SINGLE ITEM
	m_checkShelfEdgeSingle.SetCheck( EcrmanOptions.GetLabelsShelfPrintSingleFlag() );
	{
		bool bCanEnableCombo = EcrmanOptions.GetLabelsShelfPrintSingleFlag();
		m_checkShelfEdgeSingle.SetCheck( bCanEnableCombo );
		m_LabelNameHandler.FillComboBox( m_comboShelfEdgeNameSingle, EcrmanOptions.GetLabelsShelfNameSingle(), bCanEnableCombo );
	}
	m_checkShelfEdgeSinglePrompt.SetCheck( EcrmanOptions.GetLabelsShelfPromptSingleFlag() );

	//PRODUCT BATCH PRINT
	m_LabelNameHandler.FillComboBox( m_comboProductName, EcrmanOptions.GetLabelsProductNameBatch(), TRUE );
	m_checkProduct.SetCheck( EcrmanOptions.GetLabelsProductAutoBatchFlag() );
	
	//PRODUCT SINGLE ITEM
	m_checkProductSingle.SetCheck( EcrmanOptions.GetLabelsProductPrintSingleFlag() );
	{
		bool bCanEnableCombo = EcrmanOptions.GetLabelsProductPrintSingleFlag();
		m_checkProductSingle.SetCheck( bCanEnableCombo );
		m_LabelNameHandler.FillComboBox( m_comboProductNameSingle, EcrmanOptions.GetLabelsProductNameSingle(), bCanEnableCombo );
	}
	m_checkProductSinglePrompt.SetCheck( EcrmanOptions.GetLabelsProductPromptSingleFlag() );
}

/**********************************************************************/

void CPropPageOptionsLabels2::OnToggleAutoShelf() 
{
	m_checkShelfEdgePrompt.EnableWindow( m_checkShelfEdge.GetCheck() );
}

/**********************************************************************/

void CPropPageOptionsLabels2::OnToggleSingle() 
{
	bool bShelf = ( m_checkShelfEdgeSingle.GetCheck() != 0 );
	m_comboShelfEdgeNameSingle.EnableWindow( bShelf && ( m_comboShelfEdgeNameSingle.GetCount() > 1 ) );
	m_checkShelfEdgeSinglePrompt.EnableWindow( bShelf );

	bool bProduct = ( m_checkProductSingle.GetCheck() != 0 );
	m_comboProductNameSingle.EnableWindow( bProduct && ( m_comboProductNameSingle.GetCount() > 1 ) );
	m_checkProductSinglePrompt.EnableWindow( bProduct );
}

/**********************************************************************/
