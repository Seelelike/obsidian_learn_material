### OSI模型
- 物理层
	管的就是线和电信号/光信号/无线电波：
	- 网线（铜线） 
	- 光纤（激光） 
	- Wi-Fi 的电磁波（802.11 a/b/g/n/ac 等） 
	这层只是“一个口对另一个口”传信号，没有“地址”概念。
- 数据链路层
	管的是网卡和 MAC 地址。 
	通信对象是：一个网卡 → 另一个网卡（同一个局域网里）。 
	这层知道： 
	-  每个网卡的 MAC 地址（像身份证，硬件刻好的 48 位数字）。 
	-  怎么在同一根线上的很多机器之间区分“这条消息给谁”。 
	-  本地的小范围转发（局域网内的路由）。 
	- 会涉及 ARP / NDP 这类协议（后面会讲）。
- 网络层（Network） 
	管的是IP 地址和在不同局域网之间转发。 
	通信对象是：一台主机 → 另一台主机（可以跨世界）。 
	提供： 
	- 逻辑地址（IPv4、IPv6） 
	- 在全世界网络里的**路由**（怎么绕来绕去到对方）。 
	它只负责：“把包送过去”，不保证一定送到、也不保证顺序。
- 传输层
	管的是应用与应用之间的连接。 
	常听到的两个协议就在这层： 
	- TCP：可靠、有连接，像顺丰签收。 
	- UDP：不可靠、无连接，像往窗外扔纸飞机。 
	它还能把同一台机器上的多个服务“分门别类”放在不同**端口**上。
- 会话层
	比方说你现在登录了某个网站，网站服务可以保持你的登录状态不用每次都输入账号和密码，当然网站服务会管理和控制登录状态，另外会话层还负责**同步服务**，比方说你上次看到电影高潮的时刻突然停电了，再次登录账号的时候就可以自动同步岛上次看到的时间段。
- 表示层
	编码和解码数据往往还需要进行加密，比方说 HTTPS(SSL/ TLS) 就会对我们的数据进行加密和解密，另外我们可能还需要给文件瘦身
- 应用层
	就是你看到的程序：浏览器、微信、ssh 客户端…… 
	举例： 浏览器说：“我用 HTTP（应用层） 跟服务器对话， HTTP 的数据通过 TCP（传输层） 传， TCP 的包通过 IP（网络层） 跑， IP 再通过 Wi-Fi（链路/物理层） 发出去。

### `ip link`查看网卡
- 可以是真实的硬件网卡（插在主板上的那块板子），  
    也可以是虚拟网卡（虚拟机、容器、桥接接口等）。
- 每个接口都有：
    - 一种介质（网线口、Wi-Fi 天线）
    - 一个 **MAC 地址**
- 在 Linux 里，你用命令 `ip link` 看所有接口：
    - 比如：`lo`（本机回环）、`wlp9s0`（Wi-Fi）、`enp10s0`（有线网卡）等。  
        名字由 systemd 的“可预测命名”规则决定。
`ip link` 输出里会显示：
- MAC 地址
- MTU（最大包大小）
- 状态：`UP` 表示逻辑打开，`LOWER_UP` 表示物理也好了（线插上、无线连上）。

>1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether b2:81:63:e4:dd:7b brd ff:ff:ff:ff:ff:ff
    altname enp0s3
    altname ens3
3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether ca:b6:83:a4:12:0f brd ff:ff:ff:ff:ff:ff
    altname enp0s4
    altname ens4
4: virbr0: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc noqueue state DOWN mode DEFAULT group default qlen 1000
    link/ether 52:54:00:1d:5b:a5 brd ff:ff:ff:ff:ff:ff

