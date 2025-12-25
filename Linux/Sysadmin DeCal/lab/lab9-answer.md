### 加密文件
```
gpg -c -a --cipher-algo TWOFISH mydata.txt
```
- `gpg`: 调用程序。
- `-c` (或 `--symmetric`): 表示**对称加密**（使用密码，而不是公钥/私钥对）。
- `-a` (或 `--armor`): 表示输出为 **ASCII-armored** 格式（即文本格式，看起来像乱码，而不是二进制文件）。
- `--cipher-algo TWOFISH`: 强制使用 **TWOFISH** 算法（默认通常是 AES）。

### 创建PGP密钥
```
gpg --full-generate-key
```

```
gpg --armor --export "Your Name" > my_public_key.asc
```

### 导入密钥
```
gpg --import staff.asc
```

### 验证文件
```
gpg --verify directions.txt.sig directions.txt
```

### 让文件对所有者可读
```
chmod 400 filename
```

让文件只对我可读写
```
sudo chown me:me filename
chmod 600 filename
```
只有root可读，无人可写
```
sudo chown root:root filename
sudo chmod 400 filename
```
接管文件且对原主人不可见
```
sudo chown user:user filename
chmod 600 filename
```
只有我和decal组用户可见
```
sudo chown $(whoami):decal filea.txt fileb.txt

# 第二步：设置权限 (User=rw, Group=r, Others=-)
chmod 640 filea.txt fileb.txt
```
普通用户只能将文件的组更改为**自己所属的组**。