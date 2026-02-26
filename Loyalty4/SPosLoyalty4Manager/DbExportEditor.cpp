//*******************************************************************
#include "DbExportHandler.h"
#include "ExportPropertySheet.h"
#include "LoyaltyManager.h"
//*******************************************************************
#include "DbExportEditor.h"
//*******************************************************************

CDbExportEditor::CDbExportEditor( bool bLockBasicFields, CWnd* pParent)
{
	m_bLockBasicFields = bLockBasicFields;
	m_pParent = pParent;
}

//*******************************************************************

bool CDbExportEditor::EditLabel ( const char* szLabel, const char* szTitle )
{
	bool bReply = FALSE;

	CDbExportHandler exportHandler{};
	exportHandler.Read ( szLabel );

	CString strTitle = szTitle;
	if (strTitle.GetLength() > 0)
	{
		exportHandler.SetTitle(strTitle);			// overwrite default title
	}

	CExportPropertySheet propExport ( &exportHandler, m_bLockBasicFields, m_pParent, 0 );
	if ( propExport.DoModal() == IDOK )
	{
		// check for filename
		if ((exportHandler.m_strFilename.GetLength() == 0) && (exportHandler.m_nExportFlag == nDBEXPORTFIELDS_DATA))	
		{
			exportHandler.Delete(szLabel);			// no file = delete label
		}
		else
		{
			if ((bReply = exportHandler.Save(szLabel)) == FALSE)
			{
				Prompter.Error("Saving export paramaters failed", "Error");
			}
		}
	}

	return bReply;
}

//*******************************************************************
