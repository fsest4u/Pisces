/************************************************************************
**
**  Copyright (C) 2018  spdevapp <spdevapp@joara.com>
**
**  This file is part of Aquarius.
**
**  Pisces is Epub PC Viewer.
**
*************************************************************************/

#ifdef _WIN32
#define NOMINMAX
#endif


#include <stdio.h>
#include <time.h>
#include <string>

#include <QApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtCore/QStandardPaths>
#include <QtCore/QStringList>
#include <QtCore/QStringRef>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtCore/QtGlobal>
#include <QtCore/QUrl>
#include <QtCore/QUuid>
#include <QtWidgets/QMainWindow>
#include <QTextEdit>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFile>
#include <QFileInfo>


#ifndef MAX_PATH
// Set Max length to 256 because that's the max path size on many systems.
#define MAX_PATH 256
#endif
// This is the same read buffer size used by Java and Perl.
#define BUFF_SIZE 8192

#include "Utility.h"


// Copies every file and folder in the source folder
// to the destination folder; the paths to the folders are submitted;
// the destination folder needs to be created in advance
void Utility::CopyFiles(const QString &fullfolderpath_source, const QString &fullfolderpath_destination)
{
    QDir folder_source(fullfolderpath_source);
    QDir folder_destination(fullfolderpath_destination);
    // Erase all the files in this folder
    foreach(QFileInfo file, folder_source.entryInfoList()) {
        if ((file.fileName() != ".") && (file.fileName() != "..")) {
            // If it's a file, copy it
            if (file.isFile()) {
                QString destination = fullfolderpath_destination + "/" + file.fileName();
                bool success = QFile::copy(file.absoluteFilePath(), destination);

                if (!success) {
                    std::string msg = file.absoluteFilePath().toStdString() + ": " + destination.toStdString();
                    //throw(CannotCopyFile(msg));
					// todo error
                }
            }
            // Else it's a directory, copy everything in it
            // to a new folder of the same name in the destination folder
            else {
                folder_destination.mkpath(file.fileName());
                CopyFiles(file.absoluteFilePath(), fullfolderpath_destination + "/" + file.fileName());
            }
        }
    }
}

//
//   Delete a directory along with all of its contents.
//
//   \param dirName Path of directory to remove.
//   \return true on success; false on error.
//
bool Utility::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            } else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}



// Deletes the specified file if it exists
bool Utility::SDeleteFile(const QString &fullfilepath)
{
    // Make sure the path exists, otherwise very
    // bad things could happen
    if (!QFileInfo(fullfilepath).exists()) {
        return false;
    }

    QFile file(fullfilepath);
    return file.remove();
}


// Copies File from full Inpath to full OutPath with overwrite if needed
bool Utility::ForceCopyFile(const QString &fullinpath, const QString &fulloutpath)
{
    if (!QFileInfo(fullinpath).exists()) {
        return false;
    }
    if (QFileInfo::exists(fulloutpath)) {
        Utility::SDeleteFile(fulloutpath);
    }
    return QFile::copy(fullinpath, fulloutpath);
}


bool Utility::RenameFile(const QString &oldfilepath, const QString &newfilepath)
{
    // Make sure the path exists, otherwise very
    // bad things could happen
    if (!QFileInfo(oldfilepath).exists()) {
        return false;
    }

    // Ensure that the newfilepath doesn't already exist but due to case insenstive file systems
    // check if we are actually renaming to an identical path with a different case.
    if (QFileInfo(newfilepath).exists() && QFileInfo(oldfilepath) != QFileInfo(newfilepath)) {
        return false;
    }

    // On case insensitive file systems, QFile::rename fails when the new name is the
    // same (case insensitive) to the old one. This is workaround for that issue.
    int ret = -1;
#if defined(Q_OS_WIN32)
    ret = _wrename(Utility::QStringToStdWString(oldfilepath).data(), Utility::QStringToStdWString(newfilepath).data());
#else
    ret = rename(oldfilepath.toUtf8().data(), newfilepath.toUtf8().data());
#endif

    if (ret == 0) {
        return true;
    }

    return false;
}

bool Utility::copyDir(const QString source, const QString destination, const bool override)
{
	QDir directory(source);
	bool error = false;

	//	removeDir(destination);

	if (!directory.exists()) {
		return false;
	}

	QStringList dirs = directory.entryList(QDir::AllDirs | QDir::Hidden);
	QStringList files = directory.entryList(QDir::Files | QDir::Hidden);

	QList<QString>::iterator d, f;

	{
		QDir temp(destination);
		temp.mkpath(temp.path());
	}

	for (d = dirs.begin(); d != dirs.end(); ++d) {
		if ((*d) == "." || (*d) == "..") {
			continue;
		}

		if (!QFileInfo(directory.path() + "/" + (*d)).isDir()) {
			continue;
		}

		QDir temp(destination + "/" + (*d));
		temp.mkpath(temp.path());

		if (!copyDir(directory.path() + "/" + (*d), destination + "/" + (*d), override)) {
			error = true;
		}
	}

	for (f = files.begin(); f != files.end(); ++f) {
		QFile tempFile(directory.path() + "/" + (*f));


		if (QFileInfo(directory.path() + "/" + (*f)).isDir()) {
			continue;
		}

		QFile destFile(destination + "/" + directory.relativeFilePath(tempFile.fileName()));

		if (destFile.exists() && override) {
			destFile.remove();
		}

		if (!tempFile.copy(destination + "/" + directory.relativeFilePath(tempFile.fileName()))) {
			error = true;

		}
	}

	return !error;
}

