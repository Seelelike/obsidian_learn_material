基本术语

![image-20251021191717377](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021191717377.png)

欠拟合与过拟合

过拟合没有完美的解决方案。是ML的核心内容，所有的算法所有的计算都在缓解过拟合。

问自己，算法使用什么方法缓解过拟合，这种方法在什么时候会失效。可以把握算法的应用场景。

![image-20251021192916471](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021192916471.png)

如何得知模型对未来数据的表现

到底想要模型实现什么效果，对任务有哪些刻画标准

在统计意义上，模型是否是好的。



#### 评估方法

关键：怎么获得”测试集“。（测试集与训练集应互斥。）
<!--ID: 1764664440198-->


常见方法：

- 留出法。

  - 保持数据发布的一致性

  - 多次随机划分

  - 测试集不能太大或太小
   > 若令训练集 S 包含绝大多数样 本 7 则训 练出 的模型可能更接近于用 D 训 练 出的模型， 但由 于 T 比较小 ，评估 结 果 可能 不够 稳定准 确 ;若令测试集 T 多包含一些样本， 则训 练集 S 与 D 差别更大了，被评 估的模 型 与用 D 训练出 的模 型相比可能有 较大差 别?从而 降低了评估结果的保 真性 (fi d e lity) 这个问题 没有完美 的解决 方案 ， 常见做法是 将大约 2 / 3 rv 4/5 的 样本用于训练，剩余样本用 于 测试

[[机器学习 (周志华-(书签带目录)) (Z-Library) (1).pdf#page=40&selection=97,10,217,2|机器学习 (周志华-(书签带目录)) (Z-Library) (1), 页面 40]]

- k-折交叉验证法

  ![image-20251021194211272](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021194211272.png)

- 自助法

  ![image-20251021194505344](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021194505344.png)

调参与验证集

![image-20251021195338218](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021195338218.png)

性能度量

![image-20251021195848266](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021195848266.png)

- 均方误差：回归任务常用

  ![image-20251021200114446](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021200114446.png)

- 错误率-精度

  ![image-20251021200144341](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021200144341.png)

- 查准率-查全率

  ![image-20251021200239011](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021200239011.png)

  ![image-20251021200539861](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021200539861.png)

- 比较检验

  ![image-20251021201109229](C:\Users\Suife\AppData\Roaming\Typora\typora-user-images\image-20251021201109229.png)