/**********************************************************************/

void DrawListText(CDC* pDC, CString strText, CRect& rect, int nAlign, int nYShift )
{
	CString strDrawText = SSMakeShortString(pDC, strText, rect.Width());
	CSize sz = pDC->GetTextExtent(strDrawText);

	int nXOrigin = 0;
	switch (nAlign)
	{
	case TA_RIGHT:
		nXOrigin = rect.right - sz.cx;
		break;

	case TA_CENTER:
		nXOrigin = rect.left + (rect.Width() / 2) - (sz.cx / 2);
		break;

	case TA_LEFT:
	default:
		nXOrigin = rect.left;
		break;
	}

	int nYOrigin = rect.top + (rect.Height() / 2) - (sz.cy / 2) + nYShift;

	pDC->ExtTextOut(nXOrigin, nYOrigin, ETO_CLIPPED, rect, strDrawText, strDrawText.GetLength(), NULL);
}

/**********************************************************************/
