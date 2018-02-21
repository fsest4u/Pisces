/************************************************************************
**
**  Copyright (C) 2018  spdevapp <spdevapp@joara.com>
**
**  This file is part of Aquarius.
**
**  Pisces is Epub PC Viewer.
**
*************************************************************************/

#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QIODevice>

class QStringList;
class QStringRef;
class QWidget;

struct ExceptionBase;

class Utility
{
    Q_DECLARE_TR_FUNCTIONS(Utility)

public:

    // Copies every file and folder in the source folder
    // to the destination folder; the paths to the folders are submitted;
    // the destination folder needs to be created in advance
    static void CopyFiles(const QString &fullfolderpath_source, const QString &fullfolderpath_destination);

    // Johns own recursive directory removal code
    static bool removeDir(const QString &dirName);

    // Deletes the specified file if it exists
    static bool SDeleteFile(const QString &fullfilepath);

    static bool ForceCopyFile(const QString &fullinpath, const QString &fulloutpath);

    static bool RenameFile(const QString &oldfilepath, const QString &newfilepath);

	static bool copyDir(const QString source, const QString destination, const bool ov);

#if defined(Q_OS_WIN32)
	static std::wstring QStringToStdWString(const QString &str);
	static QString stdWStringToQString(const std::wstring &str);
#endif

	//static QString ReadTextFile(const QString &fullfilepath);

	// Reads the text file specified with the full file path;
	// text needs to be in UTF-8 or UTF-16; if the file cannot
	// be read, an error dialog is shown and an empty string returned
	static QString ReadUnicodeTextFile(const QString &fullfilepath);

	// Writes the provided text variable to the specified
	// file; if the file exists, it is truncated
	static void WriteUnicodeTextFile(const QString &text, const QString &fullfilepath, QIODevice::OpenModeFlag flag = QIODevice::Truncate);

	// Converts Mac and Windows style line endings to Unix style
	// line endings that are expected throughout the Qt framework
	static QString ConvertLineEndings(const QString &text);

	// Decodes XML escaped string to normal text
	// &amp; -> &    &apos; -> '  &quot; -> "   &lt; -> <  &gt; -> >
	static QString DecodeXML(const QString &text);

	// Encodes (Escapes) XML string
	// & -> &amp;    ' -> &apos;    " -> &quot;   < -> &lt;     > ->  &gt;
	static QString EncodeXML(const QString &text);

};

#endif // UTILITY_H