|行号|内容|含义与实际意义|
|---|---|---|
|1|`1: lo: <LOOPBACK,UP,LOWER_UP> ...`|**lo** 是本地回环接口（127.0.0.1）。 状态 `UP,LOWER_UP` 表示已启用且物理层正常（回环永远是这样的）。 所有 `localhost` 访问都走这里。|
|2|`2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> ...` `link/ether b2:81:63:e4:dd:7b` `altname enp0s3 altname ens3`|**eth0** 是你的主网卡（通常是公网网卡）。 MAC 地址：`b2:81:63:e4:dd:7b` 状态 `UP,LOWER_UP` = 网卡已启用且物理链路通（有线已插好）。 这是你服务器对外通信的主要接口（公网 IP 就绑定在这里）。 在 DigitalOcean、Vultr、Linode 等云厂商里，**eth0 几乎总是公网接口**。|
|3|`3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> ...` `link/ether ca:b6:83:a4:12:0f` `altname enp0s4 altname ens4`|**eth1** 是第二块网卡（很多云厂商默认给你一块私有网络网卡）。 状态同样是 UP。 通常绑定的是内网 IP（10.x、172.16.x、192.168.x），用于同一地区机器之间高速通信，或者作为管理口。 在 DigitalOcean 里这块网卡叫 “Private Network”。|
|4|`4: virbr0: <NO-CARRIER,BROADCAST,MULTICAST,UP> ...` `state DOWN`|**virbr0** 是 libvirt/KVM/QEMU 创建的虚拟网桥（默认 192.168.122.0/24）。 `NO-CARRIER` + `state DOWN` 表示目前没有虚拟机挂在这个网桥上，所以链路是 down 的。 如果你没装桌面或 KVM 虚拟机，可以完全忽略它。|

#### 参数解析
接口编号 + 名字 + 当前能力 + MTU + 排队算法 + 综合状态 + MAC 地址 + 广播地址 + 别名

| 位置/字段                         | 示例值                               | 详细含义（你以后 100% 会用到）                                                             |
| ----------------------------- | --------------------------------- | ------------------------------------------------------------------------------ |
| `2`                           | 2                                 | **接口编号**（内核内部编号），全局唯一，几乎不会变。                                                   |
| `eth0`                        | eth0                              | **接口名字**（用户可见的名字），可以改（`ip link set eth0 name wan`）。                            |
| `<...>`                       | <BROADCAST,MULTICAST,UP,LOWER_UP> | **接口当前支持/启用的能力标志**（尖括号里多个用逗号分隔）                                                |
| BROADCAST                     |                                   | 支持广播                                                                           |
| MULTICAST                     |                                   | 支持组播                                                                           |
| UP                            |                                   | 管理员已把接口启用（`ip link set eth0 up`）                                               |
| LOWER_UP                      |                                   | 物理层（Layer 1）是通的（网线插着、光信号有、Wi-Fi 已连上）                                           |
| `mtu 1500`                    | 1500                              | **最大传输单元**，以太网标准 1500 字节。改大（jumbo frame）或改小（隧道）都要在这里设置。                        |
| `qdisc fq_codel`              | fq_codel                          | **队列规则**（Queueing Discipline），控制发包排队和拥塞算法。fq_codel 是目前最常用的防 Bufferbloat 算法。    |
| `state UP`                    | UP                                | **链路整体状态**（综合管理员状态+物理状态） 可能的值：UP / DOWN / UNKNOWN / DORMANT / LOWERLAYERDOWN 等 |
| `mode DEFAULT`                | DEFAULT                           | 对无线网卡才有意义（managed、monitor 等），有线网卡基本都是 DEFAULT                                  |
| `group default`               | default                           | 接口所属的组（可以用 `ip link set dev eth0 group xxx` 分组批量操作）                            |
| `qlen 1000`                   | 1000                              | **发送队列长度**（txqueuelen），默认 1000 个包，极少需要改。                                       |
| `link/ether ...`              | link/ether b2:81:63:e4:dd:7b      | **链路层类型和硬件地址** ether = 以太网 b2:81:63:e4:dd:7b = MAC 地址                          |
| `brd ff:ff:ff:ff:ff:ff`       | ff:ff:ff:ff:ff:ff                 | **广播地址**（以太网全 ff）                                                              |
| `altname enp0s3 altname ens3` | enp0s3、ens3                       | **别名**（systemd 的 predictable network interface names），可以直接用这些名字替代 eth0，功能完全一样。 |

