# Linenoise

A minimal, zero-config, BSD licensed, readline-like replacement initiated by antirez (antirez at gmail) and forked/improved by many others.

## Features

 * Inline editing, with arrow and (some) emacs-like control sequence support
 * Auto(magic) completion in latest DOS-like mode (default) or readline-like mode with column sorted output and optional filtering
 * History loading/saving

## Can a line editing library be 20k lines of code?

Line editing with some support for history is a really important feature for command line utilities. Instead of retyping almost the same stuff again and again it's just much better to hit the up arrow and edit on syntax errors, or in order to try a slightly different command. But apparently code dealing with terminals is some sort of Black Magic: readline is 30k lines of code, libedit 20k. Is it reasonable to link small utilities to huge libraries just to get a minimal support for line editing?

So what usually happens is either:

 * Large programs with configure scripts disabling line editing if readline is not present in the system, or not supporting it at all since readline is GPL licensed and libedit (the BSD clone) is not as known and available as readline is (Real world example of this problem: Tclsh).
 * Smaller programs not using a configure script not supporting line editing at all (A problem we had with Redis-cli for instance).
 
The result is a pollution of binaries without line editing support.

So I (antirez) spent more or less two hours doing a reality check resulting in this little library: is it *really* needed for a line editing library to be 20k lines of code? Apparently not, it is possibe to get a very small, zero configuration, trivial to embed library, that solves the problem. Smaller programs will just include this, supporting line editing out of the box. Larger programs may use this little library or just checking with configure if readline/libedit is available and resorting to linenoise if not.

## Terminals, in 2010.

Apparently almost every terminal one happens to use today has some kind of support for VT100 alike escape sequences. So I (antirez) tried to write a lib using just very basic VT100 features. The resulting library appears to work everywhere I (antirez) tried to use it.

It is still young code with (possibly) remaining bugs and/or compilation/OS issues, but it's a matter of a time before it gets right. So there will be no excuses for shipping command line tools without built-in line editing support.

The library is currently less than 2000 lines of code. In order to use it in a project, just look at the *example.c* file in the source distribution, it is trivial. Linenoise is BSD code, so it can be used both in free software and commercial software.

## Tested with...

 * Linux text only console ($TERM = linux)
 * Linux KDE terminal application ($TERM = xterm)
 * Linux xterm ($TERM = xterm)
 * Mac OS X iTerm ($TERM = xterm)
 * Mac OS X default Terminal.app ($TERM = xterm)
 * OpenBSD 4.5 through an OSX Terminal.app ($TERM = screen)
 * IBM AIX 6.1
 * FreeBSD xterm ($TERM = xterm)
 * Windows (console mode, not --yet-- working in an xterm-aware terminal front)

Please test it everywhere you can and report back!

## Let's push this forward!

Please fork it, add something interesting and send a pull request. What's especially interesting are fixes, new key bindings, completion.

