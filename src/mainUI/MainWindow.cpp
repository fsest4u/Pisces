/************************************************************************
**
**  Copyright (C) 2018  spdevapp <spdevapp@joara.com>
**
**  This file is part of Aquarius.
**
**  Pisces is Epub PC Viewer.
**
*************************************************************************/

#include <QtCore/QtDebug>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "misc/SettingData.h"

static const QString SETTINGS_GROUP = "mainWindow";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_LastFolderOpen("")
{
    ui->setupUi(this);

	ReadSetting();
	InitUI();
	ConnectSignalsToSlots();
}

MainWindow::~MainWindow()
{

	WriteSetting();
	
	delete ui;
}

void MainWindow::ReadSetting()
{
	SettingData settings;
	settings.beginGroup(SETTINGS_GROUP);
	// The size of the window and it's full screen status
	QByteArray geometry = settings.value("geometry").toByteArray();

	if (!geometry.isNull()) {
		restoreGeometry(geometry);
	}

	// Last folder open
	m_LastFolderOpen = settings.value("lastfolderopen").toString();

	settings.endGroup();

}

void MainWindow::WriteSetting()
{
	SettingData settings;
	settings.beginGroup(SETTINGS_GROUP);
	// The size of the window and it's full screen status
	settings.setValue("geometry", saveGeometry());

	settings.setValue("lastfolderopen", m_LastFolderOpen);

	settings.endGroup();

}

void MainWindow::InitUI()
{

}

void MainWindow::ConnectSignalsToSlots()
{

}

void MainWindow::on_actionOpen_triggered()
{
	qDebug() << "on_actionOpen_triggered()";
	// select directory
	if (!QFileInfo(m_LastFolderOpen).exists()) {
		m_LastFolderOpen = QDir::homePath();
	}

	QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Epub Directory"), m_LastFolderOpen
														, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dirName.isEmpty()) {
		QMessageBox::warning(this, tr("Open Epub Directory"), tr("Please select a directory to open."));
		return;
	}
	m_LastFolderOpen = dirName;

}

void MainWindow::on_actionExit_triggered()
{
	qDebug() << "on_actionExit_triggered()";
	QMessageBox::StandardButton btn;
	btn = QMessageBox::question(this, tr(QCoreApplication::applicationName().toStdString().c_str()), tr("Are you sure you want to exit the application?"), QMessageBox::Ok | QMessageBox::Cancel);

	if (btn == QMessageBox::Ok) {
		this->close();
		return;
	}
}
