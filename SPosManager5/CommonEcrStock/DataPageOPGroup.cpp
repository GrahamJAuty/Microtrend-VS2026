/**********************************************************************/
#include "CommsBatchDlg.h"
#include "DownloadDateDlg.h"
#include "ListDataOPGroupInfoDlg.h"
#include "ListDataOPNameInfoDlg.h"
#include "LocationCSVArray.h"
#include "Password.h"
/**********************************************************************/
#include "PropPageSystemDataList.h"
/**********************************************************************/

void CPropPageSystemDataList::PrepareOPGroupPage()
{	
}

/**********************************************************************/

void CPropPageSystemDataList::BuildOPGroupList()
{
	RebuildOPGroupList();

	if (EcrmanOptions.GetFeaturesOPNameFirstFlag() == TRUE)
	{
		m_buttonAction1.SetWindowText("Printer\n&Names");
		m_buttonAction2.SetWindowText("&Printer\nGroups");
	
	}
	else
	{
		m_buttonAction1.SetWindowText("Printer\nGroups");
		m_buttonAction2.SetWindowText("Printer\n&Names");
	}
	
	m_buttonAction3.ShowWindow(SW_HIDE);
	m_buttonAction4.ShowWindow(SW_HIDE);
	m_buttonAction5.ShowWindow(SW_HIDE);
	m_buttonAction6.ShowWindow(SW_HIDE);

	SelectLine(m_listLocation, 0);
}

/**********************************************************************/

void CPropPageSystemDataList::RebuildOPGroupList()
{
	m_arrayLocation.RemoveAll();
	m_arrayAction.RemoveAll();

	if (EcrmanOptions.GetReportsLocationSortType() == 0)
	{
		for (int nDbIdx = 0; nDbIdx < dbDatabase.GetSize(); nDbIdx++)
		{
			if (PasswordArray.CheckLocationAccessDbIdx(nDbIdx) == TRUE)
			{
				AddDatabase(nDbIdx);
			}
		}
	}
	else
	{
		CWordArray arrayDbIdx;
		dbDatabase.SortByName(arrayDbIdx);

		for (int n = 0; n < arrayDbIdx.GetSize(); n++)
		{
			int nDbIdx = arrayDbIdx.GetAt(n);
			if (PasswordArray.CheckLocationAccessDbIdx(nDbIdx) == TRUE)
			{
				AddDatabase(nDbIdx);
			}
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectLocationOPGroup()
{
	int nAction = 0;
	CLocationSelectorEntity LocSelEntity;
	GetOPGroupAction(LocSelEntity, nAction);

	m_arrayAction.RemoveAll();
	m_listAction.DeleteAllItems();

	AddAction(999999, "System Printers");
	AddAction(999998, "Database Printers");

	int nDbIdx = LocSelEntity.GetDbIdx();

	CArray<int, int> arrayLocIdx;
	if (EcrmanOptions.GetReportsLocationSortType() == 0)
	{
		dbLocation.GetDatabaseLocIdxSet(nDbIdx, arrayLocIdx);
	}
	else
	{
		dbLocation.SortByName(arrayLocIdx, nDbIdx);
	}

	for (int n = 0; n < arrayLocIdx.GetSize(); n++)
	{
		int nLocIdx = arrayLocIdx.GetAt(n);
		if (PasswordArray.CheckLocationAccessLocIdx(nLocIdx) == TRUE)
		{
			AddAction(nLocIdx, dbLocation.GetName(nLocIdx));
		}
	}

	ForceSelectAction(nAction);
}

/**********************************************************************/

void CPropPageSystemDataList::GetOPGroupAction( CLocationSelectorEntity& LocSelEntity, int& nAction )
{
	LocSelEntity.SetSystemNode();
	nAction = 0;

	{
		int nSel = m_listLocation.GetCurSel();
		if ((nSel >= 0) && (nSel < m_arrayLocation.GetSize()))
		{
			LocSelEntity = m_arrayLocation.GetAt(nSel);
		}
	}

	{
		int nSel = m_listAction.GetCurSel();
		if ((nSel >= 0) && (nSel < m_arrayAction.GetSize()))
		{
			nAction = m_arrayAction.GetAt(nSel);
		}
	}
}

/**********************************************************************/

void CPropPageSystemDataList::SelectActionOPGroup()
{
}

/**********************************************************************/

void CPropPageSystemDataList::Button1OPGroupPage()
{
	if (EcrmanOptions.GetFeaturesOPNameFirstFlag() == TRUE)
	{
		EditOPName();
	}
	else
	{
		EditOPGroup();
	}
}

/**********************************************************************/

void CPropPageSystemDataList::Button2OPGroupPage()
{
	if (EcrmanOptions.GetFeaturesOPNameFirstFlag() == TRUE)
	{
		EditOPGroup();
	}
	else
	{
		EditOPName();
	}
}

/**********************************************************************/

void CPropPageSystemDataList::EditOPGroup()
{
    int nAction = 0;
    CLocationSelectorEntity LocSelSelect;
    GetOPGroupAction(LocSelSelect, nAction);
    int nDbIdx = LocSelSelect.GetDbIdx();

    switch (nAction)
    {
        case 999999:
        {
            CDataManagerInfo infoDM;
            if (DataManagerNonDb.OpenOPGroupSystem(DB_READWRITE, infoDM) == TRUE)
            {
                DataManagerNonDb.OpenOPNameSystem(DB_READONLY, infoDM);

                CListDataOPGroupInfoDlg dlg(NODE_SYSTEM, 0, this);

                if (dlg.DoModal() == IDOK)
                {
                    if (DataManagerNonDb.WriteOPGroupSystem(infoDM) == FALSE)
                    {
                        Prompter.WriteError(infoDM);
                    }
                }

                DataManagerNonDb.CloseOPGroupSystem(infoDM);
                DataManagerNonDb.CloseOPNameSystem(infoDM);
            }
            else
            {
                Prompter.ShareError(infoDM);
            }
        }
        break;

        case 999998:
        {
            CDataManagerInfo infoDM;
            if (DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READWRITE, infoDM) == TRUE)
            {
                DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, infoDM);

                DataManagerNonDb.OpenOPNameSystem(DB_READONLY, infoDM);
                DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, infoDM);

                CListDataOPGroupInfoDlg dlg(NODE_DATABASE, nDbIdx, this);

                if (dlg.DoModal() == IDOK)
                {
                    if (DataManagerNonDb.WriteOPGroupDatabase(infoDM) == FALSE)
                    {
                        Prompter.WriteError(infoDM);
                    }
                }

                DataManagerNonDb.CloseOPGroupSystem(infoDM);
                DataManagerNonDb.CloseOPGroupDatabase(infoDM);

                DataManagerNonDb.CloseOPNameSystem(infoDM);
                DataManagerNonDb.CloseOPNameDatabase(infoDM);
            }
            else
            {
                Prompter.ShareError(infoDM);
            }
        }
        break;

        default:
        {
            CDataManagerInfo infoDM;
            if (DataManagerNonDb.OpenOPGroupLocation(nAction, DB_READWRITE, infoDM) == TRUE)
            {
                DataManagerNonDb.OpenOPGroupSystem(DB_READONLY, infoDM);
                DataManagerNonDb.OpenOPGroupDatabase(nDbIdx, DB_READONLY, infoDM);

                DataManagerNonDb.OpenOPNameSystem(DB_READONLY, infoDM);
                DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, infoDM);
                DataManagerNonDb.OpenOPNameLocation(nAction, DB_READONLY, infoDM);

                CListDataOPGroupInfoDlg dlg(NODE_LOCATION, nAction, this);

                if (dlg.DoModal() == IDOK)
                {
                    if (DataManagerNonDb.WriteOPGroupLocation(infoDM) == FALSE)
                    {
                        Prompter.WriteError(infoDM);
                    }
                }

                DataManagerNonDb.CloseOPGroupSystem(infoDM);
                DataManagerNonDb.CloseOPGroupDatabase(infoDM);
                DataManagerNonDb.CloseOPGroupLocation(infoDM);

                DataManagerNonDb.CloseOPNameSystem(infoDM);
                DataManagerNonDb.CloseOPNameDatabase(infoDM);
                DataManagerNonDb.CloseOPNameLocation(infoDM);
            }
            else
            {
                Prompter.ShareError(infoDM);
            }
        }
        break;
    }
}

