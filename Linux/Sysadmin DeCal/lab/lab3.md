```
sudo apt install --no-install-recommends qemu-kvm qemu-utils libvirt-daemon-system libvirt-clients virtinst ovmf
```
安装包，以创建Arch Linux虚拟机
>QEMU 是一个运行虚拟机的系统，它可以使用 KVM，KVM 是一个支持底层虚拟化的内核模块。我们可以通过包 `qemu-kvm` 来安装这个。 `libvirt` （通过 `libvirt-daemon-system and libvirt-clients` 安装）是一个管理系统，用于管理这些 QEMU/KVM 虚拟机。 `qemu-utils` 提供了创建虚拟驱动器所需的工具，并由 `virt-install` 使用。 `virtinst` 将提供脚本 `virt-install` ，我们将用它来创建虚拟机。我们添加 `--no-install-recommends` ，因为 `virtinst` 有一个推荐的依赖项 `virt-viewer` ，它依赖于 X11 系统。然而，如果只有 GUI（我们不需要！）才有用的这些包很多，所以我们想跳过推荐的依赖项。最后， `ovmf` 是一个额外的库，它将让我们使用 UEFI（而不是较旧的 BIOS 规范）来启动虚拟机。

```
wget 'http://ca.us.mirror.archlinux-br.org/iso/2021.09.01/archlinux-2021.09.01-x86_64.iso'
```
从**镜像**网站获取包

```
sudo virt-install --name archvm --memory 575 --cpu host --vcpus 1 --disk size=5 --network network=default --boot uefi --graphics none --cdrom archlinux-2021.09.01-x86_64.iso
```
启动虚拟机
- 报错
	`ERROR    Requested operation is not valid: network 'default' is not active`
	解决：
		`virsh net-list --all`
		Name      State      Autostart
		default   inactive   no
		`virsh net-start default`尝试启动
		出现错误：
		`error: Cannot check dnsmasq binary /usr/sbin/dnsmasq: No such file or directory`
		原因：
			libvirt 想用 `/usr/sbin/dnsmasq` 来给虚拟机做 **NAT/DHCP**，但系统里根本没有这个可执行文件，所以 **default 网络启动不了**。libvirt 的 `default` 网络是一个 NAT 网络，会自动用 dnsmasq 起一个小 DHCP/DNS 服务，让虚拟机拿到 IP。没有 dnsmasq，它就不干了，`virsh net-start default` 就失败。
		解决：
			`apt install dnsmasq-base`
- 使用uefi启动失败
	选择UEFI QEMU DVD-ROM QM00001启动，始终在同一界面刷新
	UEFI按照设备路径访问光驱，尝试从光盘上的 EFI 引导入口加载一个.efi程序
	Secure Boot 认为这个 `.efi` 没有合法签名；启动失败
	解决：
	进入`Device Manager-Secure Boot`关闭`Attempt Secure Boot`
	 - 进入Shell，UEFI 固件没有成功从 DVD 启动。
		`map -r`发现没有FS0，UEFI 固件虽然看到了一个“块设备”（光驱/硬盘），但没能在上面识别出可用的文件系统。
		```
		root@ubuntu-s-2vcpu-4gb-sfo3-01:~# virsh dumpxml archvm1 | grep -A5 cdrom <disk type='file' device='cdrom'> <driver name='qemu' type='raw'/> <target dev='sda' bus='sata'/> <readonly/> <address type='drive' controller='0' bus='0' target='0' unit='0'/> </disk>
		```
		没有**\<source file>**,关键问题。
		解决：
		在对应位置添加路径。`connect -r`后识别处FS0
		```
		Shell> fs0:
FS0:/> ls
FS0:/> cd EFI/BOOT
FS0:/EFI/BOOT> BOOTX64.EFI
		```
		成功启动
- 改用bios启动
	ERROR Disk /var/lib/libvirt/images/archvm.qcow2 is already in use by other guests \['archvm'].
	原文件被先前创建的系统占用
	`virsh destroy archvm 2>/dev/null || true` `virsh undefine archvm --nvram`
	需要 --nvram
	>libvirt 就给这个虚拟机配了 **UEFI 固件 + 一个独立的 NVRAM 文件**（里面保存“BIOS 设置”、启动顺序之类的状态，跟真机很像）。不加，libvirt 会认为你可能还想保留这个 NVRAM 配置，所以拒绝 undefine
- 启动后报错
	```
	[    2.684458] Initramfs unpacking failed: write error
	[    3.361459] Kernel panic - not syncing: VFS: Unable to mount root fs on unknown-block(0,0)
	```
	分配的**内存**不够解压当前**Arch iso的initramfs**

`fdisk /dev/sda`
开始分区
- 报错`fdisk: cannot open /dev/sda: No such file or directory`
	>教材环境可能用的是 SATA/IDE 磁盘（Linux 里叫 `/dev/sdX`）
	你这边 virt-install 默认用的是 **virtio 磁盘**，名字就是 `/dev/vdX`

