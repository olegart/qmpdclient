/*
 * QMPDClient - An MPD client written in Qt 5.
 * Copyright (C) 2005-2008 Håvard Tautra Knutsen <havtknut@tihlde.org>
 * Copyright (C) 2009 Voker57 <voker57@gmail.com>
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

#ifndef LASTFMSUBMITTER_H
#define LASTFMSUBMITTER_H

#include "mpdsong.h"
#include "pausabletimer.h"

#include <QString>
#include <QQueue>
#include <QPair>
#include <QObject>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

class LastFmSubmitter : public QObject
{
	Q_OBJECT
public:
	LastFmSubmitter(QObject * parent = 0);
	~LastFmSubmitter();
	void setSong(const MPDSong & s);
protected:
	bool ensureHandshaked();
	void scrobbleNp(MPDSong & s);
	QByteArray getPasswordHash();
	QString m_session;
	QString m_npUrl;
	QString m_subUrl;
	int m_currentStarted;	
	QNetworkAccessManager * m_netAccess;
	QQueue<QPair<MPDSong, int> > m_songQueue, m_lastScrobbledSongs;
	MPDSong m_currentSong;
	PausableTimer * m_scrobbleTimer;
	QTimer * m_scrobbleRetryTimer;
	PausableTimer * m_npTimer;
	QTimer * m_hardFailTimer;
	bool m_npPending, m_awaitingHS, m_awaitingScrob;
	bool m_lastScrobbleFailed;
	int m_failed;
protected slots:
	void gotNetReply(QNetworkReply *);
	void scrobbleCurrent();
	void scrobbleQueued();
	void sendNowPlaying();
	void doHandshake();
	void mpdStateUpdated(bool);
signals:
	void infoMsg(QString);
private:
	void readScrobblerCache();
	void writeScrobblerCache();
	void createScrobblerCacheFileIfRequired();
	QUrl handshakeUrl();
};

#endif // LASTFMSUBMITTER_H