常见标志与状态

| 标志/状态         | 含义                  | 常见场景                      |
| ------------- | ------------------- | ------------------------- |
| UP            | 管理员已启用              | `ip link set dev eth0 up` |
| LOWER_UP      | 物理链路通               | 网线插好、无线已连上                |
| NO-CARRIER    | 物理链路不通              | 网线没插、交换机端口 down           |
| DORMANT       | 等待某种事件才能真正 up（比如拨号） | pppoe、部分无线网卡              |
| PROMISC       | 混杂模式（抓包用）           | tcpdump、wireshark         |
| ALLMULTI      | 接收所有组播包             | 某些组播应用                    |
| state DOWN    | 接口整体 down           | 没插网线 + 管理员没 up            |
| state UNKNOWN | 状态未知（极少见）           | 某些虚拟接口                    |

### MAC与APR
#### MAC 地址
- 全称 **Media Access Control**，是**硬件层的地址**。
- 长度 48 位，也就是 6 组十六进制数：如 `00:14:22:01:23:45`。
- 前 3 组表示厂家编号。
#### ARP 协议（地址解析）
问题：应用只知道对方 IP（第三层地址），但真正发包时链路层需要 MAC 地址，怎么办？  
→ 用 **ARP（Address Resolution Protocol）**。
- 作用：**把 IP 地址 → MAC 地址**。
- 操作过程（简化版）：
    1. 我想发包给 `192.168.1.1`，但不知道它 MAC。
    2. 在局域网里广播问：`192.168.1.1 是谁？请告诉我你的 MAC 地址。`
    3. 目标机器回复它的 MAC。
    4. 内核把这个对应关系（IP → MAC）缓存一段时间（如 60 秒）。
        Networking 102 FA20
Linux 里用命令 `arp` 或 `ip neigh` 看这个表；配置在 `/proc/net/arp` 等文件里。

```
ip link - manage interfaces at L2 
ip link set <iface> [up|down] - enable/disable logical interface 
ip link [add|delete] <iface> type [type] - add/remove interfaces themselves, e.g. bridge or vlan virtual devices 
static configuration (on Debian) lives in /etc/network/interfaces 
```

### `ip addr`查看ip地址

- 例子：`192.168.1.1`、`169.229.226.23`。
- 一个网卡可以绑**多个 IP 地址**。
- 也可以用“桥接设备”把多个 MAC/IP 映射到一个物理接口。
Linux 用 `ip addr` 看 IP 信息。

#### IPv4 写法和掩码（CIDR）
- IPv4 是 32 位，用四个点分十进制表示，如 `127.0.0.1`。
- 后面经常跟 `/数字`，表示“前多少位是网络号”，叫 **掩码**：
    - 例如 `169.229.226.0/24`：前 24 位是网络号，后 8 位是主机号。
- `255.255.255.255` 是广播地址，表示“给所有人”。

#### 管理 IP 地址和路由
- `ip addr`：显示/添加/删除地址。
- `ip route` 或 `ip -6 route`：看路由表、增删路由。
- 路由里会有一个 `default` 路由，指向“默认网关”。

#### DHCP：自动领 IP 的机制
手动给每台机器写 IP 很麻烦，所以有 **DHCP**：
- 设备连上网后，会向局域网广播：“有没有 DHCP 服务器，我要配置。”
- DHCP 服务器回一个“租约”（lease）：
    - 一个 IPv4 地址
    - 子网掩码
    - 网关地址
    - 可能还有 DNS 服务器地址
- 租约有时间限制，到期前客户端要去“续租”。
相关工具：
- `dhcpcd <iface>`：较新的 DHCP 客户端。
- `dhclient`：老一点，但仍有用。

