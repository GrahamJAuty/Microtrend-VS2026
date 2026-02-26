/**********************************************************************/
#include "NodeTypes.h"
/**********************************************************************/
#include "StockDeleteCloseDlg.h"
/**********************************************************************/
IMPLEMENT_DYNAMIC(CStockDeleteCloseDlg, CDialog)
/**********************************************************************/

CStockDeleteCloseDlg::CStockDeleteCloseDlg( int nType, int nCount, CWnd* pParent)
	: CDialog(CStockDeleteCloseDlg::IDD, pParent)
{
	m_nType = nType;
	m_nCount = nCount;
	m_nAction = STOCKDELETE_CANCEL;
}

/**********************************************************************/

CStockDeleteCloseDlg::~CStockDeleteCloseDlg()
{
}

/**********************************************************************/

void CStockDeleteCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_static1);
	DDX_Control(pDX, IDC_STATIC2, m_static2);
	DDX_Control(pDX, IDC_STATIC3, m_static3);
	DDX_Control(pDX, IDC_STATIC4, m_static4);
	DDX_Control(pDX, IDC_BUTTON_CONFIRM, m_buttonConfirm);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_buttonCancel);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDeleteCloseDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirm)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnButtonContinue)
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDeleteCloseDlg::OnInitDialog()
{
	SetDefID( IDC_BUTTON_CANCEL );
	
	CDialog::OnInitDialog();

	if ( NODE_STOCK == m_nType )
	{
		if ( 1 == m_nCount )
		{
			SetWindowText( "Delete Stock Record" );
			m_static1.SetWindowText( "1 stock record has been selected for deletion." );
			m_static2.SetWindowText( "1 .. The selected stock record will be deleted." );
			m_static3.SetWindowText( "2 .. Any Plu item links to this stock record will be removed." );
			m_static4.SetWindowText( "3 .. You will not be able to restore this record after deletion." );
		}
		else
		{
			SetWindowText( "Delete Stock Records" );
			
			CString str;
			str.Format( "%d stock records have been selected for deletion.", 
				m_nCount );

			m_static1.SetWindowText( str );
			m_static2.SetWindowText( "1 .. The selected stock records will be deleted." );
			m_static3.SetWindowText( "2 .. Any Plu item links to these stock records will be removed." );
			m_static4.SetWindowText( "3 .. You will not be able to restore these records after deletion." );
		}
	}
	else
	{
		if ( 1 == m_nCount )
		{
			SetWindowText( "Delete Plu Record" );
			m_static1.SetWindowText( "1 Plu record has been selected for deletion." );
			m_static2.SetWindowText( "1 .. The selected Plu record will be deleted." );
			m_static3.SetWindowText( "2 .. You will not be able to restore this record after deletion." );
			m_static4.ShowWindow( SW_HIDE );
		}
		else
		{
			SetWindowText( "Delete Plu Records" );
			
			CString str;
			str.Format( "%d Plu records have been selected for deletion.", 
				m_nCount );

			m_static1.SetWindowText( str );
			m_static2.SetWindowText( "1 .. The selected Plu records will be deleted." );
			m_static3.SetWindowText( "2 .. You will not be able to restore these records after deletion." );
			m_static4.ShowWindow( SW_HIDE );
		}
	}

	if ( 1 == m_nCount )
	{
		m_buttonConfirm.SetWindowText ( "Delete\nRecord" );
		m_buttonCancel.SetWindowText ( "Keep\nRecord" );
	}
	else
	{
		m_buttonConfirm.SetWindowText ( "Delete\nRecords" );
		m_buttonCancel.SetWindowText ( "Keep\nRecords" );
	}

	return FALSE;  
}

/**********************************************************************/

void CStockDeleteCloseDlg::OnOK()
{
	OnButtonCancel();
}

/**********************************************************************/

void CStockDeleteCloseDlg::OnCancel()
{
	OnButtonCancel();
}

/**********************************************************************/

void CStockDeleteCloseDlg::OnButtonConfirm()
{
	m_nAction = STOCKDELETE_CONFIRM;
	EndDialog( IDOK );
}

/**********************************************************************/

void CStockDeleteCloseDlg::OnButtonCancel()
{
	m_nAction = STOCKDELETE_CANCEL;
	EndDialog( IDOK );
}

/**********************************************************************/

void CStockDeleteCloseDlg::OnButtonContinue()
{
	m_nAction = STOCKDELETE_CONTINUE;
	EndDialog( IDOK );
}

/**********************************************************************/
