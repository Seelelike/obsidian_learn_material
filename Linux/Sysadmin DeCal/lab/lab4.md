### 安装防火墙
```
apt install ufw
ufw default deny incoming
ufw default allow outgoing
ufw allow OpenSSH // ufw allow 22/tcp
ufw status numbered  //查看当前配置
ufw enable
```
<!--ID: 1764664440375-->


### 配置fail2ban防SSH暴力破解
```
apt install fail2ban
vim /etc/fail2ban/jail.local
[sshd]
enabled = true
port    = ssh
filter  = sshd
logpath = /var/log/auth.log
maxretry = 5
bantime  = 600
findtime = 600
backend  = systemd
<!--ID: 1764664440383-->


systemctl start fail2ban
```

查看当前端口
`echo $SSH_CONNECTION`可看到：
`你的本地IP  你的本地端口  服务器IP  服务器端口`

运行CherryMusic服务
```
apt install python3 python3-pip git
git clone https://github.com/devsnd/cherrymusic.git
cd cherrymusic
python3 cherrymusic --setup --port 8080

ufw allow 8080/tcp

mkdir -p ~/music
scp /path/to/your/song1.mp3 root@YOUR_DROPLET_IP:~/music/

后台运行
 apt install -y screen
cd ~/cherrymusic
screen -S cherrymusic python3 cherrymusic
按 `Ctrl+A` 然后 `D` 可以“挂起” screen 会话，CherryMusic 继续在后台跑。
```

