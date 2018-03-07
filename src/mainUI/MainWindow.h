/************************************************************************
**
**  Copyright (C) 2018  fsest4u <fsest4u@gmail.com>
**
**  This file is part of Pisces.
**
**  Pisces is Epub PC Viewer.
**
*************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSettings;
class HttpListener;
class FileManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
	void on_actionOpen_triggered();
	void on_actionExit_triggered();

private:

	void ReadSetting();
	void WriteSetting();

	void InitUI();
	void ConnectSignalsToSlots();

	void CopyReader();



	// file controller
	unsigned int	m_MaxAge;
	QString			m_Encoding;
	QString			m_Path;
	QString			m_Format;
	QString			m_Filename;
	unsigned int	m_MaxCachedFileSize;
	unsigned int	m_CacheSize;
	unsigned int	m_CacheTime;

	// http listener
	unsigned int	m_Port;
	unsigned int	m_MinThread;
	unsigned int	m_MaxThread;
	unsigned int	m_CleanupInterval;
	unsigned int	m_ReadTimeout;
	unsigned int	m_MaxRequestSize;
	unsigned int	m_MaxMultiPartSize;

	HttpListener*	m_HttpListener;

	FileManager*	m_FileManager;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
