/**********************************************************************/
#include "DefEditMacroLocalDlg.h"
#include "Password.h"
#include "SPOSKeyboardBrowsePluDlg.h"
#include "SPOSKeyboardKeyHelpers.h"
#include "SPOSKeyboardPropertySheetEditKey.h"
/**********************************************************************/
#include "SPOSKeyboardPropPageEditKeyType.h"
/**********************************************************************/

CSPOSKeyboardPropPageEditKeyType::CSPOSKeyboardPropPageEditKeyType() : CSSPropertyPage(CSPOSKeyboardPropPageEditKeyType::IDD), m_MacroSelector( m_editMacro, m_comboMacro )
{
	//{{AFX_DATA_INIT(CSPOSKeyboardPropPageEditKeyType)
	m_strValue = "0";
	//}}AFX_DATA_INIT
	m_strMinValue = "0";
	m_strMaxValue = "1";
	m_bDefaultTextMode = FALSE;
	m_PluFilterArray.SetDescriptionFilter ( "" );
	m_PluFilterArray.SetShowModifiersFlag( TRUE );
	m_PluFilterArray.BuildRecordList();
	m_pParentSheet = NULL;
	m_nMacroNo = 0;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::SetKeyRecord( CSPOSKeyboardKeyRecord* pRecord )
{
	m_pKeyRecord = pRecord;
}

/**********************************************************************/

CSPOSKeyboardPropPageEditKeyType::~CSPOSKeyboardPropPageEditKeyType()
{
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPOSKeyboardPropPageEditKeyType)
	DDX_Control(pDX, IDC_EDIT_TEXT, m_editText);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_KEYTEXT, m_buttonHide);
	DDX_Control(pDX, IDC_CHECK_HIDE, m_buttonHide);
	DDX_Control(pDX, IDC_CHECK_DEFAULT, m_buttonDefaultTextAlways);
	DDX_Control(pDX, IDC_CHECK_NOSORT, m_buttonNoSort);
	DDX_Control(pDX, IDC_COMBO_VALUE, m_comboValue);
	DDX_Control(pDX, IDC_COMBO_MACRO, m_comboMacro);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_EDIT_MACRO, m_editMacro);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strValue);

	if ( m_nType != KEY_TYPE_MACRO )
	{
		DDV_IntegerString(pDX, m_strValue, m_strMinValue, m_strMaxValue); 
	}
}

/**********************************************************************/

BEGIN_MESSAGE_MAP(CSPOSKeyboardPropPageEditKeyType, CPropertyPage)
	//{{AFX_MSG_MAP(CSPOSKeyboardPropPageEditKeyType)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelectType)
	ON_CBN_SELCHANGE(IDC_COMBO_VALUE, OnSelectValue)
	ON_EN_KILLFOCUS(IDC_EDIT_VALUE, OnKillFocusValue)
	ON_EN_KILLFOCUS(IDC_EDIT_MACRO, OnKillFocusMacro)
	ON_CBN_SELCHANGE(IDC_COMBO_MACRO, OnSelectMacro)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_KEYTEXT, OnButtonKeyText)
	ON_EN_KILLFOCUS(IDC_EDIT_TEXT, RedrawKey)
	ON_BN_CLICKED(IDC_CHECK_HIDE, RedrawKey)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT, RedrawKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**********************************************************************/
