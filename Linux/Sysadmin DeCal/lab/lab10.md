> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [decal.ocf.berkeley.edu](https://decal.ocf.berkeley.edu/archives/2021-fall/labs/a10/)

Lab 10 - Containerization and Docker  Lab 10 - 容器化和 Docker
==========================================================

Facilitator: [Ja Wattanawong](/staff#ja-wattanawong)  
引导员：Ja Wattanawong

17 min read

[](#table-of-contents)Table of contents    目录
---------------------------------------------

1.  [Intro to Docker  
    Docker 简介](#intro-to-docker)
    1.  [Installing Docker   安装 Docker](#installing-docker)
    2.  [Creating your first Docker container  
        创建你的第一个 Docker 容器](#creating-your-first-docker-container)
    3.  [Running an interactive container  
        运行一个交互式容器](#running-an-interactive-container)
    4.  [Questions (answer on Gradescope)  
        问题（在 Gradescope 上作答）](#questions-answer-on-gradescope)
2.  [Basic Management  基础管理](#basic-management)
    1.  [Dockerfiles  Dockerfile](#dockerfiles)
        1.  [Questions  问题](#questions)
    2.  [Detached Containers and Ports  
        分离容器和端口](#detached-containers-and-ports)
3.  [Dungeons and docker-compose  
    龙与 docker-compose](#dungeons-and-docker-compose)
    1.  [About docker-compose  关于 docker-compose](#about-docker-compose)
        1.  [Installing  安装](#installing)
        2.  [The web application  网络应用](#the-web-application)
    2.  [Putting it all together  
        整合所有内容](#putting-it-all-together)
    3.  [Questions  问题](#questions-1)
4.  [Submission  提交](#submission)

* * *

[](#intro-to-docker)Intro to Docker    Docker 简介
================================================

This exercise is designed to give you some hands-on experience with Docker! By the end of this assignment, you should be able to:  
本练习旨在让您获得一些使用 Docker 的实践经验！通过本作业，您应该能够：

*   Create and use a Docker container interactively  
    交互式地创建和使用 Docker 容器
*   Create a Dockerfile, which allows you to declaratively define your containers  
    创建一个 Dockerfile，它允许您声明式地定义您的容器
*   Run detached containers and understand port forwarding  
    运行分离容器并理解端口转发
*   Use `docker-compose` to run a multi-container web application  
    使用 `docker-compose` 运行多容器的 Web 应用程序

Just a forewarning: this lab holds your hand until the last section. Not that the last part is super hard, but it’ll have a lot less instruction than previous portions which are designed to gently introduce you to Docker.  
提醒一下：这个实验会一直引导你到最后一个部分。并不是说最后一个部分特别难，但它的说明会比前面的部分少很多，而前面的部分是为了温和地引导你了解 Docker 而设计的。

[](#installing-docker)Installing Docker    安装 Docker
----------------------------------------------------

Install Docker from the Ubuntu repositories by following the instructions [on the Docker website](https://docs.docker.com/v17.09/engine/installation/linux/docker-ce/ubuntu/).  
从 Ubuntu 仓库中按照 Docker 网站上的说明安装 Docker。

After installing, check on the status of the docker systemctl service by running `sudo systemctl status docker`. If the service is inactivate and/or disabled, run `systemctl enable` and `systemctl start` to start it up.  
安装完成后，通过运行 `sudo systemctl status docker` 来检查 docker 的 systemctl 服务状态。如果服务处于 inactive 和/或 disabled 状态，请运行 `systemctl enable` 和 `systemctl start` 来启动它。

I recommend running the command `sudo usermod -aG docker $USER` so you can use Docker as a non-root user. This means you won’t have to type `sudo docker` all the time. This is optional but for the rest of this exercise I’m going to assume that you did this. If you see some output like  
我建议运行命令 `sudo usermod -aG docker $USER` ，这样你就可以以非 root 用户身份使用 Docker。这意味着你不需要每次都输入 `sudo docker` 。这一步是可选的，但为了本练习的其余部分，我将假设你已经完成了这一步。如果你看到类似以下的输出：

```
sent invalidate(passwd) request, exiting
sent invalidate(group) request, exiting 
```

This is normal, it’s just adding a user to a group.  
这是正常的，只是将用户添加到一个组中。

You’ll have to log out and then log back into your SSH session for the group change to take effect.  
你必须退出并重新登录你的 SSH 会话，以便组更改生效。

[](#creating-your-first-docker-container)Creating your first Docker container    创建你的第一个 Docker 容器
--------------------------------------------------------------------------------------------------

To verify that you installed things correctly, try running  
为了验证你是否正确安装了相关工具，尝试运行

`docker run hello-world`

You should see some friendly output like so (hashes are probably different, don’t worry about it):  
你应该看到一些友好的输出，如下所示（哈希值可能不同，不用担心）：

```
Unable to find image 'hello-world:latest' locally
latest: Pulling from library/hello-world
b04784fba78d: Already exists
Digest: sha256:f3b3b28a45160805bb16542c9531888519430e9e6d6ffc09d72261b0d26ff74f
Status: Downloaded newer image for hello-world:latest

Hello from Docker!
This message shows that your installation appears to be working correctly.

To generate this message, Docker took the following steps:
 1. The Docker client contacted the Docker daemon.
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
 3. The Docker daemon created a new container from that image which runs the
    executable that produces the output you are currently reading.
 4. The Docker daemon streamed that output to the Docker client, which sent it
    to your terminal.

To try something more ambitious, you can run an Ubuntu container with:
 $ docker run -it ubuntu bash

Share images, automate workflows, and more with a free Docker ID:
 https://cloud.docker.com/

For more examples and ideas, visit:
 https://docs.docker.com/engine/userguide/ 
```

**If you’re running into an out-of-memory error, the vagrant VM from a6 is probably still running. Try to `cd` into the directory where you started the VM, and run `vagrant halt` to stop it.  
如果你遇到了内存不足的错误，那么 a6 的 vagrant 虚拟机可能还在运行。尝试进入你启动虚拟机的目录，并运行 `vagrant halt` 来停止它。**

Some [quick definitions from Docker’s website:](https://docs.docker.com/get-started/#a-brief-explanation-of-containers)  
一些来自 Docker 官网的快速定义：

An **image** is a lightweight, stand-alone, executable package that includes everything needed to run a piece of software, including the code, a runtime, libraries, environment variables, and config files. Images are useful primarily for their speed, but images can also be used as a base to be built on top of in future images, as you’ll see later with Dockerfiles. In the last example hello-world was the image used to test our docker installation.  
镜像是一个轻量级、独立、可执行的软件包，包含运行软件所需的一切内容，包括代码、运行时、库、环境变量和配置文件。镜像的主要用途在于其速度，但镜像也可以作为基础，用于后续构建其他镜像，这你将在之后的 Dockerfile 中看到。在上一个示例中，hello-world 被用作测试我们的 docker 安装的镜像。

A **container** is a runtime instance of an image—what the image becomes in memory when actually executed. It runs completely isolated from the host environment by default, only accessing host files and ports if configured to do so. A container gets created upon executing docker run on an image.  
容器是镜像的运行时实例——当镜像实际运行时，它在内存中所呈现的形式。默认情况下，容器会完全隔离于主机环境，只有在配置允许的情况下才会访问主机文件和端口。当在镜像上执行 docker run 命令时，会创建一个容器。

This is similar to the distinction between objects and classes in Object Oriented Programming. Images would be classes, and containers would be objects.  
这与面向对象编程中对象和类的区别类似。镜像相当于类，容器相当于对象。

Be sure to read through the output from running the hello-world image to get an understanding of what the Docker daemon was doing.  
确保阅读运行 hello-world 镜像后的输出，以了解 Docker 守护进程在执行时做了什么。

[](#running-an-interactive-container)Running an interactive container   运行一个交互式容器
---------------------------------------------------------------------------------

We’re now going to walk you through running a container interactively. This is useful if you ever need to play around and install stuff on a bare system without messing up your current system. Try running the following command:  
我们现在将引导你运行一个交互式容器。这在你需要在一个裸系统上进行试验和安装软件而不想破坏当前系统时非常有用。尝试运行以下命令：

`docker run -it ubuntu:xenial /bin/bash`

The `-i` flag tells docker to keep `STDIN` open to your container, and the `-t` flag allocates a [pseudo-TTY](https://en.wikipedia.org/wiki/Pseudoterminal) for you. Basically you need both for you to have a way to enter text and have this display properly. At the end of the command, `/bin/bash` is just the command you want to run once the container starts up. Try installing some packages from `apt` or just play around. It should look like a bare Linux system.  
`-i` 标志告诉 docker 保持 `STDIN` 打开以连接到你的容器， `-t` 标志为你分配一个伪 TTY。基本上你需要这两个标志才能输入文本并正确显示。在命令末尾， `/bin/bash` 只是你希望在容器启动后运行的命令。尝试从 `apt` 安装一些软件包，或者随便玩玩。它应该看起来像一个基础的 Linux 系统。

You can exit the container with `CTRL+D`.  
你可以通过 `CTRL+D` 退出容器。

Notice how even though your VM is running the Bionic version of Ubuntu, you were able to run the Xenial version of Ubuntu in a container. If you are curious about other variants of Linux, you can run a lot of them inside containers as well! This all works because Linux distributions all share the Linux kernel. For that same reason, you won’t be able to run MacOS or Windows in a container. You can try running [Fedora](https://getfedora.org/) (_*tips hat*_ M’Linux), another long-running Linux distribution:  
注意，即使你的虚拟机运行的是 Ubuntu Bionic 版本，你仍然能够在容器中运行 Ubuntu Xenial 版本。如果你对其他 Linux 变种感兴趣，你也可以在容器中运行很多！这一切之所以可行，是因为所有 Linux 发行版都共享同一个 Linux 内核。出于同样的原因，你无法在容器中运行 MacOS 或 Windows。你可以尝试运行 Fedora（*致敬* M’Linux），另一个长期支持的 Linux 发行版：

`docker run -it fedora:latest /bin/bash`

[](#questions-answer-on-gradescope)Questions (answer on Gradescope)   问题（在 Gradescope 上作答）
------------------------------------------------------------------------------------------

1.  What user are you logged in as by default?  
    你默认以什么用户身份登录？
2.  If you start and then exit an interactive container, and then use the `docker run -it ubuntu:xenial /bin/bash` command again, is it the same container? How can you tell?  
    如果你启动并退出一个交互式容器，然后再使用 `docker run -it ubuntu:xenial /bin/bash` 命令，是否是同一个容器？你如何判断？

[](#basic-management)Basic Management   基础管理
============================================

The Docker CLI (Command Line Interface) has some basic commands for you to monitor running and stopped containers, downloaded images, and other information. We’ll go over the basic commands you’ll probably use, but be sure to check out [the full reference](https://docs.docker.com/engine/reference/commandline/cli/) if you’re interested.  
Docker CLI（命令行界面）有一些基本命令，可用于监控正在运行和已停止的容器、下载的镜像以及其他信息。我们将介绍你可能会用到的一些基本命令，但如果你感兴趣，建议查看完整的参考文档。

Firstly, you might want to see the running containers on a system. Use the following command:  
首先，你可能想查看系统中正在运行的容器。使用以下命令：

`docker ps`

Since you (likely) have no containers running, you probably won’t see anything interesting. However, if you pass in the `-a` flag, you’ll also be able to see containers that have stopped (make your terminal wider or it’ll display weird):  
由于你（很可能）没有正在运行的容器，因此你可能看不到什么有趣的内容。然而，如果你传入 `-a` 标志，你也将能够看到已停止的容器（请确保终端足够宽，否则显示会异常）：

```
baisang@rapture ~/d/labs> docker ps -a
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS                            PORTS               NAMES
35c048c03588        fedora:latest       "/bin/bash"              7 minutes ago       Exited (130) About a minute ago                       mystifying_edison
dd8f7cc2e0cd        fedora:latest       "/bin/bash"              10 minutes ago      Exited (1) 8 minutes ago                              romantic_mahavira 
```

This lets you see a lot of useful information about the container. Observe that each container has a unique container id and a unique human-readable name. To get more information about a container, you can use the `docker logs` command to fetch the logs of a container (whether it’s still running or exited):  
这可以让你看到关于容器的很多有用信息。请注意，每个容器都有一个唯一的容器 ID 和一个唯一的可读名称。要获取更多关于容器的信息，你可以使用 `docker logs` 命令来获取容器的日志（无论它是否仍在运行或已退出）：

`docker logs <container_id_or_name>`

This basically just gives you `stdout` and `stderr` for process(es) running in the container.  
这基本上只是为你提供 `stdout` 和 `stderr` ，用于容器中运行的进程。

At some point, you may want to cleanup containers that have exited and you don’t plan on using anymore:  
在某个时候，你可能想要清理那些已经退出且不再使用的容器：

`docker rm <container_id_or_name>`

will remove the container.  
将删除该容器。

You may have noticed when you were running the Ubuntu or Fedora containers the first time that Docker downloaded a good chunk of data before running the image. This is the image of the container. You can view all of the images you’ve downloaded with the `docker images` command:  
你可能在第一次运行 Ubuntu 或 Fedora 容器时注意到，Docker 在运行镜像之前会下载大量数据。这就是容器的镜像。你可以使用 `docker images` 命令查看你已下载的所有镜像：

```
baisang@rapture ~/d/labs> docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
fedora              latest              9110ae7f579f        2 weeks ago         235MB
ubuntu              xenial              f975c5035748        3 weeks ago         112MB 
```

Images can take up quite a bit of space on your machine, so you may want to clean up images that you don’t plan on using. This is especially relevant if you get errors about not having enough disk space on your machine:  
镜像可能会占用你机器上相当多的空间，因此你可能希望清理那些你不打算使用的镜像。这在你遇到关于机器磁盘空间不足的错误时尤其相关：

`docker rmi <image_id>`

The image files, as well as various filesystems of containers, are stored in `/var/lib/docker`.  
镜像文件以及各种容器的文件系统都存储在 `/var/lib/docker` 中。

We’ll go over more commands later on in the lab.  
我们将在实验室的后面部分介绍更多命令。

[](#dockerfiles)Dockerfiles
---------------------------

A more powerful way to interface with Docker is by using a Dockerfile. A Dockerfile allows you to define an image by specifying all of the commands you would type manually to create an image. Docker can then build images from a specified Dockerfile. These Dockerfiles can be put into version control and the images distributed as a binary to keep track of both how the image is constructed and also to keep pre-built images around.  
一种更强大的与 Docker 交互的方式是使用 Dockerfile。Dockerfile 允许你通过指定所有手动输入的命令来定义一个镜像。Docker 随后可以根据指定的 Dockerfile 构建镜像。这些 Dockerfile 可以放入版本控制系统中，并将镜像作为二进制文件分发，以跟踪镜像的构建方式，同时保留预先构建的镜像。

Dockerfiles are very powerful and have many different commands and features. We’ll go over a basic example, but you should check out the [reference page](https://docs.docker.com/engine/reference/builder/) if you are trying to do anything more complex.  
Dockerfile 非常强大，包含许多不同的命令和功能。我们将介绍一个基本示例，但如果你要进行更复杂的操作，应该查看参考页面。

Here is an example Dockerfile that will build an image that has `python3.6` installed. It will also run `python3.6` directly, so you’ll be at a python prompt instead of a bash prompt when you run it.  
以下是一个示例 Dockerfile，它将构建一个已安装 `python3.6` 的镜像。它还将直接运行 `python3.6` ，因此当你运行它时，你会进入 Python 提示符而不是 Bash 提示符。

```
FROM ubuntu:bionic

RUN apt-get update && apt-get install -y python3.6 --no-install-recommends

CMD ["/usr/bin/python3.6", "-i"] 
```

Note: there are some “best practices” for writing Dockerfiles that the above example doesn’t use, because it’s a basic example. For instance, we probably would want to delete `/var/lib/apt/lists/*`, where `apt` stores the package list information from `apt update`, after we are done installing packages. We may also choose to use Linux variants that are smaller and lighter, e.g. Alpine Linux. The general philosophy is containers should be kept as small and “light” as possible. If you’re interested in this stuff, [check out this article](https://docs.docker.com/engine/userguide/eng-image/dockerfile_best-practices/).  
注意：上面的例子没有使用一些“最佳实践”来编写 Dockerfile，因为它是一个基础示例。例如，在安装完软件包后，我们可能希望删除 `/var/lib/apt/lists/*` ，其中 `apt` 存储了来自 `apt update` 的软件包列表信息。我们还可以选择使用更小、更轻量的 Linux 变种，例如 Alpine Linux。总体原则是容器应该尽可能的小而“轻量”。如果你对此感兴趣，可以查看这篇文章。

What is this doing? We specify a base image `ubuntu:bionic` (release 18.04 of ubuntu). We then specify that we should run (`RUN`) the command `apt-get update` and then `apt-get install python3.6` so we can install `python3.6`. Then we set the default command (`CMD`) of the container to run the `python3.6` interpreter in interactive mode.  
这是在做什么？我们指定了一个基础镜像 `ubuntu:bionic` （ubuntu 的 release 18.04）。然后我们指定应该运行 `RUN` 命令 `apt-get update` ，然后 `apt-get install python3.6` 以便安装 `python3.6` 。接着我们设置容器的默认命令 `CMD` 为在交互模式下运行 `python3.6` 解释器。

Copy the contents of the Dockerfile above into a file named `Dockerfile`. Then use Docker to build it with the following command:  
将上面的 Dockerfile 内容复制到一个名为 `Dockerfile` 的文件中。然后使用 Docker 以下命令来构建它：

`docker build -t mypython:latest .`

This tells Docker to look in the current directory for a Dockerfile to build, and build it. The `-t` flag tells it to tag this build with the name `mypython:latest`. Docker will look for a Dockerfile in the current directory since you specified `.`  
这告诉 Docker 在当前目录中查找 Dockerfile 进行构建，并构建它。 `-t` 标志告诉它将此构建标记为 `mypython:latest` 。Docker 会在当前目录中查找 Dockerfile，因为您指定了 `.`

Remember, you can see all of the images you’ve built on your machine with the `docker images` command.  
请记住，你可以使用 `docker images` 命令查看你在本机上构建的所有镜像。

### [](#questions)Questions   问题

1.  Run the image you just built. Since we specified the default `CMD`, you can just do `docker run -it mypython:latest`. **What do you observe?**  
    运行你刚刚构建的镜像。由于我们指定了默认的 `CMD` ，你可以直接执行 `docker run -it mypython:latest` 。你观察到了什么？
2.  Write and build a Dockerfile that installs the packages `fortune` and `fortunes-min` and runs the `fortune` executable (located in `/usr/games/fortune` after you install it). Note that you won’t need to use the `-it` flags when you run the container as `fortune` doesn’t need `STDIN`. **Submit your Dockerfile with this lab.** _Hint:_ if you’re having trouble writing your Dockerfile, try booting an interactive container and installing both packages. How can you translate what you did interactively to a Dockerfile?  
    编写并构建一个 Dockerfile，安装 `fortune` 和 `fortunes-min` 包，并运行 `fortune` 可执行文件（在安装后位于 `/usr/games/fortune` 目录下）。请注意，当你运行容器时，不需要使用 `-it` 标志，因为 `fortune` 不需要 `STDIN` 。提交你的 Dockerfile 以完成本次实验。提示：如果你在编写 Dockerfile 时遇到困难，可以尝试启动一个交互式容器并安装这两个包。你如何将交互式操作转换为 Dockerfile？
3.  Paste the output of your `docker images` command after questions 1 and 2.  
    在问题 1 和 2 之后粘贴你的 `docker images` 命令的输出结果。

[](#detached-containers-and-ports)Detached Containers and Ports   分离容器和端口
-------------------------------------------------------------------------

You might not always want containers to be running interactively. For instance, if you are running a web server, you’ll likely want the container to continue keep running until you explicitly want to end it. Docker supports this use case with the `-d` flag, which starts containers in [detached mode](https://docs.docker.com/engine/reference/run/#detached--d).  
你并不总是希望容器以交互方式运行。例如，如果你在运行一个 Web 服务器，你可能希望容器持续运行，直到你明确要停止它。Docker 使用 `-d` 标志支持这种用例，该标志以分离模式启动容器。

We’ll explore a bit about detached containers by running a standalone Apache container. The image has already been built for you; you can find it on [Docker Hub](https://hub.docker.com/_/httpd/) as `httpd`.  
我们将通过运行一个独立的 Apache 容器来了解一下分离式容器。该镜像已经为您构建完成；您可以在 Docker Hub 上找到它，地址为 `httpd` 。

Docker creates a separate virtual network for containers, so you will need to do forward your host port to your container’s port (this is called [port forwarding](https://en.wikipedia.org/wiki/Port_forwarding), or port mapping). The container is listening on port 80, so let’s try to forward our host machine’s port 5050 to the container’s port 80 when we run the container:  
Docker 为容器创建了一个独立的虚拟网络，因此你需要将主机端口转发到容器的端口（这被称为端口转发，或端口映射）。容器在端口 80 上监听，让我们尝试在运行容器时将主机机器的端口 5050 转发到容器的端口 80：

`docker run -d -p=5050:80 httpd`

The `-p` flag takes in a colon separated pair of `HOST_PORT:CONTAINER_PORT` (it can actually accept a ton of more options, but don’t worry about that for now). You should be able to view visit `<url_of_host_machine>:5050`, assuming you don’t have anything else running on that port (if you’re not on campus, you can just `curl <url_of_host_machine>:5050` from your VM or another machine on campus, e.g. ssh.ocf.berkeley.edu), and see the words “**It works!**”. You may need to allow the port 5050 on your firewall, simply run the command `ufw allow 5050`  
`-p` 标志接受一个冒号分隔的 `HOST_PORT:CONTAINER_PORT` 对（实际上它可以接受很多更多的选项，但现在不用担心）。你应该能够访问 `<url_of_host_machine>:5050` ，假设你没有在该端口运行其他服务（如果你不在校园内，可以直接从你的虚拟机或其他校园机器上 `curl <url_of_host_machine>:5050` ，例如 ssh.ocf.berkeley.edu），并看到“它运行了！”这几个字。你可能需要在防火墙中允许 5050 端口，只需运行命令 `ufw allow 5050`

You can actually “attach” to running containers and run more commands in them, similar to how `docker run` works. Use the `docker exec` command:  
你可以实际“连接”到正在运行的容器中并执行更多命令，类似于 `docker run` 的工作方式。使用 `docker exec` 命令：

`docker exec <container_id_or_name> <command>`

To stop this container, use `docker stop <container_id_or_name>`.  
要停止此容器，请使用 `docker stop <container_id_or_name>` 。

You can restart the container using `docker restart <container_id_or_name>`.  
你可以使用 `docker restart <container_id_or_name>` 重启容器。

[](#dungeons-and-docker-compose)Dungeons and docker-compose   龙与 docker-compose
===============================================================================

Congratulations! You’ve just been hired by some trash SF Bay Area tech bubble startup as their systems administrator. Unfortunately, both the CEO and CTO are busy handling the business side, which leaves it up to you to get their web application deployed using Docker and `docker-compose`.  
恭喜！你刚刚被一家位于旧金山湾区的垃圾科技泡沫初创公司聘为系统管理员。不幸的是，CEO 和 CTO 都在忙于处理业务方面的事情，这使得部署他们的网络应用程序的任务交给了你，使用 Docker 和 `docker-compose` 。

Don’t worry though – while you may not have health insurance or a nice salary, you do have some of the CTO’s notes and equity to help you with your task. You get off BART at 12pm and enter your cramped SOMA coworking space, sitting down at the desk you share with the CTO while cracking open a cold LaCroix. Checking your email, you find the following notes from the CTO:  
不过别担心 – 虽然你可能没有医疗保险或不错的薪水，但你有一些首席技术官的笔记和股权来帮助你完成任务。你中午 12 点在巴特地铁站下车，进入你狭小的 SOMA 联合办公空间，坐在你和首席技术官共用的办公桌前，打开一瓶冰镇的 LaCroix。查看邮箱时，你发现首席技术官留下的以下笔记：

[](#about-docker-compose)About docker-compose   关于 docker-compose
-----------------------------------------------------------------

`docker-compose` lets you define applications that require more than one container to function. For example, on a web application you may want your actual web application running inside of a single container, and your database running in a different container.  
`docker-compose` 允许你定义需要多个容器才能运行的应用程序。例如，在一个 Web 应用中，你可能希望你的实际 Web 应用运行在一个容器中，而数据库运行在另一个容器中。

Typically you define applications in terms of **services**. Again, going with the web application example, there are two distinct services: the app itself, and the database backing it. `docker-compose` lets you define different services within a [YAML](https://en.wikipedia.org/wiki/YAML) file and run the services accordingly.  
通常你以服务的方式定义应用程序。同样，以网络应用程序为例，有两个不同的服务：应用程序本身，以及为其提供支持的数据库。 `docker-compose` 可以让你在一个 YAML 文件中定义不同的服务，并根据需要运行这些服务。

One of the nice things about `docker-compose` is that it automatically sets up a network for your containers in which:  
`docker-compose` 的一个优点是它会自动为你的容器设置一个网络，其中：

*   each container for a service is on the network and reachable from other containers on the network  
    每个服务的容器都在网络上，并且可以从网络上的其他容器访问到
*   each container is discoverable on the network via its container name  
    每个容器都可以通过其容器名称在网络中被发现

This means it should be pretty simple to get our web app to connect to the database.  
这意味着我们的 Web 应用连接到数据库应该非常简单。

### [](#installing)Installing   安装

Install Docker Compose using the [instructions on the official Docker website](https://docs.docker.com/compose/install/)  
按照官方 Docker 网站上的说明安装 Docker Compose

### [](#the-web-application)The web application   网页应用

The web application is written only the most badass rockstar tech, [Node.js](https://www.youtube.com/watch?v=bzkRVzciAZg). For the database, it uses the most webscale, reliable, and persistent database available on the market today, [MongoDB](https://www.youtube.com/watch?v=b2F-DItXtZs).  
网页应用仅使用最酷炫的摇滚明星技术，Node.js。对于数据库，它使用目前市场上最具有网络扩展性、可靠且持久的数据库，MongoDB。

![everything he just said was wrong](https://i.fluffy.cc/bKHw92JKd8fKRgkK73vC881PzkXj4q9V.gif)

The web application can be found on GitHub. Note that the web app listens on port 8080, so you’ll need to forward or expose that port. Don’t forget to allow it on your firewall, `ufw allow 8080`. Instructions for setting it up are located in the `README.md` of the repository: [https://github.com/0xcf/decal-sp18-a10](https://github.com/0xcf/decal-sp18-a10)  
网页应用可以在 GitHub 上找到。请注意，网页应用监听的是 8080 端口，因此你需要将该端口进行端口转发或暴露。不要忘记在防火墙中允许该端口， `ufw allow 8080` 。设置说明位于仓库的 `README.md` ：https://github.com/0xcf/decal-sp18-a10

For MongoDB, you can just use the image on [DockerHub](https://hub.docker.com/_/mongo/) (a website where people can upload built Docker images). It’s just called `mongo`. For example, if you wanted to run MongoDB within a container in detached mode:  
对于 MongoDB，你可以直接使用 DockerHub 上的镜像（一个人们可以上传构建好的 Docker 镜像的网站）。它仅仅被称为 `mongo` 。例如，如果你想在容器中以分离模式运行 MongoDB：

`docker run -d mongo:latest`

[](#putting-it-all-together)Putting it all together   综合起来
----------------------------------------------------------

Your task is to use `docker-compose` to deploy the Node.js app with the MongoDB database. The CTO has roughly mapped out a suggested order of tasks:  
你的任务是使用 `docker-compose` 部署 Node.js 应用与 MongoDB 数据库。CTO 大致列出了一个建议的任务顺序：

1.  Write a `Dockerfile` that will allow you to run the Node.js web application  
    编写一个 `Dockerfile` ，以便运行 Node.js 网络应用程序
2.  Write a `docker-compose.yml` file that will glue the Node.js app container with a MongoDB container  
    编写一个 `docker-compose.yml` 文件，将 Node.js 应用容器与 MongoDB 容器连接起来

Here is a basic skeleton for `docker-compose.yml`. You will need to fill out the `web` and `database` service entries:  
以下是 `docker-compose.yml` 的一个基本骨架。你需要填写 `web` 和 `database` 的服务条目：

```
version: '3'
services:
  web:
  database: 
```

By default, the Node.js web application is designed to look for a MongoDB instance at hostname `database`, so be sure that your MongoDB service is defined under that name. `docker-compose` will make sure that the hostname `database` maps to the container for that service.  
默认情况下，Node.js web 应用程序被设计为在主机名 `database` 上寻找 MongoDB 实例，因此请确保你的 MongoDB 服务在该名称下定义。 `docker-compose` 将确保主机名 `database` 映射到该服务的容器。

One huge caveat: your hotshot CTO unfortunately wrote the Node.js app in a crap way where if it’s not able to connect to the MongoDB database once it starts, it’ll fail and exit without retrying. Since Javascript is poison, you’ll need to find a way to make sure that the Node.js app only starts after the MongoDB container is ready to accept incoming connections _without_ modifying `server.js`. I included a wrapper script `wait-for` in the repo for the Node.js app that will allow you to wait for the MongoDB service to be ready before launching the Node.js app. But, in order to use the script, you will need to have `netcat` installed in your container, so be sure to include that in your Dockerfile. See the [repo for the script](https://github.com/Eficode/wait-for) for instructions on how to use the script. Feel free to come up with other ways to solve this issue though!  
一个巨大的注意事项：不幸的是，你的技术大牛 CTO 把 Node.js 应用写得非常糟糕，一旦它启动后无法连接到 MongoDB 数据库，就会直接失败退出而不会重试。由于 JavaScript 是毒药，你需要找到一种方法确保 Node.js 应用在 MongoDB 容器准备好并能接受连接后再启动，而不能修改 `server.js` 。我在仓库中为 Node.js 应用提供了一个包装脚本 `wait-for` ，它允许你在启动 Node.js 应用之前等待 MongoDB 服务就绪。不过，为了使用这个脚本，你需要在你的容器中安装 `netcat` ，所以请确保在 Dockerfile 中包含它。查看仓库中的脚本以获取如何使用它的说明。当然，如果你有其他解决此问题的方法，也欢迎提出！

You will likely find the [Compose File Reference](https://docs.docker.com/compose/compose-file/) useful. Additionally, the [Getting Started](https://docs.docker.com/compose/gettingstarted/) guide will help as well, although it’s a python example instead of the superior Node.js /s I suggest poking around at other docs on that site also. I expect you to run into errors and difficulties – this is intended as part of the lab. Feel free to ask in #decal-general if you ever feel especially stuck!  
你可能会发现 Compose 文件参考很有用。此外，入门指南也会有帮助，尽管它是一个 Python 示例，而不是我建议的更优的 Node.js /s。我也建议你查看该网站上的其他文档。我预计你会遇到一些错误和困难——这是实验的一部分。如果你感到特别卡住，随时可以在 #decal-general 提问！

**Hints (if you want them):  
提示（如需）：**

*   For the Node.js Dockerfile, I recommend basing it off of `ubuntu:xenial` and installing everything you need (`nodejs`, `npm`, etc.) via `apt`. These aren’t in Debian’s `apt` repository so you’d have to find another way to install them if you use Debian.  
    对于 Node.js 的 Dockerfile，我建议基于 `ubuntu:xenial` 并通过 `apt` 安装你需要的所有软件（如 `nodejs` 、 `npm` 等）。这些软件不在 Debian 的 `apt` 仓库中，所以如果你使用 Debian，就必须找到其他方式来安装它们。
*   `npm install` needs to be run within the directory containing the repository (i.e. needs to be run within the directory that has the `package.json` file). If you want to change the current working directory within your Dockerfile, use the [`WORKDIR` command](https://docs.docker.com/engine/reference/builder/#workdir)  
    `npm install` 需要在包含仓库的目录中运行（即需要在包含 `package.json` 文件的目录中运行）。如果你想在 Dockerfile 中更改当前工作目录，请使用 `WORKDIR` 命令
*   If you change your Dockerfile after running `docker-compose up`, you will need to run `docker-compose build` to rebuild your services  
    如果你在运行 `docker-compose up` 后修改了 Dockerfile，你需要运行 `docker-compose build` 来重新构建你的服务

Once you’ve set things up properly, just running `docker-compose up` in the same directory as the `docker-compose.yml` file will bring up your web application!  
一旦你正确配置了环境，只需在与 `docker-compose.yml` 文件同一目录下运行 `docker-compose up` ，就可以启动你的网络应用！

[](#questions-1)Questions   问题
------------------------------

1.  Paste your `Dockerfile` for the Node.js web application  
    粘贴你的 `Dockerfile` 用于 Node.js 网络应用
2.  Paste your `docker-compose.yml` file  粘贴你的 `docker-compose.yml` 文件

[](#submission)Submission   提交
==============================

Don’t forget to submit to Gradescope!  
别忘了提交到 Gradescope！

* * *

[![](/assets/images/digitalocean.png)](https://www.digitalocean.com) With great appreciation to [DigitalOcean](https://www.digitalocean.com) for sponsoring the VMs used in both tracks of the DeCal

 [![](/assets/images/linode.png)](https://www.linode.com) Huge thanks to [Linode](https://www.linode.com) for sponsoring the equipment used to record digital lectures for the Decal

[![Hosted by the OCF](https://www.ocf.berkeley.edu/hosting-logos/ocf-hosted-penguin.svg)](https://www.ocf.berkeley.edu) Copyright © 2017-2021 [ Open Computing Facility ](https://www.ocf.berkeley.edu) and [eXperimental Computing Facility](https://xcf.berkeley.edu) 

This website and its course materials are licensed under the terms of the [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) License. [Source Code](https://github.com/0xcf/decal-web/) available on GitHub