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
#include <QtCore/QSettings>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "httplistener.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "misc/SettingData.h"
#include "misc/RequestMapper.h"

static const QString SETTINGS_GROUP					= "mainWindow";
static const QString SETTINGS_GROUP_FILE_CONTROLLER = "docroot";
static const QString SETTINGS_GROUP_LISTENER		= "listener";


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_LastFolderOpen("")
	, m_HttpListener(NULL)
	, m_MaxAge(60000)
	, m_Encoding("UTF-8")
	, m_Path("piscesReader")
	, m_Format("testFormat")
	, m_Filename("testName")
	, m_MaxCachedFileSize(65536)
	, m_CacheSize(1000000)
	, m_CacheTime(60000)
	, m_Port(8081)
	, m_MinThread(4)
	, m_MaxThread(100)
	, m_CleanupInterval(60000)
	, m_ReadTimeout(60000)
	, m_MaxRequestSize(16000)
	, m_MaxMultiPartSize(10000000)
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

	if (m_HttpListener) {
		delete m_HttpListener;
		m_HttpListener = 0;
	}

}

void MainWindow::ReadSetting()
{

	SettingData settings;

	if (!QFileInfo(settings.fileName()).exists()) {
		WriteSetting();
	}

	/////////////////////////////////////////////////////
	// SETTINGS_GROUP
	/////////////////////////////////////////////////////
	settings.beginGroup(SETTINGS_GROUP);
	// The size of the window and it's full screen status
	QByteArray geometry = settings.value("geometry").toByteArray();

	if (!geometry.isNull()) {
		restoreGeometry(geometry);
	}

	// Last folder open
	m_LastFolderOpen = settings.value("lastfolderopen").toString();

	settings.endGroup();

	/////////////////////////////////////////////////////
	// SETTINGS_GROUP_FILE_CONTROLLER
	/////////////////////////////////////////////////////
	settings.beginGroup(SETTINGS_GROUP_FILE_CONTROLLER);
	// maxAge
	m_MaxAge = settings.value("maxAge").toInt();
	// encoding
	m_Encoding = settings.value("encoding").toString();
	// docroot
	m_Path = settings.value("path").toString();
	// format
	m_Format = settings.value("format").toString();
	// filename
	m_Filename = settings.value("filename").toString();
	// maxCacheFileSize
	m_MaxCachedFileSize = settings.value("maxCacheFileSize").toInt();
	// cache
	m_CacheSize = settings.value("cacheSize").toInt();
	// cacheTimeout
	m_CacheTime = settings.value("cacheTime").toInt();
	settings.endGroup();

	/////////////////////////////////////////////////////
	// SETTINGS_GROUP_LISTENER
	/////////////////////////////////////////////////////
	settings.beginGroup(SETTINGS_GROUP_LISTENER);
	// port
	m_Port = settings.value("port").toInt();
	// minThread
	m_MinThread = settings.value("minThread").toInt();
	// maxThread
	m_MaxThread = settings.value("maxThread").toInt();
	// cleanupInterval
	m_CleanupInterval = settings.value("cleanupInterval").toInt();
	// readTimeout
	m_ReadTimeout = settings.value("readTimeout").toInt();
	// maxRequestSize
	m_MaxRequestSize = settings.value("maxRequestSize").toInt();
	// maxMultiPartSize
	m_MaxMultiPartSize = settings.value("maxMultiPartSize").toInt();
	settings.endGroup();

}

void MainWindow::WriteSetting()
{
	SettingData settings;

	/////////////////////////////////////////////////////
	// SETTINGS_GROUP
	/////////////////////////////////////////////////////
	settings.beginGroup(SETTINGS_GROUP);
	// The size of the window and it's full screen status
	settings.setValue("geometry", saveGeometry());

	settings.setValue("lastfolderopen", m_LastFolderOpen);

	settings.endGroup();

	/////////////////////////////////////////////////////
	// SETTINGS_GROUP_FILE_CONTROLLER
	/////////////////////////////////////////////////////
	settings.beginGroup(SETTINGS_GROUP_FILE_CONTROLLER);
	// maxAge
	settings.setValue("maxAge", m_MaxAge);
	// encoding
	settings.setValue("encoding", m_Encoding);
	// docroot
	settings.setValue("path", m_Path);
	// format
	settings.setValue("format", m_Format);
	// filename
	settings.setValue("filename", m_Filename);
	// maxCacheFileSize
	settings.setValue("maxCacheFileSize", m_MaxCachedFileSize);
	// cache
	settings.setValue("cacheSize", m_CacheSize);
	// cacheTimeout
	settings.setValue("cacheTime", m_CacheTime);
	settings.endGroup();

	/////////////////////////////////////////////////////
	// SETTINGS_GROUP_LISTENER
	/////////////////////////////////////////////////////
	settings.beginGroup(SETTINGS_GROUP_LISTENER);
	// port
	settings.setValue("port", m_Port);
	// minThread
	settings.setValue("minThread", m_MinThread);
	// maxThread
	settings.setValue("maxThread", m_MaxThread);
	// cleanupInterval
	settings.setValue("cleanupInterval", m_CleanupInterval);
	// readTimeout
	settings.setValue("readTimeout", m_ReadTimeout);
	// maxRequestSize
	settings.setValue("maxRequestSize", m_MaxRequestSize);
	// maxMultiPartSize
	settings.setValue("maxMultiPartSize", m_MaxMultiPartSize);
	settings.endGroup();

}

void MainWindow::InitUI()
{
	SettingData settings;

	// Configure static file controller
	QSettings* fileSettings = new QSettings(settings.fileName(), QSettings::IniFormat, qApp);
	fileSettings->beginGroup(SETTINGS_GROUP_FILE_CONTROLLER);
	RequestMapper::staticFileController = new StaticFileController(fileSettings, qApp);

	// Configure and start the TCP listener
	QSettings* listenerSettings = new QSettings(settings.fileName(), QSettings::IniFormat, qApp);
	listenerSettings->beginGroup(SETTINGS_GROUP_LISTENER);
	new HttpListener(listenerSettings, new RequestMapper(qApp), qApp);
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
