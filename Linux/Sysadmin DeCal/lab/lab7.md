挂载NFS
`mount staff2.decal.xcf.sh:/opt/lab7/public /mnt`
将远程文件系统挂载到本地/mnt下。
- 问题：staff2.decal.xcf.sh主机名找不到，这是Berkeley DECal 实验室内部的 DNS，无法正常解析为ip地址。
- 问题：切换为ip地址后，仍然无法连接上，外部ip被防火墙拦截
- 解决：本地创建NFS服务，通过127.0.0.1进行挂载。

创建DNS服务器
```
/etc/bind/named.conf.local

zone "example.com" {
  type master;
  file "/etc/bind/db.example.com";
};
```
声明自己为example.com域名的主DNS服务器，所有关于 example.com 的答案都写在这个文件里 };
```
$TTL 604800
@       IN      SOA     ns.example.com. root.example.com. (
                        2025121003 ; Serial
                        604800 ; Refresh
                        86400  ; Retry
                        2419200 ; Expire
                        604800 ) ; Negative Cache TTL

@               IN      NS      suifeng1660.decal.xcf.sh.
@               IN      A       209.38.65.46
www             IN      A       209.38.65.46
test            IN      A       93.184.216.34

mail            IN      CNAME   suifeng1660.decal.xcf.sh.
ftp             IN      CNAME   suifeng1660.decal.xcf.sh.
_txt            IN      TXT     "suifeng1660 was here"
_service._tcp   IN      SRV     0 5 8080 suifeng1660.decal.xcf.sh.

```

|记录|意思（大白话）|实际效果（你访问时会发生什么）|
|---|---|---|
|`@ IN NS suifeng1660.decal.xcf.sh.`|官方宣布：example.com 的名字服务器就是 suifeng1660.decal.xcf.sh（也就是你这台虚拟机）|全世界 DNS 查 example.com 都会最终来问你的机器|
|`@ IN A 209.38.65.46`|example.com 这个域名直接指向你虚拟机的公网 IP|你在浏览器敲 example.com → 直接打开你虚拟机上的网页|
|`www IN A 209.38.65.46`|[www.example.com](http://www.example.com/) 也指向同一台机器（最常见写法）|访问 [www.example.com](http://www.example.com/) 同样到你机器|
|`test IN A 93.184.216.34`|Lab 要求的测试子域：test.example.com 故意指向真正的 example.com 官方 IP（93.184.216.34）|用来对比你自己的 DNS 和真实互联网 DNS 的区别|
|`mail IN CNAME suifeng1660.decal.xcf.sh.`|mail.example.com 是你虚拟机的一个别名（CNAME = 别名记录）|访问 mail.example.com → 自动跳转到 suifeng1660.decal.xcf.sh|
|`ftp IN CNAME suifeng1660.decal.xcf.sh.`|同理，ftp.example.com 也是别名|访问 ftp.example.com → 一样到你机器|
|`_txt IN TXT "suifeng1660 was here"`|TXT 记录，通常用来做域名验证、SPF 反垃圾邮件等，你在这留了个彩蛋|`dig TXT _txt.example.com` 会看到你的签名|
|`_service._tcp IN SRV 0 5 8080 suifeng1660.decal.xcf.sh.`|SRV 记录（服务发现记录）：告诉别人「我的某个服务跑在 suifeng1660.decal.xcf.sh 的 8080 端口」|一些 VoIP、游戏、Minecraft 等软件会自动读这条记录来发现服务端口|

负载均衡
```
listen stats bind 0.0.0.0:7001 ← 监听 7001 端口，校外也能访问 
mode http 
stats enable ← 打开 HAProxy 自带的 stats 页面 
stats hide-version ← 隐藏版本号（安全小习惯） 
stats uri /stats ← 访问路径是 http://我的ip地址:7001/stats

frontend fe_web 
bind *:7000 ← 监听 7000 端口（所有 IP） 
mode http ← 工作在七层（HTTP 模式） 
default_backend be_workers ← 所有流量都扔给下面的后端

backend be_workers 
mode http balance roundrobin ← 轮询算法（你刷新页面看到的 0→1→2→3→4→5 就是它）

option httpchk GET /health ← 每 2 秒自动访问每个 worker 的 /health 路径 
http-check expect status 200 ← 期望返回 200，拿不到就认为它死了

server w1 127.0.0.1:8080 check ← 6 个真实 worker，全在本机不同端口 
server w2 127.0.0.1:8081 check ... 
server w6 127.0.0.1:8085 check
```

- 自动把请求**平均轮流**发给 6 个 worker
- 每 2 秒给每个 worker 做一次心跳（访问 /health）
- 如果某个 worker 收到 /crash 自杀了 → 几秒内 HAProxy 发现 404/超时 → 自动标红 + 不再给它发流量
- 等它重启恢复后又自动变绿，继续接流量