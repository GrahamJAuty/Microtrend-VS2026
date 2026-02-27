//*******************************************************************
#include "..\SmartPay4ManagerBgnd\DbExportHandler.h"
//*******************************************************************
#include "ExportPropertySheet.h"
#include "DbExportEditor.h"
//*******************************************************************

CDbExportEditor::CDbExportEditor( CWnd* pParent)
{
	m_pParent = pParent;
}

//*******************************************************************

bool CDbExportEditor::EditExportLabel ( const char* szLabel, const char* szTitle )
{
	bool bReply = FALSE;

	CDbExportHandler exportHandler{};
	exportHandler.Read ( szLabel );

	CString strTitle = szTitle;
	if (strTitle.GetLength() > 0)
	{
		exportHandler.SetTitle(strTitle);			// overwrite default title
	}
	
	CExportPropertySheet propExport ( &exportHandler, m_pParent );
	
	if ( propExport.DoModal() == IDOK )
	{
		if (exportHandler.m_strFilename.GetLength() == 0 && (exportHandler.m_nExportFlag == nDBEXPORTFIELDS_DATA))		// check for filename
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

bool CDbExportEditor::EditExportLabelFromReportInfo(CDatabaseReportInfo& info)
{
	bool bResult = FALSE;

	CDbExportHandler exportHandler{};
	exportHandler.CopyFromReportInfo(info);

	CExportPropertySheet propExport(&exportHandler, m_pParent);
	if (propExport.DoModal() == IDOK)
	{
		exportHandler.CopyToReportInfo(info);
		bResult = TRUE;
	}

	return bResult;
}

//*******************************************************************
