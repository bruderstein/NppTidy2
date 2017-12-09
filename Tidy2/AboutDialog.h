#pragma once

#include "NppInterface/StaticDialog.h"

class AboutDialog : public StaticDialog
{
public:
	AboutDialog(void);
	~AboutDialog(void);

	void init(HINSTANCE hInst, NppData nppData)
	{
		_nppData = nppData;
		Window::init(hInst, nppData._nppHandle);
	};

    void doDialog();
    INT_PTR CALLBACK AboutDialog::run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);

private:	
	NppData _nppData;
};
