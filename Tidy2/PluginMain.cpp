// Plugin Template
//

#include "stdafx.h"

#include "PluginMain.h"
#include "AboutDialog.h"
#include "PluginDetails.h"




/* Info for Notepad++ */
CONST TCHAR TCHAR_PLUGIN_NAME[]	= _T(PLUGIN_NAME);
const int	nbFunc			= 2;
FuncItem	funcItem[nbFunc];

/* Global data */
NppData				nppData;
HANDLE				g_hModule			= NULL;
TCHAR				iniFilePath[MAX_PATH];

/* Dialogs */
AboutDialog		aboutDlg;



void doAbout();
void loadSettings();
void saveSettings();




extern "C" __declspec(dllexport) void setInfo(NppData notepadPlusData)
{
	nppData = notepadPlusData;

	aboutDlg.init((HINSTANCE)g_hModule, nppData);
	// TODO: Initialise any further dialogs

	// Load the settings from the ini file
	loadSettings();

}

extern "C" __declspec(dllexport) CONST TCHAR * getName()
{
	return TCHAR_PLUGIN_NAME;
}


extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	funcItem[1]._pFunc = doTidy;	  
	_tcscpy_s(funcItem[1]._itemName, 64, _T("Tidy"));
	funcItem[1]._init2Check = false;
	funcItem[1]._pShKey = NULL;



	funcItem[1]._pFunc = doAbout;	  
	_tcscpy_s(funcItem[1]._itemName, 64, _T("About"));
	funcItem[1]._init2Check = false;
	funcItem[1]._pShKey = NULL;

	*nbF = nbFunc;
	return funcItem;
}



HWND getCurrentHScintilla(int which)
{
	return (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
}


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	switch(notifyCode->nmhdr.code)
	{
		case NPPN_READY:
			break;

		case NPPN_SHUTDOWN:
			saveSettings();
			break;
	}

}

extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}


#ifdef _UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
	return TRUE;
}
#endif


void doAbout()
{
	aboutDlg.doDialog();
}






void loadSettings(void)
{
		TCHAR configPath[MAX_PATH];
	/* initialize the config directory */
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configPath);

	/* Test if config path exist */
	if (::PathFileExists(configPath) == FALSE) {
		::CreateDirectory(configPath, NULL);
	}

	
	
	_tcscpy_s(iniFilePath, MAX_PATH, configPath);
	_tcscat_s(iniFilePath, MAX_PATH, PLUGIN_INI);
	
	if (PathFileExists(iniFilePath) == FALSE)
	{
		::CloseHandle(::CreateFile(iniFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));
	}
	
	// TODO: Load your settings with ReadPrivateProfileString / ReadPrivateProfileInt
	
}


void saveSettings(void)
{
	// Use WritePrivateProfileString to write your settings to your ini file. iniFilePath is already initialised
	// WritePrivateProfileString(_T("SectionName"), _T("KeyName"), _T("SettingValue"), iniFilePath);
}
