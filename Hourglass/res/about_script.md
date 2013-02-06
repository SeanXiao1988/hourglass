Hourglass
===

脚本指南 v1.0
---

##IAnimation  

###类方法
####无
  
###成员方法  

####isFinished
**描述**  
查询动画是否结束  
**参数**  
*无*  
**返回**  
*bool* 若动画已经结束，则返回真  

####isAnimating
**描述**  
查询动画是否正在进行  
**参数**  
*无*  
**返回**  
*bool* 若动画正在进行中，则返回真  

####animationStage
**描述**  
查询动画的播放阶段  
**参数**  
*无*  
**返回**  
*enum*  

|value|description|
|:----|:----------|
|`ANIMATION_STAGE_FORWARD`|动画向前播放|
|`ANIMATION_STAGE_REVERSE`|动画向后播放| 
		 
####name
**描述**  
查询动画的名称  
**参数**  
*无*  
**返回**  
*uint32* 动画名字的hash值  

####typeID
**描述**  
查询动画的种类  
**参数** 
*无*  
**返回**  
*enum*  

|value|description|
|:----|:----------|
|`ANIMATION_ID_TRANSLATE`|位移|
|`ANIMATION_ID_ROTATE`|旋转|
|`ANIMATION_ID_SCALE`|缩放|
|`ANIMATION_ID_ALPHA`|透明度|
|`ANIMATION_ID_TEXTURE`|贴图|

####type
**描述**  
查询动画的播放类型
**参数**  
*无*  
**返回**  
*enum*  ANIMATION_TYPE  
<a id="ANIMATION_TYPE"></a>

|value|description|
|:----|:----------|
|`ANIMATION_TYPE_DEFAULT`|默认，只播放一次|
|`ANIMATION_TYPE_RESET`|结束后自动重置|
|`ANIMATION_TYPE_PINGPONG`|结束后反向播放|

####setType
**描述**  
设置动画的播放类型  
**参数**  
*enum* 参见[ANIMATION_TYPE](#ANIMATION_TYPE)  
**返回**  
*无*  

##AlphaAnimation  

###类方法
####new
**描述**  
创建一个新的alpha动画  
**参数**  
*float* alpha  
动画的alpha变化值   
*float* duration  
动画的持续时间  
**返回**  
动画的实例, 如果创建失败，返回*nil*

####delete  
**描述**  
释放一个*AlphaAnimation*的实例  
注意, 在释放动画之前，你需要先将其从节点中移除  
否则会导致内存错误

###成员方法  

####setAlpha
**描述**  
设置动画的alpha变化值  
**参数**  
*float*  alpha  
-255.0～255.0  
当参数为负时，动画将减小目标节点的alpha值  
**返回**  
*无*  

####setDuration
**描述**  
设置动画的持续时间  
**参数**  
*float* time  
**返回**  
*无*

###构造/析构方法

###demo
~~~
anim = OBJECTMANAGER.getComponent()
if anim:isAnimating() == true then
	print(anim:name())
end
~~~
