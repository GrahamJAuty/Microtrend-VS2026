/**********************************************************************/
#include "NodeTypes.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "DeliveryCloseDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CDeliveryCloseDlg, CDialog)
/**********************************************************************/

CDeliveryCloseDlg::CDeliveryCloseDlg( int nType, bool bStocktake, CWnd* pParent)
	: CDialog(CDeliveryCloseDlg::IDD, pParent)
{
	m_nType = nType;
	m_bStocktake = bStocktake;
	m_nAction = SESSION_FILE;
}

/**********************************************************************/

CDeliveryCloseDlg::~CDeliveryCloseDlg()
{
}

/**********************************************************************/

void CDeliveryCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FILE, m_buttonFile);
	DDX_Control(pDX, IDC_BUTTON_DISCARD, m_buttonDiscard);
	DDX_Control(pDX, IDC_BUTTON_FINALISE, m_buttonFinalise);
	DDX_Control(pDX, IDC_BUTTON_ABANDON, m_buttonAbandon);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CDeliveryCloseDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FINALISE, &CDeliveryCloseDlg::OnButtonFinalise)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CDeliveryCloseDlg::OnButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_DISCARD, &CDeliveryCloseDlg::OnButtonDiscard)
	ON_BN_CLICKED(IDC_BUTTON_ABANDON, &CDeliveryCloseDlg::OnButtonAbandon)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, &CDeliveryCloseDlg::OnButtonContinue)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CDeliveryCloseDlg::OnInitDialog()
{
	if ( ( StockOptions.GetStockFinaliseMovementsFlag() == TRUE ) || ( TRUE == m_bStocktake ) )
		SetDefID( IDC_BUTTON_FINALISE );
	else
		SetDefID( IDC_BUTTON_FILE );

	CDialog::OnInitDialog();

	switch( m_nType )
	{
	case NODE_RETURN:
		SetWindowText( "Close Returns Session" );
		m_buttonFinalise.SetWindowText( "Finalise\nReturns" );
		m_buttonAbandon.SetWindowText( "Abandon\nReturns" );
		break;

	case NODE_ADJUSTMENT_STOCK:
	case NODE_ADJUSTMENT_PLU:
		SetWindowText( "Close Adjustment Session" );
		m_buttonFinalise.SetWindowText( "Finalise\nAdjustment" );
		m_buttonAbandon.SetWindowText( "Abandon\nAdjustment" );
		break;

	case NODE_MANUALSALE_STOCK:
	case NODE_MANUALSALE_PLU:
		SetWindowText( "Close Manual Sales Session" );
		m_buttonFinalise.SetWindowText( "Finalise\nSales" );
		m_buttonAbandon.SetWindowText( "Abandon\nSales" );
		break;

	case NODE_DELIVERY:
	default:
		SetWindowText( "Close Delivery Session" );
		m_buttonFinalise.SetWindowText( "Finalise\nDelivery" );
		m_buttonAbandon.SetWindowText( "Abandon\nDelivery" );
		break;
	}

	if ( TRUE == m_bStocktake )
	{
		m_buttonFile.EnableWindow( FALSE );
		m_buttonDiscard.EnableWindow( FALSE );
		m_buttonFile.ShowWindow( SW_HIDE );
		m_buttonDiscard.ShowWindow( SW_HIDE );
	}

	return FALSE;  
}

/**********************************************************************/

void CDeliveryCloseDlg::OnOK()
{
	OnButtonContinue();
}

/**********************************************************************/

void CDeliveryCloseDlg::OnCancel()
{
	OnButtonContinue();
}

/**********************************************************************/

void CDeliveryCloseDlg::OnButtonFinalise()
{
	m_nAction = SESSION_FINALISE;
	EndDialog( IDOK );
}

/**********************************************************************/

void CDeliveryCloseDlg::OnButtonFile()
{
	m_nAction = SESSION_FILE;
	EndDialog( IDOK );
}

/**********************************************************************/

void CDeliveryCloseDlg::OnButtonDiscard()
{
	m_nAction = SESSION_DISCARD;
	EndDialog( IDOK );
}

/**********************************************************************/

void CDeliveryCloseDlg::OnButtonAbandon()
{
	m_nAction = SESSION_ABANDON;
	EndDialog( IDOK );
}

/**********************************************************************/

void CDeliveryCloseDlg::OnButtonContinue()
{
	m_nAction = SESSION_CONTINUE;
	EndDialog( IDOK );
}

/**********************************************************************/
