为什么需要包/如果没有包会带来的问题
- 文件放在哪里
- 应该如何更新
- 如何解决依赖
- 如何卸载
- 安全问题

软件包包含
- 软件需要的各种文件
- 元数据
	- 包的名字
	- 版本号
	- 依赖哪些包
	- 一些说明信息

apt安装包的过程
- 读取包列表
- 查找包的依赖项
- 检查已安装包
- 下载包
- 解包
- 处理安装后脚本

Apps（App Store 模式）： 
- 开发者直接向用户推更新； 
- 可以做“小范围灰度测试”（canary）； 
- 审批流程少，上线快，但有时不太稳定。 
Packages（发行版仓库）： 
- 更新要经过维护者审核； 
- 会避免“破坏性更新”，必要时推迟； 
- 想用新版本的用户可以选testing/unstable 或 backports 等“更激进的通道”。

周期发布与滚动发行

包结构
- control：元数据
	- 包大小
	- 版本号
	- 依赖列表
- usr/bin/
	- 包提供的可执行命令
	- 该目录会被添加进\$Path
- usr/share/
	- 文档、man手册、本地化文件等数据类东西
- /etc
全局配置文件。
- md5sums
各文件校验和，用于验证文件是否被破坏或篡改。
.deb 本质上就是一个 ar 压缩档，可以用 apt download 拉下来，再用 ar x 解开，里面有 control 信息和数据文件。

仓库配置
`deb http://mirrors/debian/ stretch-backports main contrib non-free`
- deb：二进制包来源（deb-src 是源码包）。 
- URL：服务器地址。 
- stretch-backports：对应 Debian 的版本 + backports 仓库。 
- main：只含 DFSG 自由软件； 
- contrib：本身自由，但依赖非自由软件； 
- non-free：本身不符合 Debian 自由软件指南。
