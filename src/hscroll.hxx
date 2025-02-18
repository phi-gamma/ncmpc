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

#ifndef HSCROLL_H
#define HSCROLL_H

#include "BasicMarquee.hxx"
#include "AsioServiceFwd.hxx"

#include <curses.h>

#include <boost/asio/steady_timer.hpp>

enum class Style : unsigned;

/**
 * This class is used to auto-scroll text which does not fit on the
 * screen.  Call hscroll_init() to initialize the object,
 * hscroll_clear() to free resources, and hscroll_set() to begin
 * scrolling.
 */
class hscroll {
	WINDOW *const w;

	BasicMarquee basic;

	/**
	 * The postion on the screen.
	 */
	unsigned x, y;

	/**
	 * Style for drawing the text.
	 */
	Style style;

	attr_t attr;

	/**
	 * A timer which updates the scrolled area every second.
	 */
	boost::asio::steady_timer timer;

public:
	hscroll(boost::asio::io_service &io_service,
		WINDOW *_w, const char *_separator) noexcept
		:w(_w), basic(_separator), timer(io_service)
	{
	}

	bool IsDefined() const noexcept {
		return basic.IsDefined();
	}

	/**
	 * Sets a text to scroll.  This installs a timer which redraws
	 * every second with the current window attributes.  Call
	 * hscroll_clear() to disable it.
	 */
	void Set(unsigned x, unsigned y, unsigned width, const char *text,
		 Style style, attr_t attr=0) noexcept;

	/**
	 * Removes the text and the timer.  It may be reused with
	 * Set().
	 */
	void Clear() noexcept;

	void Rewind() noexcept {
		basic.Rewind();
	}

	void Step() noexcept {
		basic.Step();
	}

	/**
	 * Explicitly draws the scrolled text.  Calling this function
	 * is only allowed if there is a text currently.
	 */
	void Paint() const noexcept;

private:
	void TimerCallback(const boost::system::error_code &error) noexcept;

	void ScheduleTimer() noexcept {
		boost::system::error_code error;
		timer.expires_from_now(std::chrono::seconds(1), error);
		timer.async_wait(std::bind(&hscroll::TimerCallback, this,
					   std::placeholders::_1));
	}
};

#endif
