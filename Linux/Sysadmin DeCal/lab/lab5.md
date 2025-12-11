### 创建用户
`adduser 用户名`
创建用户

`usermod -aG sudo 用户名`
将用户添加到sudo组，是用户具有sudo权限

```
mkdir -p /home/suifeng1660/.ssh touch /home/suifeng1660/.ssh/authorized_keys chown suifeng1660:suifeng1660 /home/suifeng1660/.ssh -R chmod 700 /home/suifeng1660/.ssh chmod 600 /home/suifeng1660/.ssh/authorized_keys chmod 755 /home/suifeng1660
```
创建密钥目录及文件夹分配权限，使可以以普通用户身份ssh

`ssh-copy-id suifeng1660@服务器IP`
上传公钥

|命令|作用 + 为什么必须这么做|如果不这么做会怎样|
|---|---|---|
|`mkdir -p /home/suifeng1660/.ssh`|创建存放公钥的专用目录（隐藏目录）|没有这个目录，sshd 直接拒绝密钥登录|
|`touch /home/suifeng1660/.ssh/authorized_keys`|创建公钥文件（如果文件不存在就新建一个空文件）|没有这个文件，sshd 找不到公钥，直接拒绝|
|`chown suifeng1660:suifeng1660 /home/suifeng1660/.ssh -R`|把 .ssh 目录和里面的文件的所有者改成 suifeng1660 本人（而不能是 root）|所有者不对，sshd 会认为“有人想偷窥别人的密钥”，直接拒绝|
|`chmod 700 /home/suifeng1660/.ssh`|把 .ssh 目录权限设为“只有本人可读写执行，其他人完全不行”|权限太松（比如 755），sshd 直接拒绝登录（安全保护机制）|
|`chmod 600 /home/suifeng1660/.ssh/authorized_keys`|把公钥文件权限设为“只有本人可读写，其他人完全不能看”|权限不是 600 或 644，sshd 直接拒绝（防止别人偷你的公钥）|
|`chmod 755 /home/suifeng1660`|把用户家目录权限设为“本人可读写执行，其他人可读和进入” （755 是 Linux 家目录的标准权限）|太严（700）会导致某些程序找不到家目录；太松（777）会被 sshd 警告|


#### 700、600、755 这些数字到底是什么意思？（八进制权限）

Linux 用 3 个八进制数字表示权限，从左到右分别代表：

|数字|二进制|含义|实际权限表示|
|---|---|---|---|
|7|111|读 + 写 + 执行（rwx）|只有所有者能干任何事|
|6|110|读 + 写 （rw-）|所有者能读写，但不能执行|
|5|101|读 + 执行 （r-x）|所有者能读和进入，但不能写|
|4|100|只读 （r--）||
|0|000|什么权限都没有（---）||

一个完整的权限是 3 位，所以：

|权限数字|实际表现|常见用途|
|---|---|---|
|700|drwx------|~/.ssh 目录（必须最严格）|
|600|-rw-------|authorized_keys 文件（必须最严格）|
|644|-rw-r--r--|普通文件（所有人可读）|
|755|drwxr-xr-x|家目录、/etc、/usr 等目录的标准权限|
|777|drwxrwxrwx|所有人可读写执行（极度危险，几乎永远不要用）|
Linux 文件权限的三个数字（或四位，如 0755）分别对应 **三类不同身份的人**，从左到右严格顺序是：

|位置|对应身份|英文名称|说明|
|---|---|---|---|
|第 1 位|文件**所有者**|Owner / User|就是 ls -l 里第三列显示的那个人（比如 suifeng1660）|
|第 2 位|文件**所属组**|Group|ls -l 里第四列显示的组（比如 suifeng1660、sudo、www-data 等）|
|第 3 位|**其他人**|Others|所有不属于上面两类的用户（包括 root 在内，只要不是 owner 就算 others）|

- SSH 对权限要求极度严格（必须记住的“三板斧”）
    - 家目录：755
    - ~/.ssh：700
    - authorized_keys：600
    - 所有者必须是用户本人，不能是 root

#### 为什么 SSH 服务器（sshd）对权限这么变态？

因为 SSH 是远程登录的唯一入口，黑客最想攻破的地方。OpenSSH 故意把规则写死来防止以下攻击：

- 别人把公钥偷偷塞到你的 authorized_keys → 你用了 600，别人根本写不进去
- 你把 .ssh 目录权限开成 755 → 别人就能在里面放木马，sshd 直接不让你登录
- authorized_keys 属于 root 而不是你本人 → sshd 认为有人在搞“中间人”，直接拒绝

### 创建服务
```
git clone https://github.com/0xcf/decal-labs.git
cd labs/b6
sudo apt install build-essential make python3-virtualenv
./run
```
使用 Python virtualenv 启动一个简单的 Flask Web 服务器，监听端口 5000。

