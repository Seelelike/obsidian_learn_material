> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [chat.zchat.tech](https://chat.zchat.tech/c/6912b03b-a5bc-832a-8a72-7c478c14034b)

> ChatGPT helps you get answers, find inspiration and be more productive. It is free to use and easy to......

我对线程切换的理解是，当用户进程一进入内核线程时，发现是定时器中断，调用 yield，yield 函数调用 sche 函数，sche 调用 swtch(&p->context, &mycpu()->context);，由于 swtch 会重装在 ra 寄存器，因此当 swtch 函数返回时，返回的是之前 schedule 函数调用 swtch 的代码处，对不对。

###### ChatGPT 说：

✅ 非常棒的理解！你已经抓住了 xv6 **线程切换的核心思想**。  
不过你的描述中有一些**细微但非常关键的地方**需要澄清一下。  
我们一步步捋清楚：

* * *

🧩 一、你的理解大体正确，但调用关系要更精确一点
-------------------------

你说：

> “当用户进程一进入内核线程时，发现是定时器中断，调用 yield，yield 调用 sched，sched 调用 swtch(&p->context, &mycpu()->context)；由于 swtch 会重装 ra 寄存器，因此当 swtch 函数返回时，返回的是之前 scheduler 函数调用 swtch 的代码处。”

这个总体逻辑 ✔️是对的，但有几个细节需要修正：

* * *

🧠 二、正确的调用链顺序（时钟中断导致的上下文切换）
---------------------------

我们来还原**整个线程切换路径**（以定时器中断为例）：

### 1️⃣ 用户态 → 内核态

*   定时器中断触发 → CPU 跳到 `usertrap()`；
    
*   `usertrap()` 检测到是时钟中断；
    
*   调用 `yield()`，主动让出 CPU。
    
<!--ID: 1764664441482-->


* * *

### 2️⃣ 进入 `yield()`

```
struct context {
  uint64 ra;
  uint64 sp;
  uint64 s0; … uint64 s11;
};
```
<!--ID: 1764664441485-->


`yield()` 调用了 `sched()`。

* * *

### 3️⃣ `sched()` 负责切换到调度器线程

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
<!--ID: 1764664441488-->


重点在这里：

*   `&p->context` 保存的是 **当前进程的内核线程上下文**；
    
*   `&mycpu()->scheduler` 保存的是 **当前 CPU 的调度器线程上下文**。
    

于是 `swtch()` 开始保存 / 恢复寄存器状态。

* * *

### 4️⃣ `swtch(&p->context, &mycpu()->scheduler)`

`swtch` 做的事情可以概括为：
<!--ID: 1764664441491-->


```
struct proc *current_proc;
```

即：

*   把**当前进程的寄存器状态**保存到 `p->context`；
    
*   把**调度器的寄存器状态**加载进 CPU；
    
*   然后从 `new->ra`（也就是调度器上次停下来的位置）继续执行！
    

* * *

### 5️⃣ 那么 swtch 返回到哪里？

这正是关键点 👇：
<!--ID: 1764664441493-->


> **返回到上一次调用 `swtch` 的地方！**

也就是说，当调度器上一次调用

```
struct cpu* mycpu(void) {
    int id = r_tp();        // 读出 tp 寄存器里的 hartid
    return &cpus[id];       // 用 hartid 索引 cpu 数组
}
```

切到该进程时，它把 `ra`（返回地址）存在了 `cpu->scheduler.ra` 中。

现在进程要切回调度器，于是：

*   `swtch(&p->context, &cpu->scheduler)`  
    会恢复 `cpu->scheduler` 保存的 `ra`；
    
*   所以 `ret` 后就回到了**调度器函数 `scheduler()` 上一次调用 swtch 的下一条指令**。
    

也就是——回到了 `scheduler()` 函数体中！

* * *

### 6️⃣ 于是控制流变为：

```
struct proc* myproc(void) {
    push_off();                  // 关闭中断
    struct cpu *c = mycpu();     // 找到当前CPU
    struct proc *p = c->proc;    // 取出该CPU当前运行的进程
    pop_off();                   // 恢复中断
    return p;
}
```
<!--ID: 1764664441496-->


所以：

> 你的那句话 “swtch 返回到 scheduler 函数调用 swtch 的地方” 是对的！

但要注意：

*   它返回的是 **scheduler()** 的 `swtch` 调用点（行号约在 `kernel/proc.c:456`），  
    **不是 sched()** 自己的地方；
    
*   也就是说 **进程的 sched() 并不会立即继续执行**；  
    它会在下次该进程被调度运行时，从自己的 `swtch` 返回点恢复。
    

* * *

🧩 三、整个线程切换的循环过程
----------------

我们可以这样画出执行路径的完整 “来回”：

<table data-start="2071" data-end="2492"><thead data-start="2071" data-end="2096"><tr data-start="2071" data-end="2096"><th data-start="2071" data-end="2076" data-col-size="sm">阶段</th><th data-start="2076" data-end="2083" data-col-size="md">调用关系</th><th data-start="2083" data-end="2090" data-col-size="sm">执行线程</th><th data-start="2090" data-end="2096" data-col-size="sm">注释</th></tr></thead><tbody data-start="2139" data-end="2492"><tr data-start="2139" data-end="2261"><td data-start="2139" data-end="2150" data-col-size="sm">① 进程正在运行</td><td data-col-size="md" data-start="2150" data-end="2234"><code data-start="2152" data-end="2233">user → syscall → usertrap → yield → sched → swtch(&amp;p-&gt;context, &amp;cpu-&gt;scheduler)</code></td><td data-col-size="sm" data-start="2234" data-end="2243">进程内核线程</td><td data-col-size="sm" data-start="2243" data-end="2261">保存进程上下文，切换到调度器</td></tr><tr data-start="2262" data-end="2347"><td data-start="2262" data-end="2272" data-col-size="sm">② 切到调度器</td><td data-col-size="md" data-start="2272" data-end="2315"><code data-start="2274" data-end="2287">scheduler()</code> 恢复执行（在 CPU 的 scheduler 栈上）</td><td data-col-size="sm" data-start="2315" data-end="2323">调度器线程</td><td data-col-size="sm" data-start="2323" data-end="2347">调度器找到下一个 RUNNABLE 进程</td></tr><tr data-start="2348" data-end="2429"><td data-start="2348" data-end="2358" data-col-size="sm">③ 调度新进程</td><td data-col-size="md" data-start="2358" data-end="2399"><code data-start="2360" data-end="2398">swtch(&amp;cpu-&gt;scheduler, &amp;p2-&gt;context)</code></td><td data-col-size="sm" data-start="2399" data-end="2413">调度器线程 → 新进程</td><td data-col-size="sm" data-start="2413" data-end="2429">恢复 p2 的内核上下文</td></tr><tr data-start="2430" data-end="2492"><td data-start="2430" data-end="2440" data-col-size="sm">④ 新进程继续</td><td data-col-size="md" data-start="2440" data-end="2474"><code data-start="2442" data-end="2449">swtch</code> 返回 → 从 <code data-start="2457" data-end="2466">sched()</code> 之后继续执行</td><td data-col-size="sm" data-start="2474" data-end="2480">新进程</td><td data-col-size="sm" data-start="2480" data-end="2492">重新运行用户进程</td></tr></tbody></table>

可以看到：

*   **sched() 和 scheduler() 是协程（coroutines）关系**；
    
*   每个 `swtch()` 都会让执行流在这两个函数之间来回跳。
    

* * *

🧠 四、为什么能 “跳回来”？
----------------

关键原因在 `ra`（return address）寄存器：

*   每次调用 `swtch` 时：
    
    *   当前函数的返回地址（`ra`）被保存进旧线程的 `context`；
        
    *   新线程恢复时会恢复它自己的 `ra`；
        
    *   因此 `ret` 后会自动跳到**它上次被切走的地方**。
        

所以：

*   对进程而言：下次被调度时，`swtch` 返回到 `sched()` 后，进程继续执行；
    
*   对调度器而言：每次被切回，`swtch` 返回到 `scheduler()` 的循环中，继续挑选新进程。
    

* * *

✅ 五、总结一句话
---------

> 当进程在 `sched()` 中调用 `swtch(&p->context, &cpu->scheduler)` 时，  
> CPU 会切换到调度器上下文。  
> 等 `swtch` 返回时，它不是回到 `sched()`，  
> 而是回到 **scheduler() 上次调用 swtch 的那一行**，  
> 也就是调度器函数内部的循环。
> 
> 这就是为什么我们说 xv6 的 `sched()` 与 `scheduler()` 是互为协程。

* * *

如果你想，我可以给你画一张栈切换示意图（标出 `p->context`、`cpu->scheduler`、`sp`、`ra` 的变化过程），帮助你更直观地理解 “为什么返回到 scheduler 而不是 sched”。  
要我画吗？

##### 您说：

void scheduler(void) { struct proc *p; struct cpu *c = mycpu(); c->proc = 0; for(;;){ // Avoid deadlock by ensuring that devices can interrupt. intr_on(); for(p = proc; p < &proc[NPROC]; p++) { acquire(&p->lock); if(p->state == RUNNABLE) { // Switch to chosen process. It is the process's job // to release its lock and then reacquire it // before jumping back to us. p->state = RUNNING; c->proc = p; swtch(&c->context, &p->context); // Process is done running for now. // It should have changed its p->state before coming back. c->proc = 0; } release(&p->lock); } } }schedule 代码如上所示，当用户一内核线程调用 swtch 后，接着执行的是 c->proc = 0 对不对，那此时的 c->proc 是什么，release 的锁是什么
<!--ID: 1764664441498-->


###### ChatGPT 说：

非常好的问题！👏  
你现在已经在钻进 xv6 **调度器真正的执行逻辑**里了，这个问题非常关键。  
我们来一步步**精确还原 `scheduler()` 调度时的执行流**，解释：

1️⃣ 调用链到底是谁切谁；  
2️⃣ 当 `swtch()` 返回后，`c->proc = 0` 为什么执行；  
3️⃣ 此时的 `c->proc`、`release(&p->lock)` 分别作用在什么对象上。

* * *

🧩 一、上下文：谁在调用 `swtch`？
----------------------

你现在贴出的这段代码：

```
每个 CPU 启动时：
    mstart() → tp = hartid

当 CPU 进入内核态：
    uservec 恢复 tp (该核的hartid)
    mycpu() 用 tp 查找 &cpus[hartid]
    myproc() 从该 cpu 取出当前运行的进程
```

这段代码发生在 **调度器线程（scheduler）** 的上下文中。

也就是说，现在 CPU 正在执行的是 `scheduler()` 函数，  
而不是某个用户进程的内核线程。

* * *

🧠 二、切换前的状态（调用 `swtch` 之前）
--------------------------

在调用 `swtch(&c->context, &p->context)` 之前：

<table data-start="511" data-end="752"><thead data-start="511" data-end="528"><tr data-start="511" data-end="528"><th data-start="511" data-end="516" data-col-size="sm">名称</th><th data-start="516" data-end="522" data-col-size="sm">当前值</th><th data-start="522" data-end="528" data-col-size="sm">含义</th></tr></thead><tbody data-start="554" data-end="752"><tr data-start="554" data-end="602"><td data-start="554" data-end="566" data-col-size="sm"><code data-start="556" data-end="565">c-&gt;proc</code></td><td data-col-size="sm" data-start="566" data-end="581">指向被选中的进程 <code data-start="577" data-end="580">p</code></td><td data-col-size="sm" data-start="581" data-end="602">表示当前 CPU 将要运行哪个进程</td></tr><tr data-start="603" data-end="646"><td data-start="603" data-end="616" data-col-size="sm"><code data-start="605" data-end="615">p-&gt;state</code></td><td data-col-size="sm" data-start="616" data-end="633">已被设为 <code data-start="623" data-end="632">RUNNING</code></td><td data-col-size="sm" data-start="633" data-end="646">表示该进程即将运行</td></tr><tr data-start="647" data-end="709"><td data-start="647" data-end="659" data-col-size="sm">CPU 当前上下文</td><td data-col-size="sm" data-start="659" data-end="698"><code data-start="661" data-end="672">scheduler</code> 的寄存器 / 栈（保存在 <code data-start="684" data-end="696">c-&gt;context</code>）</td><td data-col-size="sm" data-start="698" data-end="709">调度器线程状态</td></tr><tr data-start="710" data-end="752"><td data-start="710" data-end="721" data-col-size="sm">要切换到的上下文</td><td data-col-size="sm" data-start="721" data-end="736"><code data-start="723" data-end="735">p-&gt;context</code></td><td data-col-size="sm" data-start="736" data-end="752">进程的内核线程寄存器和栈</td></tr></tbody></table>

此时调度器执行：

```
int counter = 0;

void *thread1() {
    counter++;   // 修改共享变量
}

void *thread2() {
    printf("%d\n", counter);  // 可以看到修改后的结果
}
```

意味着：

> 保存当前调度器的寄存器状态到 `c->context`，  
> 加载 `p->context` 中保存的寄存器状态，  
> 并 “跳转” 到 `p->context` 里保存的返回地址。

* * *

🌀 三、切换后发生了什么？
--------------

执行完 `swtch(&c->context, &p->context)` 的那一刻：

*   **CPU 的寄存器 / 栈** 被换成了进程的；
    
*   **程序执行流** 跳到了进程上次被切走的地方；
    
*   换句话说，CPU 不再运行 `scheduler()` 了，  
    而是回到该进程在内核里的 `sched()` 调用之后继续执行。
    

比如上次切换时，进程调用：

```
int x = 0;   // 进程A和进程B的变量在不同地址空间
x = 1;       // A改自己的x，对B毫无影响
```

当它再次恢复时，`swtch` 返回到那条语句之后：

```
void yield(void) {
    struct proc *p = myproc();
    acquire(&p->lock);
    p->state = RUNNABLE;     // 进程准备好可以被调度
    sched();                 // 切换到调度器
    release(&p->lock);
}
```

* * *

🔁 四、什么时候回到 `scheduler()`？
--------------------------

当进程再次调用 `yield()`（或因为 sleep、exit）主动放弃 CPU 时，  
它会执行：

```
void sched(void) {
    struct proc *p = myproc();
    swtch(&p->context, &mycpu()->scheduler);
}
```

此时：

*   保存进程寄存器状态到 `p->context`；
    
*   恢复调度器寄存器状态从 `c->context`；
    
*   然后执行 `ret`；
    
*   **于是 CPU 又回到了调度器函数中，继续执行 `swtch()` 之后的下一行**：
    

```
# 伪代码（简化）
save callee-saved registers into *old
load callee-saved registers from *new
ret  # 相当于 jump to new->ra
```

* * *

✅ 五、所以你问的两个问题：
--------------

### ✳️ 问题 1：

> “当用户内核线程调用 swtch 后，接着执行的是 c->proc = 0，对不对？”
<!--ID: 1764664441502-->


✔️ **对！**

但要特别注意 “是谁在执行”：

*   当进程调用 `swtch(&p->context, &c->context)` → 切换到调度器；
    
*   调度器恢复执行（`swtch` 返回）→ 从刚才那一行继续往下跑；
    
*   所以现在执行的确实是：
    
    ```
    swtch(&cpu->scheduler, &p->context);
    ```
    
*   也就是说：
    
    > `c->proc = 0` 是在**调度器线程的上下文中执行**的，  
    > 不是在进程中执行的。
    

* * *

### ✳️ 问题 2：

> “此时的 c->proc 是什么？”
<!--ID: 1764664441505-->


当调度器恢复执行（刚从进程 swtch 回来）时：

*   `c->proc` 此刻依然指向刚才那个让出 CPU 的进程 `p`；
    
*   所以：
    
    ```
    进程 yield → sched → swtch(&p->context, &cpu->scheduler)
                        ↓
                    返回到 scheduler()
    ```
    
    这句的作用是：
    
    > 告诉系统：“当前 CPU 现在空闲了，不再运行任何进程”。
    

这一步非常关键，因为如果此时 `c->proc` 仍指向那个进程，  
系统会误以为该进程仍在 CPU 上运行，会导致调度混乱。

* * *

### ✳️ 问题 3：

> “release(&p->lock) 释放的锁是谁的锁？”
<!--ID: 1764664441509-->


这句：

```
p->state = RUNNING;
c->proc = p;
swtch(&c->context, &p->context);
c->proc = 0;
```

释放的是**刚刚被切回的进程 `p` 的锁**。

回忆前面在 `yield()` 里的逻辑：

```
swtch(&c->context, &p->context);
```

锁是在进程调用 `yield()` 时加的；  
但是它没有在 `sched()`（进程上下文）中释放，  
因为那时进程还没完全停下；  
**锁要等到调度器切回来后才能释放**。

所以 `scheduler()` 的那句：

```
swtch(&p->context, &c->context);
```

正是 “接力释放”——  
是进程放弃 CPU 之后，调度器帮它把锁放掉。

* * *

🧩 六、整个时序复盘
-----------

<table data-start="2505" data-end="2919"><thead data-start="2505" data-end="2527"><tr data-start="2505" data-end="2527"><th data-start="2505" data-end="2510" data-col-size="sm">时刻</th><th data-start="2510" data-end="2516" data-col-size="sm">执行者</th><th data-start="2516" data-end="2521" data-col-size="sm">调用</th><th data-start="2521" data-end="2527" data-col-size="sm">说明</th></tr></thead><tbody data-start="2561" data-end="2919"><tr data-start="2561" data-end="2645"><td data-start="2561" data-end="2585" data-col-size="sm">(1) 进程 P 调用 <code data-start="2575" data-end="2584">yield()</code></td><td data-col-size="sm" data-start="2585" data-end="2592">进程线程</td><td data-col-size="sm" data-start="2592" data-end="2628"><code data-start="2594" data-end="2627">swtch(&amp;p-&gt;context, &amp;c-&gt;context)</code></td><td data-col-size="sm" data-start="2628" data-end="2645">保存进程上下文，切到调度器</td></tr><tr data-start="2646" data-end="2716"><td data-start="2646" data-end="2660" data-col-size="sm">(2) 调度器恢复执行</td><td data-col-size="sm" data-start="2660" data-end="2668">调度器线程</td><td data-col-size="sm" data-start="2668" data-end="2703">从 <code data-start="2672" data-end="2681">swtch()</code> 返回，执行 <code data-start="2688" data-end="2702">c-&gt;proc = 0;</code></td><td data-col-size="sm" data-start="2703" data-end="2716">当前 CPU 空闲</td></tr><tr data-start="2717" data-end="2785"><td data-start="2717" data-end="2739" data-col-size="sm">(3) 调度器释放 <code data-start="2729" data-end="2738">p-&gt;lock</code></td><td data-col-size="sm" data-start="2739" data-end="2747">调度器线程</td><td data-col-size="sm" data-start="2747" data-end="2769"><code data-start="2749" data-end="2768">release(&amp;p-&gt;lock)</code></td><td data-col-size="sm" data-start="2769" data-end="2785">锁从进程转交给调度器释放</td></tr><tr data-start="2786" data-end="2857"><td data-start="2786" data-end="2803" data-col-size="sm">(4) 调度器选择下一个进程</td><td data-col-size="sm" data-start="2803" data-end="2811">调度器线程</td><td data-col-size="sm" data-start="2811" data-end="2848"><code data-start="2813" data-end="2847">swtch(&amp;c-&gt;context, &amp;p2-&gt;context)</code></td><td data-col-size="sm" data-start="2848" data-end="2857">切到新进程</td></tr><tr data-start="2858" data-end="2919"><td data-start="2858" data-end="2870" data-col-size="sm">(5) 新进程恢复</td><td data-col-size="sm" data-start="2870" data-end="2877">进程线程</td><td data-col-size="sm" data-start="2877" data-end="2897">从 <code data-start="2881" data-end="2888">swtch</code> 返回，继续运行</td><td data-col-size="sm" data-start="2897" data-end="2919">回到 <code data-start="2902" data-end="2911">yield()</code> 之后的代码</td></tr></tbody></table>

* * *

✅ 七、总结一句话
---------

> 当进程调用 `swtch(&p->context, &c->context)` 放弃 CPU 时，  
> CPU 切换到调度器上下文；  
> 调度器恢复执行，从自己的 `swtch()` 调用处继续运行；  
> 接着执行 `c->proc = 0`（清空当前 CPU 正在运行的进程）；  
> 然后调用 `release(&p->lock)` 释放刚刚那个进程的锁。

* * *

如果你想更形象一点，我可以画出 “进程栈 ↔ 调度器栈” 的切换示意图，  
把 `sp`（栈指针）、`ra`（返回地址）和 `lock` 的状态都标出来，看起来会更清晰。  
要我画一张图来配合解释吗？

> 本文由 [简悦 SimpRead](http://ksria.com/simpread/) 转码， 原文地址 [chat.zchat.tech](https://chat.zchat.tech/c/6912b03b-a5bc-832a-8a72-7c478c14034b)

> ChatGPT helps you get answers, find inspiration and be more productive. It is free to use and easy to......

非常好的问题，这一段内容是 **XV6 中 “线程（或进程）第一次调用 `swtch` 的特殊情况”**，它是理解操作系统如何**启动第一个进程、构造 “假上下文” 让 `swtch` 能顺利运行**的关键。  
这其实是整个调度器机制中最 “魔术” 的一部分 —— 没有它，`swtch()` 根本无法第一次返回。

下面我来分层、详细解释这段内容。

🧩 一、背景回顾：普通情况下的 swtch 是 “线程协程式跳转”
----------------------------------

正常运行时：

*   线程 A 调用 `swtch(&A->context, &B->context)`；
    
*   保存 A 的寄存器，恢复 B 的寄存器；
    
*   执行 `ret`，返回到 **B 上次调用 `swtch` 的地方**；
    
*   所以，swtch 本质上是一个**双向协程切换**：A 跳到 B，B 以后又能跳回 A。
    

> 🧠 问题：第一次创建线程（比如 fork 出来的新进程）时，B 还从没 “调用过 swtch”，那它的 `context` 是空的。  
> swtch 该怎么知道 “返回到哪里”？

这就引出了 **“第一次调用 swtch 的伪造 context”**。

🧠 二、关键问题：**第一次切换到新线程时，B 的 context 是假的！**
-----------------------------------------

XV6 通过在创建进程时手工 “伪造” 这个 context，让 swtch 以为 B 以前也调用过 swtch。

### 在哪里伪造？

在 `allocproc()` 里，也就是创建新进程（线程）的函数。
<!--ID: 1764664441512-->


🧩 三、allocproc() 里是怎么伪造的？
-------------------------

伪造的重点是两个寄存器：

<table data-start="715" data-end="865"><thead data-start="715" data-end="734"><tr data-start="715" data-end="734"><th data-start="715" data-end="721" data-col-size="sm">寄存器</th><th data-start="721" data-end="726" data-col-size="sm">含义</th><th data-start="726" data-end="734" data-col-size="sm">设置目的</th></tr></thead><tbody data-start="765" data-end="865"><tr data-start="765" data-end="809"><td data-start="765" data-end="774" data-col-size="sm"><strong data-start="767" data-end="773">sp</strong></td><td data-col-size="sm" data-start="774" data-end="795">栈指针（stack pointer）</td><td data-col-size="sm" data-start="795" data-end="809">指向分配好的内核栈顶</td></tr><tr data-start="810" data-end="865"><td data-start="810" data-end="819" data-col-size="sm"><strong data-start="812" data-end="818">ra</strong></td><td data-col-size="sm" data-start="819" data-end="842">返回地址（return address）</td><td data-col-size="sm" data-start="842" data-end="865">设置成函数 <code data-start="850" data-end="859">forkret</code> 的地址</td></tr></tbody></table>

代码（简化）如下：

```
struct context {
  uint64 ra;
  uint64 sp;
  uint64 s0; … uint64 s11;
};
```

于是当调度器第一次执行：

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

时，`swtch` 会：

*   保存调度器寄存器；
    
*   恢复 `p->context` 的寄存器；
    
*   把 `ra` 设置为 `forkret`；
    
*   把 `sp` 切到新进程的内核栈；
    
*   然后执行 `ret`。
    

于是：

> CPU 直接 “跳转到” `forkret()` 的开头执行，  
> 就好像这个新线程之前也执行过 `swtch()` 一样。

⚙️ 四、`forkret()` 是干什么的？
-----------------------

`forkret()` 是新进程的内核线程第一次执行的起点。  
因为调度器调用 `swtch(&cpu->scheduler, &p->context)` 时，“恢复” 的就是 `p->context`，所以：

*   当 `swtch()` 返回时，CPU 会从 `forkret()` 的开头开始执行；
    
*   它就像 “从 `swtch` 返回” 一样进入了新进程。
    

### forkret 的主要任务：

1.  **释放锁（p->lock）**
    
    *   因为调度器持有它，forkret 需要负责释放；
        
2.  **如果是系统第一次启动（first==1）**，执行文件系统初始化；
    
3.  **最后调用 `usertrapret()`**，返回到用户态的第一个指令（即用户进程开始执行）。
    
<!--ID: 1764664441515-->


🧠 五、为什么叫 “forkret”？
--------------------

字面意思是 “the function that fork returns to”（fork 返回到的地方）。

因为：

*   所有进程（除了第一个 init 进程）都是通过 `fork()` 复制父进程创建的；
    
*   新进程第一次运行时要看起来 “像是从 fork 返回”；
    
*   所以它从 `forkret()` 开始，完成必要的收尾（释放锁、设置 trapframe），  
    然后通过 `usertrapret()` 进入用户空间继续执行。
    

🧩 六、第一次进程启动的 “假 trapframe”
---------------------------

在 `forkret()` 的最后：

```
struct proc *current_proc;
```

这里传入的 trapframe 也是 “假的”：

*   不是因为真正的 trap（中断 / 系统调用）；
    
*   而是手动设置的；
    
*   它的作用是 “假装从一次 trap 返回到用户态”；
    
*   CPU 执行 `sret` 后，就会跳到用户程序的入口地址（PC）。
    

> 所以用户态代码就像 “从 trap 返回后” 开始执行第一条指令。

⚙️ 七、第一次进程创建时的 PC 设置
--------------------

*   一般 fork 出来的进程会拷贝父进程的 `trapframe`；
    
    *   因此它的 `pc`（程序计数器）和父进程一致；
        
*   但对 **第一个用户进程（init 进程）** 来说，没有父进程；
    
    *   所以它的 PC 必须被设置为 0 或者程序的入口地址；
        
    *   这样当它 “从 trap 返回” 时，CPU 会从正确的位置开始执行。
        

🧩 八、if(first) 的含义
------------------

在 `forkret()` 里有：

```
struct cpu* mycpu(void) {
    int id = r_tp();        // 读出 tp 寄存器里的 hartid
    return &cpus[id];       // 用 hartid 索引 cpu 数组
}
```

这是文件系统初始化逻辑。

为什么在这里做？

*   XV6 初始化文件系统需要执行磁盘 I/O；
    
*   但磁盘操作需要有一个进程上下文（因为要 sleep/wakeup）；
    
*   在系统刚启动时还没有任何进程；
    
*   所以只能等第一个进程启动后，在它的 context 下初始化文件系统。
    

> 因此 `if(first)` 的那部分只在系统第一次调用 forkret 时运行一次。

💡 九、总结整个 “第一次 swtch” 过程
------------------------

<table data-start="2615" data-end="3043"><thead data-start="2615" data-end="2639"><tr data-start="2615" data-end="2639"><th data-start="2615" data-end="2620" data-col-size="sm">阶段</th><th data-start="2620" data-end="2626" data-col-size="sm">执行者</th><th data-start="2626" data-end="2633" data-col-size="sm">关键操作</th><th data-start="2633" data-end="2639" data-col-size="sm">结果</th></tr></thead><tbody data-start="2678" data-end="3043"><tr data-start="2678" data-end="2749"><td data-start="2678" data-end="2694" data-col-size="sm">1️⃣ allocproc</td><td data-col-size="sm" data-start="2694" data-end="2699">内核</td><td data-col-size="sm" data-start="2699" data-end="2734">伪造 context（<code data-start="2712" data-end="2724">ra=forkret</code>，<code data-start="2725" data-end="2732">sp=栈顶</code>）</td><td data-col-size="sm" data-start="2734" data-end="2749">为新进程准备假的返回点</td></tr><tr data-start="2750" data-end="2829"><td data-start="2750" data-end="2766" data-col-size="sm">2️⃣ scheduler</td><td data-col-size="sm" data-start="2766" data-end="2774">内核调度器</td><td data-col-size="sm" data-start="2774" data-end="2814"><code data-start="2776" data-end="2813">swtch(&amp;cpu-&gt;scheduler, &amp;p-&gt;context)</code></td><td data-col-size="sm" data-start="2814" data-end="2829">切换到进程的伪造上下文</td></tr><tr data-start="2830" data-end="2907"><td data-start="2830" data-end="2842" data-col-size="sm">3️⃣ swtch</td><td data-col-size="sm" data-start="2842" data-end="2849">汇编代码</td><td data-col-size="sm" data-start="2849" data-end="2883">恢复 <code data-start="2854" data-end="2866">p-&gt;context</code>，ret 到 <code data-start="2873" data-end="2882">forkret</code></td><td data-col-size="sm" data-start="2883" data-end="2907">CPU 跳转到 forkret 开始执行</td></tr><tr data-start="2908" data-end="2984"><td data-start="2908" data-end="2922" data-col-size="sm">4️⃣ forkret</td><td data-col-size="sm" data-start="2922" data-end="2933">新进程的内核线程</td><td data-col-size="sm" data-start="2933" data-end="2971">释放锁、fsinit（仅第一次）、调用 <code data-start="2955" data-end="2970">usertrapret()</code></td><td data-col-size="sm" data-start="2971" data-end="2984">进入用户态执行程序</td></tr><tr data-start="2985" data-end="3043"><td data-start="2985" data-end="3003" data-col-size="sm">5️⃣ usertrapret</td><td data-col-size="sm" data-start="3003" data-end="3012">内核→用户态</td><td data-col-size="sm" data-start="3012" data-end="3033">从假 trapframe 恢复寄存器</td><td data-col-size="sm" data-start="3033" data-end="3043">进程开始运行</td></tr></tbody></table>

🔁 十、形象类比
---------

你可以把整个过程想象成：

> “第一次启动线程时，给它编造了一个假的返回地址（forkret），  
> 让 `swtch()` 以为它之前暂停在那儿，  
> 这样 CPU ‘返回’ 时，就会自然地从 forkret 开始执行。”

这样系统不需要特殊的 “启动代码路径”，  
它统一用同一个 `swtch()` 机制来切换线程，无论是：

*   普通进程上下文切换；
    
*   还是第一次启动的 “新线程”。
    

✅ 十一、简要总结一句话
------------

> 在 XV6 中，新进程第一次被调度运行时，它的 context 是 “伪造” 的：  
> `ra` 指向 `forkret`，`sp` 指向它自己的内核栈。
> 
> 当调度器第一次执行 `swtch(&cpu->scheduler, &p->context)` 时，  
> CPU 恢复这两个寄存器并 “ret” 到 forkret，  
> 从而自然地开始执行新进程的启动逻辑（释放锁、初始化文件系统、返回用户态）。
> 
> 这个机制保证了：**无论是第一次运行还是正常切换，swtch 机制完全统一。**