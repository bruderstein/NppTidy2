// Plugin Template
//

#include "stdafx.h"

#include "PluginMain.h"
#include "AboutDialog.h"
#include "PluginDetails.h"
#include "../tidy-html5/include/tidy.h"



/* Info for Notepad++ */
CONST TCHAR TCHAR_PLUGIN_NAME[]	= _T(PLUGIN_NAME);
const int	nbFunc			= 10;
FuncItem	funcItem[nbFunc];

/* Global data */
NppData				nppData;
HANDLE				g_hModule			= NULL;
TCHAR				iniFilePath[MAX_PATH];

/* Dialogs */
AboutDialog		aboutDlg;



void doAbout();
void doTidy(const char *configPath);
void doTidy1();
void doTidy2();
void doTidy3();

void editConfig(const TCHAR *configPath);
void doEditConfig1();
void doEditConfig2();
void doEditConfig3();
void doShowConfigHelp();





extern "C" __declspec(dllexport) void setInfo(NppData notepadPlusData)
{
	nppData = notepadPlusData;

	aboutDlg.init((HINSTANCE)g_hModule, nppData);

}

extern "C" __declspec(dllexport) CONST TCHAR * getName()
{
	return TCHAR_PLUGIN_NAME;
}


extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
	funcItem[0]._pFunc = doTidy1;	  
	_tcscpy_s(funcItem[0]._itemName, 64, _T("Tidy (config 1)"));
	funcItem[0]._init2Check = false;
	funcItem[0]._pShKey = NULL;

	funcItem[1]._pFunc = doTidy2;	  
	_tcscpy_s(funcItem[1]._itemName, 64, _T("Tidy (config 2)"));
	funcItem[1]._init2Check = false;
	funcItem[1]._pShKey = NULL;

	funcItem[2]._pFunc = doTidy3;	  
	_tcscpy_s(funcItem[2]._itemName, 64, _T("Tidy (config 3)"));
	funcItem[2]._init2Check = false;
	funcItem[2]._pShKey = NULL;

	funcItem[3]._pFunc = NULL;	  
	_tcscpy_s(funcItem[3]._itemName, 64, _T("--"));
	funcItem[3]._init2Check = false;
	funcItem[3]._pShKey = NULL;

	funcItem[4]._pFunc = doEditConfig1;	  
	_tcscpy_s(funcItem[4]._itemName, 64, _T("Edit config 1"));
	funcItem[4]._init2Check = false;
	funcItem[4]._pShKey = NULL;

	funcItem[5]._pFunc = doEditConfig2;	  
	_tcscpy_s(funcItem[5]._itemName, 64, _T("Edit config 2"));
	funcItem[5]._init2Check = false;
	funcItem[5]._pShKey = NULL;

	funcItem[6]._pFunc = doEditConfig3;	  
	_tcscpy_s(funcItem[6]._itemName, 64, _T("Edit config 3"));
	funcItem[6]._init2Check = false;
	funcItem[6]._pShKey = NULL;

	funcItem[7]._pFunc = doShowConfigHelp;	  
	_tcscpy_s(funcItem[7]._itemName, 64, _T("Show Config Help"));
	funcItem[7]._init2Check = false;
	funcItem[7]._pShKey = NULL;

	funcItem[8]._pFunc = NULL;	  
	_tcscpy_s(funcItem[8]._itemName, 64, _T("--"));
	funcItem[8]._init2Check = false;
	funcItem[8]._pShKey = NULL;

	funcItem[9]._pFunc = doAbout;	  
	_tcscpy_s(funcItem[9]._itemName, 64, _T("About"));
	funcItem[9]._init2Check = false;
	funcItem[9]._pShKey = NULL;

	*nbF = nbFunc;
	return funcItem;
}



HWND getCurrentHScintilla()
{
	int which;
	SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, reinterpret_cast<LPARAM>(&which));
	return (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
}


extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
	switch(notifyCode->nmhdr.code)
	{
		case NPPN_READY:
			break;

		case NPPN_SHUTDOWN:
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









class TidyReadContext {
public:
	int nextPosition;
	int length;
	HWND hScintilla;
};

class TidyWriteContext {
public:
	HWND hScintilla;
};


void doAbout()
{
	aboutDlg.doDialog();
}

int TIDY_CALL getByte(void *context) {
	TidyReadContext* tidyContext = reinterpret_cast<TidyReadContext*>(context);

	int returnByte = static_cast<unsigned char>(::SendMessage(tidyContext->hScintilla, SCI_GETCHARAT, tidyContext->nextPosition, 0));
	
	++(tidyContext->nextPosition);

	return returnByte;
}

void TIDY_CALL ungetByte(void *context, byte b) {
	TidyReadContext* tidyContext = reinterpret_cast<TidyReadContext*>(context);

	--(tidyContext->nextPosition);

}

Bool TIDY_CALL isInputEof(void *context) {
	TidyReadContext* tidyContext = reinterpret_cast<TidyReadContext*>(context);
	if (tidyContext->nextPosition >= tidyContext->length)
	{
		return yes;  // Urgh. Tidy enum.
	}
	else
	{
		return no;
	}
}

void TIDY_CALL putByte(void *context, byte b) {
	TidyWriteContext* tidyContext = reinterpret_cast<TidyWriteContext*>(context);
	::SendMessage(tidyContext->hScintilla, SCI_APPENDTEXT, 1, reinterpret_cast<LPARAM>(&b));
}

void createDefaultConfig(const char *configPath)
{
	FILE *defaultConfig = fopen(configPath, "wb");
	if (NULL != defaultConfig)
	{
		fprintf(defaultConfig, "%s", 
			"indent: auto\r\n"
			"indent-spaces: 2\r\n"
			"wrap: 132\r\n"
			"markup: yes\r\n"
			"output-xml: yes\r\n"
			"input-xml: no\r\n"
			"numeric-entities: yes\r\n"
			"quote-marks: yes\r\n"
			"quote-nbsp: yes\r\n"
			"quote-ampersand: no\r\n"
			"break-before-br: no\r\n"
			"uppercase-tags: no\r\n"
			"uppercase-attributes: no\r\n"
			"new-inline-tags: cfif, cfelse, math, mroot, \r\n"
			"  mrow, mi, mn, mo, msqrt, mfrac, msubsup, munderover,\r\n"
			"  munder, mover, mmultiscripts, msup, msub, mtext,\r\n"
			"  mprescripts, mtable, mtr, mtd, mth\r\n"
			"new-blocklevel-tags: cfoutput, cfquery\r\n"
			"new-empty-tags: cfelse\r\n");
		fclose(defaultConfig);
	}
}



void getConfigPath(char *result, int bufferSize, const char *configName)
{
#ifdef UNICODE
	TCHAR wideConfigPath[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(&wideConfigPath));

	
	WideCharToMultiByte(CP_UTF8, 0, wideConfigPath, -1, result, bufferSize, NULL, NULL);
	PathCombineA(result, result, configName);

#else
	
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, reinterpret_cast<LPARAM>(&result));
	PathCombineA(result, result, configName);
#endif

	
}

