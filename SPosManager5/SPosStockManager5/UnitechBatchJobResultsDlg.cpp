/**********************************************************************/
#include "UnitechBatchJobResultsDlg.h"
/**********************************************************************/

CUnitechBatchJobResults::CUnitechBatchJobResults()
{
	m_nDelivery = 0;
	m_nWrapper = 0;
	m_nReturn = 0;
	m_nSale = 0;
	m_nAdjust = 0;
	m_nClosing = 0;
}

/**********************************************************************/

CUnitechBatchJobResultsDlg::CUnitechBatchJobResultsDlg( CWnd* pParent )
	: CDialog("IDD_UNITECH_BATCHJOB_RESULTS", pParent)
{
	//{{AFX_DATA_INIT(CUnitechBatchJobResultsDlg)
	//}}AFX_DATA_INIT
}

/**********************************************************************/

void CUnitechBatchJobResultsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnitechBatchJobResultsDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC2_STATIC_DELIVERY, m_staticDelivery);
	DDX_Control(pDX, IDC2_STATIC_WRAPPER, m_staticWrapper);
	DDX_Control(pDX, IDC2_STATIC_RETURN, m_staticReturn);
	DDX_Control(pDX, IDC2_STATIC_SALE, m_staticSale);
	DDX_Control(pDX, IDC2_STATIC_ADJUST, m_staticAdjust);
	DDX_Control(pDX, IDC2_STATIC_CLOSING, m_staticClosing);
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CUnitechBatchJobResultsDlg, CDialog)
	//{{AFX_MSG_MAP(CUnitechBatchJobResultsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CUnitechBatchJobResultsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ShowTotal( m_staticDelivery, m_Results.m_nDelivery );
	ShowTotal( m_staticWrapper, m_Results. m_nWrapper );
	ShowTotal( m_staticReturn, m_Results.m_nReturn );
	ShowTotal( m_staticSale, m_Results.m_nSale );
	ShowTotal( m_staticAdjust, m_Results.m_nAdjust );
	ShowTotal( m_staticClosing, m_Results.m_nClosing );
	return TRUE;  
}

/**********************************************************************/

void CUnitechBatchJobResultsDlg::ShowTotal( CButton& staticCtrl, int nCount )
{
	switch( nCount )
	{
	case 0:
		staticCtrl.SetWindowText( "no data" );
		break;

	case 1:
		staticCtrl.SetWindowText( "1 line" );
		break;

	default:
		{
			CString str;
			str.Format( "%d lines", nCount );
			staticCtrl.SetWindowText( str );
		}
		break;
	}
}
		
/**********************************************************************/
