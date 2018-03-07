/************************************************************************
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
const QString PISCES_INDEX_FILE_ORIG = "index.html.orig";

const QString FILE_EXTENSION = "EPUB File (*.epub)";

const QString LOCAL_IP_ADDRESS = "http://127.0.0.1";

//const QString TEMP_INDEX_HTML =
//"<!DOCTYPE html>\n"
//"<html class=\"no-js\">\n"
//"<head>\n"
//"<meta charset=\"utf-8\">\n"
//"<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge,chrome=1\">\n"
//"<title></title>\n"
//"<meta name=\"description\" content=\"\">\n"
//"<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\">\n"
//"<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n"
//"\n"
//"<link rel=\"stylesheet\" href=\"css/normalize.css\">\n"
//"<link rel=\"stylesheet\" href=\"css/main.css\">\n"
//"<link rel=\"stylesheet\" href=\"css/popup.css\">\n"
//"\n"
//"<script src=\"js/libs/jquery.min.js\"></script>\n"
//"\n"
//"<script src=\"js/libs/zip.min.js\"></script>\n"
//"\n"
//"<script>\n"
//"\"use strict\";\n"
//"\n"
//"document.onreadystatechange=function() {\n"
//"	if (document.readyState == \"complete\") {\n"
//"		EPUBJS.filePath=\"js/libs/\";\n"
//"		EPUBJS.cssPath=window.location.href.replace(window.location.hash, '').replace('index.html', '') + \"css/\";\n"
//"		// fileStorage.filePath=EPUBJS.filePath;\n"
//"\n"
//"		//window.reader=ePubReader(\"moby-dick/\");\n"
//"		window.reader=ePubReader(\"%1\");\n"
//"	}\n"
//"};\n"
//"\n"
//"</script>\n"
//"\n"
//"<!--File Storage-->\n"
//"<!-- <script src=\"js/libs/localforage.min.js\"></script> -->\n"
//"\n"
//"<!--Full Screen-->\n"
//"<script src=\"js/libs/screenfull.min.js\"></script>\n"
//"\n"
//"<!--Render-->\n"
//"<script src=\"js/epub.min.js\"></script>\n"
//"\n"
//"<!--Hooks-->\n"
//"<script src=\"js/hooks.min.js\"></script>\n"
//"\n"
//"<!--Reader-->\n"
//"<script src=\"js/reader.min.js\"></script>\n"
//"\n"
//"<!--Plugins-->\n"
//"<!-- <script src=\"js/plugins/search.js\"></script> -->\n"
//"\n"
//"<!--Highlights-->\n"
//"<!-- <script src=\"js/libs/jquery.highlight.js\"></script> -->\n"
//"<!-- <script src=\"js/hooks/extensions/highlight.js\"></script> -->\n"
//"\n"
//"</head>\n"
//"<body>\n"
//"<div id=\"sidebar\">\n"
//"<div id=\"panels\">\n"
//"<input id=\"searchBox\" placeholder=\"search\" type=\"search\">\n"
//"\n"
//"<a id=\"show-Search\" class=\"show_view icon-search\" data-view=\"Search\">Search</a>\n"
//"<a id=\"show-Toc\" class=\"show_view icon-list-1 active\" data-view=\"Toc\">TOC</a>\n"
//"<a id=\"show-Bookmarks\" class=\"show_view icon-bookmark\" data-view=\"Bookmarks\">Bookmarks</a>\n"
//"<a id=\"show-Notes\" class=\"show_view icon-edit\" data-view=\"Notes\">Notes</a>\n"
//"\n"
//"</div>\n"
//"<div id=\"tocView\" class=\"view\">\n"
//"</div>\n"
//"<div id=\"searchView\" class=\"view\">\n"
//"<ul id=\"searchResults\"></ul>\n"
//"</div>\n"
//"<div id=\"bookmarksView\" class=\"view\">\n"
//"<ul id=\"bookmarks\"></ul>\n"
//"</div>\n"
//"<div id=\"notesView\" class=\"view\">\n"
//"<div id=\"new-note\">\n"
//"<textarea id=\"note-text\"></textarea>\n"
//"<button id=\"note-anchor\">Anchor</button>\n"
//"</div>\n"
//"<ol id=\"notes\"></ol>\n"
//"</div>\n"
//"</div>\n"
//"<div id=\"main\">\n"
//"\n"
//"<div id=\"titlebar\">\n"
//"<div id=\"opener\">\n"
//"<a id=\"slider\" class=\"icon-menu\">Menu</a>\n"
//"</div>\n"
//"<div id=\"metainfo\">\n"
//"<span id=\"book-title\"></span>\n"
//"<span id=\"title-seperator\">&amp;nbsp; &amp;nbsp; –&amp;nbsp; &amp;nbsp; </span>\n"
//"<span id=\"chapter-title\"></span>\n"
//"</div>\n"
//"<div id=\"title-controls\">\n"
//"<a id=\"bookmark\" class=\"icon-bookmark-empty\">Bookmark</a>\n"
//"<a id=\"setting\" class=\"icon-cog\">Settings</a>\n"
//"<a id=\"fullscreen\" class=\"icon-resize-full\">Fullscreen</a>\n"
//"</div>\n"
//"</div>\n"
//"\n"
//"<div id=\"divider\"></div>\n"
//"<div id=\"prev\" class=\"arrow\">‹</div>\n"
//"<div id=\"viewer\"></div>\n"
//"<div id=\"next\" class=\"arrow\">›</div>\n"
//"\n"
//"<div id=\"loader\"><img src=\"img/loader.gif\"></div>\n"
//"</div>\n"
//"<div class=\"modal md-effect-1\" id=\"settings-modal\">\n"
//"<div class=\"md-content\">\n"
//"<h3>Settings</h3>\n"
//"<div>\n"
//"<p>\n"
//"<input type=\"checkbox\" id=\"sidebarReflow\" name=\"sidebarReflow\">Reflow text when sidebars are open.\n"
//"</p>\n"
//"</div>\n"
//"<div class=\"closer icon-cancel-circled\"></div>\n"
//"</div>\n"
//"</div>\n"
//"<div class=\"overlay\"></div>\n"
//"</body>\n"
//"</html>\n";
//


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
	
	return filename;
#endif
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

	filename = "../" + PISCES_DIR_CONTENTS + "/" + source + "/";

	content = QString(content).arg(filename);
	Utility::WriteUnicodeTextFile(content, fullpath);

}

QString FileManager::LoadReader(QString filename, unsigned int port)
{
	SetReader(filename);
	return QString("%1:%2/%3/%4").arg(LOCAL_IP_ADDRESS).arg(port).arg(PISCES_DIR_READER).arg(PISCES_INDEX_FILE);
}

