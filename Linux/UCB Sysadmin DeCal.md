> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [github.com](https://github.com/juanfrans/GSAPP-AP/wiki/Basic-Terminal-Commands)

> Contribute to juanfrans/GSAPP-AP development by creating an account on GitHub.

### Commands for Navigation:
*   `pwd` - (path working directory) shows you in what directory you are
*   `ls` - (list files in current pwd) this one is probably the most useful one. There are many ways to customize it (with flags), the most common for me might be `ls -lha` which means, list all the files (including the hidden ones) in a column and add info about them (size, etc in human readable form).
*   `cat` - displays/outputs the contents of the file. It is useful for reading a file very quickly, but if the file is long you might want to do `head` or `tail` which only displays the first or last lines of the file.
*   `man` - (manual) do `man` and some command to get the manual for that command. Very useful to know the specific flags for certain commands.
*   `cd` - change directory. You can also do `cd ...` to move one up or `cd ../..` to move two up (etc).
*   `clear` - clears the terminal screen.
*   `cd -` - goest to last path.
*   `~` - goes to home directory.
*   `df -h` - displays free disk space.
*   `du -sh` - displays disk usage (the flag `s` is for summarized form).
*   `du -sh *` - displays the disk usage for every folder or file in that location.
*   Another option is to do `du -s * | sort -n` which will sort the results based on the size from smallest to largest.
*   arrow up or arrow down, brings up the last commands. Space bar to go to next page.
<!--ID: 1764664440241-->


### Commands to manage files and folders
*   `mkdir` - makes a directory.
*   `touch` - creates a blank file.
*   `echo` - prints line (ie. `echo "hello world!"`).
*   `>` - appends a line to a file (ie. `echo "hello world!" > text.txt`). **Careful, this command overwrites what's in the file.**
*   `>>` - appends to the end of the file (safer).
*   `history | less` - history and "pipe", less is a paginator. To get out of this just press `q`.
*   `cp` - copy. **Careful, it overwrites.**
*   `cp -i` - copy with warning. `-i` means "ask for confirmation".
*   `mv` - move **Careful, it overwrites.**
*   `rm` - remove **Careful, it destroys.** (ie. `rm -ri` to remove recursive and with warning, for example if you want to remove a directory and its contents.)
*   `-i` - ask for confirmation.
*   `.` - the dot means here, for example when you are copying something to your current location.
*   `locate` - finds a file.
*   `srm -rmzv` - secure removes files or folders with `m` medium grade (7 pass DoD compliant), `z` zero data. If you want to avoid the confirmations add the flag `f` (force).
*   You can join multiple files with `cat`. For example: `cat file1.txt file2.txt file3.txt > file4.txt`.
*   `zip -r foo.zip foo` - zips foo into the foo.zip file.
*   `unzip \*.zip` - unzips multiple files.
<!--ID: 1764664440245-->


### Text editor
*   `nano` - Nano is one of the text editors. You can open files with `nano file/to/open` and edit them there.
<!--ID: 1764664440248-->


### Downloading files
*   `wget` - get something, usually from a site online (ie. `wget http://etc.etc`). Probably need to install the right command through "homebrew".
<!--ID: 1764664440255-->


### Navigating in the Terminal
*   'ctrl+A' - moves to start of line.
*   'ctrl+E' - moves to end of line.
*   'ctrl+B' - moves back one character.
*   'ctrl+F' - moves forward one character.
*   'ctrl+U' - deletes from cursor to start of line.
*   'ctrl+K' - deletes from cursor to end of line.
*   'ctrl+W' - deletes from cursor to beginning of current word.
<!--ID: 1764664440260-->
