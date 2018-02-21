/************************************************************************
**
**  Copyright (C) 2018  spdevapp <spdevapp@joara.com>
**
**  This file is part of Aquarius.
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

	QString		m_LastFolderOpen;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
