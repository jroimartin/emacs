/* Flags and parameters describing terminal's characteristics.
   Copyright (C) 1985-1986, 2001-2025 Free Software Foundation, Inc.

This file is part of GNU Emacs.

GNU Emacs is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef EMACS_TERMCHAR_H
#define EMACS_TERMCHAR_H

#include <stdio.h>
#include "dispextern.h"

/* Each termcap frame points to its own struct tty_output object in
   the output_data.tty field.  The tty_output structure contains the
   information that is specific to termcap frames. */

struct tty_output
{
  /* The Emacs structure for the tty device this frame is on. */
  struct tty_display_info *display_info;

  /* There is nothing else here at the moment... */
};

enum { TERMCAP_BUFFER_SIZE = 4096 };

/* Parameters that are shared between frames on the same tty device. */

struct tty_display_info
{
  struct tty_display_info *next; /* Chain of all tty devices. */

  char *name;                   /* The name of the device file or 0 if
                                   stdin/stdout. */
  char *type;                   /* The type of the tty. */

  /* Input/output */

  FILE *input;                  /* The stream to be used for terminal input.
                                   NULL if the terminal is suspended. */
  FILE *output;                 /* The stream to be used for terminal output.
                                   NULL if the terminal is suspended. */

  /* Size of output buffer.  A value of zero means use the default of
     BUFIZE.  If non-zero, also minimize writes to the tty by avoiding
     calls to flush.  */
  size_t output_buffer_size;

  FILE *termscript;             /* If nonzero, send all terminal output
                                   characters to this stream also.  */

  struct emacs_tty *old_tty;    /* The initial tty mode bits */

  bool_bf term_initted : 1;	/* True if we have been through
				   init_sys_modes. */

  int reference_count;          /* Number of frames that are on this display. */

  struct terminal *terminal;    /* Points back to the generic terminal
                                   structure.  This is sometimes handy. */

  /* Info on cursor positioning.  */
  struct cm *Wcm;

  /* Redisplay. */

  Lisp_Object top_frame;        /* The topmost frame on this tty. */

  /* The previous frame we displayed on this tty.  */
  struct frame *previous_frame;
  int previous_color_mode;

  /* Information about the range of text currently shown in
     mouse-face.  */
  Mouse_HLInfo mouse_highlight;

  /* Buffer used internally by termcap (see tgetent in the Termcap
     manual).  Only init_tty should use this.  */
  char termcap_term_buffer[TERMCAP_BUFFER_SIZE];

  /* Buffer storing terminal description strings (see tgetstr in the
     Termcap manual).  Only init_tty should use this.  */
  char termcap_strings_buffer[TERMCAP_BUFFER_SIZE];

  /* Strings, numbers and flags taken from the termcap entry.  */

  const char *TS_ins_line;	/* "al" */
  const char *TS_ins_multi_lines; /* "AL" (one parameter, # lines to insert) */
  const char *TS_bell;		/* "bl" */
  const char *TS_clr_to_bottom;	/* "cd" */
  const char *TS_clr_line;	/* "ce", clear to end of line */
  const char *TS_clr_frame;	/* "cl" */
  const char *TS_set_scroll_region; /* "cs" (2 params, first line and last line) */
  const char *TS_set_scroll_region_1; /* "cS" (4 params: total lines,
                                   lines above scroll region, lines below it,
                                   total lines again) */
  const char *TS_del_char;	/* "dc" */
  const char *TS_del_multi_chars; /* "DC" (one parameter, # chars to delete) */
  const char *TS_del_line;	/* "dl" */
  const char *TS_del_multi_lines; /* "DL" (one parameter, # lines to delete) */
  const char *TS_delete_mode;	/* "dm", enter character-delete mode */
  const char *TS_end_delete_mode; /* "ed", leave character-delete mode */
  const char *TS_end_insert_mode; /* "ei", leave character-insert mode */
  const char *TS_ins_char;	/* "ic" */
  const char *TS_ins_multi_chars; /* "IC" (one parameter, # chars to insert) */
  const char *TS_insert_mode;	/* "im", enter character-insert mode */
  const char *TS_pad_inserted_char; /* "ip".  Just padding, no commands.  */
  const char *TS_end_keypad_mode; /* "ke" */
  const char *TS_keypad_mode;	/* "ks" */
  const char *TS_pad_char;	/* "pc", char to use as padding */
  const char *TS_repeat;	/* "rp" (2 params, # times to repeat
				   and character to be repeated) */
  const char *TS_end_standout_mode; /* "se" */
  const char *TS_fwd_scroll;	/* "sf" */
  const char *TS_standout_mode;	/* "so" */
  const char *TS_rev_scroll;	/* "sr" */
  const char *TS_end_termcap_modes; /* "te" */
  const char *TS_termcap_modes;	/* "ti" */
  const char *TS_visible_bell;	/* "vb" */
  const char *TS_cursor_normal;	/* "ve" */
  const char *TS_cursor_visible; /* "vs" */
  const char *TS_cursor_invisible; /* "vi" */
  const char *TS_set_window;	/* "wi" (4 params, start and end of window,
                                   each as vpos and hpos) */