/**********************************************************************/

void CPropPageSystemDataList::EditOPName()
{
	int nAction = 0;
	CLocationSelectorEntity LocSelSelect;
	GetOPGroupAction(LocSelSelect, nAction);
	int nDbIdx = LocSelSelect.GetDbIdx();

	switch (nAction)
	{
	case 999999:
	{
		CDataManagerInfo infoDM;
		if (DataManagerNonDb.OpenOPNameSystem(DB_READWRITE, infoDM) == TRUE)
		{
			CListDataOPNameInfoDlg dlg(NODE_SYSTEM, 0, this);

			if (dlg.DoModal() == IDOK)
			{
				if (DataManagerNonDb.WriteOPNameSystem(infoDM) == FALSE)
				{
					Prompter.WriteError(infoDM);
				}
			}

			DataManagerNonDb.CloseOPNameSystem(infoDM);
		}
		else
		{
			Prompter.ShareError(infoDM);
		}
	}
	break;

	case 999998:
	{
		CDataManagerInfo infoDM;
		if (DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READWRITE, infoDM) == TRUE)
		{
			DataManagerNonDb.OpenOPNameSystem(DB_READONLY, infoDM);

			CListDataOPNameInfoDlg dlg(NODE_DATABASE, nDbIdx, this);

			if (dlg.DoModal() == IDOK)
			{
				if (DataManagerNonDb.WriteOPNameDatabase(infoDM) == FALSE)
				{
					Prompter.WriteError(infoDM);
				}
			}

			DataManagerNonDb.CloseOPNameSystem(infoDM);
			DataManagerNonDb.CloseOPNameDatabase(infoDM);
		}
		else
		{
			Prompter.ShareError(infoDM);
		}
	}
	break;

	default:
	{
		CDataManagerInfo infoDM;
		if (DataManagerNonDb.OpenOPNameLocation(nAction, DB_READWRITE, infoDM) == TRUE)
		{
			DataManagerNonDb.OpenOPNameSystem(DB_READONLY, infoDM);
			DataManagerNonDb.OpenOPNameDatabase(nDbIdx, DB_READONLY, infoDM);

			CListDataOPNameInfoDlg dlg(NODE_LOCATION, nAction, this);

			if (dlg.DoModal() == IDOK)
			{
				if (DataManagerNonDb.WriteOPNameLocation(infoDM) == FALSE)
				{
					Prompter.WriteError(infoDM);
				}
			}

			DataManagerNonDb.CloseOPNameSystem(infoDM);
			DataManagerNonDb.CloseOPNameDatabase(infoDM);
			DataManagerNonDb.CloseOPNameLocation(infoDM);
		}
		else
		{
			Prompter.ShareError(infoDM);
		}
	}
	break;
	}
}

/**********************************************************************/

