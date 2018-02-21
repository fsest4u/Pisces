/************************************************************************
**
**  Copyright (C) 2018  spdevapp <spdevapp@joara.com>
**
**  This file is part of Aquarius.
**
**  Pisces is Epub PC Viewer.
**
*************************************************************************/

#include <QtCore/QStandardPaths>

#include "SettingData.h"



SettingData::SettingData()
	: QSettings(QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/pisces/pisces.ini", QSettings::IniFormat)
{
}


SettingData::~SettingData()
{
}
