### 新建专用用户
```
sudo apt update && sudo apt -y upgrade
sudo adduser minecraft
sudo usermod -aG sudo minecraft
```
为什么要专用用户
- 安全，避免用root跑服务，避免被攻击者拿到root权限。
- 隔离，把游戏服文件与系统文件分开，不会把文件写到系统目录，权限边界更清晰。
- 稳定，避免权限乱套，如果一会root启动、一会普通用户启动，很容易出现- 文件属主变来变去（root:minecraft / minecraft:minecraft 混杂）插件写配置失败、日志写不进去、世界保存报错 因为进程没权限写某些文件。
- 运维，systemd管理更清楚，在 `minecraft.service` 里写User=minecraft WorkingDirectory=/home/minecraft/server，- 进程权限固定、服务目录固定、重启/开机自启更可控、日志、限制（打开文件数、CPU/内存限制）都能针对这个服务设置
- 更方便做权限控制，依赖“专用用户”作为隔离边界。

### 防火墙
```
sudo ufw allow OpenSSH
sudo ufw allow 25565/tcp
sudo ufw enable
sudo ufw status
```

### 安装Java
```
PROJECT="paper"
MINECRAFT_VERSION="1.21.10"
USER_AGENT="my-mc-server/1.0 (mailto:you@example.com)"

sudo apt install -y wget jq

URL=$(curl -s -H "User-Agent: $USER_AGENT" \
  "https://fill.papermc.io/v3/projects/${PROJECT}/versions/${MINECRAFT_VERSION}/builds" \
  | jq -r 'first(.[] | select(.channel == "STABLE") | .downloads."server:default".url) // "null"')

wget -O server.jar --user-agent="$USER_AGENT" "$URL"
```
这是一个复合命令，用于从 PaperMC 的 API 获取最新稳定构建的下载 URL，并存入变量 URL。

- curl -s：静默模式请求 API 端点 https://fill.papermc.io/v3/projects/paper/versions/1.21.10/builds，返回该版本所有构建的 JSON 列表。
- -H "User-Agent: $USER_AGENT"：携带自定义 User-Agent，避免被 API 拒绝。
- jq -r：解析 JSON：
    - .\[]：遍历 builds 数组。
    - select(.channel == "STABLE")：只选择 channel 为 "STABLE" 的构建（避免实验性或不稳定构建）。
    - first(...)：取第一个匹配的（通常是最新稳定构建，因为 API 默认按时间倒序）。
    - .downloads."server:default".url：提取默认服务端 JAR 的下载 URL。
    - // "null"：如果没找到，返回 "null"（防止脚本出错）。
- 最终 URL 会像 https://fill-data.papermc.io/.../paper-1.21.10-XXX.jar 这样。

wget -O server.jar --user-agent="\$USER_AGENT" "$URL"

- 使用 wget 下载该 URL 的文件：
    - -O server.jar：保存为文件名 server.jar（覆盖同名文件）。
    - --user-agent="$USER_AGENT"：同样携带 User-Agent（下载实际文件时也需要）。
    - "$URL"：下载链接，如果 URL 为 "null" 则会失败并报错。

编写启动脚本
```
cat > start.sh << "EOF"
#!/usr/bin/env bash
cd "$(dirname "$0")"

# 4GB RAM 推荐：最大 3G，留点给系统
# 如果你是 2GB RAM：把 -Xmx3G 改成 -Xmx1500M 或 -Xmx1600M
java -Xms2G -Xmx3G -jar paper.jar --nogui
EOF
chmod +x start.sh

# ====== 首次启动：生成配置与 eula.txt（可能会退出提示同意 EULA）======
set +e
./start.sh
set -e

# ====== 自动同意 EULA ======
if [ -f eula.txt ]; then
  sed -i "s/eula=false/eula=true/g" eula.txt
  echo "[INFO] EULA accepted (eula=true)."
else
  echo "[ERROR] 没生成 eula.txt，说明服务端没有正常启动到生成阶段。请检查 Java/权限/日志。"
  exit 1
fi
```
`eula.txt`：法律协议开关，未同意就不允许启动
### 创建systemd服务
```bash
sudo cat > /etc/systemd/system/minecraft.service <<'EOF'
[Unit] Description=Minecraft Paper Server # 服务描述：Minecraft Paper 服务端 After=network.target # 在网络可用后启动

[Service] Type=simple # 服务类型：简单（主进程就是 ExecStart 启动的进程） User=minecraft # 以 minecraft 用户运行（安全，避免用 root） WorkingDirectory=/home/minecraft/server # 工作目录：服务端文件所在路径（server.jar、world 等在这里） 
ExecStart=/home/minecraft/server/start.sh # 启动命令：执行 start.sh 脚本（通常脚本里是 java -jar server.jar nogui 等） 
Restart=on-failure # 如果崩溃或异常退出，自动重启 RestartSec=5 # 重启前等待 5 秒 LimitNOFILE=100000 # 提高文件描述符上限（Minecraft 服务端需要打开很多文件，尤其是多人服务器）

[Install] WantedBy=multi-user.target # 开机自启时依赖的多用户模式
EOF
```

### 添加白名单
```
NAME="ISeeIe"

# 1) 停服
sudo systemctl stop minecraft

# 2) 查 UUID（正版/online-mode=true 适用）
ID=$(curl -s "https://api.mojang.com/users/profiles/minecraft/$NAME" | jq -r .id)
UUID=$(echo "$ID" | sed -E 's/(.{8})(.{4})(.{4})(.{4})(.{12})/\1-\2-\3-\4-\5/')
echo "UUID=$UUID"

# 3) 追加到 whitelist.json（没有文件就新建）
sudo -u minecraft -H bash -lc "cd ~/server && \
  ( [ -f whitelist.json ] || echo '[]' > whitelist.json ) && \
  jq --arg uuid \"$UUID\" --arg name \"$NAME\" \
  '(. + [{uuid:\$uuid, name:\$name}]) | unique_by(.uuid)' \
  whitelist.json > whitelist.json.tmp && mv whitelist.json.tmp whitelist.json && \
  cat whitelist.json"
  ```

### 为什么ssh退出，服务器就关了
>你的 Minecraft 进程“依附在 SSH 终端上”，SSH 一断，系统就把它一起杀掉了。

当使用ssh登陆时，linux会创建一个登陆会话与控制终端，在该终端启动的所有程序都挂在这个终端下。
```
SSH 会话
 └── bash（你的 shell）
     └── java（Minecraft 服务器）
```
为什么 systemd 就不会被 SSH 影响?
用 systemd 启动服务时,发生的是：
```
systemd（PID 1）
 └── minecraft.service
     └── java（Minecraft）
```