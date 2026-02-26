/**********************************************************************/
#include "AnalysisCategoryCSVArray.h"
/**********************************************************************/

CAnalysisCategoryCSVArray::CAnalysisCategoryCSVArray() : CSharedCSVArray()
{
	m_bGotFile = FALSE;
	m_bGotInUseArray = FALSE;
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::RemoveAll()
{
	RemoveAt( 0, GetSize() );
	m_bGotInUseArray = FALSE;
}

/**********************************************************************/

int CAnalysisCategoryCSVArray::Open( const char* szFilename, int nMode )
{
	RemoveAll();

	int nReply = CSharedCSVArray::Open ( szFilename, nMode );
	
	if ( nReply != DB_ERR_SHARE )
	{
		if ( nMode == DB_READONLY )	// if file does not exist use default values
			nReply = DB_ERR_NONE;
	}

	FillArray();

	while ( GetSize() > MAX_ACAT_NO )
		RemoveAt( GetSize() - 1 );
	
	return nReply;
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::FillArray()
{
	while ( GetSize() < MAX_ACAT_NO )
	{
		CAnalysisCategoryCSVRecord Record;
		GetDefaultRecord( GetSize() + 1, Record );
		Add( Record );
	}
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::GetDefaultRecord( int nCatNo, CAnalysisCategoryCSVRecord& Record )
{
	Record.SetEposText( "" );
	Record.SetType( ACAT_OTHER );

	switch( nCatNo )
	{
	case 1:
		Record.SetEposText( "Starter" );
		Record.SetType( ACAT_FOOD );
		break;

	case 2:
		Record.SetEposText( "Main Course" );
		Record.SetType( ACAT_FOOD );
		break;

	case 3:
		Record.SetEposText( "Dessert" );
		Record.SetType( ACAT_FOOD );
		break;

	case 4:
		Record.SetEposText( "Other Food" );
		Record.SetType( ACAT_FOOD );
		break;

	case 5:
		Record.SetEposText( "Drinks" );
		Record.SetType( ACAT_DRINKS );
		break;

	case 6:
		Record.SetEposText( "Promos" );
		Record.SetType( ACAT_OTHER );
		break;

	case 7:
		Record.SetEposText( "Accomodation" );
		Record.SetType( ACAT_OTHER );
		break;

	case 8:
		Record.SetEposText( "Misc" );
		Record.SetType( ACAT_OTHER );
		break;

	default:
		if ( ( nCatNo >= 9 ) && ( nCatNo <= MAX_ACAT_NO ) )
		{
			CString strText;
			strText.Format( "Category %d", nCatNo );
			Record.SetEposText( strText );
			Record.SetType( ACAT_OTHER );	
		}
		break;
	}
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::GetCategory( int nCatNo, CAnalysisCategoryCSVRecord& Record )
{
	int nIndex = nCatNo - 1;

	if ( nIndex >= 0 && nIndex < GetSize() )
		GetAt( nIndex, Record );
	else if ( nIndex == -1 )
	{
		Record.SetEposText( "Follow On" );
		Record.SetType( ACAT_OTHER );
	}
	else
	{
		Record.SetEposText( "" );
		Record.SetType( ACAT_OTHER );
	}
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::GetDisplayCategory( int nCatNo, CAnalysisCategoryCSVRecord& Record )
{
	int nIndex = nCatNo - 1;

	if ( nIndex >= 0 && nIndex < GetSize() )
		GetAt( nIndex, Record );
	else
		Record.SetType( ACAT_OTHER );
		
	Record.SetEposText( GetDisplayText( nCatNo ) );
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::SetCategory( int nCatNo, CAnalysisCategoryCSVRecord& Record )
{
	int nIndex = nCatNo - 1;

	if ( nIndex >= 0 && nIndex < GetSize() )
		SetAt( nIndex, Record );
}

/**********************************************************************/

const char* CAnalysisCategoryCSVArray::GetDisplayText( int nCatNo )
{
	CAnalysisCategoryCSVRecord Record;
	GetCategory( nCatNo, Record );
	m_strDisplayText = Record.GetEposText();

	if ( m_strDisplayText == "" )
	{
		GetDefaultRecord( nCatNo, Record );
		m_strDisplayText = Record.GetEposText();
	}

	if ( m_strDisplayText == "" )
		m_strDisplayText = "Unknown";

	return m_strDisplayText;
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::CreateInUseArray()
{
	if ( FALSE == m_bGotInUseArray )
	{
		CString strCategory = "Category ";

		m_arrayInUse.RemoveAll();
		for ( int nCatNo = 1; nCatNo <= MAX_ACAT_NO; nCatNo++ )
		{
			CAnalysisCategoryCSVRecord Record;
			GetCategory( nCatNo, Record );

			bool bAllow = FALSE;
			
			CString strText = Record.GetEposText();
			if ( strText != "" )
			{
				CString strNum;
				strNum.Format( "%d", nCatNo );

				if ( strText != strCategory + strNum )
					bAllow = TRUE;
			}

			if ( TRUE == bAllow )
				m_arrayInUse.Add( nCatNo );
		}

		m_bGotInUseArray = TRUE;
	}
}

/**********************************************************************/

int CAnalysisCategoryCSVArray::GetInUseCatNo( int nIdx )
{
	if ( FALSE == m_bGotInUseArray )
		return 0;

	if ( ( nIdx < 0 ) || ( nIdx >= m_arrayInUse.GetSize() ) )
		return 0;

	return m_arrayInUse.GetAt( nIdx );
}

/**********************************************************************/

const char* CAnalysisCategoryCSVArray::GetTabbedComboText( int nCatNo )
{
	m_strTabbedComboText.Format( "C%2.2d,%s",
		nCatNo,
		GetDisplayText(nCatNo) );

	return m_strTabbedComboText;
}

/**********************************************************************/

void CAnalysisCategoryCSVArray::PrepareFilterCombo(CSSTabbedComboBox& comboCategory)
{
	{
		CArray<int, int> arrayStops;
		arrayStops.Add(5);
		arrayStops.Add(40);
		comboCategory.SetTabStops(arrayStops);
	}

	comboCategory.ResetContent();

	comboCategory.AddItem(",All");
	comboCategory.SetItemData(0, 9999);

	comboCategory.AddItem(",Follow On");
	comboCategory.SetItemData(1, 0);

	DataManager.AnalysisCategory.CreateInUseArray();

	for (int i = 0; i < DataManager.AnalysisCategory.GetInUseArraySize(); i++)
	{
		int nCatNo = DataManager.AnalysisCategory.GetInUseCatNo(i);
		comboCategory.AddItem(DataManager.AnalysisCategory.GetTabbedComboText(nCatNo));
		comboCategory.SetItemData(i + 2, nCatNo);
	}

	comboCategory.SetCurSel(0);
}

/**********************************************************************/

int CAnalysisCategoryCSVArray::GetFilterFromCombo(CSSTabbedComboBox& comboCategory)
{
	int nCategory = 9999;
	{
		int nSel = comboCategory.GetCurSel();
		if ((nSel >= 0) && (nSel < comboCategory.GetCount()))
		{
			nCategory = comboCategory.GetItemData(nSel);
		}
	}

	return nCategory;
}

/**********************************************************************/
