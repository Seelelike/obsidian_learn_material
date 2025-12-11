> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [chat.zchat.tech](https://chat.zchat.tech/c/6912b03b-a5bc-832a-8a72-7c478c14034b)

> ChatGPT helps you get answers, find inspiration and be more productive. It is free to use and easy to......

好的，这一节（**7.4 Code: mycpu and myproc**）讲的是：

> xv6 是如何在多核（多 CPU）环境下正确找到 “当前 CPU” 与“当前进程”的。

* * *

一、问题背景：多核系统下 “当前进程” 不再唯一
------------------------

在单核系统中，内核可以简单地定义一个全局变量：

```
struct context {
  uint64 ra;
  uint64 sp;
  uint64 s0; … uint64 s11;
};
```

因为**任何时刻只有一个 CPU 在运行一个进程**。

但在多核系统中，每个 CPU 都可能在执行不同的进程，因此：

*   `current_proc` 不再唯一；
    
*   每个 CPU 都必须维护自己独立的 “当前进程” 指针。
    

xv6 的解决方式是：

*   为 **每个 CPU 建立一个 `struct cpu` 结构**；
    
*   其中记录：
    
    *   当前正在运行的进程 `struct proc *proc`；
        
    *   调度器线程上下文（scheduler 的寄存器）；
        
    *   嵌套自旋锁计数（防止中断嵌套出错）等。
        

* * *

二、`mycpu()`：找到 “当前 CPU 的 cpu 结构”
--------------------------------

### 1️⃣ xv6 如何区分不同的 CPU？

RISC-V 给每个 CPU 分配了一个唯一编号，称为 **hartid**（Hardware Thread ID）。
<!--ID: 1764664441518-->


xv6 让每个 CPU 在进入内核时，  
**把自己的 hartid 保存在 RISC-V 的 `tp` 寄存器**中。

这样：

*   不同 CPU 的 `tp` 值不同；
    
*   xv6 可以用 `tp` 来 “索引” 一个全局的 `cpu[]` 数组，  
    从而快速找到该 CPU 对应的 `struct cpu`。
    

* * *

### 2️⃣ 谁负责设置 `tp`？

`tp` 的设置在系统引导时完成：
<!--ID: 1764664441521-->


*   **在机器模式（machine mode）启动阶段**，`mstart()`（位于 `kernel/start.c`）给当前 CPU 设置 `tp = hartid`。
    
*   用户进程在运行时可能修改 `tp`（因为用户态程序也能用这个寄存器），  
    所以在：
    
    *   `usertrapret()` 中，xv6 会**保存当前内核的 `tp` 值**；
        
    *   当从用户态陷入内核态（`uservec`）时，又**恢复这个保存的 `tp`**。
        

这样，内核进入时总能保证：

> 每个 CPU 的 `tp` 寄存器都正确地反映出该 CPU 的编号。

* * *

### 3️⃣ `mycpu()` 如何实现？

核心逻辑：
<!--ID: 1764664441525-->


```
RUNNING（A要成立）
  寄存器在CPU；c->proc=我
  |
  | yield() 获取 p->lock，改 RUNNABLE，→ sched() → swtch 到 scheduler
  V
[过渡期：A不成立，B也未就绪 —— 必须持 p->lock]
  scheduler on its stack
  清 c->proc；确认寄存器在 p->context；无人用其内核栈
  现在 B 成立 → 释放 p->lock
  |
  | 选新进程，获取其 p->lock，标记 RUNNING，设 c->proc，swtch 到它
  V
RUNNING（A再次成立）
  在它的内核栈与寄存器上继续执行；适时释放 p->lock
```

**注意：**

> RISC-V 的 “读取 hartid” 指令只能在机器模式执行。  
> xv6 只能通过提前写入 `tp` 的方式来在 supervisor 模式下区分 CPU。

* * *

三、`myproc()`：找到 “当前 CPU 正在运行的进程”
--------------------------------

`myproc()` 依赖于 `mycpu()`：

```
struct proc *current_proc;
```

* * *

四、为什么必须**关闭中断**？
----------------

这是 xv6 的一个关键防御性设计。

考虑这样一种情况：

*   当前代码调用 `mycpu()` → 拿到当前 CPU 的指针；
    
*   但在取 `c->proc` 之前，一个时钟中断来了；
    
*   中断触发调度，当前进程被切换到别的 CPU；
    
*   那么你拿到的 `c` 就不再是 “当前 CPU” 的了！
    

因此：

*   **在 `mycpu()` 和 `myproc()` 中必须禁用中断**；
    
*   保证函数执行期间不会被切走；
    
*   等取完指针再恢复中断。
    

* * *

五、为什么 `myproc()` 返回的指针仍然 “安全”？
------------------------------

虽然 `myproc()` 执行时会临时关闭中断，  
但拿到 `struct proc *p` 后，你就可以再开启中断。

原因是：

> `struct proc *` 指针所指的结构体在调度过程中不会变。

即使这个进程从 CPU0 被迁移到 CPU1：

*   它的 `struct proc` 结构体在内存中位置不变；
    
*   所以 `p` 指针依然指向同一个进程。
    

* * *

六、总结：mycpu/myproc 设计的逻辑关系
-------------------------

<table data-start="2129" data-end="2499"><thead data-start="2129" data-end="2154"><tr data-start="2129" data-end="2154"><th data-start="2129" data-end="2134" data-col-size="sm">项目</th><th data-start="2134" data-end="2139" data-col-size="sm">功能</th><th data-start="2139" data-end="2146" data-col-size="sm">关键机制</th><th data-start="2146" data-end="2154" data-col-size="sm">注意事项</th></tr></thead><tbody data-start="2194" data-end="2499"><tr data-start="2194" data-end="2264"><td data-start="2194" data-end="2209" data-col-size="sm"><code data-start="2196" data-end="2208">struct cpu</code></td><td data-col-size="sm" data-start="2209" data-end="2223">每个 CPU 的控制块</td><td data-col-size="sm" data-start="2223" data-end="2255">保存当前运行的进程指针 <code data-start="2237" data-end="2243">proc</code>、调度器上下文、锁计数</td><td data-col-size="sm" data-start="2255" data-end="2264">每个核一个</td></tr><tr data-start="2265" data-end="2345"><td data-start="2265" data-end="2277" data-col-size="sm"><code data-start="2267" data-end="2276">mycpu()</code></td><td data-col-size="sm" data-start="2277" data-end="2303">找到当前 CPU 的 <code data-start="2290" data-end="2302">struct cpu</code></td><td data-col-size="sm" data-start="2303" data-end="2334">利用 RISC-V 的 <code data-start="2317" data-end="2321">tp</code> 寄存器存 hartid</td><td data-col-size="sm" data-start="2334" data-end="2345">调用时要禁中断</td></tr><tr data-start="2346" data-end="2421"><td data-start="2346" data-end="2359" data-col-size="sm"><code data-start="2348" data-end="2358">myproc()</code></td><td data-col-size="sm" data-start="2359" data-end="2378">找到当前 CPU 当前运行的进程</td><td data-col-size="sm" data-start="2378" data-end="2404">先 <code data-start="2382" data-end="2391">mycpu()</code> → <code data-start="2394" data-end="2403">c-&gt;proc</code></td><td data-col-size="sm" data-start="2404" data-end="2421">调用期间禁中断，之后可恢复</td></tr><tr data-start="2422" data-end="2499"><td data-start="2422" data-end="2432" data-col-size="sm"><code data-start="2424" data-end="2428">tp</code> 设置</td><td data-col-size="sm" data-start="2432" data-end="2457">启动时设定 hartid → 保存在寄存器中</td><td data-col-size="sm" data-start="2457" data-end="2468">在进入内核时恢复</td><td data-col-size="sm" data-start="2468" data-end="2499">用户态可能修改，需在 trampoline 保存 / 恢复</td></tr></tbody></table>

* * *

### 💡 整体工作流程（从硬件到内核）：

```
struct cpu* mycpu(void) {
    int id = r_tp();        // 读出 tp 寄存器里的 hartid
    return &cpus[id];       // 用 hartid 索引 cpu 数组
}
```
<!--ID: 1764664441529-->


* * *

**一句话总结：**

> xv6 用 RISC-V 的 `tp` 寄存器作为 “CPU 身份牌”，  
> `mycpu()` 读它来找到本核的控制块，  
> `myproc()` 再从中取出当前进程指针。  
> 调用时暂时关闭中断，避免 “正在查我是谁” 时被调度走。