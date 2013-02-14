Hourglass
===

基础脚本说明
---

##概述
####Hourglass是耗时一年多时间编写完成的一个2D游戏引擎，内核采用C/C++编写，渲染部分则采用了OpenGL&trade;，引擎核心只能通过C/C++进行扩展，但是游戏逻辑可以通过Lua脚本语言进行编写，我们打算通过一系列的文档来完成Hourglass引擎脚本部分的说明。

##注意
#####您可能需要一定的计算机编程基础才能比较好的继续阅读以下内容；如果您学习过C++/Java等面对对象的语言，那么您应该不会感觉到任何压力。此外，Hourglass引擎采用lua脚本语言进行扩展，所以您可能需要先行学习[lua语言](http://www.lua.org/)的相关知识。
  
##关于Lua脚本
####注释
虽然我并不打算在这里介绍lua的基本语法，但是为了避免给从来没有使用lua编程经验的读者带来困扰（不是每个人都像你一样好学，对吧？）  
<pre>
--Lua使用两个连续的‘-’号开开始一个单行注释
--这是一个单行注释
  
--[[  
Lua使用'--[[' 和 ‘--]]’来开始一个多行注释  
这是块注释  
这里也是注释
--]]  
</pre>

####常量
**这里的常量是指那些在引擎启动之后就初始化好的数据**，但是在Lua中，这些数据并非不接受修改，但是修改后的数据可能不会被引擎核心接受。
<pre>
print(BLEND_DEFAULT)
--在这里，print是lua的一个函数，作用是打印指定的内容
--BLEND_DEFAULT是Hourglass(以下均简写为HG)定义的一个常量，用于表示默认的混合模式
</pre>
上述脚本将会在终端输出2，这个数字是BLEND_DEFAULT的值，这个值在HG核心中是不能被修改的，但是在lua中可以这样写  
<pre>
print(BLEND_DEFAULT)
BLEND_DEFAULT = 9
print(BLEND_DEFAULT)
</pre>
上面的脚本输出将会是2，9，可以知道BLEND_DEFAULT的值已经被改变了，但是需要注意的是，如果你使用修改后的BLEND_DEFAULT，HG核心是不会认可的。  
如果你还不是很理解，不用担心，看看下面的描述
<pre>
初始化时
+-----+---------------+---+
| Lua | BLEND_DEFAULT | 2 |
+-----+---------------+---+
+-----+---------------+---+
| HG  | BLEND_DEFAULT | 2 |
+-----+---------------+---+

脚本执行完后
+-----+---------------+---+
| Lua | BLEND_DEFAULT | 9 |
+-----+---------------+---+
+-----+---------------+---+
| HG  | BLEND_DEFAULT | 2 |
+-----+---------------+---+
</pre>
可以看到，当尝试在Lua中修改一个常量时，修改的只是HG复制给Lua的一个拷贝。打个比方，HG中的常量相当于你在银行的存款，Lua中对应的常量就是你银行存折上的数字，当你将存折上的数字涂改成一个天文数字，然后去柜台要求取款的时候，是不可能兑现的。

####类
**类(class)是一种面向对象编程的构造，是创建对象的蓝图，描述了所创建对象共同的属性和方法。**  
如果说上面这段来自wiki百科的说明让你感到困惑，你也许应该停止阅读这份说明，并去尝试学习一门面向对象语言（个人推荐Java或是Objective-C）。   
无论如何，我还是要举个不太准确的例子：  
人(Human)是一种类(class)，小明(ming)是人的一个实例(instance)
<pre>
--小明诞生了
ming = Human.new()
--小明死去了
Human.delete(ming)
</pre>
虽然有点残酷，但是这种风格的代码将贯穿整个HG引擎的脚本部分。

####方法
**方法(method)**和**函数(function)**的区别在于方法要与类关联在一起  
我们前面已经见过lua的一个函数print，其调用方法也十分简单
<pre>
print("hello world")
</pre>
但是方法却有一些区别，举个例子
<pre>
--将窗口名称设置为"我的第一个游戏"
APPLICATION.setTitle("我的第一个游戏")
</pre>
在这里APPLICATION是一个类，setTitle是这个类的一个方法，称为**类方法**  
整个过程可以理解为“调用APPLICATION”的“setTitle”方法，参数为字符串：“我的第一个游戏”  
如果这有点令人困惑，那么还是使用之前的例子  
<pre>
--小明诞生了
ming = Human.new()
--小明的宠物猫，汤姆诞生了
tom = Cat.new()
</pre>
可以看到，Human和Cat都有一个方法叫new，功能是创建该类的一个实例