[[Decal]]
Process：
- PID
- PPID
- UID
- program that the process is running
- args of the process

Process 通过fork创建[[6s081|6s081]]

[[睡眠与唤醒|Zombie Process]]

###### Process 与 Threads
- Both run code concurrently and can take advantage of parallelism
- Threads are “lightweight” processes
- All threads share virtual address space, code, static/globals, memory, resources (open files)
- Processes must communicate over some interface
<!--ID: 1764664440270-->


###### Inter-process Communication
- Exit Codes
- Pipes (STDIN, STDOUT, STDERR)
- Sockets (UNIX socket, IP socket)
- Message Bus (e.g. dbus on Linux)
- Signals(and so on)
	- SIGTERM: tell a process to exit now
	- SIGKILL: terminate process immediately
	- SIGINT: interrupt, when you press Ctrl+C
	- SIGHUP: the user closes the terminal window
	- SIGWINCH: terminal window resized
	- SIGSTOP / SIGCONT: stop/resume
<!--ID: 1764664440276-->



###### Process两种类型
- Foreground process: chrome, vim, htop. Started and stopped by the user.
- Daemons(守护进程): background processes like sshd, nginx, postfix, etc.
	- A service is a daemon managed by your init system
<!--ID: 1764664440280-->


###### 控制Process的方法
- send signals to the process
- use CLI (Command Line Interface)tools (apachectl, prosodyctl)
- have the init system do the above 
<!--ID: 1764664440286-->


###### init process的功能
- The daemon that manages all other daemons
- First process started by the kernel
- Starts enough processes to make the system useful
- Stops processes on shutdown
- Reaps orphans that are zombies 
<!--ID: 1764664440292-->


###### Traditional 与 modern
传统（shell 脚本、PID 文件、自定义动作、难控制顺序）和现代（声明式配置文件、init 存储状态、一致 CLI 动作、依赖/事件排序）
<!--ID: 1764664440295-->


| Traditional                          | Modern                                  |
| ------------------------------------ | --------------------------------------- |
| Shell scripts that can do anything   | Declarative configuration files         |
| Stores state in PID files            | Stores state in init system             |
| Actions can be customized per script | Consistent actions provided by CLI tool |
| Difficult to control ordering        | Ordering by dependencies or events      |

![[Pasted image 20251202160508.png]]

###### systemd goals
- performance
- simple ux
- Providing “building blocks” for an operating system
<!--ID: 1764664440301-->
