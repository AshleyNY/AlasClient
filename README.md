当前Melody版本是1.20.51，存在部分问题正在修复，sdk写法与Horion较为类似，完成后可以当作base来使用：

+ 目前已知问题：
+ 1，游戏外注入异常崩溃（已经修复）：
+  异常点在于ClientInsance返回值为nullptr
+  添加if（mc.GetClientInsance != nullptr）即可避免
    
+ 2，Autocrystal崩溃（已经修复）
+  异常点在于MovePackage函数异常，以authPackage替换使用即可

+  3，Killaura（未修复）
+  Killaura存在问题：攻击自己
+  目测原因是TargetUtils有问题
