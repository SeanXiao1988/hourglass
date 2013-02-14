Hourglass
===

基础动画模块脚本说明
---



##IAnimation  
**描述**  
IAnimation为余下动画类的接口类  

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

##RotationAnimation

###类方法

####new
**描述**  
创建一个新的rotation动画  
**参数**  
*float* rotate  
动画的旋转变化值      
*float* duration  
动画的持续时间  
**返回**  
动画的实例, 如果创建失败，返回*nil*

####delete  
**描述**  
释放一个*RotationAnimation*的实例  
注意, 在释放动画之前，你需要先将其从节点中移除  
否则会导致内存错误

###成员方法
####setRotation
**描述**  
设置动画的旋转值  
**参数**  
*float* rotate  
单位为角度，正值表示顺时针方向旋转  
**返回**  
*无*  

####setDuration
**描述**  
设置动画的持续时间  
**参数**  
*float* time  
**返回**  
*无*  

##ScaleAnimation

###类方法

####new
**描述**  
创建一个新的scale动画  
**参数**  
*float* scaleX  
节点的宽度缩放变化值  
*float* scaleY  
节点的高度缩放变化值  
*float* duration  
动画的持续时间  
**返回**  
动画的实例, 如果创建失败，返回*nil*

####delete  
**描述**  
释放一个*ScaleAnimation*的实例  
注意, 在释放动画之前，你需要先将其从节点中移除  
否则会导致内存错误

###成员方法
####setScale
**描述**  
设置节点的缩放值  
**参数**  
*float* scaleX  
节点的宽度缩放变化值  
*float* scaleY  
节点的高度缩放变化值  
**返回**  
*无*  

####setDuration
**描述**  
设置动画的持续时间  
**参数**  
*float* time  
**返回**  
*无*  

##TranslateAnimation

###类方法

####new
**描述**  
创建一个新的translate动画  
**参数**  
*float* transX  
节点在X轴向上的移动值  
*float* transY  
节点在Y轴向上的移动值  
*float* duration  
动画的持续时间  
**返回**  
动画的实例, 如果创建失败，返回*nil*

####delete  
**描述**  
释放一个*TranslateAnimation*的实例  
注意, 在释放动画之前，你需要先将其从节点中移除  
否则会导致内存错误

###成员方法
####setTranslate
**描述**  
设置节点的缩放值  
**参数**  
*float* transX  
节点在X轴向上的移动值  
*float* transY  
节点在Y轴向上的移动值  
**返回**  
*无*  

####setDuration
**描述**  
设置动画的持续时间  
**参数**  
*float* time  
**返回**  
*无*  

###demo
~~~
anim = OBJECTMANAGER.getComponent()
if anim:isAnimating() == true then
	print(anim:name())
end
~~~