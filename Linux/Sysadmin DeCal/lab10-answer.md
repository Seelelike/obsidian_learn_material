### 安装docker
[on the Docker website](https://docs.docker.com/v17.09/engine/installation/linux/docker-ce/ubuntu/)
```
sudo systemctl status docker
sudo systemctl enable docker
sudo systemctl start docker
sudo usermod -aG docker $USER
```
问题：docker服务启动失败。
排除：sudo /usr/bin/dockerd
解决：
```
sudo systemctl stop docker
sudo systemctl stop docker.socket
sudo systemctl stop containerd
sudo rm -rf /var/run/docker.sock
sudo systemctl daemon-reload
sudo systemctl start docker.socket sudo systemctl start docker
```

### 创建交互式docker容器
`docker run -it ubuntu:xenial /bin/bash`
>`-i` 标志告诉 docker 保持 `STDIN` 打开以连接到你的容器， `-t` 标志为你分配一个伪 TTY。基本上你需要这两个标志才能输入文本并正确显示。在命令末尾， `/bin/bash` 只是你希望在容器启动后运行的命令。

`CTRL+D` 退出容器。

基础管理
#### 查看系统中正在运行的容器|`docker ps`

`-a`同时看到已停止的容器
每个容器都有一个唯一的容器 ID 和一个唯一的可读名称。

#### 获取容器日志|`docker logs`

`docker logs <container_id_or_name>`
这基本上只是为你提供 `stdout` 和 `stderr` ，用于容器中运行的进程。

#### 清除容器|`docker rm`
`docker rm <container_id_or_name>`
清除已退出且不再使用的容器

#### 查看已下载镜像|`docker images`
Docker 在运行镜像之前会下载大量数据。这就是容器的镜像。你可以使用 `docker images` 命令查看你已下载的所有镜像：
```
baisang@rapture ~/d/labs> docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
fedora              latest              9110ae7f579f        2 weeks ago         235MB
ubuntu              xenial              f975c5035748        3 weeks ago         112MB
```

#### 清除镜像|`docker rmi`
`docker rmi <image_id>`
镜像文件以及各种容器的文件系统都存储在 `/var/lib/docker` 中。

#### Dockerfile
Dockerfile 允许你通过指定所有手动输入的命令来定义一个镜像。Docker 随后可以根据指定的 Dockerfile 构建镜像。这些 Dockerfile 可以放入版本控制系统中，并将镜像作为二进制文件分发，以跟踪镜像的构建方式，同时保留预先构建的镜像。
[reference page](https://docs.docker.com/engine/reference/builder/)

```
FROM ubuntu:bionic

RUN apt-get update && apt-get install -y python3.6 --no-install-recommends

CMD ["/usr/bin/python3.6", "-i"]
```
我们指定了一个基础镜像 `ubuntu:bionic` （ubuntu 的 release 18.04）。然后我们指定应该运行 `RUN` 命令 `apt-get update` ，然后 `apt-get install python3.6` 以便安装 `python3.6` 。接着我们设置容器的默认命令 `CMD` 为在交互模式下运行 `python3.6` 解释器。

将上面的 Dockerfile 内容复制到一个名为 `Dockerfile` 的文件中。然后使用 Docker 以下命令来构建它。

```
docker build -t mypython:latest .
```
Docker 在当前目录中查找 Dockerfile 进行构建，并构建它。 `-t` 标志告诉它将此构建标记为 `mypython:latest` 。Docker 会在当前目录中查找 Dockerfile，因为您指定了 `.`

#### 分离容器与端口
```
docker run -d -p=5050:80 httpd
```
你并不总是希望容器以交互方式运行。例如，如果你在运行一个 Web 服务器，你可能希望容器持续运行，直到你明确要停止它。Docker 使用 `-d` 标志支持这种用例，该标志以分离模式启动容器。

Docker 为容器创建了一个独立的虚拟网络，因此你需要将主机端口转发到容器的端口（这被称为端口转发，或端口映射）。容器在端口 80 上监听，让我们尝试在运行容器时将主机机器的端口 5050 转发到容器的端口 80

`-p` 标志接受一个冒号分隔的 `HOST_PORT:CONTAINER_PORT` 对（实际上它可以接受很多更多的选项，但现在不用担心）。你应该能够访问 `<url_of_host_machine>:5050`

你可以实际“连接”到正在运行的容器中并执行更多命令，类似于 `docker run` 的工作方式。使用 `docker exec` 命令：
```
docker exec <container_id_or_name> <command>
docker stop <container_id_or_name>
docker restart <container_id_or_name>
```

#### docker compose
`docker-compose` 允许你定义需要多个容器才能运行的应用程序。例如，在一个 Web 应用中，你可能希望你的实际 Web 应用运行在一个容器中，而数据库运行在另一个容器中。

`docker-compose` 的一个优点是它会自动为你的容器设置一个网络，其中：
- 每个服务的容器都在网络上，并且可以从网络上的其他容器访问到
- 每个容器都可以通过其容器名称在网络中被发现

### 用docker-compose创建网盘服务
```
vim docker-compose.yml

version: '3.8'

services:
  db:
    image: mariadb:10.11
    container_name: nextcloud-db
    restart: unless-stopped
    command: --transaction-isolation=READ-COMMITTED --binlog-format=ROW
    environment:
      - MYSQL_ROOT_PASSWORD=强一点的root密码
      - MYSQL_DATABASE=nextcloud
      - MYSQL_USER=nextcloud
      - MYSQL_PASSWORD=强一点的db密码
    volumes:
      - db_data:/var/lib/mysql

  app:
    image: nextcloud:fpm
    container_name: nextcloud-app
    restart: unless-stopped
    depends_on:
      - db
    environment:
      - MYSQL_DATABASE=nextcloud
      - MYSQL_USER=nextcloud
      - MYSQL_PASSWORD=刚才那个db密码
      - MYSQL_HOST=db
    volumes:
      - nextcloud_data:/var/www/html

  web:
    image: nginx:stable
    container_name: nextcloud-web
    restart: unless-stopped
    depends_on:
      - app
    ports:
      - "8080:80"
    volumes:
      - nextcloud_data:/var/www/html:ro
      - ./nginx.conf:/etc/nginx/conf.d/default.conf:ro

volumes:
  db_data:
  nextcloud_data:
```

- **`version: '3.8'`**: 声明 Compose 文件的语法版本。不同的版本支持的功能略有不同，3.8 是目前比较通用且稳定的版本。
- **`services:`**: 这里面定义你要启动的所有容器。在这个例子里，我们有三个服务：`db`, `app`, `web`。
- **`volumes:`**: 定义“具名数据卷”，用于持久化保存数据（防止删除容器后数据丢失）。

```
  db:
    image: mariadb:10.11
    container_name: nextcloud-db
    restart: unless-stopped
    command: --transaction-isolation=READ-COMMITTED --binlog-format=ROW
    environment:
      - MYSQL_ROOT_PASSWORD=强一点的root密码
      # ... 其他变量
    volumes:
      - db_data:/var/lib/mysql
```
- **`db`**: 这是**服务名**。在 Docker 内部网络中，其他容器可以通过这个名字找到它（非常重要，后面会用到）。
- **`image`**: 类似于安装包，告诉 Docker 去下载哪个镜像。这里是 MariaDB 10.11 版本。
- **`container_name`**: 给容器起个好记的名字。如果不写，Docker 会自动生成一个类似 `nextcloud_db_1` 的名字。
- **`restart: unless-stopped`**: 守护策略。如果容器意外崩溃，或者服务器重启了，Docker 会自动把这个容器拉起来，除非你手动执行了 `docker stop`。
- **`environment`**: 设置环境变量。这是配置数据库密码、用户名最常用的方式。**注意：** 这里的密码一定要改。
- **`volumes`**: **关键点！**
    - 格式是 `宿主机路径:容器内路径`。
    - 这里写的是 `db_data:/var/lib/mysql`。
    - `db_data` 是一个在文件底部定义的**具名卷**。Docker 会在你的硬盘深处找个地方专门存这个卷。
    - `/var/lib/mysql` 是数据库在容器里存数据的地方。
    - **作用**：即使你删除了这个容器，`db_data` 里的数据还在，下次启动时数据依然存在。
	
```
  app:
    image: nextcloud:fpm
    # ...
    depends_on:
      - db
    environment:
      # ...
      - MYSQL_HOST=db
    volumes:
      - nextcloud_data:/var/www/html
```
- **`image: nextcloud:fpm`**: 注意这里选的是 `fpm` 版本。这个版本不带 Web 服务器，它需要配合 Nginx 使用（更加高性能的生产环境配置）。
- **`depends_on`**: 启动顺序控制。告诉 Compose：“先启动 `db`，再启动我”。
- **`MYSQL_HOST=db`**: **这是 Docker Compose 的魔法。**
    - Nextcloud 问：“数据库在哪里？”
    - 你填 `db`。
    - 因为它们在同一个 Compose 文件里，Docker 自动把它们放在同一个内部网络。Nextcloud 只要访问 `db` 这个主机名，Docker 就会自动把请求转发给上面的 MariaDB 容器。
- **`volumes`**: 使用了 `nextcloud_data` 卷，挂载到容器内的 `/var/www/html`（Nextcloud 的代码和用户文件都在这）。

```
  web:
    image: nginx:stable
    # ...
    depends_on:
      - app
    ports:
      - "8080:80"
    volumes:
      - nextcloud_data:/var/www/html:ro
      - ./nginx.conf:/etc/nginx/conf.d/default.conf:ro
```
- **`ports`**: 端口映射。
    - 格式：`"宿主机端口:容器端口"`。
    - `"8080:80"` 意味着当你访问你电脑的 `localhost:8080` 时，流量会被转发到这个 Nginx 容器的 `80` 端口。
- **`volumes`**: 这里有两个挂载，展示了两种不同的用法：
    1. `nextcloud_data:/var/www/html:ro`:
        - 这里复用了 `app` 服务用到的 `nextcloud_data` 卷。
        - **为什么？** 因为 Nginx 需要读取 Nextcloud 的静态文件（图片、CSS、JS）来直接发送给浏览器。
        - `:ro` 表示 Read-Only（只读），Nginx 只需要读，不需要改。
    2. `./nginx.conf:/etc/nginx/conf.d/default.conf:ro`:
        - 这是一个**Bind Mount（绑定挂载）**。
        - `./nginx.conf` 指的是你当前目录下的那个配置文件（对应步骤 4）。
        - 作用是把你在外面写好的配置文件，直接“映射”进容器里，覆盖掉 Nginx 默认的配置。

```
vim nginx.conf

server {
    listen 80;
    server_name _;

    root /var/www/html;
    index index.php index.html;

    client_max_body_size 512M;

    location / {
        try_files $uri $uri/ /index.php?$query_string;
    }

    location ~ \.php$ {
        include fastcgi_params;
        fastcgi_split_path_info ^(.+\.php)(/.+)$;
        fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
        fastcgi_pass app:9000;
    }

    location ~ /\.ht {
        deny all;
    }
}
```

#### 启动docker compose
```
docker-compose up -d
```
- **`up`**: 告诉 Compose 根据当前目录下的 `docker-compose.yml` 创建网络、创建卷、拉取镜像、启动容器。
- **`-d`**: Detached mode（后台运行）。如果不加这个，容器的日志会直接霸占你的终端窗口，你按 Ctrl+C 容器就停了。