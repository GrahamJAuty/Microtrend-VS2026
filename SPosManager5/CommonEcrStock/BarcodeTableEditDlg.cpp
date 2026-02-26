/**********************************************************************/
 
/**********************************************************************/
#include "BarcodePluNoTable.h"
 
/**********************************************************************/
#include "BarcodeTableEditDlg.h"
/**********************************************************************/

CBarcodeTableEditDlg::CBarcodeTableEditDlg (int nInputLen, CBarcodePluNoTableInfo& info, CWnd* pParent )
	: CSSDialog(CBarcodeTableEditDlg::IDD, pParent), m_infoFormat( info )
{
	//{{AFX_DATA_INIT(CBarcodeTableEditDlg)
	m_bChecksumBarcode = FALSE;
	m_bChecksumPlu = FALSE;
	//}}AFX_DATA_INIT

	m_nBarcodeNoID[0] = IDC_EDIT_BARCODENO_0;
	m_nBarcodeNoID[1] = IDC_EDIT_BARCODENO_1;
	m_nBarcodeNoID[2] = IDC_EDIT_BARCODENO_2;
	m_nBarcodeNoID[3] = IDC_EDIT_BARCODENO_3;
	m_nBarcodeNoID[4] = IDC_EDIT_BARCODENO_4;
	m_nBarcodeNoID[5] = IDC_EDIT_BARCODENO_5;
	m_nBarcodeNoID[6] = IDC_EDIT_BARCODENO_6;
	m_nBarcodeNoID[7] = IDC_EDIT_BARCODENO_7;
	m_nBarcodeNoID[8] = IDC_EDIT_BARCODENO_8;
	m_nBarcodeNoID[9] = IDC_EDIT_BARCODENO_9;
	m_nBarcodeNoID[10] = IDC_EDIT_BARCODENO_10;
	m_nBarcodeNoID[11] = IDC_EDIT_BARCODENO_11;
	m_nBarcodeNoID[12] = IDC_EDIT_BARCODENO_12;
	m_nBarcodeNoID[13] = IDC_EDIT_BARCODENO_13;
	m_nBarcodeNoID[14] = IDC_EDIT_BARCODENO_14;

	m_nPluNoID[0] = IDC_EDIT_PLUNO_0;
	m_nPluNoID[1] = IDC_EDIT_PLUNO_1;
	m_nPluNoID[2] = IDC_EDIT_PLUNO_2;
	m_nPluNoID[3] = IDC_EDIT_PLUNO_3;
	m_nPluNoID[4] = IDC_EDIT_PLUNO_4;
	m_nPluNoID[5] = IDC_EDIT_PLUNO_5;
	m_nPluNoID[6] = IDC_EDIT_PLUNO_6;
	m_nPluNoID[7] = IDC_EDIT_PLUNO_7;
	m_nPluNoID[8] = IDC_EDIT_PLUNO_8;
	m_nPluNoID[9] = IDC_EDIT_PLUNO_9;
	m_nPluNoID[10] = IDC_EDIT_PLUNO_10;
	m_nPluNoID[11] = IDC_EDIT_PLUNO_11;
	m_nPluNoID[12] = IDC_EDIT_PLUNO_12;
	m_nPluNoID[13] = IDC_EDIT_PLUNO_13;
	
	m_nInputLen = nInputLen;
	
	UpdateEditBoxStringsBarcode();
	UpdateEditBoxStringsPlu();
}

/**********************************************************************/

void CBarcodeTableEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeTableEditDlg)
	DDX_Check(pDX, IDC_BARCODENO_CHECKSUM, m_bChecksumBarcode);
	DDX_Check(pDX, IDC_PLUNO_CHECKSUM, m_bChecksumPlu);
	DDX_Control(pDX, IDC_STATIC_BARCODE, m_staticBarcode);
	DDX_Control(pDX, IDC_STATIC_PLU, m_staticPlu);
	DDX_Control(pDX, IDC_SPIN_PLU, m_spinPlu);
	DDX_Control(pDX, IDC_PLUNO_CHECKSUM, m_checkPluChecksum );
	DDX_Control(pDX, IDC_BITMAP3, m_bitmapPlu1);
	DDX_Control(pDX, IDC_BITMAP4, m_bitmapPlu2);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_buttonCopy);
	//}}AFX_DATA_MAP

	for ( int n = 0; n < Super.MaxBarcodeLen(); n++ )
		DDX_Text(pDX, m_nBarcodeNoID[n], m_strBarcodeNo[n] );

	for ( int n = 0; n < Super.MaxPluLen(); n++ )
		DDX_Text(pDX, m_nPluNoID[n], m_strPluNo[n] );
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CBarcodeTableEditDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeTableEditDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_BARCODE, OnDeltaPosSpinBarcode)
	ON_BN_CLICKED(IDC_BARCODENO_CHECKSUM, OnToggleChecksumBarcode)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_0, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_1, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_2, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_3, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_4, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_5, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_6, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_7, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_8, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_9, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_10, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_11, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_12, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_13, OnKillFocusEditBarcodeNo)
	ON_EN_KILLFOCUS(IDC_EDIT_BARCODENO_14, OnKillFocusEditBarcodeNo)

	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PLU, OnDeltaPosSpinPlu)
	ON_BN_CLICKED(IDC_PLUNO_CHECKSUM, OnToggleChecksumPlu)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_0, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_1, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_2, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_3, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_4, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_5, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_6, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_7, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_8, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_9, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_10, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_11, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_12, OnKillFocusEditPluNo)
	ON_EN_KILLFOCUS(IDC_EDIT_PLUNO_13, OnKillFocusEditPluNo)

	ON_BN_CLICKED(IDC_BUTTON_COPY, OnButtonCopy )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CBarcodeTableEditDlg::GetEditBarcodeNo( int n ) { return GetEdit ( m_nBarcodeNoID[n] ); }
CEdit* CBarcodeTableEditDlg::GetEditPluNo( int n ) { return GetEdit ( m_nPluNoID[n] ); }
/**********************************************************************/

BOOL CBarcodeTableEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for ( int n = 0; n < Super.MaxBarcodeLen(); n++ )
		SubclassEdit ( m_nBarcodeNoID[n], SS_NUM, 1, "%s" );

	for ( int n = 0; n < Super.MaxPluLen(); n++ )
		SubclassEdit ( m_nPluNoID[n], SS_NUM, 1, "%s" );
	
	UpdateEditBoxEnablesBarcode();
	UpdateEditBoxEnablesPlu();

	if ( m_nInputLen < 14 )
		SetWindowText( "Edit Barcode & Plu Number Entry Details" );
	else
	{
		SetWindowText( "Edit Barcode Number Entry Details" );

		m_staticBarcode.ShowWindow( SW_HIDE );
		m_staticPlu.ShowWindow( SW_HIDE );

		m_spinPlu.ShowWindow( SW_HIDE );
		
		for ( int n = 0; n < Super.MaxPluLen(); n++ )
			GetEditPluNo( n ) -> ShowWindow( SW_HIDE );

		m_checkPluChecksum.ShowWindow( SW_HIDE );
		
		m_bitmapPlu1.ShowWindow( SW_HIDE );
		m_bitmapPlu2.ShowWindow( SW_HIDE );

		m_buttonCopy.ShowWindow( SW_HIDE );
	}

	return TRUE;  
}

/**********************************************************************/

