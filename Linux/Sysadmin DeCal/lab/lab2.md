> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [decal.ocf.berkeley.edu](https://decal.ocf.berkeley.edu/archives/2021-fall/labs/a2/)

Advanced Lab 2 - Packages and Packaging and Troubleshooting  高级实验 2 - 软件包和打包及故障排除
=================================================================================

Facilitator: [Samuel Berkun](/staff#samuel-berkun)

9 min read

[](#table-of-contents)Table of contents   目录
--------------------------------------------

1.  [About This Lab  
    关于这个实验](#about-this-lab)
    1.  [Grading note  评分说明](#grading-note)
    2.  [Workflow  工作流程](#workflow)
2.  [Debian: An introduction to `apt` and `dpkg`  
    Debian： `apt` 和 `dpkg` 的简介](#debian-an-introduction-to-apt-and-dpkg)
    1.  [`apt`](#apt)
    2.  [`dpkg`](#dpkg)
3.  [Getting Started  开始](#getting-started)
4.  [Exercise 1: Compiling and Packaging  
    练习 1：编译和打包](#exercise-1-compiling-and-packaging)
    1.  [Writing and Compiling the Program  
        编写和编译程序](#writing-and-compiling-the-program)
    2.  [Packaging the executable  
        打包可执行文件](#packaging-the-executable)
5.  [Exercise 2: Troubleshooting  
    练习 2：故障排除](#exercise-2-troubleshooting)
6.  [Exercise 3: Spelunking  练习 3：探索](#exercise-3-spelunking)
7.  [For Hotshots  为热手们](#for-hotshots)
8.  [Resources  资源](#resources)

* * *

[](#about-this-lab)About This Lab   关于本实验
-----------------------------------------

### [](#grading-note)Grading note   评分说明

Labs are graded on completion. Treat this lab as seeds of exploration instead of just a grade.  
实验按完成情况评分。将此实验视为探索的种子，而不仅仅是分数。

### [](#workflow)Workflow   工作流

This lab should be completed on your student VM. You should have received an email with instructions and credentials for connecting your VM. Before starting the lab, `ssh` into your student VM. Please email us at [decal@ocf.berkeley.edu](mailto:decal@ocf.berkeley.edu) if you are having issues logging in.  
本实验应在您的学生虚拟机上进行。您应该已经收到一封包含连接虚拟机说明和凭证的邮件。在开始实验前，请登录到您的学生虚拟机。如果您在登录时遇到问题，请将问题邮件发送至 decal@ocf.berkeley.edu。

[](#debian-an-introduction-to-apt-and-dpkg)Debian: An introduction to `apt` and   Debian：简介和`dpkg`
--------------------------------------------------------------------------------------------------

In this class, we will be focused on using Debian. As noted within this week’s lecture, Debian uses apt/dpkg as its package manager. Other distributions use different package managers.  
在本课程中，我们将专注于使用 Debian。正如本周讲座中提到的，Debian 使用 apt/dpkg 作为其包管理器。其他发行版使用不同的包管理器。

### [](#apt)`apt`

The frontend package manager for Debian is `apt`. For the majority of times when you need to deal with a package manager, `apt` is usually the way to go. Before doing anything with `apt`, it is typically a good habit to update the package list so that the package manager can find and fetch the most updated versions of various packages. To do that, you can run:  
Debian 的前端包管理器是 `apt` 。在大多数需要处理包管理器的情况下，通常使用 `apt` 。在处理 `apt` 之前，通常是一个好习惯先更新包列表，以便包管理器能够找到并获取各种包的最新版本。为此，你可以运行：

`apt update`

To find a package to install:  
要查找可安装的软件包：

`apt search [package|description]`

To install a package:  
要安装软件包：

`apt install [package]`

To remove a package:  
要删除软件包：

`apt remove [package]`

Once you have been using the packages that you installed for a while, you may notice that they don’t automatically update themselves, a feature that may be present on programs written for other operating systems. To update the packages that you have installed, run:  
当你使用已安装的软件包一段时间后，可能会发现它们不会自动更新，这一功能在其他操作系统的程序中可能存在。要更新已安装的软件包，请运行：

`apt upgrade` or sometimes `apt dist-upgrade`   `apt upgrade` 或有时 `apt dist-upgrade`

It is more commonplace to use `apt upgrade` to update your packages, but there are times when you need to use `apt dist-upgrade`. You can read up more about the differences between the two [here](https://askubuntu.com/questions/194651/why-use-apt-get-upgrade-instead-of-apt-get-dist-upgrade).  
通常使用 `apt upgrade` 来更新您的软件包，但有时您需要使用 `apt dist-upgrade` 。您可以在以下链接中了解更多关于两者之间的区别。

In some circumstances, you want to be absolutely sure of the version of the package that you want to install. To list the potential versions that you can install, you can run:  
在某些情况下，您希望绝对确定要安装的软件包版本。要列出您可以安装的潜在版本，可以运行：

`apt policy [package]`

This lists the candidate version to install, according to its pin priority, along with other versions that are compatible with the system. To install a a version for a specific target release, you can run:  
这将根据其 pin 优先级列出要安装的候选版本，以及与系统兼容的其他版本。要为特定目标发布安装一个版本，可以运行：

`apt -t [targetrelease] install [package]`

There are also other commands that can remove unneeded dependencies and purge packages, but that is what the `man` pages are for. Please note that you are going to have to use `sudo` for the above commands since you are actually modifying the system itself.  
还有其他命令可以移除不需要的依赖项和清除软件包，但这些功能就是 `man` 页面的作用。请注意，由于你实际上是在修改系统本身，因此需要使用 `sudo` 来执行上述命令。

### [](#dpkg)`dpkg`

The backend package manager is `dpkg`. Traditionally, `dpkg` is used to install local packages. Using `dpkg`, you also can inspect packages and fix broken installs. To install local packages, run:  
后台包管理器是 `dpkg` 。传统上，使用 `dpkg` 来安装本地软件包。通过 `dpkg` ，你也可以检查软件包并修复损坏的安装。要安装本地软件包，请运行：

`dpkg -i [packagefilename]`

To remove a system package:  
要移除系统软件包：

`dpkg --remove [package]`

To inspect a package for more information about the package:  
要检查软件包以获取更多关于软件包的信息：

`dpkg -I [packagefilename]`

To fix/configure all unpacked but unfinished installs:  
要修复/配置所有已解压但未完成的安装：

`dpkg --configure -a`

[](#getting-started)Getting Started   入门指南
------------------------------------------

We are going to use `gcc` to compile source code and a simple utility called `fpm` to create packages in this lab.  
在这个实验中，我们将使用 `gcc` 来编译源代码，以及一个名为 `fpm` 的简单工具来创建软件包。

Using the commands above, install `gcc`, `make`, `ruby-dev`, and `ruby-ffi`.  
使用上述命令，安装 `gcc` 、 `make` 、 `ruby-dev` 和 `ruby-ffi` 。

Now check if GCC is installed by typing the followng:  
现在通过输入以下命令检查 GCC 是否已安装：

`gcc --version`

Now install `fpm` using `gem`, Ruby’s own package manager:  
现在使用 `gem` （Ruby 自带的包管理器）安装 `fpm` ：

`sudo gem install fpm`

Now check if `fpm` is installed:  
现在检查 `fpm` 是否已安装：

`fpm`

Now clone the `decal-web` repository:  
现在克隆 `decal-web` 仓库：

`git clone https://github.com/0xcf/decal-labs.git`

[](#exercise-1-compiling-and-packaging)Exercise 1: Compiling and Packaging   练习 1：编译和打包
---------------------------------------------------------------------------------------

Packaging manually for Debian can be very hard and frustrating, especially for first timers. That’s why for this class, we’ll be using a really cool Ruby package called fpm which simplifies the task of packaging a lot.  
手动为 Debian 打包可能会非常困难和令人沮丧，尤其是对于初学者来说。因此，在这个课程中，我们将使用一个非常酷的 Ruby 包 fpm，它大大简化了打包任务。

**Note:** This method is a great way to backport or package your own applications extremely quickly, but is not up to the more formal standards set by the [Debian New Maintainers’ Guide](https://www.debian.org/doc/manuals/maint-guide/). If you’re up for a challenge, feel free to try following the lab instructions, but using the guidelines [here](https://www.debian.org/doc/manuals/maint-guide/build.en.html) for `dpkg-buildpackage` instead of using `fpm`.  
注意：这种方法是快速回退或打包你自己的应用程序的绝佳方式，但不符合 Debian 新维护者指南中规定的更正式的标准。如果你愿意接受挑战，可以尝试按照实验说明进行操作，但请使用这里的 `dpkg-buildpackage` 指南代替 `fpm` 指南。

Now we will create a simplistic package using the hellopenguin executable that you will make in the coming steps. First, move into the a2 folder in the repository that you cloned in the Getting Started section:  
现在我们将使用你将在接下来的步骤中创建的 hellopenguin 可执行文件来创建一个简单的包。首先，进入 Getting Started 部分中你克隆的存储库中的 a2 文件夹：

`cd decal-labs/a2`

Now we are going to create a folder to work in for this exercise:  
现在我们将为这个练习创建一个工作文件夹：

`mkdir ex1`

And now move into the folder:  
现在进入该文件夹：

`cd ex1`

### [](#writing-and-compiling-the-program)Writing and Compiling the Program   编写和编译程序

Now, we will make a very simple application in C that prints “Hello Penguin!” named hellopenguin. Invoke:  
现在，我们将用 C 语言编写一个非常简单的应用程序，名为 hellopenguin，它将打印“Hello Penguin!”。执行：

`touch hellopenguin.c`

This will create an empty file named `hellopenguin.c`. Now, using the a preferred text editor of your choice, such as `vim`, `emacs`, or `nano`, insert the following code into `hellopenguin.c`  
这将创建一个名为 `hellopenguin.c` 的空文件。现在，使用你喜欢的文本编辑器（例如 `vim` 、 `emacs` 或 `nano` ），将以下代码插入 `hellopenguin.c`

```
#include <stdio.h>

int main()

{

   printf("Hello Penguin!\n");

   return 0;

} 
```

We will now compile the source file that you have just written:  
我们将现在编译你刚刚编写的源文件：

`gcc hellopenguin.c -o hellopenguin`

What this does is to take in a source file `hellopenguin.c` and compile it to an executable named `hellopenguin` with the `-o` output flag.  
这个操作是接收一个源文件 `hellopenguin.c` 并将其编译为一个名为 `hellopenguin` 的可执行文件，同时使用 `-o` 输出标志。

### [](#packaging-the-executable)Packaging the executable   打包可执行文件

Now, we will create the folder structure of where the executable shall reside in. In Debian, user-level packages usually reside in the folder `/usr/bin/`:  
现在，我们将创建可执行文件存放的文件夹结构。在 Debian 系统中，用户级别的软件包通常存放在 `/usr/bin/` 文件夹中。

`mkdir -p packpenguin/usr/bin`

Now move your compiled `hellopenguin` exectuable into the `packpenguin/usr/bin/` folder.  
现在将你编译的 `hellopenguin` 可执行文件移动到 `packpenguin/usr/bin/` 文件夹中。

`mv hellopenguin packpenguin/usr/bin/`

Now we will create a package called `hellopenguin`. Move into the parent directory of the `packpenguin` folder and invoke the following:  
现在我们将创建一个名为 `hellopenguin` 的软件包。进入 `packpenguin` 文件夹的父目录并执行以下命令：

`fpm -s dir -t deb -n hellopenguin -v 1.0~ocf1 -C packpenguin`

This specifies that you want to take in a directory, using the `-s` flag, and to output a `.deb` package using the `-t` flag. It takes in a directory called `packpenguin`, using the `-C` flag, and output a `.deb` file named `hellopenguin`, using the `-n`, with a version number of `1.0~ocf1`, using the `-v` flag.  
这表示您希望使用 `-s` 标志输入一个目录，并使用 `-t` 标志输出一个 `.deb` 包。它使用 `-C` 标志输入一个名为 `packpenguin` 的目录，并使用 `-n` 标志输出一个名为 `hellopenguin` 的 `.deb` 文件，版本号为 `1.0~ocf1` ，使用 `-v` 标志。

Now test it by invoking apt and installing it:  
现在通过调用 apt 来测试并安装它：

`sudo dpkg -i ./hellopenguin_1.0~ocf1_amd64.deb`

Now you should be able to run `hellopenguin` by doing the following:  
现在你应该能够通过以下方式运行 `hellopenguin` ：

`hellopenguin`

[](#exercise-2-troubleshooting)Exercise 2: Troubleshooting   练习 2：故障排除
----------------------------------------------------------------------

Now we are going to try and troubleshoot a package. Move to the other folder, `ex2`.  
现在我们要尝试解决一个软件包的问题。移动到另一个文件夹， `ex2` 。

Try installing the `ocfspy` package using `dpkg`. It should error. Take note what it is erroring on! Now try and fix it.  
尝试使用 `dpkg` 安装 `ocfspy` 软件包。它应该会报错。注意它报错的内容！现在尝试修复它。

**Hint:** Inspect the package for more details. The file to create that application is in the folder. Try compiling and packaging it. Exercise 1 may be a useful reference if you are stuck.  
提示：检查软件包以获取更多详细信息。创建该应用程序的文件位于该文件夹中。尝试编译和打包它。如果你遇到困难，练习 1 可能是一个有用的参考。

After you’re done, complete the following questions and made a submission to Gradescope.  
完成后，请回答以下问题并将提交到 Gradescope。

**Compiling and packaging  编译和打包**

1.  Will we still be able to run “hellopenguin” from any directory if we packaged it into “/usr/share” instead of “/usr/bin”?  
    如果我们将其打包到“/usr/share”而不是“/usr/bin”，我们是否仍然能够从任何目录运行“hellopenguin”？
2.  What is your rationale for the previous answer?  
    你为什么给出之前的答案？

**Debugging  调试**

1.  What package was missing after trying to install ocfspy?  
    在尝试安装 ocfspy 后，缺少了哪个软件包？
2.  What is the password that ocfspy outputs after fixing the dependency problem?  
    在解决依赖问题后，ocfspy 输出了什么密码？

Note that you may want to clean up your VM by removing `hellopenguin`, `ocfdocs`, and `ocfspy` from your system.  
请注意，您可能需要通过从系统中删除 `hellopenguin` 、 `ocfdocs` 和 `ocfspy` 来清理您的虚拟机。

[](#exercise-3-spelunking)Exercise 3: Spelunking   练习 3：探索
----------------------------------------------------------

Let’s shift gears a bit and take a look at a popular package to learn more about how it’s structured! If you recall from [lecture](https://www.youtube.com/watch?v=M0vPXQycer0&feature=youtu.be), we took at look at the contents of `htop`. For this next section, choose another package from the Debian repository to download and extract. You can choose any package that you’ve used/installed before (such as `tmux`, `sl`, or `tree`), or one from [this list](https://packages.debian.org/stable/).  
让我们稍微转换一下话题，看看一个流行的软件包，了解它的结构！如果你记得课堂上的内容，我们查看过 `htop` 的内容。在这个下一部分，从 Debian 软件仓库中选择另一个软件包来下载和提取。你可以选择任何一个你之前使用/安装过的软件包（例如 `tmux` 、 `sl` 或 `tree` ），或者从这个列表中选择一个。

Note that this exercise is mainly for exploration and learning purposes- you wouldn’t actually install a package using this method.  
请注意，这个练习主要是为了探索和学习目的——你实际上不会用这种方法来安装软件包。

**Once you’ve extracted the files (using `aunpack` as shown in lecture), answer the following questions on Gradescope:  
一旦你解压了文件（如讲座中所示使用 `aunpack` ），请在 Gradescope 上回答以下问题：**

1.  What package did you choose?  
    你选择了哪个软件包？
2.  What are the package’s dependencies? What file can you find them in?  
    这个软件包的依赖项是什么？你可以在哪个文件中找到它们？
3.  Extract `data.tar.gz` and view its contents. If there exists a folder(s) other than `usr/bin/` and `usr/share/`, pick one and briefly describe its purpose (both generally and in the context of this package). If not, explain why additional folders are not needed for this package.  
    提取 `data.tar.gz` 并查看其内容。如果除了 `usr/bin/` 和 `usr/share/` 之外还存在其他文件夹，选择其中一个并简要描述其用途（包括一般情况和在此软件包的上下文中）。如果没有，解释为什么此软件包不需要额外的文件夹。
4.  What’s one other interesting thing you learned about this package? (Binaries you never knew existed, easter eggs in documentation, a cool pre-install script…)  
    关于这个软件包，你还学到了什么其他有趣的东西？（你不知道的二进制文件、文档中的彩蛋、酷炫的预安装脚本……）

**Hints:  提示：**

*   The command to download a package is `apt download <packagename>`.  
    下载软件包的命令是 `apt download <packagename>` 。
*   To use `aunpack`, you might need to `sudo apt install atool`.  
    要使用 `aunpack` ，你可能需要 `sudo apt install atool` 。
*   Try to choose a package with a smaller filesize, so you won’t have to wait long for it to download and extract.  
    尝试选择一个文件大小较小的软件包，这样你就不必长时间等待它下载和提取。
*   The lecture demo will be quite helpful! You may want to watch it again for reference.  
    讲座演示会非常有帮助！你可能想再次观看它作为参考。

[](#for-hotshots)For Hotshots   为 Hotshots
------------------------------------------

In the past examples, we have always precompiled a given program before packaging it. One upside to this, is that the package will always work for systems similar to the one that you run. However, once we start introducing other machines with potentially different architectures, we suddenly need to create duplicate packages compiled specifically for those systems. Create a new package that unpacks the source code for a file, compiles it, moves all of the relevant files to their respective locations, before deleting the irrelevant files.  
在之前的示例中，我们总是先预编译给定的程序，然后再进行打包。这样做的好处是，包将始终适用于与您运行系统相似的系统。然而，一旦我们开始引入其他可能具有不同架构的机器，我们突然需要为这些系统创建专门编译的重复包。创建一个新的包，该包解压文件的源代码，编译它，将所有相关文件移动到它们各自的位置，然后删除不相关的文件。

[](#resources)Resources   资源
----------------------------

Below are some resources that I found helpful in the creation of this lab. If you are feeling adventurous, you may want to poke around these documents as well.  
以下是我创建这个实验时发现的一些有帮助的资源。如果您喜欢冒险，也可以浏览这些文档。

[fpm](https://github.com/jordansissel/fpm/wiki)

[TLDR pages, a more readable man page  
TLDR 页面，更易读的 man 手册](https://tldr.sh/)

[dpkg](https://linux.die.net/man/1/dpkg), alternatively `man dpkg`  dpkg，或者 `man dpkg`

[apt](https://linux.die.net/man/8/apt), alternatively `man apt`  apt，或者 `man apt`

[Debian New Maintainers’ Guide  
Debian 新维护者指南](https://www.debian.org/doc/manuals/maint-guide/)

* * *

[![](/assets/images/digitalocean.png)](https://www.digitalocean.com) With great appreciation to [DigitalOcean](https://www.digitalocean.com) for sponsoring the VMs used in both tracks of the DeCal

 [![](/assets/images/linode.png)](https://www.linode.com) Huge thanks to [Linode](https://www.linode.com) for sponsoring the equipment used to record digital lectures for the Decal

[![Hosted by the OCF](https://www.ocf.berkeley.edu/hosting-logos/ocf-hosted-penguin.svg)](https://www.ocf.berkeley.edu) Copyright © 2017-2021 [ Open Computing Facility ](https://www.ocf.berkeley.edu) and [eXperimental Computing Facility](https://xcf.berkeley.edu) 

This website and its course materials are licensed under the terms of the [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) License. [Source Code](https://github.com/0xcf/decal-web/) available on GitHub