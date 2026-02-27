//$$******************************************************************
#include "ServerPropertySheet.h"
#include "UpgradeHandler.h"
//$$******************************************************************
#include "UpgradeDlg.h"
//$$******************************************************************

CUpgradeHandler::CUpgradeHandler( CWnd* pParent )
{
	m_pParent = pParent;
	
	// get default path of any previous system
	m_strSourcePath = GetSourceFolder();			
}

//$$******************************************************************

bool CUpgradeHandler::DefaultUpgradePossible()
{
	return ( m_strSourcePath == "" ) ? FALSE : TRUE;
}

//$$******************************************************************

CString CUpgradeHandler::GetSourceFolder()
{
	CString strPathPrefix = "";

	{
		CStringArray arrayPrefixPaths;
		arrayPrefixPaths.Add("C:\\Users\\Public\\SharpSoft");
		arrayPrefixPaths.Add("C:\\Program Files\\SharpSoft");
		arrayPrefixPaths.Add("C:\\Users\\Public\\Microtrend");
		arrayPrefixPaths.Add("C:\\Program Files\\Microtrend");

		int nPathToUse = 0;
		for (int n = 0; n < arrayPrefixPaths.GetSize(); n++)
		{
			if (_access(arrayPrefixPaths.GetAt(n), 0) == 0)
			{
				nPathToUse = n;
				break;
			}
		}

		strPathPrefix = arrayPrefixPaths.GetAt(nPathToUse);
	}

	CStringArray arraySources;
	
	//SMARTPAY V2
	arraySources.Add("\\SPosEcrManager\\P44");
	arraySources.Add("\\SPosStockManager\\P44");
	arraySources.Add("\\SPosSmartPay\\P44");
	//SMARTPAY V1
	arraySources.Add("\\SPosEcrManager\\P33");
	arraySources.Add("\\SPosStockManager\\P33");
	arraySources.Add("\\EcrX500v3\\P33");
	arraySources.Add("\\StkX500v3\\P33");
	arraySources.Add("\\EcrX500v2\\P33");
	arraySources.Add("\\StkX500v2\\P33");
	arraySources.Add("\\SmartPay\\P33");

	bool bFound = FALSE;
	CString strSourcePath = "";

	for (int n = 0; n < arraySources.GetSize(); n++)
	{
		strSourcePath = strPathPrefix + arraySources.GetAt(n);

		if (::ExistSubdirectory(strSourcePath) == TRUE)
		{
			bFound = TRUE;
			break;
		}
	}

	if (FALSE == bFound)
	{
		strSourcePath = "";
	}
	else
	{
		strSourcePath = strSourcePath.Left(strSourcePath.GetLength() - 4);
	}

	return strSourcePath;
}

//$$******************************************************************

bool CUpgradeHandler::Upgrade()
{
	bool bReply = FALSE;

	CUpgradeDlg dlgUpgrade(m_strSourcePath, m_pParent);
	if (dlgUpgrade.DoModal() == IDOK)
	{
		bReply = TRUE;

		ServerCopy.CopyFrom(Server);

		CServerPropertySheet dlg(m_pParent);

		if (dlg.DoModal() == IDOK)
		{
			dlg.Save();
		}
		else
		{
			Server.CopyFrom(ServerCopy);
		}
	}
	else
	{
		CString strMsg = "";
		strMsg += "Note:\n\n";
		strMsg += "The upgrade option will be available from the main menu until SmartPay Mananger\n";
		strMsg += "is restarted with one or more accounts already in the list.";
		Prompter.Info(strMsg, "System Upgrade");
	}

	return bReply;
}

//$$******************************************************************
