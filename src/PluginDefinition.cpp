//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include "Path.h"

#include "PluginDefinition.h"
#include "menuCmdID.h"



//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE /* hModule */)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Open Path"), openFile, NULL, false);
	setCommand(1, TEXT("Extend Selection"), extendSelection, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//

const int g_textLen = 1024;

bool isPathChar(char c)
{
	const char notPathChars [] = {
		'"',
		'<',
		'>',
		'|',
		':',
		'*',
		'?',
		'\t',
		'\r',
		'\n',
		'\0',
	};

	for (int charIndex = 0; charIndex < sizeof(notPathChars) / sizeof(char); ++charIndex) {
		if (c == notPathChars[charIndex])
			return false;
	}
	return true;
}

bool isDrivePosition(char *text, int position)
{
	if (position > 0 && isPathChar(text[position - 1]))
		return false;

	if (!(
			'a' <= text[position] && text[position] <= 'z'
		||
			'A' <= text[position] && text[position] <= 'Z'
		)) {
		return false;
	}

	if (text[position + 1] != ':')
		return false;

	return true;
}

std::string getExtendedSelection(char *text, int lineStart, int selectionStart, int selectionEnd)
{
	bool absolutePath = false;

	int offsetEnd = selectionEnd - lineStart;

	// Try to extend with drive letter:
	if (!absolutePath && isDrivePosition(text, offsetEnd)) {

		offsetEnd += 2;
		absolutePath = true;
	}

	// Try to extend with drive letter:
	if (!absolutePath && offsetEnd - 1 >= 0 && isDrivePosition(text, offsetEnd - 1)) {

		offsetEnd += 2;
		absolutePath = true;
	}

	// Find the end:
	for (; offsetEnd < g_textLen; ++offsetEnd) {
		if (!isPathChar(text[offsetEnd]))
			break;
	}

	// Strip the end:
	for (; offsetEnd - 1 >= 0; --offsetEnd) {
		if (text[offsetEnd - 1] != ' ')
			break;
	}

	int offsetStart = selectionStart - lineStart;

	// Find the start:
	for (; offsetStart - 1 >= 0; --offsetStart) {
		if (!isPathChar(text[offsetStart - 1]))
			break;
	}

	// Try to extend with drive letter:
	if (!absolutePath && offsetStart >= 2 && isDrivePosition(text, offsetStart - 2)) {

		offsetStart -= 2;
		absolutePath = true;
	}

	// Strip the begining:
	// This is could be a weakness because path can start with whitespace although they should usually not.
	// This make it possible to use the plugin in situation such like:
	// keyword : my path/to the/reference.txt
	for (; offsetStart < offsetEnd; ++offsetStart) {
		if (text[offsetStart] != ' ')
			break;
	}

	text[offsetEnd] = '\0';

	char *path = text + offsetStart;
	return std::string(path);
}

std::string getSelection(char *text, int lineStart, int selectionStart, int selectionEnd)
{
	int offsetEnd = selectionEnd - lineStart;
	text[offsetEnd] = '\0';

	int offsetStart = selectionStart - lineStart - 1;
	char *path = text + offsetStart + 1;
	return std::string(path);
}

CPath getSelectedCurrentPath(HWND curScintilla)
{
	int selectionStart = static_cast<int>(::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0));
	int selectionEnd = static_cast<int>(::SendMessage(curScintilla, SCI_GETSELECTIONEND, 0, 0));

	int line = static_cast<int>(::SendMessage(curScintilla, SCI_LINEFROMPOSITION, selectionStart, 0));
	int lineStart = static_cast<int>(::SendMessage(curScintilla, SCI_POSITIONFROMLINE, line, 0));

	char text[g_textLen];
	::SendMessage(curScintilla, SCI_GETCURLINE, g_textLen, reinterpret_cast<LPARAM>(&text));

	std::string path;
	if (selectionStart != selectionEnd)
		path = getSelection(text, lineStart, selectionStart, selectionEnd);
	else
		path = getExtendedSelection(text, lineStart, selectionStart, selectionEnd);

#if _UNICODE
    TCHAR unicodePath[MAX_PATH];

	int codePage = static_cast<int>(::SendMessage(curScintilla, SCI_GETCODEPAGE, 0, 0));
    int len = MultiByteToWideChar(codePage, 0, path.c_str(), -1, NULL, 0);
    MultiByteToWideChar(codePage, 0, path.c_str(), -1, unicodePath, len);

	return CPath(unicodePath);
#else
	return CPath(path);
#endif
}

void openFile()
{
	// Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;

	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	CPath selectedPath = getSelectedCurrentPath(curScintilla);
    
	TCHAR strDocumentFolder[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTDIRECTORY, MAX_PATH, reinterpret_cast<LPARAM>(strDocumentFolder));
	CPath documentFolder(strDocumentFolder);

	CPath path = documentFolder + selectedPath;

    ::SendMessage(nppData._nppHandle, NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(path.getPath().c_str()));
}

bool isSeparator(char c, const char *separators)
{
	if (c == '\0')
		return true;

	for (int i = 0; separators[i] != '\0'; ++i) {
		if (c == separators[i])
			return true;
	}

	return false;
}

int getNextBound(char *line, int selectionEnd, const char *separators)
{
	while (!isSeparator(line[selectionEnd], separators))
		++selectionEnd;

	return selectionEnd;
}

int getPreviousBound(char *line, int selectionStart, const char *separators)
{
	while (selectionStart > 0 && !isSeparator(line[selectionStart - 1], separators))
		--selectionStart;

	return selectionStart;
}

void extendSelection()
{
	// Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;

	HWND curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;

	int selectionStart = static_cast<int>(::SendMessage(curScintilla, SCI_GETSELECTIONSTART, 0, 0));
	int selectionEnd = static_cast<int>(::SendMessage(curScintilla, SCI_GETSELECTIONEND, 0, 0));

	int line = static_cast<int>(::SendMessage(curScintilla, SCI_LINEFROMPOSITION, selectionStart, 0));
	int lineStart = static_cast<int>(::SendMessage(curScintilla, SCI_POSITIONFROMLINE, line, 0));

	char text[g_textLen];
	::SendMessage(curScintilla, SCI_GETCURLINE, g_textLen, reinterpret_cast<LPARAM>(&text));

	int offsetEnd = selectionEnd - lineStart;
	int newOffsetEnd = getNextBound(text, offsetEnd, "\")>");

	int offsetStart = selectionStart - lineStart;
	int newOffsetStart = getPreviousBound(text, offsetStart, "\"(<");

	// Set the new selection:
	::SendMessage(curScintilla, SCI_SETSELECTIONEND, selectionEnd + (newOffsetEnd - offsetEnd), 0);
	::SendMessage(curScintilla, SCI_SETSELECTIONSTART, selectionStart + (newOffsetStart - offsetStart), 0);
}

#if 0
void helloDlg()
{
    ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
}
#endif