void doTidy(const char *configName)
{
	HWND currentScintilla = getCurrentHScintilla();
	int originalLength = static_cast<int>(::SendMessage(currentScintilla, SCI_GETLENGTH, 0, 0));
	LRESULT currentBufferID = ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTBUFFERID, 0, 0);
	int encoding = static_cast<int>(::SendMessage(nppData._nppHandle, NPPM_GETBUFFERENCODING, currentBufferID, 0));
	TidyDoc tidyDoc = tidyCreate();

	char configPath[MAX_PATH];
	getConfigPath(configPath, MAX_PATH, configName);

	if (!tidyFileExists(tidyDoc, configPath))
	{
		createDefaultConfig(configPath);
	}

	tidyLoadConfigEnc(tidyDoc, configPath, "utf8");


	TidyReadContext context;
	context.nextPosition = 0;
	context.hScintilla = currentScintilla;
	context.length = originalLength;
	TidyInputSource inputSource;
	inputSource.getByte = getByte;
	inputSource.ungetByte = ungetByte;
	inputSource.sourceData = &context;
	inputSource.eof = isInputEof;
	char encodingString[30];
	
	switch(encoding)
	{
		case 1:  // UTF8
		case 2:  // UCS-2 BE, but stored in scintilla as UTF8
		case 3:  // UCS-2 LE, but stored in scintilla as UTF8
		case 6:  // UCS-2 BE No BOM - probably doesn't even exist
		case 7:  // UCS-2 LE No BOM - probably doesn't even exist
			strcpy(encodingString, "utf8");
			break;

		case 0:  // Ansi
		case 5:  // 7bit
			strcpy(encodingString, "raw");
			break;
		default:
			strcpy(encodingString, "raw");  // Assume it's some form of ASCII/Latin text
			break;

	}

	tidySetCharEncoding(tidyDoc, encodingString);
	tidyParseSource(tidyDoc, &inputSource);
	
	TidyOutputSink outputSink;
	TidyWriteContext writeContext;
	
	
	writeContext.hScintilla = reinterpret_cast<HWND>(::SendMessage(nppData._nppHandle, NPPM_CREATESCINTILLAHANDLE, 0, 0));
	if (NULL == writeContext.hScintilla) 
	{
		::MessageBox(nppData._nppHandle, _T("Could not create new Scintilla handle for tidied output (You're probably low on memory!)"), _T("Tidy Errors"), MB_ICONERROR | MB_OK);
	}
	else
	{
		::SendMessage(writeContext.hScintilla, SCI_ALLOCATE, originalLength, 0);

		outputSink.putByte = putByte;
		outputSink.sinkData = &writeContext;
		
		
		tidyCleanAndRepair(tidyDoc);

		tidySaveSink(tidyDoc, &outputSink);
		int errorCount = tidyErrorCount(tidyDoc);

		if (errorCount > 0)
		{
			::MessageBox(nppData._nppHandle, _T("Errors were encountered tidying the document"), _T("Tidy Errors"), MB_ICONERROR | MB_OK);
		}
		else
		{
			char *text = reinterpret_cast<char*>(::SendMessage(writeContext.hScintilla, SCI_GETCHARACTERPOINTER, 0, 0));
			int length = static_cast<int>(::SendMessage(writeContext.hScintilla, SCI_GETLENGTH, 0, 0));
			::SendMessage(currentScintilla, SCI_BEGINUNDOACTION, 0, 0);
			::SendMessage(currentScintilla, SCI_CLEARALL, 0, 0);
	
			::SendMessage(currentScintilla, SCI_APPENDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
			::SendMessage(currentScintilla, SCI_ENDUNDOACTION, 0, 0);
		}
	
		::SendMessage(nppData._nppHandle, NPPM_DESTROYSCINTILLAHANDLE, 0, reinterpret_cast<LPARAM>(&writeContext.hScintilla));
	}

	tidyRelease(tidyDoc);
}

void editConfig(const char *configName)
{
	char configPath[MAX_PATH];

	getConfigPath(configPath, MAX_PATH, configName);

	if (!tidyFileExists(NULL, configPath))
	{
		createDefaultConfig(configPath);
	}

#ifdef UNICODE
	TCHAR wideConfigPath[MAX_PATH];
	MultiByteToWideChar(CP_UTF8, 0, configPath, -1, wideConfigPath, MAX_PATH);
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(wideConfigPath));
#else
	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(configPath));
#endif

}

void doEditConfig1()
{
	editConfig("tidy1.cfg");
}

void doEditConfig2()
{
	editConfig("tidy2.cfg");
}

void doEditConfig3()
{
	editConfig("tidy3.cfg");
}

void doTidy1()
{
	doTidy("tidy1.cfg");
}


void doTidy2()
{
	doTidy("tidy2.cfg");
}

void doTidy3()
{
	doTidy("tidy3.cfg");
}

void doShowConfigHelp()
{
	TCHAR helpFile[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, reinterpret_cast<LPARAM>(helpFile));
	PathCombine(helpFile, helpFile, _T("plugins\\doc\\Tidy2\\quickref.html"));

	if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(helpFile))
	{
		::ShellExecute(nppData._nppHandle, _T("open"), helpFile, NULL, NULL, SW_SHOW);
	}
	else
	{
		::ShellExecute(nppData._nppHandle, _T("open"), _T("http://w3c.github.com/tidy-html5/quickref.html"), NULL, NULL, SW_SHOW);
	}
}

