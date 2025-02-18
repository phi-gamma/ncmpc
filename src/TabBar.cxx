/* ncmpc (Ncurses MPD Client)
 * (c) 2004-2019 The Music Player Daemon Project
 * Project homepage: http://musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "TabBar.hxx"
#include "PageMeta.hxx"
#include "screen_list.hxx"
#include "Styles.hxx"
#include "Bindings.hxx"
#include "GlobalBindings.hxx"
#include "i18n.h"

static void
PaintPageTab(WINDOW *w, Command cmd, const char *label, bool selected) noexcept
{
	SelectStyle(w, selected ? Style::TITLE : Style::TITLE_BOLD);
	if (selected)
		wattron(w, A_REVERSE);

	waddch(w, ' ');

	const char *key = GetGlobalKeyBindings().GetFirstKeyName(cmd);
	if (key != nullptr)
		waddstr(w, key);

	SelectStyle(w, Style::TITLE);
	if (selected)
		wattron(w, A_REVERSE);

	waddch(w, ':');
	waddstr(w, label);
	waddch(w, ' ');

	if (selected)
		wattroff(w, A_REVERSE);
}

void
PaintTabBar(WINDOW *w, const PageMeta &current_page_meta,
	    const char *current_page_title) noexcept
{
	for (unsigned i = 0;; ++i) {
		const auto *page = GetPageMeta(i);
		if (page == nullptr)
			break;

		const char *title = nullptr;
		if (page == &current_page_meta)
			title = current_page_title;

		if (title == nullptr)
			title = gettext(page->title);

		PaintPageTab(w, page->command, title,
			     page == &current_page_meta);
	}
}
