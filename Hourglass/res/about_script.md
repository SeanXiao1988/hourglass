#Hourglass
##脚本指南 v1.0

###接口名  
IAnimation  
###方法  
isFinished  
isAnimating  
animationStage  
name  
typeID  
type  
setType  

###demo
<pre>
anim = OBJECTMANAGER.getComponent()
if anim:isAnimating() == true then
	print(anim:name())
end
</pre>