#### DNS：把域名翻译成 IP
人记不住一堆数字 IP，于是有 **DNS（Domain Name System）**。
- 你的电脑里有个“DNS 解析器”，专门向 **DNS 服务器**询问：  
    “`nyx.ocf.berkeley.edu` 的 IP 是多少？” 
- 解析大概流程（简化）：
    1. 查到 **根服务器**（13 个地址写死在程序里）。
    2. 问根：“`.edu` 的服务器在哪？”
    3. 再问 `.edu`：“`berkeley.edu` 的服务器在哪？”
    4. 再问 `berkeley.edu`：“`ocf.berkeley.edu` 的服务器在哪？”
    5. 最后问 `ocf.berkeley.edu` 的服务器：“`nyx` 这个主机的 IP 是多少？”
- 这样一层层问下去，最终拿到 IP，比如 `169.229.226.231`。    

**DNS 里的记录类型**
DNS 里存的是一条条“记录（Resource Record, RR）”：
- 每条记录：`(name, value, type, TTL)`。
- 常见：
    - **A 记录**：域名 → IPv4 地址。
    - **NS 记录**：某个域名的权威 DNS 服务器是谁。

**DNS 常用命令 & 配置文件**
命令：
- `dig 域名`：详细查询。
- `host 域名`：简单查询。
- `rndc reload`：重载 bind9 配置（当你是 DNS 服务器时）。
- `nscd -i hosts`：清本地 DNS 缓存。
文件：
- `/etc/hosts`：
    - 手工写“IP 对应哪个主机名”，优先级很高。
    - 例如：`127.0.0.1 localhost`
- `/etc/resolv.conf`：
    - 配置“用哪个 DNS 服务器、搜索什么域”：
        - `nameserver 8.8.8.8`
        - `search example.com school.edu` 等。

### 传输层

#### 传输层的作用
- 第 2 层：只是把比特从一头到另一头。
- 第 3 层：把比特包装成“包”，能在多个网络之间转发。
- 第 4 层：再往上抽象成“连接”。
常见两种：
- **TCP**：可靠、有连接。
- **UDP**：不可靠、无连接。
#### TCP：靠谱但慢一点
TCP 是：
- 有状态、面向连接。    
- 保证数据 **按顺序**、不丢失地到达。
- 建立连接需要“握手”：教科书里常说 3 次握手，PPT 提到“4-way start / 3-way stop”，总之要来回几次确认。
- 这种可靠性要付出“额外开销”（多发控制包）。
- 适合：
    - 不能丢数据的场景，比如网页、文件下载、登录（SSH）。
#### UDP：不囉唆但不保证
UDP是：
- 无连接、无状态。
- 不保证一定送到、也不保证顺序。
- 好处：没有 TCP 那些握手、确认的开销，简单、延迟低。
- 常用在：
    - 流媒体（音乐、视频），偶尔丢一帧没关系；
    - 某些实时游戏、语音通话。
就像：**往对方家门口丢纸条，不管他收没收、收没收到顺序**。

#### 端口与套接字（Sockets）
- **端口（Port）**：在一台机器上区分不同服务的小编号。  
    比如同一个 IP 上：
    - 80 端口是 HTTP
    - 22 端口是 SSH
- **套接字（Socket）**：程序里用来收发网络数据的“端点对象”。
- 一个连接由两端的 socket 组成，常用“五元组”表示：
    (协议, 源 IP, 源端口, 目的 IP, 目的端口)

### /proc 里的网络信息
Linux 用 `/proc` 这个虚拟文件系统来展示内核状态。
- `/proc/net/dev`：
    - 每个网络设备收到/发出的字节数等统计。
- `/proc/net/tcp` `/proc/net/udp`：
    - 当前系统里打开的 TCP/UDP 套接字信息。
    - 命令 `ss` / `netstat` 背后就是读这些。
