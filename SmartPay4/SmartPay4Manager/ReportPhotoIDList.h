#pragma once
/**********************************************************************/
#include "PhotoIDListDlg.h"
/**********************************************************************/

class CReportPhotoIDList
{
public:
	CReportPhotoIDList ( CWnd* pParent );
	bool Display();

private:
	CWnd* m_pParent;
	CPhotoIDListDlg m_dlgList;
	CString m_strReportLabel;
};

/**********************************************************************/
