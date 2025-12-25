> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [lmarena.ai](https://lmarena.ai/zh/c/019b4ada-cc98-75e7-9010-eafd97619cbb)

> An open platform for evaluating AI through human preference

这段代码是 Clash 配置文件的核心——**分流规则 (Rules)**。它的作用是告诉 Clash：**当你想访问一个地址时，应该走哪条路。**

Clash 匹配规则的逻辑是：**从上到下，一旦命中，立即执行。**

下面详细拆解其语法和每一行的含义：

### 1. 语法的基本结构

每一行规则的标准格式如下：  
`- 类型, 匹配内容, 目标策略`

*   **类型**：指定 Clash 检查什么（域名、IP、还是关键字）。
*   **匹配内容**：具体的网址、IP 段或关键词。
*   **目标策略**：如果匹配成功，交给谁处理。可以是具体的节点（如 `校园内网隧道`），也可以是策略组（如 `🚀 节点选择`），或者是系统内置动作（如 `DIRECT` 直连、`REJECT` 拒绝）。

### 2. 逐行深度解析

#### 第一行：域名后缀匹配

`- DOMAIN-SUFFIX, edu.cn, 校园内网隧道`

*   **类型 (DOMAIN-SUFFIX)**：代表 “域名后缀匹配”。
*   **匹配内容 (edu.cn)**：指所有以 `.edu.cn` 结尾的域名。
    *   例如：`pku.edu.cn`、`lib.tsinghua.edu.cn`、`mail.sjtu.edu.cn`。
*   **目标 (校园内网隧道)**：只要发现网址是以 `edu.cn` 结尾，就强制通过你建立的那个 FRP 隧道去访问。
*   **意义**：保证所有中国教育网的资源都走校园网出口。

#### 第二行：IP 段匹配

`- IP-CIDR, 10.0.0.0/8, 校园内网隧道`

*   **类型 (IP-CIDR)**：代表 “无类别域间路由”。简单说，就是匹配一个 **IP 地址范围**。
*   **匹配内容 (10.0.0.0/8)**：这是私有局域网最常见的 IP 段。
    *   `10.0.0.0/8` 涵盖了从 `10.0.0.0` 到 `10.255.255.255` 的所有 IP。
    *   很多学校内部系统的访问地址直接就是 IP（比如 `http://10.1.2.3`），它们没有域名。
*   **目标 (校园内网隧道)**：当你访问这些内网 IP 时，走隧道。
*   **注意**：这里的 `/8` 是子网掩码。如果你学校内网用的是 `172.16.x.x`，则应改为 `172.16.0.0/12`。

#### 第三行：全匹配（兜底规则）

`- MATCH, DIRECT`

*   **类型 (MATCH)**：这是一个特殊类型，代表 “剩下的所有流量”。
*   **目标 (DIRECT)**：代表 “直连”，即不走任何代理，直接用你当前所在位置（家里、咖啡馆）的网络上网。
*   **语法逻辑**：因为它放在最后一行，所以如果一个请求不符合上面的 `edu.cn`，也不符合 `10.x.x.x`，它就会落到这一行，执行直连。
*   **意义**：防止你访问百度、腾讯视频等普通网站也去绕道学校，导致网速变慢。

### 3. 如果想加入 “科学上网” 该怎么写？

如果你想同时实现 “访问校内走隧道” 和“访问 Google 走新加坡代理”，顺序非常重要：

```
# 强制校内域名不解析 Fake-IP (如果没做 Filter) 并直连
- DOMAIN-SUFFIX,zju.edu.cn,DIRECT
- DOMAIN-SUFFIX,cc98.org,DIRECT
# 强制校内 IP 段直连，且不进行 DNS 解析 (no-resolve)
- IP-CIDR,10.0.0.0/8,DIRECT,no-resolve
```

### 4. 常见类型速查表

<table node="[object Object]"><thead><tr><th node="[object Object]">类型</th><th node="[object Object]">例子</th><th node="[object Object]">描述</th></tr></thead><tbody><tr><td node="[object Object]"><strong>DOMAIN</strong></td><td node="[object Object]"><code class="rounded px-1.5 py-0.5 bg-surface-tertiary font-mono text-[0.9em] text-text-primary font-medium" node="[object Object]">google.com</code></td><td node="[object Object]">精确匹配域名（只有 google.com 走，<a href="https://www.google.com">www.google.com</a> 不走）</td></tr><tr><td node="[object Object]"><strong>DOMAIN-SUFFIX</strong></td><td node="[object Object]"><code class="rounded px-1.5 py-0.5 bg-surface-tertiary font-mono text-[0.9em] text-text-primary font-medium" node="[object Object]">edu.cn</code></td><td node="[object Object]">匹配后缀（xxx.edu.cn 全走）</td></tr><tr><td node="[object Object]"><strong>DOMAIN-KEYWORD</strong></td><td node="[object Object]"><code class="rounded px-1.5 py-0.5 bg-surface-tertiary font-mono text-[0.9em] text-text-primary font-medium" node="[object Object]">google</code></td><td node="[object Object]">域名里只要包含 google 这几个字母就走</td></tr><tr><td node="[object Object]"><strong>IP-CIDR</strong></td><td node="[object Object]"><code class="rounded px-1.5 py-0.5 bg-surface-tertiary font-mono text-[0.9em] text-text-primary font-medium" node="[object Object]">192.168.1.0/24</code></td><td node="[object Object]">匹配特定的局域网或公网 IP 段</td></tr><tr><td node="[object Object]"><strong>GEOIP</strong></td><td node="[object Object]"><code class="rounded px-1.5 py-0.5 bg-surface-tertiary font-mono text-[0.9em] text-text-primary font-medium" node="[object Object]">CN</code></td><td node="[object Object]">根据地理位置 IP 库判断，如果是中国 IP 则执行</td></tr><tr><td node="[object Object]"><strong>MATCH</strong></td><td node="[object Object]">(无内容)</td><td node="[object Object]">放在最后一行，匹配所有未被上面规则命中的流量</td></tr></tbody></table>

**总结：** 规则的写法决定了 Clash 的 “智能程度”。