#if defined(Q_OS_WIN32)
std::wstring Utility::QStringToStdWString(const QString &str)
{
	return std::wstring((const wchar_t *)str.utf16());
}

QString Utility::stdWStringToQString(const std::wstring &str)
{
	return QString::fromUtf16((const ushort *)str.c_str());
}
#endif

//QString Utility::ReadTextFile(const QString &fullfilepath)
//{
//	QString buffer = "";
//	// TODO: throw an exception instead of
//	// returning an empty string
//	QFile file(fullfilepath);
//
//	// Check if we can open the file
//	if (!file.open(QFile::ReadOnly)) {
//		std::string msg = fullfilepath.toStdString() + ": " + file.errorString().toStdString();
//		throw(CannotOpenFile(msg));
//	}
//
//	QByteArray data = file.readAll();
//	const QTextCodec *codec = HTMLEncodingResolver::GetCodecForHTML(data);
//	file.close();
//
//	// UTF-8
//	if (HTMLEncodingResolver::IsValidUtf8(data)) {
//		if (!file.open(QFile::ReadOnly)) {
//			std::string msg = fullfilepath.toStdString() + ": " + file.errorString().toStdString();
//			throw(CannotOpenFile(msg));
//		}
//
//		QTextStream in(&file);
//		// Input should be UTF-8
//		in.setCodec("UTF-8");
//		// This will automatically switch reading from
//		// UTF-8 to UTF-16 if a BOM is detected
//		in.setAutoDetectUnicode(true);
//		buffer = ConvertLineEndings(in.readAll());
//		file.close();
//	}
//	// Finally, let Qt guess and if it doesn't know it will return the codec
//	// for the current locale.
//	else if (QTextCodec::codecForLocale() == codec) {
//		QTextCodec * codec = QTextCodec::codecForName("eucKR");
//		buffer = ConvertLineEndings(codec->toUnicode(data));
//	}
//	// UTF-16
//	else {
//		if (!file.open(QFile::ReadOnly)) {
//			std::string msg = fullfilepath.toStdString() + ": " + file.errorString().toStdString();
//			throw(CannotOpenFile(msg));
//		}
//
//		QTextStream in(&file);
//		// Input should be UTF-8
//		in.setCodec("UTF-8");
//		// This will automatically switch reading from
//		// UTF-8 to UTF-16 if a BOM is detected
//		in.setAutoDetectUnicode(true);
//		buffer = ConvertLineEndings(in.readAll());
//		file.close();
//	}
//
//	return buffer;
//
//}


// Reads the text file specified with the full file path;
// text needs to be in UTF-8 or UTF-16; if the file cannot
// be read, an error dialog is shown and an empty string returned
QString Utility::ReadUnicodeTextFile(const QString &fullfilepath)
{
	// TODO: throw an exception instead of
	// returning an empty string
	QFile file(fullfilepath);

	// Check if we can open the file
	if (!file.open(QFile::ReadOnly)) {
		std::string msg = fullfilepath.toStdString() + ": " + file.errorString().toStdString();
		//throw(CannotOpenFile(msg));
		qDebug("ReadUnicodeTextFile: msg = %s", msg);
	}

	QTextStream in(&file);
	// Input should be UTF-8
	in.setCodec("UTF-8");
	// This will automatically switch reading from
	// UTF-8 to UTF-16 if a BOM is detected
	in.setAutoDetectUnicode(true);
	return ConvertLineEndings(in.readAll());
}


// Writes the provided text variable to the specified
// file; if the file exists, it is truncated
void Utility::WriteUnicodeTextFile(const QString &text, const QString &fullfilepath, QIODevice::OpenModeFlag flag)
{
	QFile file(fullfilepath);

	if (!file.open(QIODevice::WriteOnly |
		flag |
		QIODevice::Text
	)
		) {
		std::string msg = file.fileName().toStdString() + ": " + file.errorString().toStdString();
		//throw(CannotOpenFile(msg));
		qDebug("ReadUnicodeTextFile: msg = %s", msg);
	}

	QTextStream out(&file);
	// We ALWAYS output in UTF-8
	out.setCodec("UTF-8");
	out << text;
}

// Converts Mac and Windows style line endings to Unix style
// line endings that are expected throughout the Qt framework
QString Utility::ConvertLineEndings(const QString &text)
{
	QString newtext(text);
	return newtext.replace("\x0D\x0A", "\x0A").replace("\x0D", "\x0A");
}

// Decodes XML escaped string to normal text                                                                                
// &amp; -> "&"    &apos; -> "'"  &quot; -> "\""   &lt; -> "<"  &gt; -> ">"
QString Utility::DecodeXML(const QString &text)
{
	QString newtext(text);
	newtext.replace("&apos;", "'");
	newtext.replace("&quot;", "\"");
	newtext.replace("&lt;", "<");
	newtext.replace("&gt;", ">");
	newtext.replace("&amp;", "&");
	return newtext;
}

QString Utility::EncodeXML(const QString &text)
{
	QString newtext(text);
	return newtext.toHtmlEscaped();
}
