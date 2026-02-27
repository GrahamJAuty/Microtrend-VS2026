#pragma once
/**********************************************************************/
#include "Resource.h"
/**********************************************************************/
#include "..\SmartPay4Shared\PasswordData.h"
/**********************************************************************/
#include "SelectMultipleEntityDlg.h"
/**********************************************************************/

class CSelectMultiplePasswordDlg : public CSelectMultipleEntityDlg
{
public:
	CSelectMultiplePasswordDlg( CPasswordData* pPasswords, CArray<CSelectMultipleEntityInfo,CSelectMultipleEntityInfo>& arrayEntityInfo, int nSourceIdx, CWnd* pParent = NULL);   
	
private:
	virtual void SetListColumnns();
	virtual void GetListData();

private:
	CPasswordData* m_pPasswords;
};

/**********************************************************************/

