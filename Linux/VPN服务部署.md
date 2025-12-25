![[Pasted image 20251224155157.png]]
### 安装Mihomo内核
```
mkdir /etc/mihomo
cd /etc/mihomo
wget https://github.com/MetaCubeX/mihomo/releases/download/v1.18.3/mihomo-linux-amd64-v1.18.3.gz
gunzip mihomo-linux-amd64-v1.18.3.gz
mv mihomo-linux-amd64-v1.18.3 mihomo
chmod +x mihomo
```

### 配置config.yaml
```
# /etc/mihomo/config.yaml
port: 7890
socks-port: 7891
allow-lan: true
mode: direct
log-level: info

# 定义服务端监听
# 这里以 Shadowsocks 为例
listeners:
  - name: ss-in
    type: shadowsocks
    port: 8388
    listen: 0.0.0.0
    password: "你的密码" # 建议设置复杂的密码
    cipher: aes-256-gcm
    udp: true
```

### 开防火墙
```
ufw allow 8388/tcp
ufw allow 8388/udp
```

### 设置systemd
创建服务文件 `/etc/systemd/system/mihomo.service`
```
[Unit]
Description=Mihomo Daemon
After=network.target

[Service]
ExecStart=/etc/mihomo/mihomo -d /etc/mihomo
Restart=always

[Install]
WantedBy=multi-user.target
```
执行：`systemctl enable --now mihomo`

## 添加访问校内网功能
### 下载FRP
```
wget https://github.com/fatedier/frp/releases/download/v0.54.0/frp_0.54.0_linux_amd64.tar.gz
tar -zxvf frp_0.54.0_linux_amd64.tar.gz
cd frp_0.54.0_linux_amd64
```
### 编辑配置文件
```
bindPort = 7700             # frp 服务端监听端口
auth.token = "你的强密码"    # 安全认证令牌，防止别人蹭你的隧道
```
问题：systemd服务启动一直失败。
排查：
```
sudo journalctl -u frps -n 20 --no-pager
sudo ss -tulpn | grep 7000
```
解决：发现7000端口被其他服务占据，更换端口为7700解决。
### 创建systemd
为方便管理，将文件与配置文件移动到系统标准路径
```
# 移动二进制文件
sudo cp /etc/mihomo/frp_0.54.0_linux_amd64/frps /usr/local/bin/

# 创建配置目录并移动配置文件
sudo mkdir -p /etc/frp
sudo cp /etc/mihomo/frp_0.54.0_linux_amd64/frps.toml /etc/frp/
```

```
sudo nano /etc/systemd/system/frps.service

[Unit]
Description=FRP Server Service
After=network.target network-online.target nss-lookup.target

[Service]
Type=simple
User=root
# 启动命令：指定程序路径和配置文件路径
ExecStart=/usr/local/bin/frps -c /etc/frp/frps.toml
# 如果程序崩溃，5秒后自动重启
Restart=always
RestartSec=5s
# 限制日志输出大小，防止占满磁盘
StandardOutput=syslog
StandardError=inherit

[Install]
WantedBy=multi-user.target

```
## 校内电脑配置
下载FRP、GOST
### 配置frpc.toml
```
serverAddr = "你的新加坡服务器IP"
serverPort = 7700
auth.token = "你在服务器设置的密码"

[[proxies]]
name = "campus-proxy"
type = "tcp"
localIp = "127.0.0.1"
localPort = 11080           # 对应下面 gost 开启的端口
remotePort = 10080         # 对应新加坡服务器开放的端口
```
### 编写启动脚本
```
@echo off
start /b gost.exe -L user123:pass123@:11080
start /b frpc.exe -c frpc.toml
echo 校园网中转服务已启动...
pause
```
### 配置服务
1. 按下 `Win + R`，输入 `taskschd.msc`。
2. 点击右侧的 **创建任务**。
3. **常规**：名称填“CampusProxy”，勾选“不管用户是否登录都要运行”。
4. **触发器**：点击新建，选择“制定计划时”改为“**启动时**”。
5. **操作**：点击新建，选择“启动程序”。
    - 程序或脚本：填入你的 `start.bat` 路径。
    - **起始于**：**必须填** `start.bat` 所在的目录路径（否则找不到配置文件）。
6. **条件**：取消勾选“只有在交流电源下才启动”（防止断电后笔记本模式不运行）。
7. 保存时会要求输入 Windows 登录密码。

**NSSM (Non-Sucking Service Manager)** 是 Windows 社区公认最像 `systemd` 的工具。它可以让你的程序在开机未登录时就运行，且崩溃自动重启。

1. **下载 NSSM**：
    - 去 [nssm.cc](https://nssm.cc/download) 下载并解压。
2. **安装 GOST 为服务**：
    - 以**管理员身份**打开 PowerShell，进入 nssm 所在目录。
    - 输入：`.\nssm.exe install GostService`
    - 在弹出的窗口中：
        - **Path**: 选择 `gost-windows-amd64.exe` 的路径。
        - **Arguments**: 填入 `-L user123:pass123@:11080`。
    - 点击 **Install service**。
3. **安装 FRPC 为服务**：
    - 输入：`.\nssm.exe install FrpcService`
    - 在弹出的窗口中：
        - **Path**: 选择 `frpc.exe` 的路径。
        - **Arguments**: 填入 `-c frpc.toml`。
    - 点击 **Install service**。
4. **启动服务**：
    - 在 Windows 搜索框搜“服务”，找到 `GostService` 和 `FrpcService`，右键点击**启动**。
    - 以后这台电脑只要通电，这两个程序就会在后台静默运行。

## 配置本地Clash
```
port: 7890
socks-port: 7891
allow-lan: true
mode: rule
log-level: info
external-controller: 127.0.0.1:9090
# 1. 节点信息（这里填你服务器的详细参数）
proxies:
  - name: "新加坡"
    type: ss
    server: 165.22.110.4
    port: 8388  # 你在服务器防火墙开放的那个端口
    cipher: aes-256-gcm
    password: "wang655971."
  - name: "校园内网"
    type: socks5
    server: 165.22.110.4
    port: 10080  # 你在服务器防火墙开放的那个端口
    username: suifeng1660
    password: wang655971
# 2. 策略组（决定怎么用这个节点）
proxy-groups:
  - name: 🚀 节点选择
    type: select
    proxies:
      - "新加坡"
      - "校园内网"
      - DIRECT
# 3. 规则（哪些网站走代理）
rules:
  # --- 校园网规则 ---
  - DOMAIN-SUFFIX,zju.edu.cn,校园内网
  - DOMAIN-SUFFIX,cc98.org,校园内网
  - IP-CIDR,10.0.0.0/8,校园内网
  # --- 国际互联网规则 ---
  - DOMAIN-SUFFIX,youtube.com,新加坡
  - DOMAIN-SUFFIX,github.com,新加坡
  - DOMAIN-SUFFIX,google.com,新加坡   # 新增：补充 Google
  # --- 兜底规则 ---
  - GEOIP,CN,DIRECT        # 中国 IP 直连
  - MATCH,🚀 节点选择       # 其他全部（如 Google 等）走节点选择里选中的那个
```

[[Clash规则解析]]

[[GOST与FRP介绍]]
