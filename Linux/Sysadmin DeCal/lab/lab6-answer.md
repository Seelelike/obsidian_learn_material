### 1.py
``` python
import lib.util as util
command = "ifdown {iface}"
routing_entries = util.get_default_routing_information() #获取当前所有路由表条目
default_entry = next(
    (e for e in routing_entries if util.is_default_gateway(e)),  #找出默认网关所在的接口（通常是 eth0、ens3 等）
    None
)
util.run(command.format(iface=default_entry.iface))  # 执行 ifdown eth0（或对应的网卡）
```
功能：相当于`sudo ifdown eth0`,关闭网卡。
为什么无效：
	因为我使用的这台 DigitalOcean 云服务器的 Ubuntu 22.04 **已经完全抛弃了传统的 ifdown/ifup 机制**，改用 Netplan + systemd-networkd 管理网络。
-  ifdown 命令在现代 Ubuntu 上要么不存在，要么什么都不干（直接返回 0）
- 即使存在，也根本影响不了 systemd-networkd 控制的网卡
- 所以 1.py 执行完 ifdown eth0 后，系统完全无感，网络照样通 → 你看到的“没坏”
解决方法：`sudo ifup eth0`

#### 路由、网关与网卡
> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [gk.zchat.tech](https://gk.zchat.tech/c/dae38c28-4da8-4650-a29d-3806dd1fe57e?rid=4af3f3b3-cd25-45e3-8da9-a6d8f52abcc5)

> Grok is a free AI assistant designed by xAI to maximize truth and objectivity. Grok offers real-time ......

##### 1. 网卡（Interface）——物理 / 虚拟的 “网线口”

*   相当于你电脑后面的网口，或者云服务器里的虚拟网卡。
*   你服务器现在的网卡有这几个：
    ```
    eth0      ← 公网网卡（真正能上网的那个）
    eth1      ← 私有网络（10.124.0.0/20）
    lo        ← 本地回环 127.0.0.1
    ```
*   网卡负责把数据包从操作系统 “扔出去” 或“收进来”。

##### 2. 网关（Gateway）——“下一跳” 的 IP 地址
*   就是 “如果你不知道去哪儿，就把包发给它，它知道怎么转发”。
*   你服务器的网关是：
    ```
    ip route show | grep default
    default via 209.38.64.1 dev eth0 ...
    
    ```
    → 网关 IP = 209.38.64.1 → 网关所在的网卡 = eth0
**记住一句话：网关永远是一个 IP 地址，而且必须和某个网卡在同一个网段。**
##### 3. 路由（Route / Routing Table）——“交通规则表”
*   告诉系统：“去这个目的地，请走哪张网卡、交给哪个网关”。
*   你现在完整的路由表长这样（简化版）：<table dir="auto" class="w-fit min-w-[calc(var(--content-width)-13px)] [&amp;&gt;thead&gt;tr&gt;th:last-child]:pr-8"><thead class="sticky [top:var(--thead-sticky-top)] [&amp;_th]:h-10 [background-color:var(--thead-bg-color)] [box-shadow:0_1px_0_0_var(--thead-border-b-color)] border-b-0"><tr><th data-col-size="xl">目的地</th><th data-col-size="md">网关（下一跳）</th><th data-col-size="sm">出口网卡</th><th data-col-size="lg">含义</th></tr></thead><tbody><tr><td data-col-size="xl">0.0.0.0/0 (默认路由)</td><td data-col-size="md">209.38.64.1</td><td data-col-size="sm">eth0</td><td data-col-size="lg">我不知道去哪儿 → 都扔给 209.38.64.1</td></tr><tr><td data-col-size="xl">209.38.64.0/20</td><td data-col-size="md">(直接连接)</td><td data-col-size="sm">eth0</td><td data-col-size="lg">本地局域网，直接发</td></tr><tr><td data-col-size="xl">10.48.0.0/16</td><td data-col-size="md">(直接连接)</td><td data-col-size="sm">eth0</td><td data-col-size="lg">另一个私有网段</td></tr><tr><td data-col-size="xl">10.124.0.0/20</td><td data-col-size="md">(直接连接)</td><td data-col-size="sm">eth1</td><td data-col-size="lg">私有网络</td></tr><tr><td data-col-size="xl">127.0.0.0/8</td><td data-col-size="md">(直接连接)</td><td data-col-size="sm">lo</td><td data-col-size="lg">本地回环</td></tr></tbody></table>

##### 三者的真实关系（用你发 google.com 的包来举例）

当你执行 ping google.com 时，实际流程是：

1.  系统先解析 DNS → 得到 142.251.46.206
2.  去路由表里查 “我该怎么去 142.251.46.206？” → 没有具体条目 → 匹配到默认路由（0.0.0.0/0）
3.  路由表说：“走 eth0 网卡，把包发给网关 209.38.64.1”
4.  网卡 eth0 把包扔出去 → 交给 DigitalOcean 的上游路由器 → 最终到 Google


### 2.py
``` python
routing_entries = util.get_default_routing_information() default_entry = next((e for e in routing_entries if util.is_default_gateway(e)), None) # 第1步：找到当前默认网关条目（例如：default via 209.38.64.1 dev eth0）

default_iface_entry = next( (e for e in routing_entries if not util.is_default_gateway(e) and e.iface == default_entry.iface), None ) # 第2步：在同一张网卡（eth0）上找到“本地直连”条目（例如：209.38.64.0/20 dev eth0 ...） # 这一条里可以提取到本机的 IP（例如 209.38.65.46）

command = "ip route delete default" util.run(command) # 第3步：直接删掉原来的默认路由！（这一刻开始你已经半死）

command = "ip route add default via {gateway}".format( gateway=util.get_iface_ip_address(default_iface_entry.iface) ) util.run(command) # 第4步：把默认网关改成你自己本机的公网 IP（209.38.65.46） # 结果：所有本该发给 209.38.64.1 的包，现在都发给自己 → 形成路由环路 → 全网瞬间黑洞！
```
功能：把默认网关从上游网关改成了自己的公网ip，→ 所有出站包都发给自己 → 自己再把自己发给自己 → 死循环 → 彻底失联，连 SSH 都被主动断开。
解决：将默认网关改为原网关
```
ip route del default
ip route add default via 209.38.64.1 dev eth0
```

### 3.py
```
import lib.util as util

HOSTS_FILE = "/etc/hosts"
ENTRY = "72.66.115.13 google.com\n"

with open(HOSTS_FILE, "a") as host_file:
    host_file.write(ENTRY)
```
功能：修改DNS解析优先级，系统就永远不会去问 8.8.8.8，而是直接把 google.com 当成 72.66.115.13。
解决：
`sudo sed -i '/google.com/d' /etc/hosts`删除文件中google对应条目。

### 4.py
```python
RESOLV_FILE = "/etc/resolv.conf"
lines = []
with open(RESOLV_FILE, "r+") as resolv_file:
    for line in resolv_file:
        lines.append("#" + line)      # 把每一行前面加上 #，变成注释
    resolv_file.seek(0, 0)
    resolv_file.write(''.join(lines)) # 原地覆盖回去
```
功能：将DNS配置注释掉，当想要进行域名解析时，无可用DNS服务器。
解决：`sudo systemctl restart systemd-resolved`
为什么 /etc/resolv.conf 里只有 nameserver 127.0.0.53？:
	现代ubuntu默认启用了 systemd-resolved 本地 DNS 缓存/转发服务。
	真实 DNS 查询流程（2025 年 Ubuntu）： 你的程序（ping、curl 等） ↓ 询问本机 127.0.0.53（systemd-resolved 监听的本地端口） ↓ systemd-resolved 再把请求转发给真正的上游 DNS（8.8.8.8、1.1.1.1、云厂商 DNS 等） ↓ 把结果缓存并返回给你
- 127.0.0.53 = 本机的“DNS 中继站”
-  真正的上游 DNS 服务器藏在 systemd-resolved 内部配置里，不会直接写死在 /etc/resolv.conf
	因此，解决方案，直接重启systemd-resolved即可。

### 5.py
```
import lib.util as util
from shutil import copyfile

RESOLV_FILE = "/etc/resolv.conf"
ENTRY = "nameserver {local_ns}\n"
DNSMASQ_HOST_FILE_SRC = "config/dnsmasq.hosts"
DNSMASQ_HOST_FILE_DST = "/etc/dnsmasq.hosts"
DNSMASQ_CONFIG_FILE_SRC = "config/dnsmasq.conf"
DNSMASQ_CONFIG_FILE_DST = "/etc/dnsmasq.conf"

routing_entries = util.get_default_routing_information()
default_entry = next((e for e in routing_entries if util.is_default_gateway(e)), None)
default_iface_entry = next(
    (e for e in routing_entries if not util.is_default_gateway(e) and e.iface == default_entry.iface),
    None
)

ENTRY = ENTRY.format(local_ns=util.get_iface_ip_address(default_iface_entry.iface))

copyfile(DNSMASQ_HOST_FILE_SRC, DNSMASQ_HOST_FILE_DST)
copyfile(DNSMASQ_CONFIG_FILE_SRC, DNSMASQ_CONFIG_FILE_DST)

with open(RESOLV_FILE, "r+") as resolv_file:
    original = resolv_file.read()
    resolv_file.seek(0, 0)
    resolv_file.write(ENTRY + original)

command = "killall -9 dnsmasq"
util.run(command)
command = "dnsmasq"
util.run(command)
```
功能：在resolv.conf中把本机公网 IP（209.38.65.46）写成唯一的 nameserver，并将自己作为DNS服务器，本机的所有DNS查询发向自己。所有程序（包括你自己的浏览器、curl、ping）发的 DNS 查询都被劫持到本机 → 本机的 dnsmasq 按 config/dnsmasq.hosts 强行返回假 IP → 你访问 google.com 永远去到一个假站点。
为什么失效：dnsmasq 根本没权限监听 53 端口！你虽然执行了 dnsmasq，但它启动失败了（被系统阻止），所以 DNS 查询依然走正常的 systemd-resolved → google.com 解析正常。

### 6.py
``` python
import lib.util as util

INTERFACES_FILE = "/etc/network/interfaces"

routing_entries = util.get_default_routing_information()
default_entry = next((e for e in routing_entries if util.is_default_gateway(e)), None)
default_iface = default_entry.iface

lines = []
with open(INTERFACES_FILE, "r+") as iface_file:
    for line in iface_file:
        if default_iface in line and "dhcp" in line:
            lines.append(line.replace("dhcp", "manual"))
        else:
            lines.append(line)
    iface_file.seek(0, 0)
    iface_file.write(''.join(lines))

command = "ip addr flush {iface}".format(iface=default_iface)
util.run(command)
```
功能：将 /etc/network/interfaces 中默认网卡的 dhcp 改为 manual，执行 ip addr flush eth0 清空接口地址。
为什么失效：本机为 Ubuntu 22.04，网络由 Netplan + cloud-init 管理，/etc/network/interfaces 早已废弃 ip addr flush 后 systemd-networkd 立即重新分配 IP