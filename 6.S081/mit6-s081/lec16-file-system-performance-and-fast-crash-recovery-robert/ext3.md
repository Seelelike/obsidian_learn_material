log结构
transaction
- 序列号
- 一系列block编号
- 一系列handle
>transaction只能在所有已经开始了的系统调用都执行了stop之后才能commit。所以transaction需要记住所有已经开始了的handle，这样才能在系统调用结束的时候做好记录。


superblock + transaction(descriptor + data + commit) + transaction ...
![](https://mit-public-courses-cn-translatio.gitbook.io/mit6-s081/~gitbook/image?url=https%3A%2F%2F1977542228-files.gitbook.io%2F%7E%2Ffiles%2Fv0%2Fb%2Fgitbook-legacy-files%2Fo%2Fassets%252F-MHZoT2b_bcLghjAOPsJ%252F-MT3OCM7LxI_e3yHrCXg%252F-MT65R6eIdHZpXO6ZZ8S%252Fimage.png%3Falt%3Dmedia%26token%3D755ffa58-48f6-4784-a94d-cfa9c86aa7ca&width=768&dpr=4&quality=100&sign=6cb2cd0d&sv=2)

提升性能的三种方法
- async异步系统调用，系统调用不需要等待commit结束就可提前返回。
- batching，执行批量执行，将多个系统调用打包成一个transaction
- concurrency

系统调用的返回并不能表示系统调用应该完成的工作实际完成了。
>flush

batching好处
- 多个系统调用之间分摊了transaction带来的固有的损耗。
- 更容易触发write absorption。
- disk scheduling。

concurrency
- open transaction
- comming to log
- writing to home
- freed

如果一个block cache正在被更新，而这个block又正在被写入到磁盘的过程中，会怎样呢？
>COW

commit transaction步骤
- 阻止系统调用
- 等待当前系统调用们结束
- 开启新transaction并继续系统调用
- 更新descriptor block
- 写入log
- commit block
- 等待commit block结束
- 重用该transaction对应log空间

为什么新transaction需要等待前一个transaction中系统调用完成再执行。
>会破坏transaction中的原子性。