```shell
root@archiso ~ # fdisk -l
Disk /dev/vda: 5 GiB, 5368709120 bytes, 10485760 sectors //5G 虚拟硬盘
...
Disk /dev/loop0: 961.48 MiB, 1008189440 bytes, 1969120 sectors //这是 **Arch 安装 ISO 的内容**，相当于光盘
...
```

### 磁盘分区
`Command (m for help): g`
创建GPT表
<!--ID: 1764664440387-->


```
Command (m for help): n
Partition number (1-128, default 1):   ← 直接回车用默认 1
First sector ...:                      ← 直接回车用默认
Last sector ... +sectors or +size{K,M,G,T,P}: +512M
```
- `/dev/vda1` 大约 **512M**，Type：**EFI System**
- `/dev/vda2` 大约 **4G**，Type：**Linux filesystem**
- `/dev/vda3` 大约 **512M**，Type：**Linux swap**

### 创建文件系统
`mkfs.fat -F32 /dev/sda1`
创建FAT32文件系统
<!--ID: 1764664440391-->


`cryptsetup -y -v luksFormat /dev/sda2`
加密分区

`cryptsetup open /dev/sda2 cryptroot`
打开分区

`mkfs.ext4 /dev/mapper/cryptroot`
创建ext4文件系统

`mkswap /dev/sda3` `swapon /dev/sda3`
创建交换空间并启用

### 挂载文件系统
>挂载是什么意思？ `man` 命令的 `mount` 页面提供了一个很好的描述：“Unix 系统中所有可访问的文件都排列在一个大的树状结构中，即文件层次结构，其根目录为 /。这些文件可以分布在多个设备上。挂载命令用于将某个设备上的文件系统连接到大的文件树中。相反，umount(8) 命令会将其断开连接。” 我们目前处于 Arch live 环境的“文件树”中，其中不包括我们为未来 Arch 安装创建的新 ESP 和根文件系统。使用 `mount` ，我们可以将这些文件系统连接到我们的 live 环境文件系统中，并像访问挂载点的目录一样访问它们。一旦 ESP 和根文件系统对当前文件系统可见，我们就可以开始将引导加载程序安装到 ESP 文件系统中，并将我们的永久 Arch OS 安装到根文件系统中。
```
mount /dev/mapper/cryptroot /mnt
mkdir /mnt/boot
mount /dev/sda1 /mnt/boot
```
<!--ID: 1764664440396-->

### 安装系统
过程参考：[安装指南 - ArchWiki --- Installation guide - ArchWiki](https://wiki.archlinux.org/title/Installation_guide#Installation)
```
pacstrap -K /mnt \
  base linux linux-firmware \
  nano vim \
  networkmanager \
  man-db man-pages texinfo
```
把新系统安装进/mnt
<!--ID: 1764664440400-->


`genfstab -U /mnt >> /mnt/etc/fstab`
生成的 `/mnt/etc/fstab` 记录了：
- “UUID=xxx 挂到 /，类型 ext4，选项 …”
- “UUID=yyy 挂到 /boot”
- “UUID=zzz 作为 swap”
以后系统启动时就根据这个文件自动挂载。

`arch-chroot /mnt`
假装 `/mnt` 是新的根 `/`，进去以后你就像在新系统里一样。此时我们从 live 环境“钻进”新系统里，配置它的各种文件。

通过在/etc文件夹中生成各种配置文件，对系统进行时间、网络等基础配置。

启动前小系统
```
/etc/mkinitcpio.conf
HOOKS=(base udev autodetect ...)
mkinitcpio -p linux
```
生成基于改配置文件的新initramfs，告诉initramfs支持磁盘加密

### Boot loader
`bootctl --path=/boot install`
安装systemd-boot到ESP上
```
/boot/loader/loader.conf
  default  arch
  timeout  3
  editor   no
  
/boot/loader/entries/arch.conf
title   Arch Linux
linux   /vmlinuz-linux
initrd  /initramfs-linux.img
options cryptdevice=UUID=290d6a44-2964-48a0-a71e-ea3df0525987:cryptroot root=/dev/mapper/cryptroot rw console=ttyS0
```
- `/boot` 是你的 ESP（EFI System Partition）
- UEFI 固件会从 `/EFI/BOOT/BOOTX64.EFI` 启动 → 这是 systemd-boot
- systemd-boot 再根据 `loader.conf` 和 `entries/*.conf`：
    - 显示菜单
    - 找到内核与 initramfs
    - 把 `options` 里的内核参数传给内核
`cryptdevice=UUID=...:cryptroot` + `root=/dev/mapper/cryptroot` 这组合就是告诉 initramfs：
- 解锁哪个 LUKS 容器
- 解锁出来的设备叫 cryptroot
- 根文件系统在 `/dev/mapper/cryptroot`
<!--ID: 1764664440403-->