```
touch /etc/systemd/system/toy.service

[Unit] Description=Toy Web Server # 服务描述，便于 `systemctl status` 显示 Requires=network.target # 依赖网络（服务器需联网） After=network.target # 在网络启动后运行

[Install] WantedBy=multi-user.target # 系统引导到多用户模式时自动启用

[Service] ExecStart=/path/to/a5/run # 启动命令：绝对路径到 run 脚本 User=yourusername # 以非 root 用户运行（创建专用用户，避免 root 风险） WorkingDirectory=/path/to/a5 # 工作目录，确保相对路径有效 Restart=always # （稍后添加）崩溃时自动重启 RestartSec=10 # 重启延迟 10 秒，避免循环崩溃
```

```
systemctl start toy.service
systemctl enable toy.service
systemctl status toy.service
journalctl -u toy.service
```

> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [gk.zchat.tech](https://gk.zchat.tech/c/cdd4002d-eb44-4c02-8991-cf46c38f2d4a?rid=0593f25b-adec-4e9a-bdf7-2909df97f678)

> Grok is a free AI assistant designed by xAI to maximize truth and objectivity. Grok offers real-time ......

### 执行实验的学习收获总结

此实验的核心是通过构建和管理一个简单的 Python Web 服务器（“toy server”）来介绍 systemd——现代 Linux 发行版（如 Ubuntu）的默认 init 系统。systemd 负责服务的启动、停止、监控和自动恢复，取代了旧的 SysV init。该实验强调系统管理的实际技能：从手动启动服务，到自动化管理，再到故障恢复。通过逐步构建，您将学习如何将自定义应用转化为生产级服务，同时理解安全性和调试原则。

总体学习目标：

*   **理解服务管理基础**：服务（services）是后台进程（如 Web 服务器），systemd 提供声明式配置来管理它们，确保系统引导时可靠启动。
*   **实践系统安全**：避免 root 运行服务，强调最小权限原则。
*   **故障处理技能**：使用日志和监控工具诊断问题，配置自动重启以实现高可用性。
*   **Linux 运维思维**：实验鼓励实验（如添加自定义字段），培养 “试错 - 调试” 的 sysadmin 心态。

以下是对实验每个主要步骤的详细分析，包括其目的、技术原理和学习点。分析基于任务描述，并映射到实际执行（使用上述替代仓库）。

#### 1. 获取代码并安装依赖（git clone / wget + tar xvf + apt install）

*   **做什么**：下载实验材料（a5 目录，包含 run 脚本、Python 应用如 app.py——一个 Flask 服务器，支持 / 根路径和 /crash 端点）。然后更新包索引并安装构建工具（build-essential：GCC 等编译器）、Make（构建自动化）和 python3-virtualenv（隔离 Python 环境，避免全局依赖冲突）。
*   **为什么**：实验需要一个可运行的 Web 应用作为起点；依赖确保在干净 VM 上构建环境。
*   **学习点**：
    *   **包管理**：apt 是 Debian/Ubuntu 的工具，update 刷新仓库元数据，install 处理依赖链（e.g., virtualenv 需要 Python 头文件）。
    *   **隔离环境**：virtualenv 防止库版本冲突，教导 “沙箱” 原则——生产中常用 venv 或 Docker。
    *   **潜在问题**：如果 VM 无 sudo，需配置权限；学习 apt 输出解析（e.g., 警告 vs. 错误）。

#### 2. 启动 Web 服务器（./run）

*   **做什么**：执行 run 脚本，它创建 virtualenv、安装 Flask（轻量 Web 框架）、激活环境并运行 python app.py。服务器监听 5000 端口，提供基本 HTTP 响应；/crash 端点模拟故障。
*   **为什么**：验证手动启动应用，模拟真实 Web 服务（e.g., 处理 GET/POST 请求）。
*   **学习点**：
    *   **Web 基础**：理解 HTTP 服务器如何绑定端口（app.run(port=5000)），并需防火墙放行（ufw allow 5000）。
    *   **脚本自动化**：run 封装复杂命令（e.g., virtualenv env; source env/bin/activate; pip install flask），教导 shell 脚本在运维中的作用。
    *   **测试**：用 curl http://localhost:5000 检查；浏览器访问强化 “端到端” 验证。

#### 3. 创建 systemd 服务文件（/etc/systemd/system/toy.service）

*   **做什么**：使用文本编辑器（e.g., sudo nano /etc/systemd/system/toy.service）填充骨架：
    
    text
    
    Copy
    
    ```
    [Unit]
    Description=Toy Web Server  # 服务描述，便于 `systemctl status` 显示
    Requires=network.target     # 依赖网络（服务器需联网）
    After=network.target        # 在网络启动后运行
    
    [Install]
    WantedBy=multi-user.target  # 系统引导到多用户模式时自动启用
    
    [Service]
    ExecStart=/path/to/a5/run   # 启动命令：绝对路径到 run 脚本
    User=yourusername           # 以非 root 用户运行（创建专用用户，避免 root 风险）
    WorkingDirectory=/path/to/a5  # 工作目录，确保相对路径有效
    Restart=always              # （稍后添加）崩溃时自动重启
    RestartSec=10               # 重启延迟 10 秒，避免循环崩溃
    
    ```
    
    *   提示问题解答：
        *   **依赖**：systemctl --type=target 列出目标如 network.target（网络必须先就绪，否则服务器绑定失败）。
        *   **启动脚本**：ExecStart 指向 run，systemd 会 fork 执行。
        *   **安全**：默认 root 易受攻击（e.g., 漏洞利用提权）；用 User=toyuser（先 sudo useradd -r toyuser 创建系统用户）限制权限。
    
*   **为什么**：将手动 ./run 转化为 systemd 管理的持久服务，实现开机自启和监控。
*   **学习点**：
    *   **systemd 结构**：INI-like 格式——[Unit] 定义元数据 / 依赖，[Install] 控制启用，[Service] 指定执行。参考 DigitalOcean 指南学习更多字段（如 Environment= 设置变量）。
    *   **依赖管理**：After= 确保顺序（e.g., 无网络的服务器无意义）；Requires= 强制依赖。
    *   **安全最佳实践**：最小权限（least privilege）——非 root 用户减少攻击面；实验鼓励添加 PrivateTmp=true（隔离 /tmp）等。
    *   **自定义**：尝试 Type=simple（默认，前台进程）或 KillMode=process（仅杀主进程）。

#### 4. 启动并启用服务（systemctl start toy.service + systemctl enable toy.service）

*   **做什么**：start 立即运行服务（fork run）；enable 创建符号链接到 /etc/systemd/system/multi-user.target.wants/，确保重启后自动启动。
*   **为什么**：从手动到自动化过渡，模拟生产部署。
*   **学习点**：
    *   **systemctl 命令**：status toy 检查 PID / 日志；省略 .service 加速，但需指定类型（如 target）。
    *   **引导集成**：multi-user.target 是命令行模式；理解 systemd 的 “目标” 树（systemctl list-dependencies multi-user.target）。

#### 5. 调试服务（systemctl status + journalctl -u toy.service）

*   **做什么**：status 显示运行状态（active/running）、PID、最近日志；journalctl 查询单元日志（-u 过滤，-f 实时跟踪）。
*   **为什么**：服务可能失败（e.g., 端口冲突、权限错），需诊断。
*   **学习点**：
    *   **日志分析**：journalctl 是 systemd 的中心日志系统（二进制、结构化）；过滤如 -e（末尾）或 --since "2025-12-03"。教导 “arcane error messages” 解读（e.g., “Permission denied” → 检查 User=）。
    *   **sysadmin 心态**：鼓励不气馁，迭代调试（edit → systemctl daemon-reload → restart）。

#### 6. 崩溃服务并配置自动恢复（POST 到 /crash 或 kill + 添加 Restart=always）

*   **做什么**：
    *   崩溃：curl -X POST -d '{"crash":"true"}' http://localhost:5000/crash（JSON 触发 app 自杀）或 kill $(pgrep -f app.py)（信号杀进程）。
    *   编辑 toy.service 添加 Restart=always（任何退出码重启）和 RestartSec=10（延迟防风暴）。
    *   sudo systemctl daemon-reload 重新加载配置（不中断运行服务）；重启并崩溃，观察 10 秒后恢复（status 验证）。
*   **为什么**：演示 systemd 的高可用性——无需 cron 监控脚本。
*   **学习点**：
    *   **故障注入**：cURL 测试 API；kill 教信号（如 SIGTERM vs. SIGKILL）。
    *   **重启策略**：always 覆盖 exit 0-255；RestartSec 防资源耗尽。生产中用 on-failure 仅异常重启。
    *   **动态更新**：daemon-reload 热重载配置，体现 systemd 的灵活性。
    *   **问题 3 示例命令**：curl -X POST http://localhost:5000/crash -H "Content-Type: application/json" -d '{"crash":"true"}'。

#### 7. 最终输出（上传 toy.service 到 Gradescope）

*   **做什么**：保存完整文件（包含所有字段），提交以验证。
*   **为什么**：评估您的配置（e.g., 是否有安全 User=、正确依赖）。
*   **学习点**：版本控制思维——文件是 “声明即代码”（infrastructure as code）。

通过此实验，您将从初学者转向能独立管理服务的专业人士。预计 1-2 小时完成，重点在迭代调试。如果使用替代仓库遇到问题，提供 ls 输出，我可进一步指导。