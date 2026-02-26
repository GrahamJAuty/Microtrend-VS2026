/**********************************************************************/
#include "PluCSVArray.h"
#include "PluRangeModes.h"
#include "..\SPosStockManager5\StockOptionsIni.h"
/**********************************************************************/
#include "PluRangeCopyDlg.h"
/**********************************************************************/

CPluRangeCopyDlg::CPluRangeCopyDlg( CPluAddOrCopyInfo& PluInfo, CWnd* pParent )
	: CSSDialog(CPluRangeCopyDlg::IDD, pParent), m_PluInfo( PluInfo )
{
	//{{AFX_DATA_INIT(CPluRangeCopyDlg)
	//}}AFX_DATA_INIT
	m_PluInfo.m_strNewPluNo = "";
	m_PluInfo.m_lNewPluCopyCount = 1;
	m_PluInfo.m_bNewPluCreateStock = FALSE;
}

/**********************************************************************/

void CPluRangeCopyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluRangeCopyDlg)
	DDX_Control(pDX, IDC_CHECK_CREATE, m_checkCreate);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_RANGE, m_PluInfo.m_lNewPluCopyCount);
	DDV_MinMaxInt ( pDX, m_PluInfo.m_lNewPluCopyCount, 1, 1000 );
	DDX_IntegerString(pDX, IDC_EDIT_START, m_PluInfo.m_strNewPluNo);
	DDV_IntegerString(pDX, m_PluInfo.m_strNewPluNo, "1", Super.PluEnd() );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluRangeCopyDlg, CDialog)
	//{{AFX_MSG_MAP(CPluRangeCopyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluRangeCopyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SubclassEdit ( IDC_EDIT_START, SS_NUM, Super.MaxPluLen(), "%s" );
	SubclassEdit ( IDC_EDIT_RANGE, SS_NUM, 4, "%d" );

	CString strTitle = "";

	CPluCSVRecord PluRecord;
	DataManager.Plu.GetAt ( m_PluInfo.m_nSourcePluIdx, PluRecord );
	strTitle.Format ( "Copy Plu %s, %s", PluRecord.GetPluNoString(), PluRecord.GetReportText() );
		
#ifdef STOCKMAN_SYSTEM

	CRecipeCSVRecord Recipe;
	DataManager.Plu.GetRecipe ( m_PluInfo.m_nSourcePluIdx, PluRecord.GetPluNoInt(), Recipe );

	switch ( Recipe.GetSize() )
	{
	case 0:		m_PluInfo.m_nPluRangeMode = PLURANGE_MODE_NOLINK;	break;
	case 1:		m_PluInfo.m_nPluRangeMode = PLURANGE_MODE_SINGLE;	break;
	default:	m_PluInfo.m_nPluRangeMode = PLURANGE_MODE_RECIPE;	break;
	}
	
	switch ( m_PluInfo.m_nPluRangeMode )
	{
	case PLURANGE_MODE_NOLINK:
	case PLURANGE_MODE_RECIPE:	
		ShowAndEnableWindow( &m_checkCreate, FALSE );									
		break;
	
	case PLURANGE_MODE_SINGLE:	
		m_checkCreate.SetWindowText ( "Create separate stock item for each Plu" );
		m_checkCreate.SetCheck ( TRUE );
		break;
	}

#else

	m_PluInfo.m_nPluRangeMode = PLURANGE_MODE_NOLINK;
	ShowAndEnableWindow( &m_checkCreate, FALSE );									
		
#endif
	
	SetWindowText ( strTitle );
	return TRUE;  
}

/**********************************************************************/

void CPluRangeCopyDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		m_PluInfo.m_bNewPluCreateStock = ( m_checkCreate.GetCheck() != 0 );
		EndDialog ( IDOK );
	}
}

/**********************************************************************/
