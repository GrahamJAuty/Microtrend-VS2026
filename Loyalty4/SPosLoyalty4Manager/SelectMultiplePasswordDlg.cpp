/**********************************************************************/
#include "PasswordData.h"
/**********************************************************************/
#include "SelectMultiplePasswordDlg.h"
/**********************************************************************/

CSelectMultiplePasswordDlg::CSelectMultiplePasswordDlg(CPasswordData* pPasswords, CArray<CSelectMultipleEntityInfo, CSelectMultipleEntityInfo>& arrayEntityInfo, int nSourceIdx, CWnd* pParent)
	: CSelectMultipleEntityDlg("", arrayEntityInfo, TRUE, FALSE, pParent)
{
	m_pPasswords = pPasswords;

	CString strUserName = pPasswords->GetUserName(nSourceIdx);
	::TrimSpacesFromString(strUserName, FALSE);
	if (strUserName == "")
	{
		strUserName = "No User Name";
	}

	m_strTitle.Format("Clone Password Settings (%2.2d, %s)", nSourceIdx, (const char*) strUserName );
}

/**********************************************************************/

void CSelectMultiplePasswordDlg::SetListColumnns()
{
	m_listEntities.InsertColumn(0, "Num", LVCFMT_LEFT, 60);
	m_listEntities.InsertColumn(1, "User Name", LVCFMT_LEFT, 380);
}

/**********************************************************************/

void CSelectMultiplePasswordDlg::GetListData()
{
	m_nColumnCount = 2;

	for (int nIndex = 0; nIndex < m_arrayEntityInfo.GetSize(); nIndex++)
	{
		CSelectMultipleEntityInfo infoNode = m_arrayEntityInfo[nIndex];

		infoNode.m_strText[0].Format("%2.2d", infoNode.m_nPasswordIdx);
		infoNode.m_strText[1] = m_pPasswords->GetUserName(infoNode.m_nPasswordIdx);

		if (infoNode.m_strText[1] == "")
		{
			infoNode.m_strText[1] = "No User Name";
		}

		infoNode.m_bSelected = FALSE;
		m_arrayEntityInfo[nIndex] = infoNode;
	}
}

/**********************************************************************/

