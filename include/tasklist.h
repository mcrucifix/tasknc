/*
 * tasklist.c - tasklist window
 * for tasknc
 * by mjheagle
 */

#ifndef _TASKLIST_H
#define _TASKLIST_H

#include <time.h>
#include "common.h"

void key_tasklist_add(const char* arg);
void key_tasklist_complete(void);
void key_tasklist_delete(void);
void key_tasklist_edit(void);
void key_tasklist_filter(const char* arg);
void key_tasklist_modify(const char* arg);
void key_tasklist_annotate(const char* arg);
void key_tasklist_reload(void);
void key_tasklist_scroll(const int direction);
void key_tasklist_scroll_down(void);
void key_tasklist_scroll_end(void);
void key_tasklist_scroll_home(void);
void key_tasklist_scroll_up(void);
void key_tasklist_search(const char* arg);
void key_tasklist_search_next(void);
void key_tasklist_sort(const char* arg);
void key_tasklist_sync(void);
void key_tasklist_toggle_started(void);
void key_tasklist_undo(void);
void key_tasklist_view(void);
void key_tasklist_view_annotations(void);
void tasklist_check_curs_pos(void);
void tasklist_print_task(const int tasknum, const struct task* this, const int count);
void tasklist_print_task_list(void);
void tasklist_remove_task(struct task* this);
void tasklist_task_add(void);
void tasklist_window(void);

extern bool reload;
extern bool redraw;
extern bool done;
extern char* active_filter;
extern char* searchstring;
extern struct config cfg;
extern FILE* logfp;
extern int cols;
extern int rows;
extern int selline;
extern int taskcount;
extern short pageoffset;
extern struct task* head;
extern time_t sb_timeout;
extern WINDOW* tasklist;
extern WINDOW* header;
extern WINDOW* statusbar;

#endif

// vim: et ts=4 sw=4 sts=4