void CBarcodeTableEditDlg::OnToggleChecksumBarcode() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		UpdateFormatStringBarcode();

		if ( m_bChecksumBarcode == TRUE )
		{
			if ( m_infoFormat.m_strBarcodeFormat.Right( 1) == "n" )
			{
				CString strTemp = m_infoFormat.m_strBarcodeFormat.Right ( Super.MaxBarcodeLen() - 1 );
				m_infoFormat.m_strBarcodeFormat.Format ( "%sc", (const char*) strTemp );
			}
			else 
			{
				CString strTemp = m_infoFormat.m_strBarcodeFormat.Left ( Super.MaxBarcodeLen() - 1 );
				m_infoFormat.m_strBarcodeFormat.Format ( "%sc", (const char*) strTemp );
			}
		}
		else
		{
			CString strTemp = m_infoFormat.m_strBarcodeFormat.Left ( Super.MaxBarcodeLen() - 1 );
			m_infoFormat.m_strBarcodeFormat.Format ( "%s0", (const char*) strTemp );
		}

		UpdateEditBoxStringsBarcode();
		UpdateEditBoxEnablesBarcode();
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CBarcodeTableEditDlg::OnToggleChecksumPlu() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		UpdateFormatStringPlu();

		if ( m_bChecksumPlu == TRUE )
		{
			if ( m_infoFormat.m_strPluFormat.Right ( 1) == "n" )
			{
				CString strTemp = m_infoFormat.m_strPluFormat.Right ( Super.MaxPluLen() - 1 );
				m_infoFormat.m_strPluFormat.Format ( "%sc", (const char*) strTemp );
			}
			else 
			{
				CString strTemp = m_infoFormat.m_strPluFormat.Left ( Super.MaxPluLen() - 1 );
				m_infoFormat.m_strPluFormat.Format ( "%sc", (const char*) strTemp );
			}
		}
		else
		{
			CString strTemp = m_infoFormat.m_strPluFormat.Left ( Super.MaxPluLen() - 1 );
			m_infoFormat.m_strPluFormat.Format ( "%s0", (const char*) strTemp );
		}

		UpdateEditBoxStringsPlu();
		UpdateEditBoxEnablesPlu();
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CBarcodeTableEditDlg::OnDeltaPosSpinBarcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString strTemp;

	if ( UpdateData ( TRUE ) == TRUE )
	{
		UpdateFormatStringBarcode();

		if ( pNMUpDown -> iDelta > 0 )				//shift left
		{
			if ( m_infoFormat.m_strBarcodeFormat.Left ( 1 ) != "n" )
			{
				if ( m_infoFormat.m_strBarcodeFormat.Right ( 1 ) != "c" )
				{
					strTemp = m_infoFormat.m_strBarcodeFormat.Right ( Super.MaxBarcodeLen() - 1 );
					m_infoFormat.m_strBarcodeFormat.Format ( "%s0", (const char*) strTemp );
				}
				else
				{
					strTemp = m_infoFormat.m_strBarcodeFormat.Mid ( 1, Super.MaxBarcodeLen() - 2 );
					m_infoFormat.m_strBarcodeFormat.Format ( "%s0c", (const char*) strTemp );
				}
			}
		}
		else										//shift right
		{
			if ( m_infoFormat.m_strBarcodeFormat.Right ( 1 ) != "n" && m_infoFormat.m_strBarcodeFormat.Right( 2 ) != "nc" )
			{
				if ( m_infoFormat.m_strBarcodeFormat.Right ( 1 ) != "c" )
				{
					strTemp = m_infoFormat.m_strBarcodeFormat.Left ( Super.MaxBarcodeLen() - 1 );
					m_infoFormat.m_strBarcodeFormat.Format ( "0%s", (const char*) strTemp );
				}
				else
				{
					strTemp = m_infoFormat.m_strBarcodeFormat.Left ( Super.MaxBarcodeLen() - 2 );
					m_infoFormat.m_strBarcodeFormat.Format ( "0%sc", (const char*) strTemp );
				}
			}
		}

		UpdateEditBoxStringsBarcode();
		UpdateData ( FALSE );
		UpdateEditBoxEnablesBarcode();
	}
	
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeTableEditDlg::OnDeltaPosSpinPlu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString strTemp;

	if ( UpdateData ( TRUE ) == TRUE )
	{
		UpdateFormatStringPlu();

		if ( pNMUpDown -> iDelta > 0 )				//shift left
		{
			if ( m_infoFormat.m_strPluFormat.Left ( 1 ) != "n" )
			{
				if ( m_infoFormat.m_strPluFormat.Right ( 1 ) != "c" )
				{
					strTemp = m_infoFormat.m_strPluFormat.Right ( Super.MaxPluLen() - 1 );
					m_infoFormat.m_strPluFormat.Format ( "%s0", (const char*) strTemp );
				}
				else
				{
					strTemp = m_infoFormat.m_strPluFormat.Mid ( 1, Super.MaxPluLen() - 2 );
					m_infoFormat.m_strPluFormat.Format ( "%s0c", (const char*) strTemp );
				}
			}
		}
		else										//shift right
		{
			if ( m_infoFormat.m_strPluFormat.Right ( 1 ) != "n" && m_infoFormat.m_strPluFormat.Right( 2 ) != "nc" )
			{
				if ( m_infoFormat.m_strPluFormat.Right ( 1 ) != "c" )
				{
					strTemp = m_infoFormat.m_strPluFormat.Left ( Super.MaxPluLen() - 1 );
					m_infoFormat.m_strPluFormat.Format ( "0%s", (const char*) strTemp );
				}
				else
				{
					strTemp = m_infoFormat.m_strPluFormat.Left ( Super.MaxPluLen() - 2 );
					m_infoFormat.m_strPluFormat.Format ( "0%sc", (const char*) strTemp );
				}
			}
		}

		UpdateEditBoxStringsPlu();
		UpdateData ( FALSE );
		UpdateEditBoxEnablesPlu();
	}
	
	*pResult = 0;
}