还有 `/proc/sys/net` 这一大堆文件，是**内核网络参数的设置界面**：
- 你可以用： 
    - `echo 值 > 某个文件`
    - 或者 `sysctl -w key=value` 来修改。
- 为了开机后自动生效，可以写在 `/etc/sysctl.conf` 里。

举一些参数：
- `icmp_echo_ignore_all`：是否响应 ping。
- `ip_forward`：是否允许这台机当“路由器”（帮别人转发包）。
- `ip_default_ttl`：发包时默认 TTL。
- `ip_local_port_range`：本机临时端口的范围。

### 常用工具
| 分类             | 命令                   | 主要用途                                                       | 经典一击必杀命令（直接复制）                                          |
| -------------- | -------------------- | ---------------------------------------------------------- | ------------------------------------------------------- |
| **DNS 相关**     | dig                  | 最强大的 DNS 查询工具，能看到完整解析过程                                    | `dig +short google.com` `dig @8.8.8.8 google.com`       |
|                | host                 | 简单快速查 DNS（比 dig 简洁）                                        | `host google.com`                                       |
|                | nslookup             | 老派但到处都有的 DNS 查询（互动模式）                                      | `nslookup google.com`                                   |
| **连通性/延迟**     | ping                 | 测试 ICMP Internet Control Message Protocol（互联网控制消息协议）可达性和延迟 | `ping -c 4 8.8.8.8`                                     |
|                | traceroute / mtr     | 查看去目标的完整路径和每跳延迟（mtr 是动态版 traceroute）                       | `mtr 8.8.8.8` `traceroute google.com`                   |
| **IP/接口查看**    | ip (addr/link/route) | 现代 iproute2 套件，取代旧的 ifconfig/route                         | `ip a` `ip r` `ip neigh`                                |
|                | arp                  | 查看/操作 ARP 表（IP ↔ MAC）                                      | `arp -n`                                                |
|                | netstat              | 传统查看连接、监听端口、路由表（已被 ss 取代）                                  | `netstat -tuln` `netstat -rn`                           |
|                | ss                   | 更快更强的 netstat 替代品                                          | `ss -tuln` `ss -tlnp \| grep :80`                       |
| **防火墙**        | iptables             | 底层状态防火墙（nftables 后继者）                                      | `iptables -L -n -v` `iptables -t nat -L`                |
|                | nft                  | 新一代防火墙框架（Ubuntu 20.04+ 默认）                                 | `nft list ruleset`                                      |
|                | ufw                  | iptables 的超简单前端（Ubuntu 自带）                                 | `ufw status verbose` `ufw allow 5000`                   |
|                | firewalld            | CentOS/RHEL 的动态防火墙（不在 Ubuntu 默认）                           | `firewall-cmd --list-all`                               |
| **路由/转发**      | ip route             | 查看/管理路由表                                                   | `ip r` `ip route get 8.8.8.8`                           |
| **抓包**         | tcpdump              | 命令行抓包神器                                                    | `tcpdump -i eth0 port 80` `tcpdump -i any host 1.1.1.1` |
|                | wireshark/tshark     | 图形/命令行高级抓包分析                                               | `tshark -i eth0 -f "port 53"`                           |
| **HTTP/网络交互**  | curl                 | 万能 HTTP 客户端（支持几乎所有协议）                                      | `curl -I http://ip:5000` `curl ifconfig.me`             |
|                | wget                 | 简单下载工具（也常用来测试连通性）                                          | `wget -qO- http://ip:port`                              |
| **TCP/UDP 测试** | nc (netcat)          | 瑞士军刀级端口测试工具，可做简单服务器/客户端                                    | 服务端：`nc -lvkp 5000` 客户端：`nc 127.0.0.1 5000`             |
| **综合工具**       | ifconfig             | 传统查看接口（已被 ip 取代，但很多脚本还在用）                                  | `ifconfig eth0`                                         |
|                | route                | 传统路由表（已被 ip route 取代）                                      | `route -n`                                              |
