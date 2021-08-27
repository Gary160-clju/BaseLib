条件变量是非常底层的同步原语，很少直接使用，一般都是用它来实现高层的同步措施，如 BlockingQueue<T>或 CountDownLatch。
用这三样东西（thread、mutex、condition）可以完成任何多线程编程任务。当然我们一般也不会直接使用它们（mutex 除外），而是使
用更高层的封装，例如 mutex∶∶ThreadPool和 mutex∶∶CountDownLatch 等。一个多线程程序里如果大量使用 mutex 和condition variable 来同步，基本跟用铅笔刀锯大树没啥区别。
