/**********************************************************************/
 
/**********************************************************************/
//#include "DataManager.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "Password.h"
 
#include "StockDeleteDatabaseDlg.h"
/**********************************************************************/
#include "StockDeleteDlg.h"
/**********************************************************************/

CStockDeleteDlg::CStockDeleteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockDeleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockDeleteDlg)
	m_bKeepSales = FALSE;
	//}}AFX_DATA_INIT
	m_bKeepApparent = TRUE;
	m_bKeepOnOrder = TRUE;
	m_bKeepSales = TRUE;
}

/**********************************************************************/

void CStockDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockDeleteDlg)
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Check(pDX, IDC_CHECK_KEEPAPPARENT, m_bKeepApparent);
	DDX_Check(pDX, IDC_CHECK_KEEPONORDER, m_bKeepOnOrder);
	DDX_Check(pDX, IDC_CHECK_KEEPSALES, m_bKeepSales);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CStockDeleteDlg, CDialog)
	//{{AFX_MSG_MAP(CStockDeleteDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CStockDeleteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CWordArray arrayTypes;
	arrayTypes.Add ( NODE_DATABASE );

	if ( PasswordArray.GetLocationAccessType() != LOCATION_ACCESS_SYSTEM )
		m_LocationSelector.SetSingleDbIdx( PasswordArray.GetLocationAccessDbIdx() );

	m_LocationSelector.BuildList( arrayTypes );
	m_LocationSelector.FillLocationCombo ( &m_comboDatabase );
	return TRUE;  
}

/**********************************************************************/

void CStockDeleteDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_STOCK );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( LocSelEntity.GetDbIdx(), arrayReadWrite, info, TRUE, TRUE ) == FALSE )
			Prompter.ShareError( info );
		else
		{
			int nResult = DeleteStockInternal();
			DataManager.CloseDatabase();

			switch ( nResult )
			{
			case 1:
				Prompter.Error ( "No unlinked stock records were found." );
				break;

			case 2:
				Prompter.Info ( "Stock records were deleted as requested." );
				break;
			}
		}
	}
}

/**********************************************************************/

int CStockDeleteDlg::DeleteStockInternal()
{
	DataManager.StockPlu.Invalidate();
	CStockDeleteDatabaseDlg dlgDelete ( ( m_bKeepOnOrder != 0 ), ( m_bKeepApparent != 0 ), ( m_bKeepSales != 0 ), this );
		
	if ( dlgDelete.GetFilterSize() == 0 )
		return 1;

	CDWordArray dw;
	dw.Add(dlgDelete.GetFilterSize());
	dw.Add(DataManager.Stock.GetSize() );
	MessageLogger.LogSystem( 4, 2, TRUE, dw );

	dlgDelete.DoModal();

	if ( dlgDelete.GetDoneDeleteFlag() == TRUE )
		return 2;
	else
		return 0;
}

/**********************************************************************/
