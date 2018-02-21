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
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QtWidgets/QWidget>
#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtCore/QStringList>

class QListWidgetItem;
class MainWindow;
class EpubParser;
class BookListCtrl;
class BookInfo;


class FileManager : public QWidget
{
	Q_OBJECT

public:
    FileManager(QWidget* parent = 0);
    virtual ~FileManager();

	QString			OpenEpubFile(QString lastFolderOpen);
	QString			LoadReader(QString baseName, unsigned int port);

signals:

public slots:
	

private:

	void			ReadSetting();
	void			WriteSetting();

	void			ConnectSignalsToSlots();

	void			SetReader(QString baseName);

};

#endif // FILE_MANAGER_H
