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

#include "playlistpanel.h"
#include "playlistmodel.h"
#include "mpd.h"
#include "config.h"

PlaylistPanel::PlaylistPanel() {
	setupUi(this);

	// Random and such
	connect(MPD::instance(), SIGNAL(randomUpdated(bool)), randomButton, SLOT(setChecked(bool)));
	connect(MPD::instance(), SIGNAL(repeatUpdated(bool)), repeatButton, SLOT(setChecked(bool)));
	connect(Config::instance(), SIGNAL(autoAddSongsChanged(bool)), dynamicButton, SLOT(setChecked(bool)));
	connect(randomButton, SIGNAL(toggled(bool)), MPD::instance(), SLOT(setRandom(bool)));
	connect(repeatButton, SIGNAL(toggled(bool)), MPD::instance(), SLOT(setRepeat(bool)));
	dynamicButton->setChecked(Config::instance()->autoAddSongs());
	connect(dynamicButton, SIGNAL(toggled(bool)), Config::instance(), SLOT(setAutoAddSongs(bool)));

	// Button clicks
	connect(cropButton, SIGNAL(clicked()), playlistView, SLOT(cropItems()));
	connect(removeButton, SIGNAL(clicked()), playlistView, SLOT(removeItems()));
	connect(saveAsButton, SIGNAL(clicked()), playlistView, SLOT(savePlaylist()));
	connect(clearButton, SIGNAL(clicked()), MPD::instance(), SLOT(clearPlaylist()));
	connect(shuffleButton, SIGNAL(clicked()), MPD::instance(), SLOT(shufflePlaylist()));

	// Toggle actions
	connect(playlistView, SIGNAL(toggleActions(bool)), removeButton, SLOT(setEnabled(bool)));
	connect(playlistView, SIGNAL(toggleActions(bool)), cropButton, SLOT(setEnabled(bool)));
	
	// Filter
	connect(filter, SIGNAL(textChanged(const QString &)), this, SLOT(updateFilter(const QString &)));
}

void PlaylistPanel::updateTranslation() {
	retranslateUi(this);
}

void PlaylistPanel::updateFilter(const QString &needle) {
	playlistView->setFilter(needle);
	playlistView->setCurrentIndex(playlistView->model()->index(0, 0));
}