/**********************************************************************/

void CBarcodeTableEditDlg::OnKillFocusEditBarcodeNo() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		UpdateFormatStringBarcode();
		UpdateEditBoxStringsBarcode();
		UpdateEditBoxEnablesBarcode();
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CBarcodeTableEditDlg::OnKillFocusEditPluNo() 
{
	if ( UpdateData ( TRUE ) == TRUE )
	{
		UpdateFormatStringPlu();
		UpdateEditBoxStringsPlu();
		UpdateEditBoxEnablesPlu();
		UpdateData ( FALSE );
	}
}

/**********************************************************************/

void CBarcodeTableEditDlg::OnButtonCopy() 
{
	CString strTest = m_infoFormat.m_strBarcodeFormat.Left(1);
	
	if ( ( strTest < "0" ) || ( strTest > "9" ) )
	{
		CString strMsg = "";
		strMsg += "This barcode entry format cannot be used for Plu numbers\n";
		strMsg += "because the first digit of the barcode is not fixed.";
		Prompter.Error( strMsg );
	}
	else
	{
		m_infoFormat.m_strPluFormat = m_infoFormat.m_strBarcodeFormat.Right( Super.MaxPluLen() );
		UpdateEditBoxStringsPlu();
		UpdateEditBoxEnablesPlu();
		UpdateData ( FALSE );
	}	
}

/**********************************************************************/

void CBarcodeTableEditDlg::UpdateEditBoxStringsBarcode()
{
	if ( m_infoFormat.m_strBarcodeFormat.GetLength() == Super.MaxBarcodeLen() )
	{
		for ( int n = 0; n < Super.MaxBarcodeLen(); n++ )
			m_strBarcodeNo[n] = m_infoFormat.m_strBarcodeFormat.GetAt(n);

		m_bChecksumBarcode = ( m_strBarcodeNo[Super.MaxBarcodeLen() - 1] == "c" ) ? TRUE : FALSE;
	}
}

/**********************************************************************/

void CBarcodeTableEditDlg::UpdateEditBoxStringsPlu()
{
	if ( m_infoFormat.m_strPluFormat.GetLength() == Super.MaxPluLen() )
	{
		for ( int n = 0; n < Super.MaxPluLen(); n++ )
			m_strPluNo[n] = m_infoFormat.m_strPluFormat.GetAt(n);

		m_bChecksumPlu = ( m_strPluNo[Super.MaxPluLen() - 1] == "c" ) ? TRUE : FALSE;
	}
}

/**********************************************************************/

void CBarcodeTableEditDlg::UpdateEditBoxEnablesBarcode()
{
	for ( int n = 0; n < Super.MaxBarcodeLen(); n++ )
		GetEditBarcodeNo(n) -> EnableWindow ( ! ( m_strBarcodeNo[n] == 'c' || m_strBarcodeNo[n] == 'n' ) );
}

/**********************************************************************/

void CBarcodeTableEditDlg::UpdateEditBoxEnablesPlu()
{
	for ( int n = 0; n < Super.MaxPluLen(); n++ )
		GetEditPluNo(n) -> EnableWindow ( ! ( m_strPluNo[n] == 'c' || m_strPluNo[n] == 'n' ) );
}

/**********************************************************************/

void CBarcodeTableEditDlg::UpdateFormatStringBarcode()
{
	m_infoFormat.m_strBarcodeFormat = "";
	for (int n = 0; n < Super.MaxBarcodeLen(); n++)
	{
		if (m_strBarcodeNo[n] != "")
		{
			m_infoFormat.m_strBarcodeFormat += m_strBarcodeNo[n];
		}
		else
		{
			m_infoFormat.m_strBarcodeFormat += "0";
		}
	}
}

/**********************************************************************/

void CBarcodeTableEditDlg::UpdateFormatStringPlu()
{
	m_infoFormat.m_strPluFormat = "";
	for (int n = 0; n < Super.MaxPluLen(); n++)
	{
		if (m_strPluNo[n] != "")
		{
			m_infoFormat.m_strPluFormat += m_strPluNo[n];
		}
		else
		{
			m_infoFormat.m_strPluFormat += "0";
		}
	}
}

/**********************************************************************/
