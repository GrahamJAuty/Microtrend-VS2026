/**********************************************************************/
#include "DatabaseCSVArray.h"
#include "MessageLogger.h"
#include "NodeTypes.h"
#include "Password.h"
#include "..\CommonEcrStock\PluDeleteDatabaseDlg.h"
/**********************************************************************/
#include "PluDeleteUnlinkedDlg.h"
/**********************************************************************/

CPluDeleteUnlinkedDlg::CPluDeleteUnlinkedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPluDeleteUnlinkedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluDeleteUnlinkedDlg)
	//}}AFX_DATA_INIT
	m_nAction = 0;
}

/**********************************************************************/

void CPluDeleteUnlinkedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluDeleteUnlinkedDlg)
	DDX_Control(pDX, IDC_COMBO_DATABASE, m_comboDatabase);
	DDX_Radio(pDX, IDC_RADIO_ACTION, m_nAction);
	//}}AFX_DATA_MAP
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CPluDeleteUnlinkedDlg, CDialog)
	//{{AFX_MSG_MAP(CPluDeleteUnlinkedDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/

BOOL CPluDeleteUnlinkedDlg::OnInitDialog() 
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

void CPluDeleteUnlinkedDlg::OnOK() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		CLocationSelectorEntity LocSelEntity;
		m_LocationSelector.GetSelectedEntity( LocSelEntity );

		CWaitCursor wait;
		
		CWordArray arrayReadWrite;
		arrayReadWrite.Add( NODE_PLU );

		CDataManagerInfo info;
		if ( DataManager.OpenDatabaseReadWrite( LocSelEntity.GetDbIdx(), arrayReadWrite, info, FALSE ) == FALSE )
			Prompter.ShareError( info );
		else
		{
			int nResult = PluDeleteInternal();
			DataManager.CloseDatabase();

			switch( nResult )
			{
			case 1:
				Prompter.Error ( "No matching items were found." );
				break;

			case 2:
				if ( 1 == m_nAction )
					Prompter.Info( "Selected items were deleted as requested" );
				else
					Prompter.Info( "Selected items were marked as inactive" );
				break;
			}
		}
	}
}

/**********************************************************************/

int CPluDeleteUnlinkedDlg::PluDeleteInternal()
{
	CPluDeleteDatabaseDlg dlgDelete( ( 1 == m_nAction ), this );

	if ( dlgDelete.GetFilterSize() == 0 )
		return 1;
	
	CDWordArray dw;
	dw.Add( dlgDelete.GetFilterSize() );
	dw.Add( DataManager.Plu.GetSize() );
	MessageLogger.LogSystem( 5, 1, TRUE, dw );

	dlgDelete.DoModal();
	
	if ( dlgDelete.GetDoneDeleteFlag() == TRUE )
		return 2;
	else
		return 0;
}

/**********************************************************************/
