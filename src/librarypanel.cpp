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

#include "librarypanel.h"
#include "config.h"

LibraryPanel::LibraryPanel() {
	setupUi(this);
	albumView->setArtistView(artistView);
	albumView->setSongView(songView);
	artistView->setAlbumView(albumView);
	
	libraryVSplit->restoreState(Config::instance()->libraryVSplitterSizes());
	libraryHSplit->restoreState(Config::instance()->libraryHSplitterSizes());

	connect(artistFilter, SIGNAL(textChanged(const QString &)), artistView, SLOT(filter(const QString &)));
	connect(albumFilter, SIGNAL(textChanged(const QString &)), albumView, SLOT(filter(const QString &)));
	connect(songFilter, SIGNAL(textChanged(const QString &)), songView, SLOT(setFilter(const QString &)));
}

void LibraryPanel::updateTranslation() {
	retranslateUi(this);
}

void LibraryPanel::on_libraryHSplit_splitterMoved(int, int) {
	Config::instance()->setLibraryHSplitterSizes(libraryHSplit->saveState());
}

void LibraryPanel::on_libraryVSplit_splitterMoved(int, int) {
	Config::instance()->setLibraryVSplitterSizes(libraryVSplit->saveState());
}
