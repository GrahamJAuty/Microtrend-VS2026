/**********************************************************************/

#include "local.h"

/**********************************************************************/

void CSysset::DisplayWatermark(CDC* pDC, CWnd* pWnd)
{
	CString strUser = GetField(0);
	CString strDealer = GetField(4);
	CString strAdd5 = GetField(5);
	CString strDemo = "Demonstration System";

	bool bDemo = ((strAdd5.Find("DEMO") != -1) || (strAdd5.Find("DEALER") != -1)) ? TRUE : FALSE;

	CRect rc;
	pWnd->GetClientRect(&rc);

	CFont font;
	CFont* pOldFont;
	int nPointSize = 480;	// max 48 point

	while (TRUE)
	{
		font.CreatePointFont(nPointSize, "Arial", pDC);
		pOldFont = pDC->SelectObject(&font);

		int nTextWidth;

		if (bDemo == TRUE)
		{
			nTextWidth = max(pDC->GetTextExtent(strDealer).cx, pDC->GetTextExtent(strDemo).cx);
		}
		else
		{
			nTextWidth = pDC->GetTextExtent(strUser).cx;
		}

		if (nTextWidth <= ((rc.Width() * 80) / 100))	// 80% of screen width
		{
			break;
		}

		pDC->SelectObject(pOldFont);
		font.DeleteObject();

		nPointSize -= (480 / 10);		// 10 steps
		if (nPointSize <= 0)
		{
			return;
		}
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(pDC->GetBkColor() - 0x080808);
	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	int nTextHeight = pDC->GetTextExtent(" ").cy;
	int x = rc.Width() / 2;
	int y = (rc.Height() - nTextHeight) / 2;

	if (bDemo == TRUE)
	{
		int nSpacing = nTextHeight / 4;
		pDC->TextOut(x, y - nTextHeight - nSpacing, strDealer);			// dealer name
		pDC->TextOut(x, y, strDemo);									// Demonstration System
		pDC->TextOut(x, y + nTextHeight + nSpacing, "Not for resale");	// "Not for resale"
	}
	else
	{
		pDC->TextOut(x, y, strUser);		// username
	}

	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

/**********************************************************************/
