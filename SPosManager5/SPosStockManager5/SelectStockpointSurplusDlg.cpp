/**********************************************************************/
#include "StockpointCSVArray.h"
/**********************************************************************/
#include "SelectStockpointSurplusDlg.h"
/**********************************************************************/

CSelectStockpointSurplusDlg::CSelectStockpointSurplusDlg( CArray<int,int>& arraySpIdx, int nSpIdx, CWnd* pParent )
	: CDialog(CSelectStockpointSurplusDlg::IDD, pParent), m_arraySpIdx ( arraySpIdx )
{
	//{{AFX_DATA_INIT(CSelectStockpointSurplusDlg)
	//}}AFX_DATA_INIT
	m_nSpIdx = nSpIdx;
}

/**********************************************************************/

void CSelectStockpointSurplusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectStockpointSurplusDlg)
	DDX_Control(pDX, IDC_COMBO_STOCKPOINT, m_comboStockpoint);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSelectStockpointSurplusDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectStockpointSurplusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CSelectStockpointSurplusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int nPos = -1;

	for ( int nIndex = 0; nIndex < m_arraySpIdx.GetSize(); nIndex++ )
	{
		int nSpIdx = m_arraySpIdx.GetAt ( nIndex );
		m_comboStockpoint.AddString ( dbStockpoint.GetName( nSpIdx ) );
		if ( nSpIdx == m_nSpIdx ) nPos = nIndex;
	}

	if ( m_comboStockpoint.GetCount() < 2 )
		m_comboStockpoint.EnableWindow ( FALSE );

	m_comboStockpoint.SetCurSel ( nPos );
	return TRUE;  
}

/**********************************************************************/

void CSelectStockpointSurplusDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		int nSelection = m_comboStockpoint.GetCurSel();

		if ( nSelection >= 0 && nSelection < m_arraySpIdx.GetSize() )
			m_nSpIdx = m_arraySpIdx.GetAt ( nSelection );

		EndDialog ( IDOK );
	}
}

/**********************************************************************/


