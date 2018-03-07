﻿/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Pisces.
**
**  Pisces is Epub PC Viewer.
**
*************************************************************************/

#include <QtDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QStandardPaths>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "FileManager.h"
#include "PiscesConstants.h"
#include "PiscesFeatures.h"

#include "misc/Utility.h"


const QString PISCES_LOCATION_TEMP = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/pisces";
const QString PISCES_DIR_READER = "reader";
const QString PISCES_DIR_CONTENTS = "contents";
const QString PISCES_INDEX_FILE = "index.html";
const QString PISCES_INDEX_FILE_ORIG = "index.orig.html";

const QString FILE_EXTENSION = "EPUB File (*.epub)";

const QString LOCAL_IP_ADDRESS = "http://127.0.0.1";



FileManager::FileManager(QWidget* parent)
	: QWidget(parent)
{
	ReadSetting();

	ConnectSignalsToSlots();
}

FileManager::~FileManager()
{
	WriteSetting();

}

void FileManager::ReadSetting()
{
}

void FileManager::WriteSetting()
{
}

void FileManager::ConnectSignalsToSlots()
{
}

QString FileManager::OpenEpubFile(QString lastFolderOpen)
{
	QString filename;

#ifdef FEATURE_OPEN_EPUB_DIRECTORY

	// select directory
	if (!QFileInfo(lastFolderOpen).exists()) {
		lastFolderOpen = QDir::homePath();
	}

	QString source = QFileDialog::getExistingDirectory(this, tr("Open Epub Directory"), lastFolderOpen, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (source.isEmpty() || !QFileInfo(source + "/mimetype").exists()) {
		QMessageBox::warning(this, tr("Open Epub Directory"), tr("Please select a directory to open a epub."));
		return QString("");
	}

	filename = QFileInfo(source).completeBaseName();
	QString destination = PISCES_LOCATION_TEMP + "/"+ PISCES_DIR_CONTENTS +"/" + filename;
	// check directory
	if (!QFileInfo(destination).exists()) {
		// copy directory
		Utility::copyDir(source, destination, false);
	}
	else {
		// copy directory
		Utility::copyDir(source, destination, true);
	}

#else // FEATURE_OPEN_EPUB_FILE

	QString default_filter = "*";
	QString source = QFileDialog::getOpenFileName(this,
		tr("Open Epub File"),
		lastFolderOpen,
		FILE_EXTENSION,
		&default_filter);

	filename = QFileInfo(source).fileName();
	if (!filename.isEmpty()) {
		QString destination = PISCES_LOCATION_TEMP + "/" + PISCES_DIR_CONTENTS + "/" + filename;
		// copy file
		//Utility::CopyFiles(source, destination);
		bool bFail = QFile::copy(source, destination);
		// exist epub file
		if (!bFail) {
			QFile::remove(destination);
			QFile::copy(source, destination);
		}
	}

#endif

	return filename;
}


void FileManager::SetReader(QString source)
{
	// read index.html
	QString oriPath = PISCES_LOCATION_TEMP + "/" + PISCES_DIR_READER + "/" + PISCES_INDEX_FILE_ORIG;
	QString fullpath = PISCES_LOCATION_TEMP + "/" + PISCES_DIR_READER + "/" + PISCES_INDEX_FILE;
	QString content = Utility::ReadUnicodeTextFile(oriPath);

	// remove index.html
	if (QFileInfo(fullpath).exists()) {
		QFile file(fullpath);
		file.remove();
	}

	// write index.html
	QString filename;
#ifdef FEATURE_OPEN_EPUB_DIRECTORY
	filename = "../" + PISCES_DIR_CONTENTS + "/" + source + "/";
#else	// FEATURE_OPEN_EPUB_FILE
	filename = "../" + PISCES_DIR_CONTENTS + "/" + source;
#endif
	content = QString(content).arg(filename);
	Utility::WriteUnicodeTextFile(content, fullpath);

}

QString FileManager::LoadReader(QString filename, unsigned int port)
{
	SetReader(filename);
	return QString("%1:%2/%3/%4").arg(LOCAL_IP_ADDRESS).arg(port).arg(PISCES_DIR_READER).arg(PISCES_INDEX_FILE);
}

