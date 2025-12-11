> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [chat.zchat.tech](https://chat.zchat.tech/c/693923ac-d204-8333-a885-754b94dd43ea)

> ChatGPT 是一款供日常使用的 AI 聊天机器人。与最先进的 AI 模型互动，探索创意、解决问题以及提升学习效率。

这节课其实就讲三件事：  
**1）什么是虚拟机，2）什么是容器 / Docker，3）一堆机器一起工作时（分布式系统）要怎么设计。**

Advanced 10 - Virtualization, C…

我用生活比喻一点点讲，你当成故事看就行。

一、为什么要搞这些东西？
------------

之前的课说过：

*   当你有**很多服务器**的时候，「有机器随时坏掉」是常态，不是意外。
    
    Advanced 10 - Virtualization, C…
    
*   配置管理可以帮你**自动把一堆新机器装好环境**，省得一台台手动搞。
    

但还有个更难的问题：

> 某台服务器突然挂了，**用户几乎感觉不到**，这是怎么做到的？

比如 Google / Facebook / Netflix：

*   后面有成千上万台服务器
    
*   总有机器在坏
    
*   但你刷网页、刷视频基本不会突然打不开
    

**要做到这种「无缝切换」，就需要：容器 + 容器编排 + 分布式架构。**

Advanced 10 - Virtualization, C…

二、虚拟机：在电脑里再装一台电脑
----------------

先讲老办法——**虚拟机（Virtual Machine，VM）**。

Advanced 10 - Virtualization, C…

可以想象成：

> 在一台真实电脑里，用软件「假装」出几台小电脑。

每个虚拟机里面有：

*   自己的**虚拟硬件**（假 CPU、假网卡、假硬盘）
    
*   自己的**操作系统**（Guest OS）
    
*   自己的应用程序
    

好处：

*   **隔离好**：一个虚拟机里的病毒，理论上伤不到别的虚拟机和宿主机。
    

坏处：

*   每个虚拟机都要**整套操作系统** → 很占资源
    
*   启动像开一台新电脑 → **很慢**
    

（PPT 第 4 页右边那张图就是：一个 hypervisor 底下跑多个虚拟机，每个都有自己的 Guest OS。

Advanced 10 - Virtualization, C…

）

三、进程：操作系统里的「正在跑的程序」
-------------------

再讲一个更小的单位——**进程（process）**。

Advanced 10 - Virtualization, C…

简单说：

> 进程 = 正在运行的一个程序 + 它自己的那块内存 + 它开的文件等。

特点：

*   每个进程有自己的**虚拟地址空间**（自己的一片内存）
    
*   有程序代码、数据、栈、寄存器等
    
*   可以有多个线程一起跑（共享内存，但执行位置不同）
    

（PPT 第 5 页的图就画了好几个进程，各有自己的 code / data / files。

Advanced 10 - Virtualization, C…

）

四、容器：介于「虚拟机」和「进程」之间
-------------------

现在登场主角：**容器（container）**。

Advanced 10 - Virtualization, C…

可以这样理解：

> 虚拟机：每个房客都自己盖一栋房子（自己的 OS）  
> 容器：大家住同一栋大楼（同一个 OS），但**每人有独立房间 + 独立家具（文件 / 库）**

容器的特点（看 PPT 第 6–8 页的对比图）：

Advanced 10 - Virtualization, C…

1.  **共享 Host OS**
    
    *   它们不再各自装一套操作系统
        
    *   只在同一个 OS 上，给每个容器分开看见的「文件、库、进程环境」
        
2.  **资源隔离**
    
    *   一个容器里的程序看不到别的容器里的文件 / 进程
        
    *   就像进程 + 自己的「迷你文件系统」
        
3.  **比虚拟机轻量很多**
    
    *   因为不再重复装很多 OS
        
    *   **启动超快**，适合一挂就拉起来再跑一份
        
4.  **安全隔离比 VM 弱**
    
    *   共享同一个内核
        
    *   容器逃逸漏洞存在
        
    *   所以**一般会在虚拟机里再跑容器**（双层：VM 保护，容器方便）。
        
        Advanced 10 - Virtualization, C…
        

五、Docker：最常见的容器工具
-----------------

这节课主要用的是 **Docker**。

Advanced 10 - Virtualization, C…

### 1. Image 和 Container

*   **Image（镜像）**：
    
    *   像「模具」「应用程序安装包」
        
    *   里面包含运行这个应用需要的一切：
        
        *   程序代码
            
        *   运行环境（比如 Python、JRE）
            
        *   依赖库
            
        *   配置、环境变量等
            
            Advanced 10 - Virtualization, C…
            
*   **Container（容器）**：
    
    *   就是「真正跑起来的那一份」
        
    *   和「类 vs 对象」很像：
        
        *   Image = 类
            
        *   Container = 用这个类 new 出来的对象
            
*   **镜像可以上传 / 下载**：
    
    *   公共仓库：Docker Hub
        
    *   也可以建**私有仓库**（像 OCF 就有自己的私有 Docker registry）。
        
        Advanced 10 - Virtualization, C…
        

### 2. 镜像是怎么做出来的？——Dockerfile

要造一个 Image，用一个文本文件：**Dockerfile**。

Advanced 10 - Virtualization, C…

特点：

*   一行行写「要执行的命令」：
    
    *   用哪个基础系统（FROM ubuntu:xenial）
        
    *   要 apt-get 装啥包
        
    *   要 pip 装啥 Python 包
        
    *   要把你本地的哪些文件拷进去（ADD/COPY）
        
    *   容器启动时跑什么命令（CMD）
        
*   镜像是 ** 分层（layer）** 的：
    
    *   很像洋葱（PPT 第 10 页就画了层）
        
        Advanced 10 - Virtualization, C…
        
    *   一层是「FROM ubuntu」
        
    *   下一层是「RUN apt-get install ...」
        
    *   再下一层是「COPY 代码」
        
    *   多个镜像用同一底层时，这部分只存一份 → 节省空间、加速构建
        

PPT 第 11 页那个例子：

Advanced 10 - Virtualization, C…

*   FROM ubuntu:xenial
    
*   安装 git 和 python3
    
*   用 pip 安装 flask
    
*   把 `hello.py` 拷进 `/app`
    
*   暴露 5000 端口
    
*   设环境变量 FLASK_APP
    
*   最后 CMD 用 `flask run` 跑起来
    

### 3. Docker daemon：幕后黑手

PPT 第 12 页：

Advanced 10 - Virtualization, C…

*   有个后台程序叫 **Docker daemon**
    
*   你在命令行敲：
    
    *   `docker build`
        
    *   `docker pull`
        
    *   `docker run`
        
*   其实都是和 daemon 说话，让它去：
    
    *   管理镜像
        
    *   启动 / 停止容器
        
    *   和镜像仓库通信（下载 / 上传）
        

六、容器编排：很多容器怎么「自动」跑起来？
---------------------

光有容器还不够。问题来了：

> 我有一堆机器，每台上跑一堆容器。  
> 挂了一台 / 挂了一个容器，要**自动补上**，不能手动 SSH 上去重启 1 万次。

这就需要 **容器编排（Container Orchestration）**。

Advanced 10 - Virtualization, C…

常见工具：

*   Kubernetes
    
*   Mesos + Marathon
    
*   Docker Swarm
    
*   课程实验里用的是更简单的 Docker Compose。
    
    Advanced 10 - Virtualization, C…
    

编排器干的事：

*   确保「**有正确数量的容器在跑**」
    
*   某个容器挂了 → 自动拉起新的
    
*   把容器分配到「合适的机器」上
    
*   有请求进来时，把流量分给健康的容器
    

七、分布式系统小尝味：主从结构 + 共识算法
----------------------

PPT 第 15–17 页讲的是「分布式系统魔法」。

Advanced 10 - Virtualization, C…

### 1. Master / Worker 结构

*   有一批 **Master（协调者）**
    
    *   决定：哪些容器在哪台机器上跑
        
    *   保存整个集群的「状态」
        
*   有一堆 **Worker（干活的机器）**：
    
    *   真正跑容器
        

但是：一个 Master 挂了怎么办？

*   所以会有多个 Master
    
*   需要一个**大家都同意的状态（quorum / 共识）**
    

为此会用到：

*   **etcd / ZooKeeper**：
    
    *   分布式 key-value 存储（像分布式字典）
        
    *   帮你维持「多个节点看到的是同一个状态」
        
        Advanced 10 - Virtualization, C…
        
*   它们底层用共识算法：
    
    *   2PC（Two-phase commit）：太慢
        
    *   Paxos：论文很难懂
        
    *   Raft：说是更好理解，但讲师也说「我也没仔细读」
        

核心思想：

> 别自己发明分布式算法，**用现成的（etcd、ZooKeeper）**。

八、架构例子：一个文件存储服务怎么一步步长大？
-----------------------

后面几页（20–25 页）用一个**文件存储服务**当例子，讲架构怎么演进。

Advanced 10 - Virtualization, C…

### 1. 初始版：全都堆一起

*   客户端：上传 / 下载文件
    
*   一个服务（File request handler）：同时处理上传 + 下载
    
*   一个数据库：存文件或文件元数据
    

→ 问题：负载一大，这台处理器 CPU 爆了。

### 2. 改进一：拆成多个服务 + 加负载均衡

PPT 第 21–22 页：

Advanced 10 - Virtualization, C…

*   把服务拆成两个：
    
    *   Upload Handler（专门处理上传）
        
    *   Download Handler（专门处理下载）
        
*   每个还可以开多份：
    
    *   多个 upload handler
        
    *   多个 download handler
        
*   前面加 **负载均衡（Load Balancer, LB）**
    
    *   Upload LB 把上传请求分给多个上传服务
        
    *   Download LB 把下载请求分给多个下载服务
        

好处：

*   某个 handler 挂了，还有别人顶上
    
*   水平扩展（多开几个实例）就能扛更多流量
    

新问题：

*   数据库磁盘 / IO 被打满了，成新的瓶颈。
    

### 3. 改进二：加缓存（Cache）

PPT 第 23 页：

Advanced 10 - Virtualization, C…

*   在 handler 和数据库之间加一层 **缓存**：
    
    *   比如 Redis、Memcached
        
    *   数据放在内存里，**读写很快**
        
*   尽量把缓存放离用户近一点（越接近客户端越好）
    

效果：

*   热门文件直接从缓存里拿，不去骚扰数据库
    
*   减轻数据库压力
    

新问题：

*   如果数据库挂了，咋办？
    

### 4. 改进三：数据库复制（Replication）

PPT 第 24 页：

Advanced 10 - Virtualization, C…

*   给数据库弄个**副本（replica）**
    
    *   主库写入
        
    *   副本同步数据
        
*   好处：
    
    *   主库挂了，可以切到副本
        
    *   部分读请求可以走副本，减轻主库压力
        

新问题：

*   上 Reddit 头条，整个库的磁盘又打满了……
    

### 5. 改进四：数据库分片（Partition / Sharding）

PPT 第 25 页：

Advanced 10 - Virtualization, C…

*   把数据库**拆成多台**：DB1、DB2……
    
    *   每个存一部分数据（比如按用户 ID 分段）
        
*   每个 DB 还有自己的 replica
    
*   前面有层逻辑负责：
    
    *   收到请求时，把它转发到「正确的 DB」上
        

结果：

*   既可以多开应用服务器（handler）
    
*   也可以多开数据库服务器
    
*   整个系统能**水平扩展**，扛非常大的流量
    

代价：

*   架构变复杂了（服务多、数据路由复杂、部署也复杂）
    

九、整节课的主线一句话总结
-------------

**当你的服务变大：**

1.  单机不够用 → 用**容器**来标准化、快速部署应用
    
2.  机器多了 → 用**容器编排 + 分布式系统**自动拉起、迁移、调度容器
    
3.  流量更大 → 用**拆服务 + 负载均衡 + 缓存 + 复制 + 分片**的架构，把压力分散出去
    

所有设计里，都在做三件事的平衡：

*   **性能**（扛得住多少流量）
    
*   **可靠性**（坏了还能不能继续跑）
    
*   **复杂度**（你自己还能不能搞得懂、维护得了）
    
    Advanced 10 - Virtualization, C…
    

如果你愿意，我可以下一步：

*   把「虚拟机 vs 容器」单独画一张简图解释，
    
*   或者带你写一个**最简单的 Dockerfile + docker-compose.yml**，实操一遍就更好懂了。