  const char *TS_enter_bold_mode; /* "md" -- turn on bold (extra bright mode).  */
  const char *TS_enter_italic_mode; /* "ZH" -- turn on italics mode.  */
  const char *TS_enter_dim_mode; /* "mh" -- turn on half-bright mode.  */
  const char *TS_enter_reverse_mode; /* "mr" -- enter reverse video mode.  */
  const char *TS_exit_underline_mode; /* "us" -- start underlining.  */
  const char *TS_enter_underline_mode; /* "ue" -- end underlining.  */
  const char *TS_enter_strike_through_mode; /* "smxx" -- turn on strike-through
					       mode.  */

  /* "as"/"ae" -- start/end alternate character set.  Not really
     supported, yet.  */
  const char *TS_enter_alt_charset_mode;
  const char *TS_exit_alt_charset_mode;

  const char *TS_exit_attribute_mode; /* "me" -- switch appearances off.  */

  /* Value of the "NC" (no_color_video) capability, or 0 if not present.  */
  int TN_no_color_video;

  int TN_max_colors;            /* "Co" -- number of colors.  */

  /* "op" -- SVr4 set default pair to its original value.  */
  const char *TS_orig_pair;

  /* "AF"/"AB" or "Sf"/"Sb"-- set ANSI or SVr4 foreground/background color.
     1 param, the color index.  */
  const char *TS_set_foreground;
  const char *TS_set_background;

  int TF_hazeltine;             /* termcap hz flag. */
  int TF_insmode_motion;        /* termcap mi flag: can move while in insert mode. */
  int TF_standout_motion;       /* termcap mi flag: can move while in standout mode. */
  int TF_underscore;            /* termcap ul flag: _ underlines if over-struck on
                                   non-blank position.  Must clear before writing _.  */
  int TF_teleray;               /* termcap xt flag: many weird consequences.
                                   For t1061. */
  const char *TF_set_underline_style; /* termcap Smulx entry: Switches the underline
					 style based on the parameter.  Param should
					 be one of: 0 (none), 1 (straight), 2 (double-line),
					 3 (wave), 4 (dots), or 5 (dashes).  */
  const char *TF_set_underline_color; /* Enabled when TF_set_underline_style is set:
                                         Sets the color of the underline.  Accepts a
                                         single parameter, the color index.  */

  int RPov;                     /* # chars to start a TS_repeat */

  bool_bf delete_in_insert_mode : 1;    /* delete mode == insert mode */

  bool_bf costs_set : 1;	/* True if costs have been calculated. */

  bool_bf insert_mode : 1;	/* True when in insert mode.  */
  bool_bf standout_mode : 1;	/* True when in standout mode.  */

  /* 1 if should obey 0200 bit in input chars as "Meta", 2 if should
     keep 0200 bit in input chars.  0 to ignore the 0200 bit.  */

  int meta_key;

  /* Size of window specified by higher levels.
   This is the number of lines, from the top of frame downwards,
   which can participate in insert-line/delete-line operations.

   Effectively it excludes the bottom frame_lines - specified_window_size
   lines from those operations.  */

  int specified_window;

  /* Flag used in tty_show/hide_cursor.  */

  bool_bf cursor_hidden : 1;

  /* True means use ^S/^Q for flow control.  */

  bool_bf flow_control : 1;

  /* True means we are displaying a TTY menu on this tty.  */
  bool_bf showing_menu : 1;

  /* True means spaces in the text must actually be output;
     can't just skip over some columns to leave them blank.  */
  bool_bf must_write_spaces : 1;

  /* True if TTY can insert and delete lines.  */
  bool_bf line_ins_del_ok : 1;

  /* True if TTY can insert and delete chars.  */
  bool_bf char_ins_del_ok : 1;

  /* True if TTY supports setting the scroll window.  */
  bool_bf scroll_region_ok : 1;

  /* True if TTY remembers lines scrolled off bottom.  */
  bool_bf memory_below_frame : 1;

  /* Cost of setting the scroll window, measured in characters.  */
  int scroll_region_cost;
};

/* A chain of structures for all tty devices currently in use. */
extern struct tty_display_info *tty_list;


#define FRAME_TTY(f)                            \
  (((f)->output_method == output_termcap	\
    || (f)->output_method == output_msdos_raw)	\
   ? (f)->terminal->display_info.tty            \
   : (emacs_abort (), (struct tty_display_info *) 0))

#define CURTTY() FRAME_TTY (SELECTED_FRAME())

struct input_event;
extern Lisp_Object tty_handle_tab_bar_click (struct frame *, int, int, bool,
					     struct input_event *);

#endif /* EMACS_TERMCHAR_H */
