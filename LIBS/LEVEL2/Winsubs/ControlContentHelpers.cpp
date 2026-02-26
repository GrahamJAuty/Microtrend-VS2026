/**********************************************************************/

bool CControlContentHelpers::IsTicked(CButton& button)
{
	return (button.GetCheck() != 0);
}

/**********************************************************************/

const char* CControlContentHelpers::GetEditBoxText(CEdit& edit)
{
	edit.GetWindowText(m_strEditBoxText);
	return m_strEditBoxText;
}

/**********************************************************************/

int CControlContentHelpers::GetEditBoxInt(CEdit& edit)
{
	CString str;
	edit.GetWindowText(str);
	return atoi(str);
}

/**********************************************************************/

void CControlContentHelpers::SetEditBoxInt(CEdit& edit, int n)
{
	CString str;
	str.Format("%d", n);
	edit.SetWindowText(str);
}

/**********************************************************************/

__int64 CControlContentHelpers::GetEditBoxInt64(CEdit& edit)
{
	CString str;
	edit.GetWindowText(str);
	return _atoi64(str);
}

/**********************************************************************/

void CControlContentHelpers::SetEditBoxInt64(CEdit& edit, __int64 n)
{
	CString str;
	str.Format("%I64d", n);
	edit.SetWindowText(str);
}

/**********************************************************************/

double CControlContentHelpers::GetEditBoxDouble(CEdit& edit)
{
	CString str;
	edit.GetWindowText(str);
	return atof(str);
}

/**********************************************************************/

void CControlContentHelpers::SetEditBoxDouble(CEdit& edit, double d, int nDPs)
{
	CString str;
	str.Format("%.*f", nDPs, d);
	edit.SetWindowText(str);
}

/**********************************************************************/

const char* CControlContentHelpers::GetComboBoxText(CComboBox& combo)
{
	combo.GetWindowText(m_strComboBoxText);
	return m_strComboBoxText;
}

/**********************************************************************/
