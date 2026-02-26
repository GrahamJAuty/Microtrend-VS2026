/**********************************************************************/
 
/**********************************************************************/
//#include "globalfunctions.h"
/**********************************************************************/
#include "LabelNameHandler.h"
/**********************************************************************/

CLabelNameHandler::CLabelNameHandler()
{
	m_strFilename = "Bardat\\barman.dat";
	GetDataProgramPath( m_strFilename );
}

/**********************************************************************/

void CLabelNameHandler::Read()
{
	CSSFile fileLabelNames;
	if ( fileLabelNames.Open ( m_strFilename, "rb" ) == FALSE )
		return;

	m_arrayLabelNames.RemoveAll();

	CString strBuffer;
	while ( fileLabelNames.ReadString( strBuffer ) )
	{
		CCSV csv ( strBuffer );
		m_arrayLabelNames.Add( csv.GetString(1) );
	}
}

/**********************************************************************/

void CLabelNameHandler::FillComboBox ( CSSComboBox& combo, const char* szName, bool bCanEnableCombo )
{
	CString strName = szName;

	combo.ResetContent();
	
	int nPos = -1;
	for ( int nIndex = 0; nIndex < m_arrayLabelNames.GetSize(); nIndex++ )
	{
		CString strArrayName = m_arrayLabelNames.GetAt( nIndex );
		
		if ( ( strName == strArrayName ) && ( nPos == -1 ) )
			nPos = nIndex;

		combo.AddString ( strArrayName );
	}

	int nCount = combo.GetCount();

	if ( nPos != -1 )
		combo.SetCurSel( nPos );
	else if ( nCount > 0 )
		combo.SetCurSel ( 0 );

	combo.EnableWindow ( ( nCount > 1 ) && ( TRUE == bCanEnableCombo ) );
}

/**********************************************************************/