CEdit* CSPOSKeyboardPropPageEditKeyType::GetEditValue() { return GetEdit( IDC_EDIT_VALUE ); }
/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyType::OnInitDialog() 
{
	m_nType = m_pKeyRecord -> GetType();
	m_strValue.Format( "%I64d", m_pKeyRecord -> GetValue() );
	
	CPropertyPage::OnInitDialog();

	m_nMacroNo = 0;
	if ( KEY_TYPE_MACRO == m_nType )
	{
		__int64 nValue = m_pKeyRecord -> GetValue();

		if ( ( nValue >= 0 ) && ( nValue <= 9999 ) )
		{
			m_nMacroNo = (int) nValue;
		}

		ShowAndEnableWindow( &m_comboValue, FALSE );
		ShowAndEnableWindow( GetEditValue(), FALSE );
		m_buttonBrowse.SetWindowText( "Edit" );
		ShowAndEnableWindow( &m_buttonBrowse, PasswordArray.GetEnable( PasswordTicks::SetupMenuMacros ) );
	}
	else
	{
		ShowAndEnableWindow( &m_comboMacro, FALSE );
		ShowAndEnableWindow( &m_editMacro, FALSE );
		m_buttonBrowse.SetWindowText( "Browse" );
		ShowAndEnableWindow( &m_buttonBrowse, KEY_TYPE_PLUNUMBER == m_nType );
	}

	m_editMacro.LimitText( 4 );
	m_MacroSelector.FillMacroCombo(m_nMacroNo);

	SetupEditBox( "1", "9", 1 );

	m_comboType.AddString( "Empty" );
	m_comboType.AddString( "Function" );
	m_comboType.AddString( "Payment" );
	m_comboType.AddString( "Modifier" );
	m_comboType.AddString( "Control" );
	m_comboType.AddString( "List" );
	m_comboType.AddString( "Svr Preset" );
	m_comboType.AddString( "Macro" );
	m_comboType.AddString( "Plu Number" );
	m_comboType.SetItemData ( 0, KEY_TYPE_EMPTY );
	m_comboType.SetItemData ( 1, KEY_TYPE_FUNCTION );
	m_comboType.SetItemData ( 2, KEY_TYPE_PAYMENT );
	m_comboType.SetItemData ( 3, KEY_TYPE_MODIFIER );
	m_comboType.SetItemData ( 4, KEY_TYPE_CONTROL );	
	m_comboType.SetItemData ( 5, KEY_TYPE_LIST );	
	m_comboType.SetItemData ( 6, KEY_TYPE_SVRPRESET );
	m_comboType.SetItemData ( 7, KEY_TYPE_MACRO );
	m_comboType.SetItemData ( 8, KEY_TYPE_PLUNUMBER );

	if ( SelectComboItemType( m_nType ) == FALSE )
	{
		m_nType = KEY_TYPE_EMPTY;
		SelectComboItemType( m_nType );
	}

	if ( KEY_TYPE_MACRO != m_nType )
	{
		FillValueCombo( FALSE );
	}

	m_editText.LimitText( 80 );
	CString strText = m_pKeyRecord -> GetEditableKeyText();
	m_editText.SetWindowText ( strText );

	m_buttonHide.SetCheck( m_pKeyRecord -> GetHideTextFlag() );
	m_buttonDefaultTextAlways.SetCheck( m_pKeyRecord -> GetDefaultTextAlwaysFlag() );
	m_buttonNoSort.SetCheck( m_pKeyRecord -> GetGJANoSortFlag() );
	
	if ( m_bDefaultTextMode == TRUE )
	{
		m_comboType.EnableWindow( FALSE );
		m_comboValue.EnableWindow( FALSE );
		GetEditValue() -> EnableWindow( FALSE );
		m_buttonBrowse.ShowWindow( SW_HIDE );
		m_buttonNoSort.ShowWindow( SW_HIDE );
		m_buttonNoSort.EnableWindow( FALSE );
	}

	return TRUE;  
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::SetupEditBox ( const char* szMinValue, const char* szMaxValue, int nWidth )
{
	m_strMinValue = szMinValue;
	m_strMaxValue = szMaxValue;
	SubclassEdit ( IDC_EDIT_VALUE, SS_NUM, nWidth, "%s" );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::FillValueCombo( bool bReset )
{	
	UpdateData( TRUE, FALSE );

	m_comboValue.ResetContent();
	m_arrayComboValues.RemoveAll();

	CSPOSKeyboardDefaultTextArray* pFunctionTable = NULL;

	switch ( m_nType )
	{
	case KEY_TYPE_FUNCTION:		pFunctionTable = SPOSKeyboardManager.GetFunctionTable();		break;
	case KEY_TYPE_PAYMENT:		pFunctionTable = SPOSKeyboardManager.GetPaymentTable();			break;
	case KEY_TYPE_CONTROL:		pFunctionTable = SPOSKeyboardManager.GetControlTable();			break;
	case KEY_TYPE_MODIFIER:		pFunctionTable = SPOSKeyboardManager.GetModifierTable();		break;
	case KEY_TYPE_SVRPRESET:	pFunctionTable = SPOSKeyboardManager.GetSvrPresetTable();		break;
	case KEY_TYPE_EMPTY:		pFunctionTable = SPOSKeyboardManager.GetEmptyTable();		break;
	}

	if ( pFunctionTable != NULL )
	{
		for ( int nTextIdx = 0; nTextIdx < pFunctionTable -> GetSize(); nTextIdx++ )
		{
			CSPOSKeyboardDefaultTextRecord TextRecord;
			pFunctionTable -> GetTextRecord( nTextIdx, TextRecord );
			__int64 nValue = TextRecord.GetValue();

			if ( m_nType == KEY_TYPE_LIST )
			{
				int nListIdx;
				if ( SPOSKeyboardManager.GetListManager() -> FindList( nValue, nListIdx ) == FALSE )
					continue;
			}
			
			CSPOSKeyboardKeyHelpers KeyHelpers;
			int nPos = m_comboValue.AddString ( KeyHelpers.GetCombiText( m_nType, TextRecord.GetValue() ) );
			
			CString strValue;
			strValue.Format( "%d", TextRecord.GetValue() );
			m_arrayComboValues.Add( strValue );
		}

		CString strMinVal;
		strMinVal.Format( "%d", pFunctionTable -> GetMinValue() );

		CString strMaxVal;
		strMaxVal.Format( "%d", pFunctionTable -> GetMaxValue() );
		
		SetupEditBox ( strMinVal, strMaxVal, strMaxVal.GetLength() );
		
		if ( bReset ) 
			m_strValue = strMinVal;
	}
	else if ( m_nType == KEY_TYPE_PLUNUMBER )
	{	
		SetupEditBox( "1", "99999999999999", 14 );

		for ( int nIndex = 0; nIndex < SPOSKeyboardManager.GetPluDatabase() -> GetSize(); nIndex++ )
		{
			CSPOSKeyboardImportPluCSVRecord ImportPlu;
			SPOSKeyboardManager.GetPluDatabase() -> GetAt ( nIndex, ImportPlu );
			ImportPlu.AddToCombo( m_comboValue );

			CString strValue;
			strValue.Format( "%I64d", ImportPlu.GetPluNumber() );
			m_arrayComboValues.Add( strValue );
		}

		if ( bReset )
		{
			if ( m_arrayComboValues.GetSize() > 0 )
				m_strValue = m_arrayComboValues.GetAt(0);
			else
				m_strValue = "1";
		}
	}
	else if ( m_nType == KEY_TYPE_LIST )
	{
		SetupEditBox( "1", "32767", 5 );
		
		for ( int nListIdx = 0; nListIdx < SPOSKeyboardManager.GetListManager() -> GetListCount(); nListIdx++ )
		{
			CSPOSKeyboardListRecord ListRecord;
			SPOSKeyboardManager.GetListManager() -> GetListRecord( nListIdx, ListRecord );

			CSPOSKeyboardKeyHelpers KeyHelpers;
			__int64 nListNo = ListRecord.GetListNo();
			CString strCombiText = KeyHelpers.GetCombiText( KEY_TYPE_LIST, nListNo );

			int nPos = m_comboValue.AddString( strCombiText );
			
			CString strValue;
			strValue.Format( "%d", ListRecord.GetListNo() );
			m_arrayComboValues.InsertAt( nPos, strValue );
		}
		
		if ( bReset )
		{
			if ( m_arrayComboValues.GetSize() > 0 )
				m_strValue = m_arrayComboValues.GetAt(0);
			else
				m_strValue = "1";
		}
	}
	
	GetEditValue() -> EnableWindow( m_nType != KEY_TYPE_EMPTY );
	UpdateData( FALSE );

	m_comboValue.EnableWindow ( m_comboValue.GetCount() >= 2 );
	SelectComboItemValue();
		
	m_buttonBrowse.EnableWindow( KEY_TYPE_PLUNUMBER == m_nType );
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyType::OnKillActive() 
{
	bool bResult = ( CPropertyPage::OnKillActive() != 0 );
	SaveRecord();
	return bResult;
}

/**********************************************************************/

BOOL CSPOSKeyboardPropPageEditKeyType::OnSetActive() 
{
	bool bResult = ( CPropertyPage::OnSetActive() != 0 );
	return bResult;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::UpdateKeyTypeAndValue()
{
	m_pKeyRecord -> SetType( m_nType );

	if ( KEY_TYPE_MACRO != m_nType )
	{
		m_pKeyRecord -> SetValue( _atoi64( m_strValue ) );
	}
	else
	{
		m_pKeyRecord -> SetValue( m_nMacroNo );
	}

	CSPOSKeyboardKeyHelpers KeyHelpers;
	CSPOSKeyboardDefaultTextRecord TextRecord;
	KeyHelpers.GetDefaultTextRecord( *m_pKeyRecord, TextRecord );

	CString strText = "";
	TextRecord.GetDefaultText( strText );
	
	m_pKeyRecord -> SetKeyText( strText );
	m_pKeyRecord -> SetImageFilename( TextRecord.GetImageFilename() );
	m_pKeyRecord -> SetImageScale( TextRecord.GetImageScale() );

	m_editText.SetWindowText ( m_pKeyRecord -> GetEditableKeyText() );
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::RedrawKey()
{
	if ( m_pParentSheet != NULL )
	{
		SaveRecord();
		( (CSPOSKeyboardPropertySheetEditKey*) m_pParentSheet ) -> RedrawKey();
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::OnSelectType() 
{
	int nSel = m_comboType.GetCurSel();
	if ( nSel < 0 || nSel >= m_comboType.GetCount() )
		return;

	int nType = m_comboType.GetItemData(nSel);
	if ( nType == m_nType )
		return;

	m_nType = nType;

	bool bIsMacro = ( KEY_TYPE_MACRO == m_nType );
	bool bIsPlu = ( KEY_TYPE_PLUNUMBER == m_nType );

	if ( FALSE == bIsMacro )
	{
		FillValueCombo( TRUE );
		m_buttonBrowse.SetWindowText( "Browse" );
	}
	else
	{
		m_buttonBrowse.SetWindowText( "Edit" );
	}
	
	ShowAndEnableWindow( &m_comboValue, !bIsMacro );
	ShowAndEnableWindow( GetEditValue(), !bIsMacro );
	ShowAndEnableWindow( &m_comboMacro, bIsMacro );
	ShowAndEnableWindow( &m_editMacro, bIsMacro );

	if ( TRUE == bIsMacro )
	{
		ShowAndEnableWindow( &m_buttonBrowse, PasswordArray.GetEnable( PasswordTicks::SetupMenuMacros ) );
	}
	else if ( TRUE == bIsPlu )
	{
		ShowAndEnableWindow( &m_buttonBrowse, TRUE );
	}
	else 
	{
		ShowAndEnableWindow( &m_buttonBrowse, FALSE );
	}

	UpdateKeyTypeAndValue();
	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::OnSelectValue() 
{
	CString strOldValue = m_strValue;

	UpdateData ( TRUE, FALSE );

	int nSel = m_comboValue.GetCurSel();
	if ( ( nSel >= 0 ) && ( nSel < m_arrayComboValues.GetSize() ) )
		m_strValue = m_arrayComboValues.GetAt( nSel );
	else
		m_strValue = "0";

	UpdateData( FALSE );

	if ( m_strValue != strOldValue )
		UpdateKeyTypeAndValue();

	RedrawKey();
}

/**********************************************************************/

bool CSPOSKeyboardPropPageEditKeyType::SelectComboItemType( DWORD wItemData )
{
	for ( int nSel = 0; nSel < m_comboType.GetCount(); nSel++ )
	{
		if ( m_comboType.GetItemData( nSel ) == wItemData ) 
		{
			m_comboType.SetCurSel( nSel );
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

bool CSPOSKeyboardPropPageEditKeyType::SelectComboItemValue()
{
	for ( int nSel = 0; nSel < m_arrayComboValues.GetSize(); nSel++ )
	{
		if ( _atoi64(m_strValue) == _atoi64(m_arrayComboValues.GetAt( nSel ) ) ) 
		{
			m_comboValue.SetCurSel( nSel );
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::OnKillFocusValue() 
{
	CString strOldValue = m_strValue;

	UpdateData( TRUE, FALSE );
	if ( SelectComboItemValue() == FALSE )
		m_comboValue.SetCurSel(-1);
	
	if ( m_strValue != strOldValue )
		UpdateKeyTypeAndValue();

	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::OnButtonKeyText()
{
	CString strWindowText;
	m_editText.GetWindowText( strWindowText );

	CSPOSKeyboardKeyHelpers KeyHelpers;
	CSPOSKeyboardDefaultTextRecord TextRecord;
	KeyHelpers.GetDefaultTextRecord( *m_pKeyRecord, TextRecord );

	CString strDefaultText = "";
	TextRecord.GetDefaultText( strDefaultText );

	CSPOSKeyboardKeyRecord KeyRecord;
	KeyRecord.SetKeyText( strDefaultText );
	CString strEditableText = KeyRecord.GetEditableKeyText();

	if ( strWindowText == strEditableText )
		Prompter.Info( "Defult text is already selected for this key" );
	else
	{
		CString strPrompt = "";
		strPrompt += "Restore default key text:\n\n";
		strPrompt += strDefaultText;

		if ( Prompter.YesNo( strPrompt ) == IDYES )
		{	
			m_pKeyRecord -> SetKeyText( strDefaultText );
			m_editText.SetWindowText( strEditableText );
			RedrawKey();
		}
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::OnButtonBrowse() 
{
	UpdateData ( TRUE, FALSE );

	switch ( m_nType )
	{
	case KEY_TYPE_PLUNUMBER:
		{
			CSPOSKeyboardBrowsePluDlg dlg ( m_PluFilterArray, this );
			if ( dlg.DoModal() == IDOK )
			{
				CString strOldValue = m_strValue;
				m_strValue.Format( "%I64d", dlg.GetPluNo() );
				UpdateData( FALSE );

				if ( SelectComboItemValue() == FALSE )
					m_comboValue.SetCurSel(-1);
	
				if ( m_strValue != strOldValue )
					UpdateKeyTypeAndValue();
				
				GetEditValue() -> SetFocus();
				GetEditValue() -> SetSel(0,-1);

				RedrawKey();
			}
		}
		break;

	case KEY_TYPE_MACRO:
		{
			CDataManagerInfo infoDM;
			if ( DataManagerNonDb.OpenCCMacroKeyboard( DB_READWRITE, infoDM ) == TRUE )
			{
				SPOSKeyboardManager.MergeParentMacros();

				int nMacroIdx;
				if ( DataManagerNonDb.CCMacroKeyboard.FindMacroByNumber( m_nMacroNo, nMacroIdx ) == FALSE )
				{
					CString strName;
					strName.Format( "Macro %d", m_nMacroNo );

					CCascadingMacroCSVRecord Macro;
					Macro.SetMacroNo( m_nMacroNo );
					Macro.SetParentMacroTypeExternal( NODE_PLACEHOLDER );
					Macro.SetParentMacroName( "" );
					Macro.SetParentMacroData( "" );
					Macro.SetLocalMacroFlag( TRUE );
					Macro.SetLocalMacroData( "" );
					Macro.SetLocalMacroName( strName );
					DataManagerNonDb.CCMacroKeyboard.InsertAt( nMacroIdx, Macro );
				}

				CListDataDlgChangeFlag DummyFlag;
				CDefEditMacroLocalDlg dlgEdit( nMacroIdx, NODE_DBKEYBOARDSET, FALSE, DummyFlag, this );
				
				if ( dlgEdit.DoModal() == IDOK )
				{
					if ( DataManagerNonDb.WriteCCMacroKeyboard( infoDM ) == FALSE )
					{
						Prompter.WriteError( infoDM );
					}
					else
					{
						SPOSKeyboardManager.CreateActiveMacroList();
						m_MacroSelector.FillMacroCombo( m_nMacroNo );
					}
				}
			
				DataManagerNonDb.CloseCCMacroKeyboard( infoDM );
			}	
			else
			{
				Prompter.ShareError( infoDM );
			}
		}
		break;
	}
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::OnSelectMacro()
{
	m_MacroSelector.SelectMacroFromCombo();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
	UpdateKeyTypeAndValue();
	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::OnKillFocusMacro()
{
	m_MacroSelector.SelectMacroFromEditBox();
	m_nMacroNo = m_MacroSelector.GetMacroNo();
	UpdateKeyTypeAndValue();
	RedrawKey();
}

/**********************************************************************/

void CSPOSKeyboardPropPageEditKeyType::SaveRecord()
{
	m_pKeyRecord -> SetType( m_nType );

	if ( KEY_TYPE_MACRO != m_nType )
	{
		m_pKeyRecord -> SetValue( _atoi64( m_strValue ) );
	}
	else
	{
		m_pKeyRecord -> SetValue( m_nMacroNo );
	}

	m_pKeyRecord -> SetHideTextFlag( m_buttonHide.GetCheck() != 0 );
	m_pKeyRecord -> SetDefaultTextAlwaysFlag( m_buttonDefaultTextAlways.GetCheck() != 0 );
	m_pKeyRecord -> SetGJANoSortFlag( m_buttonNoSort.GetCheck() != 0 );

	CString strText;
	m_editText.GetWindowText ( strText );
	m_pKeyRecord -> SetEditableKeyText( strText );
}

/**********************************************************************/
