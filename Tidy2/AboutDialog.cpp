#include "StdAfx.h"
#include "AboutDialog.h"
#include "resource.h" 

AboutDialog::AboutDialog(void)
{
}

AboutDialog::~AboutDialog(void)
{
}

void AboutDialog::doDialog()
{
    if (!isCreated())
        create(IDD_ABOUTDIALOG);

/* want to set the VERSION/DATE from the build 'version.txt' */
#if 0 /* 00000000000000000 this FAILED? WHY? */
    // HWND hwnd = this.GetSafeHwnd(); //  m_hWnd;
    HWND label = GetDlgItem((HWND)this, IDC_VERSION);
    if (label)
    {
        TCHAR buf[1024];
        swprintf(buf, sizeof(buf), _T("Using HTML Tidy date " TIDY2_DATE ", version " TIDY2_VERSION ", from https://github.com/htacg/tidy-html5"));
        SetWindowText(label, buf);
    }
#endif /* 0000000000000000000 */

	goToCenter();
}


BOOL CALLBACK AboutDialog::run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message) 
	{
        case WM_INITDIALOG :
		{
			return TRUE;
		}
		


		case WM_COMMAND : 
		{
			
				
				switch (wParam)
				{
					case IDOK :
					case IDCANCEL :
						display(FALSE);
						return TRUE;

					default :
						break;
				}
			
		}
	}
	return FALSE;
}