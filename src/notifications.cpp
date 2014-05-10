/*
 * QMPDClient - An MPD client written in Qt 5.
 * Copyright (C) 2005-2008 Håvard Tautra Knutsen <havtknut@tihlde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "config.h"
#include "debug.h"
#include "notifications.h"
#include "passivepopup.h"
#include "richtext.h"
#include "coverartdialog.h"
#include <QApplication>
#include <QDesktopWidget>

QString Notifications::name(Type t) {
	switch (t) {
		case FREEDESKTOP:
			return "Freedesktop";
		default:
			return "QMPDClient";
	}
}

QString Notifications::makeTitle(const MPDSong &s) {
	int desktopWidth = QApplication::desktop()->width();
	QString title = elideRichText("", s.title().isEmpty() ? s.filename() : s.title()  , "", desktopWidth / 2) + "\n";
	QString artist = elideRichText("", s.artist(), "", desktopWidth / 4);
	QString album = elideRichText("", s.album(), "", desktopWidth / 4);
	
	if (!artist.isEmpty())
		title += artist + "\n";
	if (!album.isEmpty())
		title += album;
	return title;
}

void Notifications::notify(const QString &text) {
	if (Config::instance()->notifier() == FREEDESKTOP && m_dbus) {
		m_dbus = notifyDBus(text);
		if (m_dbus) // DBus notify succeeded
			return;
		DEBUG("DBus notify failed, falling back to custom notifier.");
	}

	// TODO: height and width set optionaly
	QPixmap icon;
	if (Config::instance()->showCoverArt() && m_coverArt->hasCoverArt()) {
		icon = m_coverArt->coverArt();
		if (icon.height() > 64) icon = icon.scaledToHeight(64, Qt::SmoothTransformation);
		if (icon.width() > 64) icon = icon.scaledToWidth(64, Qt::SmoothTransformation);
	} else {
		icon = QPixmap(":/icons/48x48/qmpdclient.png");
	}

	PassivePopup::Position pos = static_cast<PassivePopup::Position>(Config::instance()->notificationsPosition());
	new PassivePopup("QMPDClient", text, icon, pos, Config::instance()->notificationsTimeout());
}

void Notifications::setSong(const MPDSong &s) {
	if (m_previousSong.isNull() || m_previousSong == s || !Config::instance()->notificationsEnabled() || Config::instance()->notificationsTimeout() < 1) {
		m_previousSong = s;
		return;
	}
	if(!s.isNull())
	{
		m_coverArt->setSong(s);
		notify(makeTitle(s));
	}
	m_previousSong = s;
}
