> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [www.bilibili.com](https://www.bilibili.com/video/BV1EU4y1v7ju/?spm_id_from=333.337.search-card.all.click&vd_source=1a5d59fdde582303f3cfd34924e38898)

> 开放式系统互联通信参考模型（英语：Open System Interconnection Reference Model，缩写为 OSI），简称为 OSI 模型（OSI model），一种概念模型，由国际标准化组织提出。

![](http://i1.hdslb.com/bfs/face/4e61453562c7f8e1b7ce967f9c28752c1e64d846.jpg@96w_96h_1c_1s_!web-avatar.avif) 小塔 - Zrea关注2023-01-18 10:0546 粉丝

OSI 网络参考模型

![](http://i1.hdslb.com/bfs/note/997758893fd1a7d53823c6f980d656f1f0e74424.jpg@640w_!web-note.webp)

仅作为参考，也就是说 OSI 网络实际中并不使用

我们只是把 OSI 网络模型作为参考，在网络出现问题的时候，可以从一个宏观的整体去分析和解决问题

而且搭建网络的时候也并不一定需要划分为 7 层

![](http://i1.hdslb.com/bfs/note/9e30ce14a3c649e1d9169baefa20c2575dbf3a06.jpg@640w_!web-note.webp)

但是当今互联网广泛使用的是 TCP/IP 网络模型

他原本是四层，实际上划分为五层更符合实

﻿01:35﻿

OSI 模型的目的：解决主机之间的网络通讯

例子：

华为电脑用浏览器访问苹果电脑搭建的网站

两个应用具体需要如何进行交互就是应用层的事情了

应用层并不是说你需要使用什么应用程序

而是应用如何沟通

常用的应用层协议 HTTP 协议

![](http://i1.hdslb.com/bfs/note/84095f166ed6c247b6fc63ccd8dff39fd1316469.jpg@640w_!web-note.webp)

使得应用之间可以实现沟通

应用层就是最接近用户的那一层

但是应用层只不过是逻辑上把两个两个应用连通

实际物理上的连通是需要物理层的

我们要发送出去的数据在计算机里只不过是无数的 0 和 1

0 或者 1 就叫做比特

物理层就要把这些比特用不同的媒介传输出去

![](http://i1.hdslb.com/bfs/note/c447e6a3db80e6ae52740a8a629df6af6ce96e2b.jpg@640w_!web-note.webp)

可以用电，用光或者其他形式的电磁波来表示和传输信号

数据从网络接口出去以后会经过不同的网络拓扑

![](http://i1.hdslb.com/bfs/note/c67448acebcdf101387ab42ef53c24a4f62698cb.jpg@640w_!web-note.webp)

并不是一条线走到底

因此需要中继器和集线器这样的设备

![](http://i1.hdslb.com/bfs/note/2948d3feaa72d3fbcef02142c6dd0e1e9cc7c3aa.jpg@640w_!web-note.webp)

但还是不够

信号要去到哪台设备是需要定向的

﻿03:08﻿

因此需要高级一点的网络模型

在数据链路层这里

比特会被封装成帧

![](http://i1.hdslb.com/bfs/note/4156daa3c111d8074f62a905631a37072a4cd949.jpg@640w_!web-note.webp)

帧就是这一层表示数据的特殊名字而已

在封装的时候会加上 MAC 地址

也就是传说中的物理地址

网卡出厂的时候就有着全球唯一的 MAC 地址

![](http://i1.hdslb.com/bfs/note/a6817f9574b5163bc7dc6bc2e62ab98009fb511a.jpg@640w_!web-note.webp)

为了可以通过 MAC 地址对不同设备进行数据的传输

就出现了交换机

![](http://i1.hdslb.com/bfs/note/2e9533b05e7005221660d117ad977d8e73a694f5.jpg@640w_!web-note.webp)

这里说的是二层交换机

![](http://i1.hdslb.com/bfs/note/3a4403747ef31418ae02b81a4b7d59f52059305f.jpg@640w_!web-note.webp)

比方说这里有一台交换机

连接多台主机

发送端发送数据的时候

交换机就知道了发送端的 MAC 地址

如果此时交换机也知道接收端的 MAC 地址

就可以把数据直接发送过去了

物理地址就是这样一跳一跳地进行传递

![](http://i1.hdslb.com/bfs/note/904844122f5de61b3eb5443e154ff1624705a197.jpg@640w_!web-note.webp)

另外因为物理层在传输 0 和 1 的时候

![](http://i1.hdslb.com/bfs/note/69507817c90c491ff969dd68d27d012b2df4f3dc.jpg@640w_!web-note.webp)

可能会 0 变成 1，或者 1 变成 0

会进行差错检测

以及一定的差错纠正

另外设备之间的传输能力以及接受能力也是个问题

很可能这边 “喷水” 式传输，另一边 “夹缝式” 接受

因此需要流控制来避免这种不对称

我们知道互联网是一张大网

如果用 MAC 物理地址来作为唯一的寻址方法是不科学的

﻿04:55﻿

![](http://i1.hdslb.com/bfs/note/164debd50b618092730df0629cadba453d40b387.jpg@640w_!web-note.webp)

比如我和你买了同一个厂的网卡

我和你你的网卡差别只有一个字母

但是我和你距离十万八千里

物理地址此时就很难做出快速定位

相当于我有你的名字

但不知道你住哪里，找不到你

因此需要 IP 地址来进行寻址和路由选择

![](http://i1.hdslb.com/bfs/note/7295b22c43e4c5e9ce8068fcfe501f985eb1d823.jpg@640w_!web-note.webp)

IP 这样的逻辑地址就是实现端到端的基础了

而不是物理地址那样的跳到跳传输

说到路由选择

也就是说路由器也是网络层的核心

包就是网络层里数据的名字

在封装为二层的帧之前就是包

路由器根据包里 IP 地址进行路由转发

![](http://i1.hdslb.com/bfs/note/d0e22e3cc534b3b1dd3241f1058fdf91467bf964.jpg@640w_!web-note.webp)

地址管理和路由选择就是这一层的核心

虽然有 MAC 和 IP 地址可以抵达对方主机

但是对方主机可能运行着无数多个软件进程

假设我用谷歌和火狐浏览器同时登录网站

如何让数据去到指定的软件服务上

就需要用到端口号作为地址来定位了

比如客户端这里生成不同的端口号

![](http://i1.hdslb.com/bfs/note/27b432945a0f3b71c207c49290cea87be9507ab4.jpg@640w_!web-note.webp)

即时同时访问 HTTP 端口 80 也是没问题的

根据不同的源端口号来作出响应就可以了

所以传输层（运输层）在网络层的端到端基础上

实现了服务进程到服务进程的传输

段就是传输层里数据的名字

![](http://i1.hdslb.com/bfs/note/53056a49c96b50c7fe1cfd29dd42d988475ed314.jpg@640w_!web-note.webp)

在封装为三层包之前就是段

等会名字会总结，不用害怕

传输层管理两个节点之间数据的传输

负责可靠传输和不可靠传输

![](http://i1.hdslb.com/bfs/note/c0401bd3e4bb0cf048a626d83852eb4e30045b03.jpg@640w_!web-note.webp)

既 TCP UDP

另外还有一个新的叫 QUIC

其中 TCP 允许应用把字节流变成多份段

![](http://i1.hdslb.com/bfs/note/c72c162a979735fc59e149be2701d78569404d0e.jpg@640w_!web-note.webp)

而不是整个字节数据完整地发送出去

![](http://i1.hdslb.com/bfs/note/720ffbb7873a38d1d5e809d304224d72b209d538.jpg@640w_!web-note.webp)

传输层还有流量控制来确保传输速度

再加上错误控制来进行数据完整的接收

**接下来会话层也比较好理解**

﻿06:30﻿

比方说你现在登录了某个网站

网站服务可以保持你的登录状态

不用每次都输入账号和密码

当然网站服务会管理和控制登录状态

另外会话层还负责同步服务

比方说你上次看到电影高潮的时刻突然停电了

再次登录账号的时候就可以自动同步岛上次看到的时间段

不同计算机内部的各自表达方式可能不太相同

﻿06:59﻿

表示层就来负责这样的转换

也就是编码和解码

数据往往还需要进行加密

比方说 HTTPS(SSL/ TLS) 就会对我们的数据进行加密和解密

另外我们可能还需要给文件瘦身

压缩也是这一层负责的

应用层，表示层和会话层的数据统称为应用数据或者应用负载也可以叫上层数据

同时也是教科书上说的报文

数据在各层的名字分别是

![](http://i1.hdslb.com/bfs/note/b20dac7e59dd966ac3fe914cf0cd6eebf8e4e984.jpg@640w_!web-note.webp)

报文，段，包，帧和比特

就地址来说

![](http://i1.hdslb.com/bfs/note/f96136be794c3be8c896a324651bae141f6f44ca.jpg@640w_!web-note.webp)

有端口号，IP 逻辑地址和 MAC 物理地址

就传输功能来讲

![](http://i1.hdslb.com/bfs/note/1d4a869dd6deee7c309a8461e7579c9895011ede.jpg@640w_!web-note.webp)

有服务进程到服务进程，端到端，跳到跳

了解了各层的作用

现在就可以把全部关联起来

客户端要发送数据，也就是报文

报文来到传输层加上端口号，封装成段

段来到了网络层，加上 IP 地址，封住成包

注意这里的包是含有目标 IP 地址的

毕竟你要知道数据要发送到什么地方

![](http://i1.hdslb.com/bfs/note/ebf9f9f862a01f6d34f83d1f54226ed84d1f95a4.jpg@640w_!web-note.webp)

但因为目标 IP 地址不是同一个网络下的

要发送到其他的网络就需要经过默认网关

现在就出现了一个问题

客户端主机最初并不知道默认网关的 MAC 地址

没有办法封装成帧

这个时候就可以用 ARP 协议进行广播

找到网关 IP 对应的 MAC 地址把包封装成帧

![](http://i1.hdslb.com/bfs/note/e30d78b1a13a8eeedfc6316edd2012de376cd8d6.jpg@640w_!web-note.webp)

源 MAC 地址填自己的

目标 MAC 地址填广播地址

假设当前网络有个二层交换机

这个交换机只需要记录下不同的接口对应的 MAC 地址就好了

交换机收到广播后就帮忙发送出去 “人手一份”

所以默认网关收到消息后查看了帧

发现了发送端的 MAC 地址

再解封发现包里面的 IP 地址

就会把客户端 MAC 地址和 IP 地址关联为一台主机

同时默认网关会把自己的 IP 地址放入包里

再结合自己的 MAC 地址封装成帧

默认网关就这样做出响应，这样原路返回

发送端就知道默认网关的 MAC 地址了

现在就可以封装成帧，并且发送数据

![](http://i1.hdslb.com/bfs/note/ed77c4a2bc2ba53a01335363736dbfbc620f16e5.jpg@640w_!web-note.webp)

比特流到了默认网关的时候

解封为帧发现是送给自己的

那解封为包查看到目标 IP 地址是在另一网络中的

就会进行路由转发，最终到达了目的网络

如果目标的网关知道目标 IP 地址和 MAC 地址是哪台主机

封装成帧就可以直接发送过去了

![](http://i1.hdslb.com/bfs/note/a844c6887c580e6326f145cb09331a440177a1ab.jpg@640w_!web-note.webp)

如果不知道也还是可以用 ARP 喊下街就好

目标主机收到包确认是自己的 IP 地址以后

解封查看段可以发现源和目标端口号

用目标端口号给到指定的应用程序

应用程序处理好以后

就可以按照源的信息做出响应

回去的原理也是一样的

现在你对 OSI 模型就应该有更具体的概念了
