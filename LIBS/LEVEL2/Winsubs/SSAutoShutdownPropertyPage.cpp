/**********************************************************************/

CSSAutoShutdownPropertyPage::CSSAutoShutdownPropertyPage(CSSAutoShutdownHelper& AutoShutdownHelper, UINT nIDTemplate, UINT nIDCaption)
	: CSSPropertyPage(nIDTemplate, nIDCaption), m_AutoShutdownHelper( AutoShutdownHelper )
{
}

/**********************************************************************/

CSSAutoShutdownPropertyPage::CSSAutoShutdownPropertyPage(CSSAutoShutdownHelper& AutoShutdownHelper, LPCTSTR lpszTemplateName, UINT nIDCaption)
	: CSSPropertyPage(lpszTemplateName, nIDCaption), m_AutoShutdownHelper(AutoShutdownHelper)
{
}

/**********************************************************************/

CSSAutoShutdownPropertyPage::CSSAutoShutdownPropertyPage(CSSAutoShutdownHelper& AutoShutdownHelper )
	: CSSPropertyPage(), m_AutoShutdownHelper(AutoShutdownHelper)
{
}

/**********************************************************************/

bool CSSAutoShutdownPropertyPage::UpdateData(bool bSave, bool bValidate)
{
	CWnd* pWnd = m_AutoShutdownHelper.GetActiveWindow();
	m_AutoShutdownHelper.SetActiveWindow(NULL);
	bool bResult = CSSPropertyPage::UpdateData(bSave, bValidate);
	m_AutoShutdownHelper.SetActiveWindow(pWnd);
	return bResult;
}

/**********************************************************************/
