---
annotation-target: "[[book.pdf]]"
---



>%%
>```annotation-json
>{"created":"2025-11-07T05:58:51.606Z","updated":"2025-11-07T05:58:51.606Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":60494,"end":60634},{"type":"TextQuoteSelector","exact":"竞争是一种同时访问内存位置且至少一次访问是写入的情况。竞争通常是错误的迹象，要么是更新丢失（如果访问是写入），要么是读取未完全更新的数据结构。竞争的结果取决于编译器生成的机器代码、所涉及的两个CPU的时序以及内存系统如何排序它们的内存操作，这可能会使竞争引起的错误难以重现和调试。","prefix":"值涉及的元素将丢失。行 16 处丢失的更新是一个示例race 。","suffix":"例如，调试时添加打印语句push 可能会改变执行的时间，足以使竞"}]}]}
>```
>%%
>*%%PREFIX%%值涉及的元素将丢失。行 16 处丢失的更新是一个示例race 。%%HIGHLIGHT%% ==竞争是一种同时访问内存位置且至少一次访问是写入的情况。竞争通常是错误的迹象，要么是更新丢失（如果访问是写入），要么是读取未完全更新的数据结构。竞争的结果取决于编译器生成的机器代码、所涉及的两个CPU的时序以及内存系统如何排序它们的内存操作，这可能会使竞争引起的错误难以重现和调试。== %%POSTFIX%%例如，调试时添加打印语句push 可能会改变执行的时间，足以使竞*
>%%LINK%%[[#^0vlezvv7xopl|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^0vlezvv7xopl


>%%
>```annotation-json
>{"created":"2025-11-07T06:05:15.600Z","updated":"2025-11-07T06:05:15.600Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":62441,"end":62618},{"type":"TextQuoteSelector","exact":"由于锁被广泛使用，多核处理器通常提供实现原子版本的指令25 和26 。在RISC-V 上，这条指令是amoswap r, a 。amoswap 读取内存地址a 处的值，将寄存器r 的内容写入该地址，并将读取到的值放入r 中。也就是说，它交换了寄存器的内容和内存地址。它以原子方式执行此序列，使用特殊硬件来防止任何其他CPU 使用读取和写入之间的内存地址。","prefix":"的是一种使25 和26 行作为atomic（即不可分割的）步骤。","suffix":"54Xv6 的acquire (kernel/spinlock."}]}]}
>```
>%%
>*%%PREFIX%%的是一种使25 和26 行作为atomic（即不可分割的）步骤。%%HIGHLIGHT%% ==由于锁被广泛使用，多核处理器通常提供实现原子版本的指令25 和26 。在RISC-V 上，这条指令是amoswap r, a 。amoswap 读取内存地址a 处的值，将寄存器r 的内容写入该地址，并将读取到的值放入r 中。也就是说，它交换了寄存器的内容和内存地址。它以原子方式执行此序列，使用特殊硬件来防止任何其他CPU 使用读取和写入之间的内存地址。== %%POSTFIX%%54Xv6 的acquire (kernel/spinlock.*
>%%LINK%%[[#^a3hue0e23g|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^a3hue0e23g


>%%
>```annotation-json
>{"created":"2025-11-07T06:34:28.047Z","updated":"2025-11-07T06:34:28.047Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":64981,"end":65048},{"type":"TextQuoteSelector","exact":"如果通过内核的代码路径必须同时持有多个锁，则所有代码路径以相同的顺序获取这些锁非常重要。如果不这样做，则存在deadlock 的风险。","prefix":"ufferFigure 6.3: 锁定xv66.4 死锁和锁顺序","suffix":"假设有两条代码路径inxv6 需要锁A和B，但是代码路径1 按照"}]}]}
>```
>%%
>*%%PREFIX%%ufferFigure 6.3: 锁定xv66.4 死锁和锁顺序%%HIGHLIGHT%% ==如果通过内核的代码路径必须同时持有多个锁，则所有代码路径以相同的顺序获取这些锁非常重要。如果不这样做，则存在deadlock 的风险。== %%POSTFIX%%假设有两条代码路径inxv6 需要锁A和B，但是代码路径1 按照*
>%%LINK%%[[#^n2sdp346aac|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^n2sdp346aac


>%%
>```annotation-json
>{"created":"2025-11-11T05:10:05.160Z","updated":"2025-11-11T05:10:05.160Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":70793,"end":71101},{"type":"TextQuoteSelector","exact":"首先，如何从一个进程切换到另一个进程？虽然上下文切换的想法很简单，但实现是xv6 中一些最不透明的代码。其次，如何以对用户进程透明的方式强制切换？Xv6 使用标准技术，其中硬件定时器的中断驱动上下文切换。第三，所有CPU 在同一共享进程集之间切换，并且需要一个锁定计划来避免竞争。第四，进程退出时必须释放进程的内存和其他资源，但它本身无法完成所有这些操作，因为（例如）它无法在仍在使用自己的内核堆栈时释放它。第五，多核机器的每个核心必须记住它正在执行哪个进程，以便系统调用影响正确进程的内核状态。最后，sleep 和wakeup 允许进程放弃CPU 并等待被另一个进程或中断唤醒。需要小心避免导致唤醒通知丢失的竞争。","prefix":"建每个进程都有自己的内存的假象一样。实施多路复用带来了一些挑战。","suffix":"Xv6试图尽可能简单地解决这些问题，但生成的代码仍然很棘手。7."}]}]}
>```
>%%
>*%%PREFIX%%建每个进程都有自己的内存的假象一样。实施多路复用带来了一些挑战。%%HIGHLIGHT%% ==首先，如何从一个进程切换到另一个进程？虽然上下文切换的想法很简单，但实现是xv6 中一些最不透明的代码。其次，如何以对用户进程透明的方式强制切换？Xv6 使用标准技术，其中硬件定时器的中断驱动上下文切换。第三，所有CPU 在同一共享进程集之间切换，并且需要一个锁定计划来避免竞争。第四，进程退出时必须释放进程的内存和其他资源，但它本身无法完成所有这些操作，因为（例如）它无法在仍在使用自己的内核堆栈时释放它。第五，多核机器的每个核心必须记住它正在执行哪个进程，以便系统调用影响正确进程的内核状态。最后，sleep 和wakeup 允许进程放弃CPU 并等待被另一个进程或中断唤醒。需要小心避免导致唤醒通知丢失的竞争。== %%POSTFIX%%Xv6试图尽可能简单地解决这些问题，但生成的代码仍然很棘手。7.*
>%%LINK%%[[#^9gn6q428p7s|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^9gn6q428p7s


>%%
>```annotation-json
>{"created":"2025-11-11T05:12:30.753Z","updated":"2025-11-11T05:12:30.753Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":71953,"end":72057},{"type":"TextQuoteSelector","exact":"中断结束时的一种可能性是usertrap 调用yield 。yield 依次调用sched ，它调用swtch 将当前上下文保存在p->context 并切换到先前保存的调度程序上下文cpu->context","prefix":"流程swtch 进入调度程序。我们在Chapter 4 中看到，","suffix":" (kernel/proc.c:497)。swtch (kern"}]}]}
>```
>%%
>*%%PREFIX%%流程swtch 进入调度程序。我们在Chapter 4 中看到，%%HIGHLIGHT%% ==中断结束时的一种可能性是usertrap 调用yield 。yield 依次调用sched ，它调用swtch 将当前上下文保存在p->context 并切换到先前保存的调度程序上下文cpu->context== %%POSTFIX%%(kernel/proc.c:497)。swtch (kern*
>%%LINK%%[[#^iqranfi359|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^iqranfi359


>%%
>```annotation-json
>{"created":"2025-11-11T05:19:43.821Z","updated":"2025-11-11T05:19:43.821Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":72478,"end":72541},{"type":"TextQuoteSelector","exact":"当我们一直在追踪swtch 返回时，它返回的不是sched 而是scheduler ，堆栈指针位于当前CPU 的调度程序堆栈中","prefix":"l/proc.c:463) 切换到现在放弃CPU 的进程保存的。","suffix":"。627.3 代码：调度最后一部分着眼于底层细节 swtch ;"}]}]}
>```
>%%
>*%%PREFIX%%l/proc.c:463) 切换到现在放弃CPU 的进程保存的。%%HIGHLIGHT%% ==当我们一直在追踪swtch 返回时，它返回的不是sched 而是scheduler ，堆栈指针位于当前CPU 的调度程序堆栈中== %%POSTFIX%%。627.3 代码：调度最后一部分着眼于底层细节 swtch ;*
>%%LINK%%[[#^ufbow2rpelf|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^ufbow2rpelf


>%%
>```annotation-json
>{"created":"2025-11-11T05:26:30.176Z","updated":"2025-11-11T05:26:30.176Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":73021,"end":73323},{"type":"TextQuoteSelector","exact":"我们刚刚看到xv6 成立p->lock 跨调用swtch ：调用者swtch 必须已经持有该锁，并且该锁的控制权将传递给切换到的代码。这种约定对于锁来说是不常见的。通常，获取锁的线程也负责释放锁，这使得更容易推断正确性。对于上下文切换，有必要打破这个约定，因为p->lock 保护进程的不变量state 和执行时不正确的context 字段swtch 。一个可能出现的问题的例子p->lock 期间未举行swtch ：不同的CPU 可能决定在之后运行该进程yield 已将其状态设置为RUNNABLE ，但之前swtch 导致它停止使用自己的内核堆栈。结果将是两个CPU 运行在同一个堆栈上，这会导致混乱","prefix":"序继续其for 循环，找到要运行的进程，切换到它，然后重复循环。","suffix":"。内核线程放弃其 CPU 的唯一地方是 sched ，它总是切换"}]}]}
>```
>%%
>*%%PREFIX%%序继续其for 循环，找到要运行的进程，切换到它，然后重复循环。%%HIGHLIGHT%% ==我们刚刚看到xv6 成立p->lock 跨调用swtch ：调用者swtch 必须已经持有该锁，并且该锁的控制权将传递给切换到的代码。这种约定对于锁来说是不常见的。通常，获取锁的线程也负责释放锁，这使得更容易推断正确性。对于上下文切换，有必要打破这个约定，因为p->lock 保护进程的不变量state 和执行时不正确的context 字段swtch 。一个可能出现的问题的例子p->lock 期间未举行swtch ：不同的CPU 可能决定在之后运行该进程yield 已将其状态设置为RUNNABLE ，但之前swtch 导致它停止使用自己的内核堆栈。结果将是两个CPU 运行在同一个堆栈上，这会导致混乱== %%POSTFIX%%。内核线程放弃其 CPU 的唯一地方是 sched ，它总是切换*
>%%LINK%%[[#^3lhxg21u2jz|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^3lhxg21u2jz


>%%
>```annotation-json
>{"created":"2025-11-11T05:46:05.672Z","updated":"2025-11-11T05:46:05.672Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":73977,"end":74307},{"type":"TextQuoteSelector","exact":"考虑调度代码结构的一种方法是，它对每个进程强制执行一组不变量，并保持每当这些不变量不成立时，p->lock 。一个不变量是，如果一个进程是RUNNING ，定时器中断yield 必须能够安全地从进程切换；这意味着CPU 寄存器必须保存进程的寄存器值（即swtch 尚未将它们移至context ），以及c->proc 必须引用该进程。另一个不变量是，如果一个进程RUNNABLE ，对于空闲CPU 来说必须是安全的scheduler 运行它；这意味着p->context 必须保存进程的寄存器（即，它们实际上不在真实寄存器中），没有CPU 在进程的内核堆栈上执行，并且没有CPU c->proc 指的是进程。观察到这些属性通常不正确，而p->lock 被保持。","prefix":"h 开始运行它(kernel/proc.c:458-463) 。","suffix":"保持上述不变量就是 xv6 经常获取的原因 p->lock 在一"}]}]}
>```
>%%
>*%%PREFIX%%h 开始运行它(kernel/proc.c:458-463) 。%%HIGHLIGHT%% ==考虑调度代码结构的一种方法是，它对每个进程强制执行一组不变量，并保持每当这些不变量不成立时，p->lock 。一个不变量是，如果一个进程是RUNNING ，定时器中断yield 必须能够安全地从进程切换；这意味着CPU 寄存器必须保存进程的寄存器值（即swtch 尚未将它们移至context ），以及c->proc 必须引用该进程。另一个不变量是，如果一个进程RUNNABLE ，对于空闲CPU 来说必须是安全的scheduler 运行它；这意味着p->context 必须保存进程的寄存器（即，它们实际上不在真实寄存器中），没有CPU 在进程的内核堆栈上执行，并且没有CPU c->proc 指的是进程。观察到这些属性通常不正确，而p->lock 被保持。== %%POSTFIX%%保持上述不变量就是 xv6 经常获取的原因 p->lock 在一*
>%%LINK%%[[#^blcoxl40sj|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^blcoxl40sj


>%%
>```annotation-json
>{"created":"2025-11-11T08:04:22.215Z","updated":"2025-11-11T08:04:22.215Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":75686,"end":75830},{"type":"TextQuoteSelector","exact":"xv6 内核在这些情况（以及许多其他情况）中使用称为睡眠和唤醒的机制。睡眠允许内核线程等待特定事件；另一个线程可以调用wakeup 来指示等待事件的线程应该恢复。睡眠和唤醒通常称为sequence coordination 或conditional synchronization 机制。","prefix":"等待子进程退出；等等。而读取磁盘的进程需要等待磁盘硬件完成读取。","suffix":"睡眠和唤醒提供了相对低级的同步接口。为了激发它们在xv6 中的工"}]}]}
>```
>%%
>*%%PREFIX%%等待子进程退出；等等。而读取磁盘的进程需要等待磁盘硬件完成读取。%%HIGHLIGHT%% ==xv6 内核在这些情况（以及许多其他情况）中使用称为睡眠和唤醒的机制。睡眠允许内核线程等待特定事件；另一个线程可以调用wakeup 来指示等待事件的线程应该恢复。睡眠和唤醒通常称为sequence coordination 或conditional synchronization 机制。== %%POSTFIX%%睡眠和唤醒提供了相对低级的同步接口。为了激发它们在xv6 中的工*
>%%LINK%%[[#^00q88tvw0fiik|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^00q88tvw0fiik


>%%
>```annotation-json
>{"created":"2025-11-11T08:08:17.504Z","updated":"2025-11-11T08:08:17.504Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":77038,"end":77275},{"type":"TextQuoteSelector","exact":"然而，事实证明设计起来并不简单sleep 和具有此接口的wakeup 不会遇到所谓的lost wake-up 问题。假设P 发现s->count == 0 上线 212。尽管P 位于 212 和 213 行之间，V 在另一个CPU 上运行：它发生了变化s->count 为非零并调用wakeup ，它发现没有进程处于睡眠状态，因此不执行任何操作。现在P 继续在line 213 处执行：它调用sleep 并进入睡眠状态。这会导致一个问题：P 正在休眠，等待已发生的V 调用。","prefix":">lock);217 }P 现在放弃CPU 而不是旋转，这很好。","suffix":"除非我们运气好，制片人打电话来再次V ，即使计数不为零，消费者也"}]}]}
>```
>%%
>*%%PREFIX%%>lock);217 }P 现在放弃CPU 而不是旋转，这很好。%%HIGHLIGHT%% ==然而，事实证明设计起来并不简单sleep 和具有此接口的wakeup 不会遇到所谓的lost wake-up 问题。假设P 发现s->count == 0 上线 212。尽管P 位于 212 和 213 行之间，V 在另一个CPU 上运行：它发生了变化s->count 为非零并调用wakeup ，它发现没有进程处于睡眠状态，因此不执行任何操作。现在P 继续在line 213 处执行：它调用sleep 并进入睡眠状态。这会导致一个问题：P 正在休眠，等待已发生的V 调用。== %%POSTFIX%%除非我们运气好，制片人打电话来再次V ，即使计数不为零，消费者也*
>%%LINK%%[[#^c3artk8jcba|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^c3artk8jcba


>%%
>```annotation-json
>{"created":"2025-11-11T08:10:31.174Z","updated":"2025-11-11T08:10:31.174Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":77774,"end":77944},{"type":"TextQuoteSelector","exact":"我们将通过更改来修复前面的方案sleep ’sinterface：调用者必须将condition lock 传递给sleep 因此它可以在调用进程被标记为睡眠并在睡眠通道上等待后释放锁。锁会强制并发V 等待P 完成使其自身进入睡眠状态，以便wakeup 将找到正在睡眠的消费者并将其唤醒。一旦消费者再次醒来sleep 在返回之前重新获取锁。","prefix":"，但也陷入了僵局：P 在休眠时持有锁，因此V 将永远阻塞等待锁。","suffix":"我们新的正确睡眠/唤醒方案可按如下方式使用（更改以黄色突出显示）"}]}]}
>```
>%%
>*%%PREFIX%%，但也陷入了僵局：P 在休眠时持有锁，因此V 将永远阻塞等待锁。%%HIGHLIGHT%% ==我们将通过更改来修复前面的方案sleep ’sinterface：调用者必须将condition lock 传递给sleep 因此它可以在调用进程被标记为睡眠并在睡眠通道上等待后释放锁。锁会强制并发V 等待P 完成使其自身进入睡眠状态，以便wakeup 将找到正在睡眠的消费者并将其唤醒。一旦消费者再次醒来sleep 在返回之前重新获取锁。== %%POSTFIX%%我们新的正确睡眠/唤醒方案可按如下方式使用（更改以黄色突出显示）*
>%%LINK%%[[#^o94zvxaho3b|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^o94zvxaho3b


>%%
>```annotation-json
>{"created":"2025-11-11T08:12:10.066Z","updated":"2025-11-11T08:12:10.066Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":78259,"end":78360},{"type":"TextQuoteSelector","exact":"P 成立s->lock 阻止V 试图在之间唤醒它P ’ 检查s->count 及其调用sleep 。但请注意，我们需要sleep 以原子方式释放s->lock 并将消耗进程置于睡眠状态，以避免丢失唤醒。","prefix":"1;416 release(&s->lock);417 }事实是","suffix":"7.6 代码：睡眠和唤醒Xv6 的sleep (kernel/p"}]}]}
>```
>%%
>*%%PREFIX%%1;416 release(&s->lock);417 }事实是%%HIGHLIGHT%% ==P 成立s->lock 阻止V 试图在之间唤醒它P ’ 检查s->count 及其调用sleep 。但请注意，我们需要sleep 以原子方式释放s->lock 并将消耗进程置于睡眠状态，以避免丢失唤醒。== %%POSTFIX%%7.6 代码：睡眠和唤醒Xv6 的sleep (kernel/p*
>%%LINK%%[[#^75lv3zomj9m|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^75lv3zomj9m


>%%
>```annotation-json
>{"created":"2025-11-11T08:23:14.831Z","updated":"2025-11-11T08:23:14.831Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":79581,"end":79763},{"type":"TextQuoteSelector","exact":"有时会出现多个进程在同一个通道上休眠的情况；例如，多个进程从管道中读取数据。一次调用wakeup 会将它们全部唤醒。其中一个将首先运行并获取锁sleep 被调用，并且（在管道的情况下）读取管道中正在等待的任何数据。其他进程会发现，尽管被唤醒，却没有数据可读取。从他们的角度来看，这次唤醒是“虚假的”，他们必须再次入睡。因此，sleep 始终在检查条件的循环内调用。","prefix":"akeup 将看到睡眠进程并将其唤醒（除非有其他东西先唤醒它）。","suffix":"如果两次使用睡眠/唤醒意外选择相同的通道，则不会造成任何损害：它"}]}]}
>```
>%%
>*%%PREFIX%%akeup 将看到睡眠进程并将其唤醒（除非有其他东西先唤醒它）。%%HIGHLIGHT%% ==有时会出现多个进程在同一个通道上休眠的情况；例如，多个进程从管道中读取数据。一次调用wakeup 会将它们全部唤醒。其中一个将首先运行并获取锁sleep 被调用，并且（在管道的情况下）读取管道中正在等待的任何数据。其他进程会发现，尽管被唤醒，却没有数据可读取。从他们的角度来看，这次唤醒是“虚假的”，他们必须再次入睡。因此，sleep 始终在检查条件的循环内调用。== %%POSTFIX%%如果两次使用睡眠/唤醒意外选择相同的通道，则不会造成任何损害：它*
>%%LINK%%[[#^7c41e65jdro|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^7c41e65jdro


>%%
>```annotation-json
>{"created":"2025-11-11T08:37:04.476Z","updated":"2025-11-11T08:37:04.476Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":81598,"end":81652},{"type":"TextQuoteSelector","exact":"一个挑战是避免同时出现的父进程和子进程之间的竞争和僵局wait 和exit ，以及同时exit 和exit ","prefix":"久调用wait；因此每个子进程都有一个父进程要在其之后进行清理。","suffix":"。wait 首先获取wait_lock (kernel/proc"}]}]}
>```
>%%
>*%%PREFIX%%久调用wait；因此每个子进程都有一个父进程要在其之后进行清理。%%HIGHLIGHT%% ==一个挑战是避免同时出现的父进程和子进程之间的竞争和僵局wait 和exit ，以及同时exit 和exit== %%POSTFIX%%。wait 首先获取wait_lock (kernel/proc*
>%%LINK%%[[#^u92h34a6das|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^u92h34a6das


>%%
>```annotation-json
>{"created":"2025-11-19T06:42:52.678Z","updated":"2025-11-19T06:42:52.678Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":86378,"end":86446},{"type":"TextQuoteSelector","exact":"文件系统的目的是组织和存储数据。文件系统通常支持用户和应用程序之间的数据共享，以及persistence ，以便数据在重新启动后仍然可用","prefix":"就不会暂停任何核心。72Chapter 8File system","suffix":"。xv6 文件系统提供类Unix 的文件、目录和路径名（请参阅第"}]}]}
>```
>%%
>*%%PREFIX%%就不会暂停任何核心。72Chapter 8File system%%HIGHLIGHT%% ==文件系统的目的是组织和存储数据。文件系统通常支持用户和应用程序之间的数据共享，以及persistence ，以便数据在重新启动后仍然可用== %%POSTFIX%%。xv6 文件系统提供类Unix 的文件、目录和路径名（请参阅第*
>%%LINK%%[[#^9qvxzpzuwuj|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^9qvxzpzuwuj


>%%
>```annotation-json
>{"created":"2025-11-19T06:42:55.668Z","updated":"2025-11-19T06:42:55.668Z","document":{"title":"xv6: a simple, Unix-like teaching operating system","link":[{"href":"urn:x-pdf:170101f2a5a1f18c638cb14c66635bc1"},{"href":"vault:/6.S081/book.pdf"}],"documentFingerprint":"170101f2a5a1f18c638cb14c66635bc1"},"uri":"vault:/6.S081/book.pdf","target":[{"source":"vault:/6.S081/book.pdf","selector":[{"type":"TextPositionSelector","start":86512,"end":86519},{"type":"TextQuoteSelector","exact":"解决了几个挑战","prefix":"章），并将其数据存储在virtio 磁盘上以实现持久性。文件系统","suffix":"：• 文件系统需要磁盘上的数据结构来表示命名目录和文件的树，记录"}]}]}
>```
>%%
>*%%PREFIX%%章），并将其数据存储在virtio 磁盘上以实现持久性。文件系统%%HIGHLIGHT%% ==解决了几个挑战== %%POSTFIX%%：• 文件系统需要磁盘上的数据结构来表示命名目录和文件的树，记录*
>%%LINK%%[[#^3eo5wjb72ic|show annotation]]
>%%COMMENT%%
>
>%%TAGS%%
>
^3eo5wjb72ic
