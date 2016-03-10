#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linenoise.h"

#ifndef whitespace
#define whitespace(c) ({ char _c = (c); ( (_c == ' ') || (_c == '\t') || (_c == '\n') || (_c == '\r') || (_c == '\f') ); })
#endif

#ifdef DEBUG
#define debug(format, ...) { fprintf (stderr, "\n\r" __FILE__ ":%d:%s():DEBUG: " format "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__); }
#else
#define debug(format, ...) {}
#endif

#ifndef NO_COMPLETION
void completion(const char *text, int start, int end, linenoiseCompletions *lc) {
  char *buf = linenoise_buffer;

  debug ("text = \"%s\", start / end  = %d/%d", text, start, end);

    if (buf[0] == 'h') {
        linenoiseAddCompletion(lc,"hello");
        linenoiseAddCompletion(lc,"hello there");
    }
    if (buf[0] == 'e') {
        linenoiseAddCompletion(lc,"example here");
        linenoiseAddCompletion(lc,"example there");
        linenoiseAddCompletion(lc,"example everywhere");
    }
    if (buf[0] == 'c') {
        linenoiseAddCompletion(lc,"cello");
    }
}

static char  *cmd_arg0[]     = { "hello", "example", "cello", NULL };
static char  *hello_arg1[]   = { "there", NULL };
static char  *example_arg1[] = { "here", "there", "everywhere", NULL };
static char  *cello_arg1[]   = { NULL };
static char **cmd_arg1[]     = { hello_arg1, example_arg1, cello_arg1, NULL };

void
completion_rl (const char *text, int start, int end, linenoiseCompletions *lc) {
  char *line = linenoise_buffer;
  char  len = strlen (text);

  debug ("ENTRY: text = \"%s\", start / end  = %d/%d", text, start, end);

  // Parse the whole line, counting command/arguments
  int i, argc = 0, wp = 1, w, cmd_start = 0, cmd_end = 0;
  for (i = 0; i <= end; i++) {

    // Is current character whitespace or not?
    w = whitespace (line[i]);

    // Catch beginning/end of command name
    if (argc == 0) {
      if ( wp && !w)                { cmd_start = i;                    }
      if (!wp && ((i == end) || w)) { cmd_end   = (i == end) ? i : i-1; }

      // Catch-up a line full of space
      if ( wp && (i == end))        { cmd_start = i; cmd_end = i; }
    }

    // Count cmd/arguments as they are clearly separated
    if (!wp && w) {
      argc++;
    }

    // Update previous whitespace indicator
    wp = w;
  }
  if (cmd_end < cmd_start) {
    debug ("CMD: error invalid cmd_start / cmd_end = %d/%d", cmd_start, cmd_end);
    return;
  }


  unsigned int cmd_len = (cmd_end - cmd_start);
  char cmd[cmd_len + 1];
  strncpy (cmd, line + cmd_start, cmd_len);
  cmd[cmd_len] = 0;
  debug ("CMD: argc = %d,  cmd = \"%s\", cmd start / end = %d/%d", argc, cmd, cmd_start, cmd_end);

  // Now attempt completion on command name...
  if (argc == 0) {
    int i;

    int match;
    for (i = 0; cmd_arg0[i]; i++) {
      match = (strncmp (text, cmd_arg0[i], len) == 0);
      debug ("text/\"%s\" %c= cmd_arg0[%d]/\"%s\"", text, (match ? '=' : '!'), i, cmd_arg0[i]);
      if (match) {
        linenoiseAddCompletion(lc, cmd_arg0[i]);
      }
    }
  }

  // ... or arguments if command identified...
  else if (argc == 1) {
    int i, j;

    int match;
    for (i = 0; cmd_arg0[i]; i++) {
      match = (strncmp (cmd, cmd_arg0[i], cmd_len) == 0);
      debug ("cmd/\"%s\" %c= cmd_arg0[%d]/\"%s\"", cmd, (match ? '=' : '!'), i, cmd_arg0[i]);
      if (match) {
	char **ca1 = cmd_arg1[i];
	for (j = 0; ca1[j]; j++) {
	  match = (strncmp (text, ca1[j], len) == 0);
	  debug ("text/\"%s\" %c= cmd_arg1[%d][%d]/\"%s\"", text, (match ? '=' : '!'), i, j, ca1[j]);
	  if (match) {
	    linenoiseAddCompletion(lc, ca1[j]);
	  }
	}
      }
    }
  }
}

#endif

int main(int argc, char *argv[]) {
    int   comprl = 0;

    /* Parse options, with --multiline we enable multi line editing. */
    char *prgname = argv[0];
    while(argc > 1) {
        argc--;
        argv++;

    /* Note: multiline support has not yet been integrated */
#define USAGE_MULTILINE
#ifdef HAVE_MULTILINE
#undef USAGE_MULTILINE
#define USAGE_MULTILINE "[--multiline]"
        if (!strcmp(*argv,"--multiline")) {
            linenoiseSetMultiLine(1);
            printf("Multi-line mode enabled.\n");
        } else
#endif
#define USAGE_KEYCODES
#ifdef HAVE_KEYCODES
#undef USAGE_KEYCODES
#define USAGE_KEYCODES " [--keycodes]"
	if (!strcmp(*argv,"--keycodes")) {
            linenoisePrintKeyCodes();
            exit(0);
        } else
#endif
#define USAGE_READLINE
#ifndef NO_COMPLETION
#undef USAGE_READLINE
#define USAGE_READLINE " [--readline]"
	if (!strcmp(*argv,"--readline")) {
	    comprl = 1;
        } else
#endif
	{
            fprintf(stderr, "Usage: %s" USAGE_MULTILINE USAGE_KEYCODES USAGE_READLINE "\n", prgname);
            exit(1);
        }
    }

#ifndef NO_COMPLETION
    /* Set the completion callback. This will be called every time the
     * user uses the <tab> key. */
    if (!comprl) {
      linenoiseSetListAll(0);
      linenoiseSetCompletionCallback(completion);
    } else {
      linenoiseSetListAll(1);
      linenoiseSetCompletionCallback(completion_rl);
    }
#endif

    /* Load history from file. The history file is just a plain text file
     * where entries are separated by newlines. */
    linenoiseHistoryLoad("history.txt"); /* Load the history at startup */

    /* Now this is the main loop of the typical linenoise-based application.
     * The call to linenoise() will block as long as the user types something
     * and presses enter.
     *
     * The typed string is returned as a malloc() allocated string by
     * linenoise, so the user needs to free() it. */
    char *line;
    while((line = linenoise("hello> ")) != NULL) {
        /* Do something with the string. */
        if (line[0] != '\0' && line[0] != '/') {
            printf("echo: '%s'\n", line);
            linenoiseHistoryAdd(line); /* Add to the history. */
            linenoiseHistorySave("history.txt"); /* Save the history on disk. */
        } else if (!strncmp(line,"/historylen",11)) {
            /* The "/historylen" command will change the history len. */
            int len = atoi(line+11);
            linenoiseHistorySetMaxLen(len);
        } else if (line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }
        free(line);
    }
    return 0;
}
