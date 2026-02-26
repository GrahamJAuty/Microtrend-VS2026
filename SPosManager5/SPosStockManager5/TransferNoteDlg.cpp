/**********************************************************************/
 
/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "NodeTypes.h"
#include "StockpointCSVArray.h"
#include "StockOptionsIni.h"
/**********************************************************************/
#include "TransferNoteDlg.h"
/**********************************************************************/

CTransferNoteDlg::CTransferNoteDlg( int nSpIdx, CWnd* pParent)
	: CDialog(CTransferNoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferNoteDlg)
	m_strReference1 = _T("");
	m_strReference2 = _T("");
	//}}AFX_DATA_INIT

	m_bAutoNum = StockOptions.GetAutoNumXferFlag();

	if ( m_bAutoNum == TRUE )
		m_strReference1.Format ( "%s%6.6d", 
			StockOptions.GetAutoNumXferPrefix(),
			StockOptions.GetAutoNumXfer() + 1 );

	m_nSourceSpIdx = nSpIdx;
	m_nDestSpIdx = -1;

	int nDbIdx;
	int nDbNo = dbStockpoint.GetDbNo ( m_nSourceSpIdx );
	dbDatabase.FindDatabaseByNumber ( nDbNo, nDbIdx );

	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_STOCKPOINT );
	m_LocationSelector.SetSingleDbIdx ( nDbIdx );
	m_LocationSelector.SetExcludeSpIdx ( m_nSourceSpIdx );
	m_LocationSelector.SetStockTransferFlag();
	m_LocationSelector.BuildList ( arrayTypes );

	if ( m_LocationSelector.GetListSize() == 1 )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetListEntry ( 0, LocSelEntity );
		m_nDestSpIdx = LocSelEntity.GetEntityIdx();
	}

	//m_bIsRelevant = ( ( m_LocationSelector.GetListSize() >= 2 ) || ( StockOptions.GetAutoNumTransfersFlag() == FALSE ) );
	m_bIsRelevant = TRUE;
}

/**********************************************************************/

void CTransferNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferNoteDlg)
	DDX_Control(pDX, IDC_EDIT_REFERENCE1, m_editReference1);
	DDX_Control(pDX, IDC_COMBO_STOCKPOINT, m_comboStockpoint);
	DDX_Text(pDX, IDC_EDIT_REFERENCE1, m_strReference1);
	DDX_Text(pDX, IDC_EDIT_REFERENCE2, m_strReference2);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CTransferNoteDlg, CDialog)
	//{{AFX_MSG_MAP(CTransferNoteDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CTransferNoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	CString strTitle;
	strTitle.Format ( "Transfer stock from %s", dbStockpoint.GetName( m_nSourceSpIdx ) );
	SetWindowText ( strTitle );

	m_editReference1.EnableWindow ( m_bAutoNum == FALSE );
	m_LocationSelector.FillLocationCombo ( &m_comboStockpoint );
	
	return TRUE;  
}

/**********************************************************************/

void CTransferNoteDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == FALSE )
		return;

	CLocationSelectorEntity LocSelEntity;
	m_LocationSelector.GetSelectedEntity( LocSelEntity );
	m_nDestSpIdx = LocSelEntity.GetEntityIdx();
	if ( m_nDestSpIdx == -1 )
		return;

	EndDialog ( IDOK );
}

/**********************************************************************/
