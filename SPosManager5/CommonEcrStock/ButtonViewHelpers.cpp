/**********************************************************************/
#include "..\CommonEcrStock\BackgroundLayoutInfo.h"
#include "..\CommonEcrStock\ButtonDefines.h"
#include "..\CommonEcrStock\ButtonManager.h"
#include "..\CommonEcrStock\ButtonOptions.h"
/**********************************************************************/
#include "ButtonViewHelpers.h"
/**********************************************************************/

void CButtonViewHelpers::DrawView(CDC* pDC, CRect rFrame)
{
	CDC* pDCMem = new CDC;
	CBitmap* pOldBitmap = NULL;
	CBitmap bmpCanvas;

	pDCMem->CreateCompatibleDC(pDC);

	bmpCanvas.CreateCompatibleBitmap(pDC, rFrame.Width(), rFrame.Height());

	pOldBitmap = pDCMem->SelectObject(&bmpCanvas);

	/**********/

	int nTotalWindowWidth = BackgroundLayoutInfo.GetTotalWindowWidth();
	int nUpperAreaHeight = BackgroundLayoutInfo.GetUpperAreaHeight();
	int nLowerAreaHeight = BackgroundLayoutInfo.GetLowerAreaHeight();
	bool bGotUpperImage = BackgroundLayoutInfo.GotUpperImage();
	bool bGotLowerImage = BackgroundLayoutInfo.GotLowerImage();
	int nUpperXOffset = BackgroundLayoutInfo.GetUpperImageXOffset();
	int nUpperYOffset = BackgroundLayoutInfo.GetUpperImageYOffset();
	int nLowerXOffset = BackgroundLayoutInfo.GetLowerImageXOffset();
	int nLowerYOffset = BackgroundLayoutInfo.GetLowerImageYOffset();

	//BACKGROUND FILL FOR UPPER AREA
	//if ( ( nUpperXOffset != 0 ) || ( nUpperYOffset != 0 ) || ( FALSE == bGotUpperImage ) )
	{
		CRect rect(0, 0, nTotalWindowWidth, nUpperAreaHeight);
		pDCMem->FillSolidRect(&rect, COLORREF(ButtonOptions.GetUpperAreaFillColourInt()));
	}

	//UPPER AREA IMAGE
	if (TRUE == bGotUpperImage)
	{
		int nUpperImageWidth = BackgroundLayoutInfo.GetUpperImageWidth();
		int nUpperImageHeight = BackgroundLayoutInfo.GetUpperImageHeight();

		if (ButtonOptions.GetUpperImageStretch() == FALSE)
		{
			pDCMem->BitBlt(nUpperXOffset, nUpperYOffset, nUpperImageWidth, nUpperImageHeight,
				BackgroundLayoutInfo.GetUpperBitmapDC(), 0, 0, SRCCOPY);
		}
		else
		{
			int nMarginLeft = ButtonOptions.GetUpperImageMarginLeft();
			int nMarginRight = ButtonOptions.GetUpperImageMarginRight();
			int nMarginTop = ButtonOptions.GetUpperImageMarginTop();
			int nMarginBottom = ButtonOptions.GetUpperImageMarginBottom();

			pDCMem->StretchBlt(
				nMarginLeft,
				nMarginTop,
				nTotalWindowWidth - nMarginRight - nMarginLeft,
				nUpperAreaHeight - nMarginBottom - nMarginTop,
				BackgroundLayoutInfo.GetUpperBitmapDC(), 0, 0,
				nUpperImageWidth,
				nUpperImageHeight,
				SRCCOPY);
		}
	}

	//LOWER AREA IMAGE
	if (ButtonManager.GetButtonManagerMode() == BUTMGR_MODE_EDIT_LIST)
	{
		int nListNo = ButtonManager.GetCurrentListNo();

		CString strList;
		strList.Format("Edit List %2.2d (%s)",
			nListNo,
			ButtonManager.GetListName(nListNo));

		CStringArray arrayTexts;
		arrayTexts.Add(strList);

		CRect rect(0, nUpperAreaHeight, nTotalWindowWidth, nUpperAreaHeight + nLowerAreaHeight);
		pDCMem->FillSolidRect(&rect, COLORREF(0xFFFFFF));
		DrawTextInDeviceContext(pDCMem, arrayTexts, rect, TA_CENTER, 24, COLORREF(0x0));
	}
	else
	{
		CRect rect(0, nUpperAreaHeight, nTotalWindowWidth, nUpperAreaHeight + nLowerAreaHeight);
		pDCMem->FillSolidRect(&rect, COLORREF(ButtonOptions.GetLowerAreaFillColourInt()));

		if (TRUE == bGotLowerImage)
		{
			int nLowerImageWidth = BackgroundLayoutInfo.GetLowerImageWidth();
			int nLowerImageHeight = BackgroundLayoutInfo.GetLowerImageHeight();

			if (ButtonOptions.GetLowerImageStretch() == FALSE)
			{
				pDCMem->BitBlt(nLowerXOffset, nUpperAreaHeight + nLowerYOffset, nLowerImageWidth, nLowerImageHeight,
					BackgroundLayoutInfo.GetLowerBitmapDC(), 0, 0, SRCCOPY);
			}
			else
			{
				int nMarginLeft = ButtonOptions.GetLowerImageMarginLeft();
				int nMarginRight = ButtonOptions.GetLowerImageMarginRight();
				int nMarginTop = ButtonOptions.GetLowerImageMarginTop();
				int nMarginBottom = ButtonOptions.GetLowerImageMarginBottom();

				pDCMem->StretchBlt(
					nMarginLeft,
					nUpperAreaHeight + nMarginTop,
					nTotalWindowWidth - nMarginRight - nMarginLeft,
					nLowerAreaHeight - nMarginBottom - nMarginTop,
					BackgroundLayoutInfo.GetLowerBitmapDC(), 0, 0,
					nLowerImageWidth,
					nLowerImageHeight,
					SRCCOPY);
			}
		}
	}

	ButtonManager.CalculateButtonRects();
	if (ButtonManager.GetButtonCount() != 0)
	{
		for (int nLineNo = 0; nLineNo < ButtonOptions.GetButtonBarLineCount(); nLineNo++)
		{
			for (int nButtonNo = 0; nButtonNo < ButtonOptions.GetButtonBarLineSize(); nButtonNo++)
			{
				CButtonInfo infoDisplay;
				if (ButtonManager.GetDisplayButton(nLineNo, nButtonNo, infoDisplay) == TRUE)
				{
					if (infoDisplay.GetButtonType() != BUTTON_TYPE_HIDE)
					{
						CStringArray arrayTexts;
						infoDisplay.GetDisplayTexts(arrayTexts);

						int nRounding = ButtonOptions.GetButtonRounding() * 10;

						{
							CBrush Brush;
							Brush.CreateSolidBrush(COLORREF(infoDisplay.GetDisplayBackColour()));

							CBrush* pOldBrush = pDCMem->SelectObject(&Brush);
                           
                            CRect rectDisplay = infoDisplay.GetDisplayRect();
                            pDCMem->RoundRect(rectDisplay, CPoint(nRounding, nRounding));
							pDCMem->SelectObject(pOldBrush);
							Brush.DeleteObject();
						}

						CRect rectText = infoDisplay.GetDisplayRect();

						switch (infoDisplay.GetButtonType())
						{
						case BUTTON_TYPE_ACTION:
						case BUTTON_TYPE_LIST:
						{
							ButtonManager.LoadImage(ButtonManager.GetCurrentListNo(), infoDisplay, pDC);
							ButtonManager.SetDisplayButton(nLineNo, nButtonNo, infoDisplay);

							if (ButtonManager.GotDisplayableImage(infoDisplay) == TRUE)
							{
								CRect rectButton = infoDisplay.GetDisplayRect();
								CBitmapInfo* pBitmapInfo = ButtonManager.GetBitmapInfo(infoDisplay);

								int nWidth = rectButton.Width();
								int nLeftOffset = (nWidth * ButtonOptions.GetButtonImageLeftMarginPercent()) / 100;
								int nRightOffset = (nWidth * ButtonOptions.GetButtonImageRightMarginPercent()) / 100;

								int nHeight = rectButton.Height();
								int nTopOffset = (nHeight * ButtonOptions.GetButtonImageTopMarginPercent()) / 100;
								int nBottomOffset = (nHeight * ButtonOptions.GetButtonImageBottomMarginPercent()) / 100;

								if (infoDisplay.GetImageTransparentFlag() == TRUE)
								{
									pDCMem->TransparentBlt(
										rectButton.left + nLeftOffset,
										rectButton.top + nTopOffset,
										rectButton.Width() - (nLeftOffset + nRightOffset),
										rectButton.Height() - (nTopOffset + nBottomOffset),
										pBitmapInfo->GetDCImage(), 0, 0,
										pBitmapInfo->GetWidth(),
										pBitmapInfo->GetHeight(),
										0xFFFFFF);
								}
								else
								{
									pDCMem->StretchBlt(
										rectButton.left + nLeftOffset,
										rectButton.top + nTopOffset,
										rectButton.Width() - (nLeftOffset + nRightOffset),
										rectButton.Height() - (nTopOffset + nBottomOffset),
										pBitmapInfo->GetDCImage(), 0, 0,
										pBitmapInfo->GetWidth(),
										pBitmapInfo->GetHeight(),
										SRCCOPY);
								}

								rectText.top += (rectText.Height() - nBottomOffset);
							}
						}
						break;
						}

						DrawTextInDeviceContext(pDCMem, arrayTexts, rectText, TA_CENTER, infoDisplay.GetFontSize(), COLORREF(infoDisplay.GetDisplayTextColour()));

						if (ButtonManager.IsEditingButton(infoDisplay) == TRUE)
						{
							{
								CRect rectCursor = infoDisplay.GetDisplayRect();
								rectCursor.InflateRect(10, 10);

								CPen Pen;
								Pen.CreatePen(PS_SOLID, 5, GetCursorColour(infoDisplay.GetDisplayBackColour()));
								CPen* pOldPen = pDCMem->SelectObject(&Pen);
								CBrush* pOldBrush = (CBrush*)pDCMem->SelectStockObject(NULL_BRUSH);
								pDCMem->RoundRect(rectCursor, CPoint(nRounding, nRounding));
								pDCMem->SelectObject(pOldBrush);
								pDCMem->SelectObject(pOldPen);
								Pen.DeleteObject();
							}
						}
					}
				}
			}
		}
	}

	// END DRAW   -------------------------------------------------
	pDC->BitBlt(0, 0, rFrame.Width(), rFrame.Height(), pDCMem, 0, 0, SRCCOPY);
	pDCMem->SelectObject(pOldBitmap);
	delete pDCMem;
}

/**********************************************************************/

int CButtonViewHelpers::GetCursorColour(int nKeyColour)
{
	return nKeyColour;
}

/**********************************************************************/
