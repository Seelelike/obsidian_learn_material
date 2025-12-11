> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [decal.ocf.berkeley.edu](https://decal.ocf.berkeley.edu/archives/2021-fall/labs/a6/)

Lab 6 - Networking 102  实验 6 - 网络技术 102
=======================================

26 min read

[](#table-of-contents)Table of contents   目录
--------------------------------------------

1.  [Overview  概述](#overview)
2.  [Network Interfaces  网络接口](#network-interfaces)
    1.  [/etc/network/interfaces](#etcnetworkinterfaces)
    2.  [Checkpoint  检查点](#checkpoint)
3.  [/proc filesystem  /proc 文件系统](#proc-filesystem)
    1.  [/proc/net/](#procnet)
        1.  [/proc/net/dev](#procnetdev)
        2.  [/proc/net/[tcp|udp|raw]](#procnettcpudpraw)
        3.  [/proc/net/route](#procnetroute)
        4.  [/proc/net/arp](#procnetarp)
        5.  [/proc/net/snmp](#procnetsnmp)
    2.  [/proc/sys/](#procsys)
        1.  [/proc/sys/net/core/](#procsysnetcore)
        2.  [/proc/sys/net/ipv4/](#procsysnetipv4)
    3.  [Checkpoint  检查点](#checkpoint-1)
4.  [ARP configuration  ARP 配置](#arp-configuration)
5.  [DNS configuration  DNS 配置](#dns-configuration)
    1.  [/etc/hosts](#etchosts)
    2.  [/etc/resolv.conf](#etcresolvconf)
    3.  [/etc/nsswitch.conf](#etcnsswitchconf)
6.  [DHCP client configuration  
    DHCP 客户端配置](#dhcp-client-configuration)
    1.  [Timing   定时](#timing)
7.  [Sysadmin commands  系统管理员命令](#sysadmin-commands)
    1.  [ifupdown](#ifupdown)
    2.  [mtr](#mtr)
    3.  [iptables](#iptables)
    4.  [Checkpoint  检查点](#checkpoint-2)
8.  [Exercises  练习](#exercises)
    1.  [🔥 This is fine 🔥  
        🔥 这很好 🔥](#-this-is-fine-)
        1.  [IMPORTANT NOTE  重要提示](#important-note)
        2.  [Setup  设置](#setup)
        3.  [Problem Instructions  问题说明](#problem-instructions)
        4.  [Debugging Bank  调试库](#debugging-bank)
    2.  [Net Ninjas (Optional)  网络忍者（可选）](#net-ninjas-optional)

* * *

[](#overview)Overview   概述
--------------------------

This lab will go over some concepts of networking and how certain parts of a network stack are implemented and configured in linux systems. **It is assumed that you are familiar with basic networking concepts such as those presented in [Lab b5](/labs/b5).**  
本实验将介绍一些网络概念，以及 Linux 系统中网络堆栈的某些部分是如何实现和配置的。假设你已经熟悉实验室 b5 中介绍的基本网络概念。

**Additional information about certain files discussed here can be found in their corresponding `man` pages by typing `man <filename>`.  
关于这里讨论的某些文件的更多信息，可以通过输入 `man <filename>` 在相应的 `man` 页面找到。**

As with the previous labs, there will be questions sprinkled throughout- head over to Gradescope to submit your answers to them!  
和之前的实验一样，全文中会穿插一些问题——请前往 Gradescope 提交你的答案！

* * *

[](#network-interfaces)Network Interfaces   网络接口
------------------------------------------------

**Network interfaces represent a point of connection between a computer and a network.** Typically network interfaces are associated with a physical piece of hardware, like a network interface card. However, interfaces can also be entirely implemented in software and have no physical counterpart – take the loopback interface `lo` for example. `lo` is a virtual interface; it simulates a network interface with only software.  
网络接口代表计算机和网络之间的连接点。通常网络接口与一块物理硬件相关联，比如网卡。然而，接口也可以完全在软件中实现，没有任何物理对应物——以回环接口 `lo` 为例。 `lo` 是一个虚拟接口；它仅通过软件模拟网络接口。

### [](#etcnetworkinterfaces)/etc/network/interfaces

Network interface configurations are stored under the **/etc/network/interfaces** file on your system. Here, there is plenty of room for complexity. For example, you can have certain interfaces automatically brought up by hooking them to system boot scripts or specify some interfaces to only be available under certain circumstances, with some of the provided control flow options.  
网络接口配置存储在系统中的 /etc/network/interfaces 文件下。这里有很多复杂性的空间。例如，你可以通过将它们连接到系统启动脚本来自动启动某些接口，或者使用提供的控制流选项指定某些接口只在特定情况下可用。

This lab will go over some common configuration keywords, but there is much more to the file. For a detailed page of the features and syntax of the file simply type `man interfaces` to pull up the `man` page for the file.  
本实验将介绍一些常见的配置关键字，但该文件还有更多内容。要查看该文件的功能和语法的详细页面，只需输入 `man interfaces` 即可调出 `man` 页面。

Firstly, configurations are logically divided into units known as **[stanzas](https://askubuntu.com/questions/863274/what-is-a-stanza-in-linux-context-and-where-does-the-world-come-from)**. The **/etc/network/interfaces** file is comprised of zero or more stanzas which begin with `iface`, `mapping`, `auto`, `allow-`, `source`, or `source-directory`. For brevity, we will go over the two most commonly used stanzas `auto` and `iface`.  
首先，配置在逻辑上被划分为称为节（stanzas）的单位。/etc/network/interfaces 文件由零个或多个以 `iface` 、 `mapping` 、 `auto` 、 `allow-` 、 `source` 或 `source-directory` 开头的节组成。为了简洁，我们将介绍两个最常用的节 `auto` 和 `iface` 。

The `auto` stanza is fairly simple, its syntax is `auto <iface>`. The `auto` stanza flags an interface to be brought up whenever `ifup` is run with the `-a` option (More on `ifup` below). Since system boot scripts use `ifup` with the `-a` option, these interfaces are brought up during boot. Multiple `auto` stanzas will be executed in the same order as they are written in the file.  
`auto` 节相对简单，其语法是 `auto <iface>` 。 `auto` 节用于标记接口，在运行 `ifup` 时使用 `-a` 选项时（更多关于 `ifup` 的内容见下文）会启动该接口。由于系统启动脚本使用 `ifup` 与 `-a` 选项，这些接口在启动时会被激活。多个 `auto` 节将按照文件中写入的顺序执行。

The `iface` stanzas lets you express complex configurations for individual interfaces by leveraging its features. Its syntax is `iface <iface> <address-family> <method>`. **Let’s go over some of the arguments the stanza takes.**  
`iface` 节通过利用其特性，允许你为单个接口表达复杂的配置。其语法是 `iface <iface> <address-family> <method>` 。让我们来看看该节接受的参数。

`<address-family>` identifies the addressing that the interface will be using. The most common address families that you’re probably familiar with are:  
`<address-family>` 用于标识接口将使用的地址类型。你可能熟悉的最常见的地址类型包括：

*   IPv4 denoted by `inet` in the file  
    IPv4，在文件中以 `inet` 表示
*   IPv6 denoted by `inet6` in the file  
    IPv6，在文件中以 `inet6` 表示

Address families can be configured via different methods expressed by the `<method>` option. Some common methods you should be familiar with are:  
地址类型可以通过 `<method>` 选项以不同方法进行配置。你应该熟悉的一些常见方法包括：

*   `loopback` defines this interface as the [loopback](https://www.juniper.net/documentation/en_US/junos/topics/concept/interface-security-loopback-understanding.html).  
    `loopback` 将此接口定义为回环接口。
*   `dhcp` is for interface configuration via a DHCP server.  
    `dhcp` 用于通过 DHCP 服务器进行接口配置。
*   `static` is for static interface configuration.  
    `static` 用于静态接口配置。
*   `manual` brings up the interface with **no** default configuration.  
    `manual` 以无默认配置的方式启动接口。

Methods also have options that let you supply certain configuration parameters. For example, for the `static` method you can additionally use the `address <ip-address>` and `netmask <mask>` options to specify the static IP address and netwask you want the interface to use.  
方法还提供选项，允许你提供某些配置参数。例如，对于 `static` 方法，你可以额外使用 `address <ip-address>` 和 `netmask <mask>` 选项来指定接口要使用的静态 IP 地址和网络。

Moreover, the `iface` stanza additionally has its own options compatible with all families and methods. To present just a few, we have:  
此外， `iface` 节还拥有与所有系列和方法兼容的选项。仅举几例，我们有：

*   `pre-[up|down] <command>` runs the given `<command>` before the interface is either taken up or down  
    `pre-[up|down] <command>` 在接口被启用或禁用之前运行给定的 `<command>`
*   `post-[up|down] <command>` runs the given `<command>` after the interface is either taken up or down  
    `post-[up|down] <command>` 在接口被启用或禁用后运行 `<command>`

As a final note, any changes to the configurations done in this file during runtime are **not** applied automatically. Changes have to be reloaded via calls to `ifupdown`, the de facto command suite for interacting with **/etc/network/interfaces**.  
最后，如果在运行时对此文件中的配置进行任何更改，这些更改不会自动应用。必须通过调用 `ifupdown` 重新加载，它是与/etc/network/interfaces 交互的实际命令集。

### [](#checkpoint)Checkpoint   检查点

**Question 1a:** Is the result of running `ping` enough to determine whether or not you can reach a server? Why or why not?  
问题 1a：运行 `ping` 的结果足以确定您是否可以访问服务器吗？为什么可以或不可以？

**Question 1b:** Here’s a quick check for your understanding – below is a very common default configuration for `/etc/network/interfaces`:  
问题 1b：这是一个快速检查你理解程度的题目——下面是 `/etc/network/interfaces` 的一个非常常见的默认配置：

```
auto lo
iface lo inet loopback 
```

In your own words, explain what this configuration does. What would happen if you deleted these lines and rebooted?  
用自己的话解释一下这个配置的作用。如果你删除这些行并重启，会发生什么？

**Question 1c:** Write a few stanzas that configure an interface called `test` that is brought up on boot and given the following address: `192.168.13.37/16`.  
问题 1c：编写几行配置，使名为 `test` 的接口在启动时启用，并分配以下地址： `192.168.13.37/16` 。

[](#proc-filesystem)/proc filesystem   /proc 文件系统
-------------------------------------------------

`proc` is a **virtual filesystem** that presents runtime system information in a file-like structure. This file-like interface provides a standardized method for querying and interacting with the kernel, which dumps metrics in the read-only files located in this directory. Using a tool like `cat`, you can dynamically read those files at runtime. But keep in mind, **there are no ‘real’ files within `proc`.**  
`proc` 是一个虚拟文件系统，以文件结构形式展示运行时系统信息。这种文件式接口为查询和交互内核提供了一种标准化方法，内核将指标输出到位于此目录的只读文件中。使用像 `cat` 这样的工具，你可以在运行时动态读取这些文件。但请注意， `proc` 中没有“真实”的文件。

### [](#procnet)/proc/net/

We will be focusing on certain portions of `proc`, the first of which being **/proc/net/**. This subdirectory in `proc` contains information about various parts of the network stack in the form of virtual files. Many commands, such as netstat, use these files when you run them.  
我们将重点关注 `proc` 中的某些部分，首先是/proc/net/。这个子目录在 `proc` 中包含了关于网络栈各个部分的虚拟文件信息。许多命令，如 netstat，在运行时会使用这些文件。

#### [](#procnetdev)/proc/net/dev

This file contains information and statistics on network devices. `ifconfig` is an example command that reads from this file. Take a look below and notice how the information presented in the ifconfig output corresponds to data dumped in `dev` on how many bytes and packets have been received or transmitted by an interface. ![alt text](https://i.imgur.com/MC03IMA.png "/proc/net/dev")  
这个文件包含有关网络设备的信息和统计数据。 `ifconfig` 是一个示例命令，它从这个文件中读取信息。看看下面，注意 ifconfig 输出中显示的信息如何与 `dev` 中转发的字节数和数据包数对应。 ![alt text](https://i.imgur.com/MC03IMA.png "/proc/net/dev")

#### [](#procnettcpudpraw)/proc/net/[tcp|udp|raw]

The`tcp`, `raw`, and `udp` files each contain metrics on open system sockets for their respective protocols, i.e. reading `tcp` displays info on TCP sockets. As a side note, raw sockets are network sockets that offer a finer degree of control over the header and payload of packets at each network layer as opposed to leaving that responsibility to the kernel. They are ideal for uses cases that send or receive packets of a type not explicitly supported by a kernel, think ICMP. For additional information check [this article](http://opensourceforu.com/2015/03/a-guide-to-using-raw-sockets/) out. These files are read by `ss`, `netstat`, etc. Check out the example for tcp below. ![alt text](https://i.imgur.com/5ETFK84.png "/proc/net/tcp")  
`tcp` 、 `raw` 和 `udp` 文件分别包含各自协议的开放系统套接字的指标，例如读取 `tcp` 会显示 TCP 套接字的信息。顺便一提，原始套接字是网络套接字，它们在每一层网络中提供对数据包头部和有效载荷更精细的控制，而不是将这一责任交给内核。它们非常适合发送或接收内核未明确支持的类型的数据包，例如 ICMP。想了解更多信息，可以查看这篇文章。这些文件由 `ss` 、 `netstat` 等程序读取。下面是 tcp 的示例。 ![alt text](https://i.imgur.com/5ETFK84.png "/proc/net/tcp")

#### [](#procnetroute)/proc/net/route

This file contains information about the kernel routing table. Some commands that use this file include `ip` and `netstat`. Take a look at how the file is parsed and processed by the `netstat` command.  
这个文件包含有关内核路由表的信息。一些使用此文件的命令包括 `ip` 和 `netstat` 。看看 `netstat` 命令如何解析和处理这个文件。

![alt text](https://i.imgur.com/tD3oKfO.png)

#### [](#procnetarp)/proc/net/arp

This file contains a dump of the system’s ARP cache. The `arp` command reads from this file. For example, look at how closely the output of the `arp` command resembles the raw text dumped by the kernel into the file.  
这个文件包含了系统 ARP 缓存的转储内容。 `arp` 命令从这个文件中读取数据。例如，看看 `arp` 命令的输出如何与内核转储到文件中的原始文本高度相似。

![alt text](https://i.imgur.com/CI7nUJL.png)

#### [](#procnetsnmp)/proc/net/snmp

This file contains statistics intended to be used by SNMP agents, which are a part of the Simple Network Management Protocol (SNMP). Regardless of whether or not your system is running SNMP, the data in this file is useful for investigating the network stack. Take the screenshot below for example, examining the fields we see `InDiscards` which according to [RFC 1213](https://tools.ietf.org/html/rfc1213) indicates packets that are discarded since problems were encountered that prevented their continued processing. Lack of buffer space is a possible cause of having a high number of discards. Having a statistic like this one, amonst others, can help pinpoint a network issue. For additional information on fields please refer to the header file [here](https://elixir.bootlin.com/linux/v4.4/source/include/net/snmp.h). The image is a bit small so feel free to **right click -> “Open image in a new tab” to magnify the output.**  
这个文件包含用于 SNMP 代理的统计数据，而 SNMP 代理是简单网络管理协议（SNMP）的一部分。无论你的系统是否运行 SNMP，这个文件中的数据都可用于调查网络栈。以下面的截图为例，我们可以看到字段 `InDiscards` ，根据 RFC 1213，这表示由于遇到问题而无法继续处理的数据包。缓冲区空间不足是导致丢弃数量较高的可能原因。拥有像这样的统计数据，以及其他数据，可以帮助确定网络问题。有关字段的更多信息，请参考这里的头文件。图像有点小，所以你可以右键点击 -> “在新标签页中打开图像”来放大输出。

![alt text](https://i.imgur.com/5vomFYZ.png)

### [](#procsys)/proc/sys/

Whereas the files and subdirectories mentioned above are read-only that isn’t true about the **/proc/sys** subdirectory which contains virtual files that also allow writes. You can not only query for system runtime parameters but also write new parameters into these files. This means you have the power to adjust kernel behavior without the need for a reboot or recompilation.  
上述提到的文件和子目录是只读的，但/proc/sys 子目录却不是这样，它包含虚拟文件，这些文件也允许写入。你不仅可以查询系统运行时参数，还可以向这些文件写入新参数。这意味着你拥有调整内核行为的能力，而无需重启或重新编译。

**Mind-blowing.  令人惊叹。**

While the **/proc/sys** directory contains a variety of subdirectories corresponding to aspects of the machine, the one we will be focusing on is **/proc/sys/net** which concerns various networking topics. Depending on configurations at the time of kernel compilation, different subdirectories are made available in **/proc/sys/net**, such as `ethernet/`, `ipx/`, `ipv4/`, and etc. Given the sheer variety of possible configurations, we will confine the scope of this discussion to the most common directories.  
虽然/proc/sys 目录包含多种对应机器不同方面的子目录，但我们将关注的是/proc/sys/net，它涉及各种网络主题。根据内核编译时的配置，/proc/sys/net 中会提供不同的子目录，如 `ethernet/` 、 `ipx/` 、 `ipv4/` 等。考虑到可能的配置种类繁多，我们将讨论范围限定在最常用的目录上。

#### [](#procsysnetcore)/proc/sys/net/core/

`core/` is the first subdirectory that we’ll cover. As its name implies, it deals with core settings that direct how the kernel interacts with various networking layers.  
`core/` 是我们首先要讨论的子目录。顾名思义，它处理核心设置，这些设置决定了内核如何与各种网络层交互。

Now we can go over some specific files in this directory, their functionality, and motivations behind adjusting them.  
现在我们可以查看这个目录中的一些特定文件，了解它们的功能以及调整它们的动机。

*   `message_burst` and `message_cost` Both of these parameters take a single integer argument and together control the logging frequency of the kernel. `message_burst` defines entry frequency and `message_cost` defines time frequency in seconds. For example, let’s take a look at their defaults. `message_burst` defaults to 10 and `message_cost` defaults to 5. This means the kernel is limited to logging 10 entries every 5 seconds.  
    `message_burst` 和 `message_cost` 这两个参数都接受一个整数参数，共同控制内核的日志记录频率。 `message_burst` 定义入口频率， `message_cost` 定义每秒的时间频率。例如，让我们看看它们的默认值。 `message_burst` 默认为 10， `message_cost` 默认为 5。这意味着内核每 5 秒最多记录 10 条日志。
    
    When adjusting the parameters in these two files, a sysadmin must keep in mind that the tradeoff here is between the granularity of the logs and the performance/storage limitations of the system. Increasing overall logging frequency can translate to a hit to system performance or huge log files eating up disk. But if logging is too infrequent, parts of the network may fail silently and bugs may become much harder to identify.  
    在调整这两个文件中的参数时，系统管理员必须牢记，这里的权衡在于日志的粒度与系统的性能/存储限制之间。增加整体日志频率可能会导致系统性能下降或巨大的日志文件占用磁盘空间。但如果日志记录过于频繁，网络的部分区域可能会无声无息地失效，而错误也可能会变得更加难以识别。
    
*   `netdev_max_backlog` This file takes one integer parameter that defines the maximum number of packets allowed to queue on a particular interface.  
    `netdev_max_backlog` 该文件接受一个整数参数，用于定义在特定接口上允许排队的数据包的最大数量。
*   `rmem_default` and `rmem_max` These files define the default and maximum buffer sizes for receive sockets, respectively.  
    `rmem_default` 和 `rmem_max` 这些文件分别定义了接收套接字和发送套接字的默认和最大缓冲区大小。
*   `smem_default` and `smem_max` These files define the default and maximum buffer sizes for send sockets, respectively.  
    `smem_default` 和 `smem_max` 这些文件分别定义了接收套接字和发送套接字的默认和最大缓冲区大小。
    
    For the above sets of system parameters, adjusting queue lengths have the nice effect of allowing our system to hold more packets and avoid dropping packets due to a fast sender for example. This boils down to optimizing flow control. However, there is no such thing as a free lunch. Increasing queue sizes can only mitigate problems with arrival rates being greater than service rates for so long. For more information on why that is check out [queueing theory](https://en.wikipedia.org/wiki/Queueing_theory). Moreover, having many packets stored in long queues also has its own drawback. Storing packet information isn’t free and the more packets stored in the queue, the more resources the system needs. As a result, too many packets may lead to increased [paging](https://en.wikipedia.org/wiki/Paging) and ultimately [thrashing](https://en.wikipedia.org/wiki/Thrashing_(computer_science)). Once again, we have another tradeoff, but this time between flow control and paging.  
    对于上述系统参数，调整队列长度可以很好地让系统容纳更多数据包，避免因发送方过快而导致数据包丢失。这归根结底是流量控制的问题。然而，天下没有免费的午餐。增加队列大小只能暂时缓解到达率高于服务率的问题。想了解更多原因，可以研究排队论。此外，在长队列中存储大量数据包也有其弊端。存储数据包信息是有成本的，队列中存储的数据包越多，系统需要的资源就越多。结果，过多的数据包可能导致页面交换增加，最终引发颠簸。再一次，我们面临权衡，这次是在流量控制和页面交换之间。
    

As a sysadmin many of the configuration decisions you make will be balancing between two extremes and the optimal point isn’t hard and fast. Many times you’ll have to adjust system parameters on a case-by-case basis and, after empirical testing, come to a good point.  
作为一名系统管理员，你做出的许多配置决策都将在两个极端之间进行权衡，而最佳点并非固定不变。很多时候你不得不根据具体情况调整系统参数，并在经过实证测试后找到一个较好的点。

#### [](#procsysnetipv4)/proc/sys/net/ipv4/

`ipv4/` is another common subdirectory that contains setting relevant to IPv4. Often the settings used in this subdirectory are used, in conjunction with other tools, as a security measure to mitigate network attacks or to customize behavior when the system acts as a router.  
`ipv4/` 是另一个常见的子目录，其中包含与 IPv4 相关的设置。通常，这个子目录中使用的设置与其他工具结合使用，作为一种安全措施来减轻网络攻击，或在系统作为路由器时自定义行为。

*   `icmp_echo_ignore_all` This file configures the system’s behavior towards ICMP ECHO packets. This file has two states `0` for off and `1` for on. If on, the system will ignore ICMP ECHO packets from every host.  
    `icmp_echo_ignore_all` 此文件配置系统对 ICMP 回显数据包的行为。此文件有两种状态 `0` 关闭和 `1` 开启。如果开启，系统将忽略来自所有主机的 ICMP 回显数据包。
*   `icmp_echo_ignore_broadcasts` This file is similar to the one above, except turning this parameter on only makes the system ignore ICMP ECHO packets from broadcast and multicast.  
    `icmp_echo_ignore_broadcasts` 这个文件与上面那个类似，不同之处在于启用这个参数只会让系统忽略广播和多播的 ICMP 回显数据包。

One argument against disabling ICMP is that it makes obtaining diagnostic information about servers much harder. The output of tools that rely on ICMP, i.e. `ping`, are no longer as useful. On the other hand, allowing ICMP might be a bad idea if your goal is to hide certain machines. Additionally, ICMP has been used in the past in [DOS attacks](https://en.wikipedia.org/wiki/Ping_of_death).  
反对禁用 ICMP 的一个论点是，这使获取服务器诊断信息变得非常困难。依赖 ICMP 的工具（如 `ping` ）的输出不再那么有用。另一方面，如果你的目标是隐藏某些机器，允许 ICMP 可能是个坏主意。此外，ICMP 过去曾被用于拒绝服务攻击。

*   `ip_forward` Turning this parameter on permits interfaces on the system to forward packets. Take for example, if your computer has two interfaces, each connected to two different subnets, `A` and `B`. While your machine can individually send and receive traffic to hosts on either network, machines on `A` cannot send packets to machines on `B` via your machine. Turning `ip_forward` on is the first step to configuring your linux machine to act as a router. It is common to see this on machines that act as VPN servers, forwarding traffic on behalf of hosts.  
    `ip_forward` 开启这个参数允许系统中的接口转发数据包。例如，如果你的计算机有两个接口，每个接口连接到两个不同的子网， `A` 和 `B` ，虽然你的计算机可以单独向这两个网络中的主机发送和接收流量，但 `A` 上的计算机无法通过你的计算机向 `B` 上的计算机发送数据包。开启 `ip_forward` 是配置你的 Linux 计算机作为路由器的第一步。常见于作为 VPN 服务器运行的计算机，代表主机转发流量。
    
*   `ip_default_ttl` This is a simple file that configures the default TTL (time to live) for outbound IP packets.  
    `ip_default_ttl` 这是一个配置出站 IP 数据包默认 TTL（生存时间）的简单文件。
*   `ip_local_port_range` This file takes two integer parameters. The first integer specifies the lower bound of the range and the second specifies the upper bound. Together, the two numbers define the range of ports that can be used by TCP or UDP when a local port is needed. For example, when a socket is instantiated to send a TCP SYN, the port given to the socket is selected by the operating system and lies within the specified range. The ports in this range are as known as [ephemeral ports](https://en.wikipedia.org/wiki/Ephemeral_port).  
    `ip_local_port_range` 该文件接受两个整数参数。第一个整数指定范围的下限，第二个整数指定上限。这两个数字共同定义了当需要本地端口时，TCP 或 UDP 可以使用的端口范围。例如，当实例化一个用于发送 TCP SYN 的套接字时，操作系统会选择分配给套接字的端口，该端口位于指定的范围内。这个范围内的端口被称为临时端口。
*   `tcp_syn_retries` This file limits the number of times the system re-transmits a SYN packet when attempting to make a connection. When attempting to connect to either a ‘flaky’ host or over a ‘flaky’ network, setting this number higher might be desirable. But this comes at the cost of adding additional traffic to the network and potential blocking other processes while waiting for a SYN-ACK that might never come.  
    `tcp_syn_retries` 该文件限制了系统在尝试建立连接时重传 SYN 数据包的次数。当尝试连接到一个“不稳定”的主机或通过一个“不稳定”的网络时，将此数字设置得更高可能更可取。但这以增加网络额外流量和等待可能永远不会到来的 SYN-ACK 时可能阻塞其他进程为代价。
*   `tcp_retries1` This file limits the number of re-transmissions before signaling the network layer about a potential problem with the connection.  
    `tcp_retries1` 该文件限制重传次数，并在可能存在连接问题时向网络层发出信号。
*   `tcp_retries2` This file limits the number of re-transmissions before killing active connections. This implies the following relationship: `tcp_retries2` >= `tcp_retries1`. The two retry values configure how ‘patient’ your system should be when it comes to waiting on [RTOs](https://www.extrahop.com/company/blog/2016/retransmission-timeouts-rtos-application-performance-degradation/).  
    `tcp_retries2` 该文件限制在终止活动连接之前允许重传的次数。这意味着以下关系： `tcp_retries2` >= `tcp_retries1` 。这两个重试值配置了系统在等待 RTO 时应该有多“耐心”。

**Additional information on configurable system parameters can be found either at this [tutorial](https://www.frozentux.net/ipsysctl-tutorial/chunkyhtml/index.html) or in documentation via [kernel.org](https://www.kernel.org/doc/Documentation/networking/ip-sysctl.txt) or [bootlin](https://elixir.bootlin.com/linux/v4.4/source/Documentation/networking/ip-sysctl.txt).  
有关可配置系统参数的更多信息，可以在本教程中找到，也可以通过 kernel.org 或 bootlin 的文档获取。**

### [](#checkpoint-1)Checkpoint   检查点

**Question 2a:** Describe the `tcp_syncookies` `sysctl` option. How can we toggle this value on, and when would we want this on?  
问题 2a：描述 `tcp_syncookies` `sysctl` 选项。我们如何开启这个值，以及什么时候需要开启它？

[](#arp-configuration)ARP configuration   ARP 配置
------------------------------------------------

The entries in the kernel’s arp cache can be read during system runtime via **/proc/net/arp** as mentioned above.  
如前所述，可以通过/proc/net/arp 在系统运行时读取内核的 ARP 缓存条目。

Additionally, ARP can be configured with persistent static entries. This typically done via a file. Batches of static entries can be included in such a file. The line-by-line format should be `<mac-address> <ip-address>`. To load the file’s entries into the system’s ARP cache one can run `arp -f <file>`. Typically the file that holds these entries has the path **/etc/ethers**.  
此外，ARP 可以配置为具有持久的静态条目。这通常通过一个文件完成。静态条目的批次可以包含在这样的文件中。行格式应为 `<mac-address> <ip-address>` 。要加载文件中的条目到系统的 ARP 缓存中，可以运行 `arp -f <file>` 。通常包含这些条目的文件路径为 /etc/ethers。

Static ARP entries are cleared from the system ARP cache on reboot, meaning one would have to run the above command on each boot if we wanted the mappings to ‘persist’. To automate the procedure of running the command we can leverage the interface configuration workflow. Recall that **/etc/network/interfaces** provides the `auto` stanza to identify interfaces to be automatically configured on boot. Used in conjunction with the `iface` stanza and its `post-up <command>` option, we can execute the `arp -f /etc/ethers` command. This effectively has static entries ‘persist’ by having them added alongside interface configuration during boot.  
静态 ARP 条目在系统重启时会被清除，这意味着如果我们想要映射关系“持久化”，就需要在每次启动时运行上述命令。为了自动化运行命令的过程，我们可以利用接口配置流程。回想一下 /etc/network/interfaces 提供了 `auto` 段来标识需要在启动时自动配置的接口。与 `iface` 段及其 `post-up <command>` 选项结合使用时，我们可以执行 `arp -f /etc/ethers` 命令。这有效地通过在启动时将静态条目添加到接口配置中，使静态条目“持久化”。

[](#dns-configuration)DNS configuration   DNS 配置
------------------------------------------------

Some of the DNS configuration files that we will be going over are **/etc/hosts**, **/etc/resolv.conf**, **/etc/nsswitch.conf**.  
我们将要讨论的一些 DNS 配置文件包括 /etc/hosts、/etc/resolv.conf、/etc/nsswitch.conf。

### [](#etchosts)/etc/hosts

This is simple text file that stores static mappings from IP addresses to hostnames. The format for each line is `<ip-address> <cannonical-hostname> [aliases]`. An example line would be `31.13.70.36 www.facebook.com fb ZuccBook`  
这是一个简单的文本文件，用于存储从 IP 地址到主机名的静态映射。每行的格式为 `<ip-address> <cannonical-hostname> [aliases]` 。一个示例行是 `31.13.70.36 www.facebook.com fb ZuccBook`

Thanks to this entry we have mapped `www.facebook.com` and any aliases we listed to `31.13.70.36`. A very common example entry is `localhost` which also has the aliases `ip6-localhost`,`ip6-loopback` which explains why running something like `ping localhost` or `ping ip6-loopback` works. This file is one way to manually define translations for certain hostnames.  
由于这个条目，我们将 `www.facebook.com` 和列出的任何别名映射到 `31.13.70.36` 。一个非常常见的示例条目是 `localhost` ，它还有别名 `ip6-localhost` 、 `ip6-loopback` ，这解释了为什么运行 `ping localhost` 或 `ping ip6-loopback` 这样的命令可以工作。这个文件是手动为某些主机名定义映射的一种方式。

### [](#etcresolvconf)/etc/resolv.conf

Whereas **/etc/hosts** is for static translations of specific hostnames, many times we want to dynamically resolve names by issuing a query to a name server. There are usually many nameservers, public or private, available to fufill such a query and deciding which ones to query is the job of **/etc/resolv.conf** amongst other configuration options.  
而/etc/hosts 用于静态翻译特定主机名，但很多时候我们希望通过向名称服务器发起查询来动态解析名称。通常有多个名称服务器（公有的或私有的）可以满足此类查询，而决定查询哪些服务器是/etc/resolv.conf 及其他配置选项的工作。

**/etc/resolv.conf** is the configuration file for the system resolver which is the entity that communciates with DNS name servers on your machine’s behalf. If this file does not exist, queries will default to the name server on your local machine. This file consists of one `domain` or `search` lines up to three `nameserver` lines and any number of options. Let’s dive into the details behind these configuration options.  
/etc/resolv.conf 是系统解析器的配置文件，该解析器代表你的机器与 DNS 名称服务器进行通信。如果此文件不存在，查询将默认使用你本地机器上的名称服务器。该文件包含最多三条 `nameserver` 行和任意数量的选项。让我们深入了解这些配置选项的细节。

*   `domain` Using this option will specific a local domain name. Short queries, which are queries that don’t contain any domain identifiers, then have the local domain appended to them during DNS queries.  
    `domain` 使用此选项将指定本地域名。短查询（即不包含任何域名标识符的查询）在 DNS 查询过程中将自动追加本地域名。
    
    To understand this better take `death` as one of the machines within the OCF domain, `ocf.berkeley.edu`. One can issue a DNS query for death by typing `dig death.ocf.berkeley.edu` but that’s an awful lot to type. By specifying `domain ocf.berkeley.edu` in **/etc/resolv.conf** the query can be shortened to just `dig death`. In fact, any tool that takes a domain name can now use this shortened version, i.e. `ping death`. This is because your machine’s resolver is responsible for translating this domain name, and the `domain` configuration automatically appends the written domain to these short queries.  
    为了更好地理解这一点，将 `death` 视为 OCF 域中的一个机器 `ocf.berkeley.edu` 。可以通过键入 `dig death.ocf.berkeley.edu` 发起一个 DNS 查询来查询 death，但这输入起来太麻烦了。通过在 /etc/resolv.conf 中指定 `domain ocf.berkeley.edu` ，查询可以缩短为只需 `dig death` 。事实上，任何接收域名作为输入的工具现在都可以使用这个缩短版本，即 `ping death` 。这是因为你的机器的解析器负责将这个域名进行转换，而 `domain` 配置会自动将这些简短查询中添加上所写的域名。
    
*   `search` The format for this option is `search <search-list>`. Using this options specifies a list of domain names to iterate through when attempting to look up queries.  
    `search` 此选项的格式为 `search <search-list>` 。使用此选项指定在尝试查询时需要遍历的域名列表。
    
    Let’s examine an example use case, imagine we owned two networks `ocf.berkeley.edu` and `xcf.berkeley.edu` and wanted to query a machine which may either be in either network. To enable this we can simply add the line `search ocf.berkeley.edu xcf.berkeley.edu`. Queries to resolve a domain name will now append those listed domains in order until a successful DNS response. If we assume `death` is on `ocf.berkeley.edu` and another machine, `life`, is on `xcf.berkeley.edu`, both `dig death` and `dig life` are now resolved properly thanks to our configuration.  
    让我们来看一个示例用例，假设我们拥有两个网络 `ocf.berkeley.edu` 和 `xcf.berkeley.edu` ，并且想要查询一台可能位于这两个网络中的机器。要实现这一点，我们只需添加行 `search ocf.berkeley.edu xcf.berkeley.edu` 。现在，解析域名查询时会按顺序附加所列域名，直到获得成功的 DNS 响应。如果我们假设 `death` 位于 `ocf.berkeley.edu` 上，而另一台机器 `life` 位于 `xcf.berkeley.edu` 上，那么由于我们的配置， `dig death` 和 `dig life` 都可以正确解析。
    

One thing to note is that **`search` and `domain` are mutually exclusive keywords** and having both defined causes the last instance to take precedence and override earlier entries.  
需要注意的是， `search` 和 `domain` 是互斥的关键字，同时定义两者会导致最后定义的实例优先级更高，并覆盖之前的条目。

*   `nameserver` The `nameserver` keyword is fairly self explanatory and follows the format of `nameserver <ip-address>` where `<ip-address>` is the IP address of the intended name server. One can have up to `MAXNS` (default 3) `nameserver` entries in this file. The resolver will query nameservers in the same order as they are written in the file.  
    `nameserver` `nameserver` 关键字的意思比较明确，其格式为 `nameserver <ip-address>` ，其中 `<ip-address>` 是目标名称服务器的 IP 地址。此文件中最多可以有 `MAXNS` （默认为 3） `nameserver` 条目。解析器会按照文件中写入的顺序查询名称服务器。

Following are additional useful configurable options in this file. Options are defined in this format `options <option1> [additional-options]`. Some example options follow below:  
以下是此文件中一些其他有用的可配置选项。选项定义的格式为 `options <option1> [additional-options]` 。以下是一些示例选项：

*   `ndots` This option, formatted as `ndots:n`, configures the threshold,`n`, at which an initial absolute query is made. Since the default value for this option is 1, any name with at least 1 dot will first be queried as an absolute name before appending domains from `search`. When less than `ndots` are present, the queries automatically begin appending elements in `<search-list>`.  
    `ndots` 这个选项，格式为 `ndots:n` ，配置了初始绝对查询的阈值 `n` 。由于该选项的默认值为 1，任何至少包含 1 个点的名称将首先被作为绝对名称查询，然后再追加来自 `search` 的域。当少于 `ndots` 时，查询将自动开始追加 `<search-list>` 中的元素。
    
    Take `death.ocf.berkeley.edu` as an example, and let’s assume we have the following line `search ocf.berkeley.edu` in our configuration. Running `ping death` works because there are 0 dots in `death` and the query automatically appends `search` elements so that our query becomes `death.ocf.berkeley.edu`. If we instead ran `ping death.` the resolver will first issue a query for `death.` since it has 1 dot, which fails.  
    以 `death.ocf.berkeley.edu` 为例，假设我们在配置中有以下行 `search ocf.berkeley.edu` 。运行 `ping death` 是有效的，因为 `death` 中有 0 个点，查询会自动追加 `search` 元素，使我们的查询变为 `death.ocf.berkeley.edu` 。如果我们运行 `ping death.` ，解析器将首先为 `death.` 发起查询，因为它有 1 个点，这将失败。
    
*   `timeout` This opton is in the format `timeout:n` and configures the amount of time `n`, in seconds, that a resolver will wait for a response from a name server before retrying the query via another name server.  
    `timeout` 这个选项格式为 `timeout:n` ，配置了解析器在通过另一个名称服务器重试查询之前，将等待名称服务器响应的秒数 `n` 。
    
*   `attempts` This option is in the format `attempts:n` and configures the number of attempts `n` that the resolver will make to the entire list of name servers in this file.  
    `attempts` 这个选项格式为 `attempts:n` ，配置了解析器将针对此文件中的所有名称服务器列表进行的尝试次数 `n` 。
    

### [](#etcnsswitchconf)/etc/nsswitch.conf

With multiple sources of information for resolving hostnames, one can’t help but wonder how the system decides which sources to query and in what order. This is answered with the **/etc/nsswitch.conf** file. It is this file’s responsibility to list sources of information and configure prioritization between sources. Similar information sources can be grouped into categories that are referred to as ‘databases’ within the context of the file. The format of the file is as follows: `database [sources]`. While this file provides configuration for a wide array of name-service databases, we will focus on an example relevant to the topic at hand.  
由于存在多个解析主机名的信息源，人们不禁会想系统如何决定查询哪些信息源以及查询顺序。这由 /etc/nsswitch.conf 文件来解答。该文件负责列出信息源并配置信息源之间的优先级。在文件上下文中，相似的信息源可以分组为“数据库”。该文件的格式如下： `database [sources]` 。虽然该文件为多种名称服务数据库提供配置，但我们将关注一个与主题相关的示例。

The `hosts` database configures the behavior of system name resolution. So far we have introduced two ways to resolve names:  
`hosts` 数据库配置了系统名称解析的行为。到目前为止，我们已经介绍了两种解析名称的方法：

1.  Using entries in **/etc/hosts**  
    使用 /etc/hosts 中的条目
2.  Using a resolver to issue DNS queries to DNS name servers  
    使用解析器向 DNS 域名服务器发起 DNS 查询

To let the system know about the above two sources of information there are corresponding keywords, `files` and `dns`, respectively.  
为了让系统知道上述两种信息来源，分别有对应的键值， `files` 和 `dns` 。

We can then configure name resolution by writing the line `hosts: files dns` The example syntax above tells the system to first prioritize files before issuing DNS queries. Naturally, this can be customized to best fit your use case.  
然后我们可以通过写入行 `hosts: files dns` 来配置名称解析。上述示例语法告诉系统首先优先使用文件再发起 DNS 查询。当然，这可以根据你的使用情况自定义以最佳适应。

[](#dhcp-client-configuration)DHCP client configuration   DHCP 客户端配置
--------------------------------------------------------------------

The Internet Systems Consortium DHCP client, known as _dhclient_, ships with Debian and can be configured via **/etc/dhcp/dhclient.conf**. Lines in this file are terminated with a semicolon unless contained within brackets, like in the C programming language. Some potentially interesting parameters to configure include:  
互联网系统协会 DHCP 客户端，即 dhclient，随 Debian 一起提供，可以通过 /etc/dhcp/dhclient.conf 进行配置。该文件中的行以分号结束，除非它们位于括号内，类似于 C 语言。一些可能有趣的配置参数包括：

### [](#timing)Timing   时间

*   `timeout` This format for this statement is `timeout <time>` and defines time to the maximum amount of time, in seconds, that a client will wait for a response from a DHCP server.  
    `timeout` 此语句的格式为 `timeout <time>` ，并定义了客户端等待 DHCP 服务器响应的最大时间（以秒为单位）。
    
    Once a timeout has occured the client will look for static leases defined in the configuration file, or unexpired leases in **/var/lib/dhclient/dhclient.leases**. The client will loop through these leases and if it finds one that appears to be valid, it will use that lease’s address. If there are no valid static leases or unexpired leases in the lease database, the client will restart the protocol after the defined `retry` interval.  
    一旦发生超时，客户端将查找配置文件中定义的静态租约，或在 /var/lib/dhclient/dhclient.leases 中查找未过期的租约。客户端将遍历这些租约，如果找到一个看似有效的租约，它将使用该租约的地址。如果租约数据库中没有有效的静态租约或未过期的租约，客户端将在定义的 `retry` 间隔后重新启动协议。
    
*   `retry` The format for this statement is `retry <time>` and configures the amount of time, in seconds, that a client must wait after a timeout before attempting to contact a DHCP server again.  
    `retry` 此语句的格式为 `retry <time>` ，用于配置客户端在超时后必须等待的秒数，才能再次尝试联系 DHCP 服务器。
    

A client with multiple network interfaces may require different behaviour depending on the interface being configured. Timing parameters and certain declarations can be enclosed in an interface declaration, and those parameters will then be used only for the interface that matches the specified name.  
一个具有多个网络接口的客户机可能需要根据正在配置的接口表现出不同的行为。定时参数和某些声明可以包含在接口声明中，然后这些参数将仅用于与指定名称匹配的接口。

The syntax for an example interface snippet is:  
一个示例接口片段的语法是：

```
interface <iface-name> {
    send host-name "death.ocf.berkeley.edu";
    request subnet-mask, broadcast-address, time-offset, routers,
       domain-search, domain-name, domain-name-servers, host-name;
    [additional-declarations];
} 
```

As mentioned above this file also supports defining static leases via a `lease` declaration. Defining such leases may be useful as a fallback in the event that a DHCP server cannot be contacted.  
如上所述，此文件还支持通过 `lease` 声明定义静态租约。在 DHCP 服务器无法联系的情况下，定义此类租约可能作为备用方案有用。

The syntax for a example static lease is:  
一个静态租约的语法示例为：

```
lease {
#  interface "eth0";
#  fixed-address 192.33.137.200;
#  option host-name "death.ocf.berkeley.edu";
#  option subnet-mask 255.255.255.0;
#  option broadcast-address 192.33.137.255;
#  option routers 192.33.137.250;
#  option domain-name-servers 127.0.0.1;
#  renew 2 2000/1/12 00:00:01;
#  rebind 2 2000/1/12 00:00:01;
#  expire 2 2000/1/12 00:00:01;
#} 
```

While the function of most keywords in the above snippet can be inferred from their syntax, more information can be found by simply reading the `man` page for this file (`man dhclient.conf`).  
虽然上述代码片段中大多数关键字的功能可以通过其语法推断出来，但通过简单地阅读该文件的 `man` 页面（ `man dhclient.conf` ），可以获取更多信息。

[](#sysadmin-commands)Sysadmin commands   系统管理员命令
-------------------------------------------------

### [](#ifupdown)ifupdown

`ifupdowm` is a simple suite of commands for interacting with network interfaces. The two commands you’ll be using most are `ifup` and `ifdown` which are relatively self-explanatory. `ifup` brings and interface up and vice versa for `ifdown`. These two commands should be your de facto commands for bringing interfaces up or down since using these commands loads configurations defined in **/etc/network/interfaces**.  
`ifupdowm` 是一套用于交互网络接口的简单命令。你最常使用的两个命令是 `ifup` 和 `ifdown` ，它们相对容易理解。 `ifup` 用于启动接口，而 `ifdown` 则相反。这两个命令应该是你实际用于启动或关闭接口的默认命令，因为使用这些命令会加载 /etc/network/interfaces 中定义的配置。

### [](#mtr)mtr

`mtr` is a command that combines the functionality of `traceroute` with that of `ping`. Take a look at [this article](https://linode.com/docs/networking/diagnostics/diagnosing-network-issues-with-mtr/) for a good primer for using `mtr` and interpreting its output.  
`mtr` 是一个结合了 `traceroute` 和 `ping` 功能的命令。查看这篇文章，了解如何使用 `mtr` 及其输出结果的解读。

### [](#iptables)iptables

In favor of not reinventing the wheel please check out these excellent and pretty short articles by **DigitalOcean**, who sponsored this semester’s offering of the decal by supplying us with VMs.  
为了不重复造轮子，请查看由 DigitalOcean 赞助的这学期 decal 提供的优秀且简短的文章，他们通过提供虚拟机来支持这项活动。

1.  [An Introduction to iptables  
    iptables 简介](https://www.digitalocean.com/community/tutorials/how-the-iptables-firewall-works)
2.  [Adding rules  添加规则](https://www.digitalocean.com/community/tutorials/how-to-set-up-a-firewall-using-iptables-on-ubuntu-14-04)
3.  [Deleting rules  删除规则](https://www.digitalocean.com/community/tutorials/how-to-list-and-delete-iptables-firewall-rules)
4.  [Common rules and tips  
    常见规则和技巧](https://www.digitalocean.com/community/tutorials/iptables-essentials-common-firewall-rules-and-commands)

### [](#checkpoint-2)Checkpoint   检查点

**Question 3a:** If we preferred name resolution be done dynamically rather than using static entries in **/etc/hosts** what file do we need to edit and what is the line we should add?  
问题 3a：如果我们希望动态解析名称而不是使用 /etc/hosts 中的静态条目，我们需要编辑哪个文件以及我们应该添加哪一行？

**Question 3b:** Assume the following information:  
问题 3b：假设以下信息：

*   **/etc/resolv.conf** file has 3 `nameserver` entries and a `options timeout:1` entry.  
    /etc/resolv.conf 文件有 3 个 `nameserver` 条目和一个 `options timeout:1` 条目。
*   A successful DNS response takes 20 ms.  
    一个成功的 DNS 响应需要 20 毫秒。
    
    You need to add the `attempts:n` option so that you retry a query as many times as possible but the total time to resolve a name, irrelevant of success or failure, takes less than **5** seconds. What should the value of n be?  
    你需要添加 `attempts:n` 选项，以便尽可能多次重试查询，但解析名称的总时间（无论成功与否）应少于 5 秒。n 的值应该是多少？
    

[](#exercises)Exercises   练习
============================

Now, let’s ~break things~ do some experimentation! Remember to submit your answers to Gradescope when you’re done. **(Also, don’t forget to submit the checkpoint questions from the sections above!)**  
现在，让我们来分解问题并进行一些实验！完成后记得将答案提交到 Gradescope。（另外，别忘了提交上面各部分中的检查点问题！）

The files for these exercises can be found in the [decal-labs](https://github.com/0xcf/decal-labs) repository. Clone it now: `git clone https://github.com/0xcf/decal-labs`  
这些练习的文件可以在 decal-labs 仓库中找到。现在克隆它： `git clone https://github.com/0xcf/decal-labs`

[](#-this-is-fine-)🔥 This is fine 🔥   🔥 这很好 🔥
-------------------------------------------------

This section will have you thinking like a sysadmin.  
这一部分将让你像系统管理员一样思考。

### [](#important-note)IMPORTANT NOTE   重要提示

**Do not run the scripts directly in your student VM!** These scripts are **dangerous** and will brick your VM so **please follow the provided setup instructions.** However, if you have physical access (or out-of-band management access) to a Linux machine, feel free to run the scripts directly and reboot when necessary, as all changes made are temporary.  
不要直接在您的学生虚拟机中运行脚本！这些脚本很危险，会损坏您的虚拟机，所以请遵循提供的设置说明。但是，如果您有对 Linux 机器的物理访问权限（或带外管理访问权限），可以自由直接运行脚本并在需要时重启，因为所有更改都是临时的。

Each script might make changes to your network stack with the intent of damaging your machine’s connectivity. To confine the scope of the ‘attacks’, scripts will specifically try to alter your connectivity to `google.com` and `ocf.berkeley.edu`.  
每个脚本可能会更改您的网络栈，意图是损害您机器的连接性。为了限制“攻击”的范围，脚本将尝试专门更改您与 `google.com` 和 `ocf.berkeley.edu` 的连接。

### [](#setup)Setup   设置

1.  If you haven’t already, ssh into your student VM (`username@username.decal.xcf.sh`) and clone [decal-labs](https://github.com/0xcf/decal-labs).  
    如果您还没有，请通过 ssh 进入您的学生虚拟机（ `username@username.decal.xcf.sh` ）并克隆 decal-labs。
2.  Go into the vm directory: `cd decal-labs/a6/vm`  
    进入虚拟机目录： `cd decal-labs/a6/vm`
3.  Get Virtualbox: `sudo apt install virtualbox`  获取 Virtualbox： `sudo apt install virtualbox`
4.  Get Vagrant: `curl -O https://releases.hashicorp.com/vagrant/2.2.18/vagrant_2.2.18_x86_64.deb` then `sudo apt install ./vagrant_2.2.18_x86_64.deb`  
    获取 Vagrant： `curl -O https://releases.hashicorp.com/vagrant/2.2.18/vagrant_2.2.18_x86_64.deb` 然后获取 `sudo apt install ./vagrant_2.2.18_x86_64.deb`
5.  Get Ansible: `sudo apt install ansible`  获取 Ansible： `sudo apt install ansible`
6.  Start a Vagrant instance: `vagrant up`  
    启动一个 Vagrant 实例： `vagrant up`
7.  Enter your Vagrant instance: `ssh vagrant@192.168.42.42`. The default password is `vagrant`.  
    进入你的 Vagrant 实例： `ssh vagrant@192.168.42.42` 。默认密码是 `vagrant` 。
8.  The `decal-labs` repo should be available in the Vagrant instance. If it isn’t there, you can `sudo apt install git` then clone it again.  
    Vagrant 实例中应该有 `decal-labs` 仓库。如果没有，你可以 `sudo apt install git` 然后重新克隆它。
9.  `cd decal-labs/a6/scenario`.

### [](#problem-instructions)Problem Instructions   问题说明

There should be 6 scripts, named `1.py` to `6.py`. Your task is to **choose at least 3 of these to run and attempt to fix the problem that they cause (if any).**  
应该有 6 个脚本，命名为 `1.py` 到 `6.py` 。你的任务是选择其中至少 3 个来运行，并尝试修复它们造成的问题（如果有）。

Launch each script with sudo, i.e. `sudo python3 <script.py>`.  
以 sudo 启动每个脚本，即 `sudo python3 <script.py>` 。

For each script, follow this two step process. **Only move onto another script once you have finished resolving your current one.**  
每个脚本都遵循以下两步流程。只有完成当前脚本的解决后，才能继续下一个脚本。

1.  Analyze whether or not your connectivity has been damaged. If your stack has been damaged identify the issue or which part of your network is no longer functioning as intended.  
    分析你的连接是否受损。如果你的堆栈受损，请识别问题或哪个网络部分不再按预期工作。
    
2.  If you concluded there was a problem, resolve the issue. What commands did you use and how did you conclude things were fully functional again?  
    如果你确定存在问题，请解决该问题。你使用了哪些命令，以及如何得出结论一切已完全恢复正常？
    
    **Additionally, for each step you must explain the tools you used and how you came to your conclusions i.e.  
    此外，对于每一步，你必须解释你使用的工具以及你是如何得出结论的，即**
    
    > I ran `example --pls --fix computer` and I noticed that line 3: `computer-is-broken` meant my machine was f*****.  
    > 我运行了 `example --pls --fix computer` ，并注意到第 3 行： `computer-is-broken` 意味着我的机器出问题了。
    
    > This script damaged my ability to connect to google.com by poisoning my arp cache with bogus entries.  
    > 这个脚本通过用虚假条目毒害我的 ARP 缓存，损坏了我连接到 google.com 的能力。
    

### [](#debugging-bank)Debugging Bank   调试银行

**I can’t ssh into my Vagrant instance!  
我无法 ssh 连接到我的 Vagrant 实例！**

*   You might need to run `vagrant provision` and then re-run `vagrant up`.  
    你可能需要运行 `vagrant provision` ，然后重新运行 `vagrant up` 。
*   Also, make sure you’re running `ssh 192.168.42.42` and not `vagrant ssh`.  
    另外，确保你运行的是 `ssh 192.168.42.42` 而不是 `vagrant ssh` 。

**I accidentally bricked something, how do I reset my Vagrant instance?  
我不小心搞坏了某个东西，如何重置我的 Vagrant 实例？**

*   Run `vagrant destroy` and then `vagrant up` in the `vm` folder.  
    在 `vm` 文件夹中运行 `vagrant destroy` ，然后运行 `vagrant up` 。

**I’ve tried running `vagrant provision` a bunch of times and it never seems to work :(  
我试了很多次运行 `vagrant provision` ，但似乎总是不行：(**

*   As an alternative to installing Vagrant on your student VM, you can also try [installing it on your local machine](https://www.vagrantup.com/downloads).  
    在学生虚拟机中安装 Vagrant 的替代方案是，你也可以尝试在你的本地计算机上安装它。

**I accidentally ran the scripts in my student VM and not in the Vagrant instance and now I can’t log in. What do I do?  
我不小心在我的学生虚拟机中运行了脚本，而不是在 Vagrant 实例中，现在我无法登录。我该怎么办？**

*   You’ll need to ask us to reset your VM. Contact us in #decal-general or over email to arrange this.  
    你需要让我们重置你的虚拟机。请在#decal-general 频道或通过电子邮件联系我们安排此事。

[](#net-ninjas-optional)Net Ninjas (Optional)   网络忍者（可选）
--------------------------------------------------------

**Run this in your Vagrant instance. The file should be found in `decal-labs/a6/`.  
在你的 Vagrant 实例中运行这个。文件应该可以在 `decal-labs/a6/` 找到。**

1.  The ninja has spent a few years training in a dojo and has mastered fireball (`火球`) jutsu. He can use his new skills to tamper with your network stack, incinerating your attempts to catch him. Run `sudo python3 advanced_ninja_port.py`. Fix the damage he has done and then successfully send him a `found you` message!  
    忍者花了几年在道场训练，掌握了火球术（ `火球` ）。他可以用新学的技能干扰你的网络栈，焚毁你抓捕他的企图。运行 `sudo python3 advanced_ninja_port.py` 。修复他造成的损害，然后成功给他发送一条 `found you` 信息！

* * *

[![](/assets/images/digitalocean.png)](https://www.digitalocean.com) With great appreciation to [DigitalOcean](https://www.digitalocean.com) for sponsoring the VMs used in both tracks of the DeCal

 [![](/assets/images/linode.png)](https://www.linode.com) Huge thanks to [Linode](https://www.linode.com) for sponsoring the equipment used to record digital lectures for the Decal

[![Hosted by the OCF](https://www.ocf.berkeley.edu/hosting-logos/ocf-hosted-penguin.svg)](https://www.ocf.berkeley.edu) Copyright © 2017-2021 [ Open Computing Facility ](https://www.ocf.berkeley.edu) and [eXperimental Computing Facility](https://xcf.berkeley.edu) 

This website and its course materials are licensed under the terms of the [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) License. [Source Code](https://github.com/0xcf/decal-web/) available on GitHub