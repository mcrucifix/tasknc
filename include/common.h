/*
 * common.h
 * for tasknc
 * by mjheagle
 */

#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>
#include <time.h>

/* ncurses settings */
enum ncurses_mode {
    NCURSES_MODE_STD,
    NCURSES_MODE_STD_BLOCKING,
    NCURSES_MODE_STRING
};

/* var type */
enum var_type {
    VAR_UNDEF,
    VAR_CHAR,
    VAR_STR,
    VAR_INT
};

/* variable permissions */
enum var_perms {
    VAR_RW,
    VAR_RC,
    VAR_RO
};

/**
 * variable container struct
 * name  - the name of the variable
 * type  - the type of variable
 * perms - the permissions of the variable
 * ptr   - a pointer to the variable
 */
struct var {
    char* name;
    enum var_type type;
    enum var_perms perms;
    void* ptr;
};

/**
 * task struct - the main structure in this program!
 * the fields thru description are data from the taskwarrior json
 * selpair - the cached color pair to be used when this task is selected
 * pair    - the cached color pair to be used when this task is not selected
 * prev    - the previous task struct
 * next    - the next task struct
 */
struct task {
    /* taskwarrior data */
    unsigned short index;
    char* uuid;
    char* tags;
    time_t start;
    time_t end;
    time_t entry;
    time_t due;
    time_t totalactivetime;
    char* project;
    char priority;
    char* description;
    char* category; 
    float urgency;
    /* color caching */
    int selpair;
    int pair;
    /* linked list pointers */
    struct task* prev;
    struct task* next;
};

/* program modes */
enum prog_mode {
    MODE_TASKLIST,
    MODE_PAGER,
    MODE_ANY
};

/* format fields */
enum fmt_field_type {
    FIELD_DATE,
    FIELD_PROJECT,
    FIELD_DESCRIPTION,
    FIELD_CATEGORY, 
    FIELD_ENTRY,
    FIELD_URGENCY,
    FIELD_DUE,
    FIELD_TOTALACTIVETIME,
    FIELD_PRIORITY,
    FIELD_UUID,
    FIELD_INDEX,
    FIELD_STRING,
    FIELD_VAR,
    FIELD_CONDITIONAL,
    FIELD_TIME, 
};

struct conditional_fmt_field; /* forward declaration */

/**
 * format field struct - for describing portions of format strings
 * type        - the field type which is to be printed
 * variable    - variable struct
 * field       - raw string data
 * conditional - a structure describing a conditional
 * length      - the length of the data contained
 * width       - the width that the field will be padded or cut to
 * right_align - whether the field should be right aligned
 * next        - the next format field struct
 */
struct fmt_field {
    enum fmt_field_type type;
    struct var* variable;
    char* field;
    struct conditional_fmt_field* conditional;
    unsigned int length;
    unsigned int width;
    bool right_align;
    struct fmt_field* next;
};

/**
 * conditional format field struct
 * condition - the condition to be evaluated
 * positive  - the string to be printed if condition was true
 * negative  - the string to be printed if condition was false
 */
struct conditional_fmt_field {
    struct fmt_field* condition;
    struct fmt_field* positive;
    struct fmt_field* negative;
};


/**
 * function maps - map between a function name, mode and pointer
 * name     - the name of the function
 * function - a pointer to the function
 * argn     - the number of arguments the function takes
 * mode     - the mode that this function should be run in
 */
struct funcmap {
    char* name;
    void (*function)();
    int argn;
    enum prog_mode mode;
};

/* log levels */
enum log_mode {
    LOG_DEFAULT,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_DEBUG_VERBOSE
};

/**
 * runtime configuration struct
 * history_max       - the max number of history items stored (used in prompt history)
 * nc_timeout        - the time ncurses will wait for a keypress in ms
 * statusbar_timeout - the time a statusbar message will display before timing out
 * loglvl            - which log messages should be printed
 * version           - the task warrior version being wrapped
 * sortmode          - the active sort mode
 * follow_task       - whether a task will be followed when it moves in the list
 * formats           - string and compiled printing formats
 * fieldlengths      - width of some task data fields
 */
struct config {
    int history_max;
    int nc_timeout;
    int statusbar_timeout;
    enum log_mode loglvl;
    char* version;
    char* sortmode;
    bool follow_task;
    struct {
        char* task;
        struct fmt_field* task_compiled;
        char* title;
        struct fmt_field* title_compiled;
        char* view;
        struct fmt_field* view_compiled;
    } formats;
    struct {
        int description;
        int date;
        int project;
    } fieldlengths;
};

/* string comparison */
#define str_starts_with(x, y)           (strncmp((x),(y),strlen(y)) == 0)
#define str_eq(x, y)                    (strcmp((x), (y))==0)
#define check_free(x)                   if (x!=NULL) free(x);

/* regex options */
#define REGEX_OPTS REG_ICASE|REG_EXTENDED|REG_NOSUB|REG_NEWLINE

/* logical xor */
#define XOR(x, y)                       ((x || y) && !(x && y))

/* min function */
#define MIN(x, y)                       (x < y ? x : y)

/* functions */
bool match_string(const char* haystack, const char* needle);
char* utc_date(const time_t timeint);
char* utc_time(const time_t timeint);
char* var_value_message(struct var* v, bool printname);

#endif

// vim: et ts=4 sw=4 sts=4
