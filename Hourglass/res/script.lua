                        
ass = SCENEMANAGER.getNode("shitNode")
ass:setWorldXY(100, 100)
n = ass:getHashName()
print(n)
print(HASH.stringForHash(n))
SCENEMANAGER.deleteNode(ass)

function hg_begin()
    print("hg_begin")
end

--[[--------------------------------------------------------
变量: ANIMATIONCOMPILER
说明: 动画编译器

方法: clear
参数: n/a
返回: n/a
说明: 清空编译历史

方法: parseXML
参数: string, xml文件名
返回: animation_header_t* 指向编译结果的userdata
说明: 编译指定的动画文件

方法: deleteAnimation
参数: 指向动画的userdata
返回: n/a
说明: 删除并释放指定的动画
--]]

--[[--------------------------------------------------------
变量: HASH
说明: hash编码

方法: make
参数: string, 需要计算hash值的字符串
返回: uint32_t, 目标字符串的hash值
说明: n/a

方法: stringForHash
参数: uint32_t, 字符串的hash值
返回: hash值对应的字符串
说明: 只能够还原使用make计算过的字符串

--]]

--[[--------------------------------------------------------
变量: QuadEntity
说明: 矩形实体类

方法: new
参数: n/a
返回: 一个矩形实体对象
说明: 创建一个矩形实体对象, 类方法, QuadEntity.new()

方法: delete
参数: 矩形实体对象
返回: n/a
说明: 销毁一个矩形实体对象, 类方法, QuadEntity.delete(quad)

方法: setTexture
参数: uint32_t, 贴图的ID
返回: n/a
说明: 为一个矩形实体设置贴图

方法: setTextureRect
参数: float x,y,w,h
返回: n/a
说明: 为一个矩形实体设置贴图参数

方法: setTextureRect
参数: float x,y,w,h
返回: n/a
说明: 为一个矩形实体设置贴图参数

方法: setColor
参数: uint32_t rgba, int i, 颜色的rgba值, 指定的顶点编号, 默认为-1
返回: n/a
说明: 为一个矩形实体设置顶点颜色, 默认设置4个顶点, quad:setColor(0xRRGGBBAA)

方法: setVertexAlpha
参数: uint8_t alpha, int i
返回: n/a
说明: 为一个矩形实体设置顶点alpha, 默认设置4个顶点

方法: setVertexZ
参数: float z, int i
返回: n/a
说明: 为一个矩形实体设置顶点Z序, 默认设置4个顶点, 目前无效

方法: setBlendMode
参数: int
返回: n/a
说明: 为一个矩形实体设置渲染模式, 渲染模式参照引擎常量条目

方法: setCenterPoint
参数: float x,y
返回: n/a
说明: 为一个矩形实体设置中心点

方法: setWidth
参数: float w
返回: n/a
说明: 为一个矩形实体设置宽度

方法: setHeight
参数: float h
返回: n/a
说明: 为一个矩形实体设置高度

方法: getQuad
参数: n/a
返回: Quad类型
说明: 获取矩形实体的Quad对象

方法: getTexture
参数: n/a
返回: uint32_t
说明: 获取矩形实体的贴图ID, 0为无效贴图

方法: getTextureRect
参数: n/a
返回: float x,y,w,h
说明: 获取矩形实体的贴图参数

方法: getColor
参数: int
返回: uint32_t RRGGBBAA
说明: 获取矩形实体的顶点颜色, 默认获取第0个顶点

方法: getVertexAlpha
参数: int
返回: uint8_t AA
说明: 获取矩形实体的顶点alpha, 默认获取第0个顶点

方法: getVertexZ
参数: int
返回: float
说明: 获取矩形实体的顶点z序, 默认获取第0个顶点

方法: getBlendMode
参数: n/a
返回: int
说明: 获取矩形实体的渲染模式

方法: getCenterPoint
参数: n/a
返回: float x,y
说明: 获取矩形实体的中心点坐标

方法: initAnimation
参数: animation_header_t*
返回: n/a
说明: 为矩形实体初始化一个动画对象

方法: deInitAnimation
参数: n/a
返回: n/a
说明: 释放矩形实体内的动画对象

方法: playAnimation
参数: uint32_t
返回: n/a
说明: 播放指定的动画序列, 参数为动画名的hash值

方法: pauseAnimation
参数: n/a
返回: n/a
说明: 暂停当前的动画序列

方法: resumeAnimation
参数: n/a
返回: n/a
说明: 恢复播放当前的动画序列

方法: stopAnimation
参数: n/a
返回: n/a
说明: 停止当前的动画序列

方法: nextFrame
参数: n/a
返回: n/a
说明: 强制切换到动画序列的下一帧

方法: prevFrame
参数: n/a
返回: n/a
说明: 强制切换到动画序列的上一帧

方法: isAnimating
参数: n/a
返回: boolean
说明: 查询当前是否在播放动画序列
--]]

--[[--------------------------------------------------------
变量: RENDER
说明: 渲染管理器

方法: setClearColor
参数: uint32_t RRGGBBAA
返回: n/a
说明: 设置缓冲区清理颜色

方法: textureCreate
参数: int w,h
返回: 贴图的ID
说明: 创建指定大小的空白贴图

方法: textureFree
参数: uint32_t texID
返回: n/a
说明: 释放指定的贴图资源, 资源ID可能会被复用

方法: textureGetWidth
参数: uint32_t texID
返回: int
说明: 计算指定的贴图资源的宽度

方法: textureGetHeight
参数: uint32_t texID
返回: int
说明: 计算指定的贴图资源的高度

方法: textureLock
参数: uint32_t texID
返回: n/a
说明: 锁定指定的贴图

方法: textureUnlock
参数: uint32_t texID
返回: n/a
说明: 解锁指定的贴图

方法: textureLoad
参数: string filename
返回: n/a
说明: 从文件中载入贴图资源, 目前只支持png格式

方法: textureFreeAll
参数: n/a
返回: n/a
说明: 立即释放所有贴图, 这个操作是强制的

方法: rtargetCreate
参数: int w,h, bool zbuff
返回: uint32_t rtarID
说明: 创建一个渲染目标

方法: rtargetFree
参数: uint32_t rtarID
返回: n/a
说明: 释放一个渲染目标, 渲染目标ID可能会被复用

方法: rtargetGetTexture
参数: uint32_t rtarID
返回: uint32_t texID
说明: 获取指定渲染目标的贴图ID

方法: rtargetFreeAll
参数: n/a
返回: n/a
说明: 立即释放所有的渲染目标, 这个操作是强制的

方法: shaderLoad
参数: string filename, bool activate
返回: uint32_t shaderID
说明: 从文件中载入着色器并编译, 为了简单起见, 载入的文件为[filename].frag和[filename].vert, 如果指定了activate为真则会激活该着色器.

方法: shaderFree
参数: uint32_t shaderID
返回: n/a
说明: 释放指定的着色器

方法: shaderActive
参数: uint32_t shaderID
返回: n/a
说明: 激活指定的着色器

方法: shaderDeactive
参数: n/a
返回: n/a
说明: 使当前的着色器失效

方法: shaderPush
参数: n/a
返回: n/a
说明: 使当前的着色器失效并压栈

方法: shaderPop
参数: n/a
返回: n/a
说明: 从着色器栈中弹出栈顶的着色器并激活

方法: getWidth
参数: n/a
返回: int
说明: 获取窗口宽度

方法: getHeight
参数: n/a
返回: int
说明: 获取窗口高度
--]]

--[[--------------------------------------------------------
变量: SceneNode
说明: 场景节点

方法: getHashName
参数: n/a
返回: n/a
说明: 获取场景节点名的hash值

方法: setAlpha
参数: uint8_t
返回: n/a
说明: 设置场景节点的alpha

方法: setX
参数: float
返回: n/a
说明: 设置场景节点的x坐标

方法: setY
参数: float
返回: n/a
说明: 设置场景节点的y坐标

方法: setXY
参数: float x,y
返回: n/a
说明: 设置场景节点的坐标

方法: setScaleX
参数: float
返回: n/a
说明: 设置场景节点在x轴上的比例

方法: setScaleY
参数: float
返回: n/a
说明: 设置场景节点在y轴上的比例

方法: setScaleXY
参数: float sx,sy
返回: n/a
说明: 设置场景节点的比例

方法: setRotation
参数: float
返回: n/a
说明: 设置场景节点的旋转角度, 正角度为顺时针

方法: setVisibility
参数: boolean
返回: n/a
说明: 设置场景节点的可视与否

方法: setWorldX
参数: float
返回: n/a
说明: 设置场景节点在屏幕上的绝对坐标

方法: setWorldY
参数: float
返回: n/a
说明: 设置场景节点在屏幕上的绝对坐标

方法: getAlpha
参数: n/a
返回: uint8_t
说明: 获取场景节点的alpha

方法: getX
参数: n/a
返回: float
说明: 获取场景节点的x坐标

方法: getY
参数: n/a
返回: float
说明: 获取场景节点的y坐标

方法: getXY
参数: n/a
返回: float x,y
说明: 获取场景节点的坐标

方法: getScaleX
参数: n/a
返回: float
说明: 获取场景节点在x轴上的比例

方法: getScaleY
参数: n/a
返回: float
说明: 获取场景节点在y轴上的比例

方法: getScaleXY
参数: n/a
返回: float
说明: 获取场景节点的比例

方法: getRotation
参数: n/a
返回: float
说明: 获取场景节点的旋转角度

方法: getVisibility
参数: n/a
返回: boolean
说明: 获取场景节点的可视性

方法: getWorldX
参数: n/a
返回: float
说明: 获取场景节点在屏幕上的绝对坐标

方法: getWorldY
参数: n/a
返回: float
说明: 获取场景节点在屏幕上的绝对坐标

方法: getWorldXY
参数: n/a
返回: float
说明: 获取场景节点在屏幕上的绝对坐标

方法: setParent
参数: SceneNode
返回: n/a
说明: 设置场景节点的父节点, 这个操作将会替换掉原有的关系

方法: getChild
参数: n/a
返回: SceneNode
说明: 获取场景节点的子节点

方法: addChild
参数: SceneNode
返回: n/a
说明: 向场景节点添加子节点, 这个操作将会替换掉子节点原有的关系

方法: removeChild
参数: SceneNode
返回: n/a
说明: 从场景节点中移除指定的子节点

方法: removeAll
参数: n/a
返回: n/a
说明: 从场景节点中移除所有的子节点

方法: getEntity
参数: n/a
返回: IEntity
说明: 获取场景节点的渲染实体
--]]

--[[--------------------------------------------------------
变量: SCENEMANAGER
说明: 场景管理器

方法: createNode
参数: string name
返回: SceneNode
说明: 创建一个新的场景节点

方法: deleteNode
参数: string name
返回: n/a
说明: 销毁一个场景节点

方法: getNode
参数: string name
返回: SceneNode
说明: 查询场景节点

方法: getRoot
参数: n/a
返回: SceneNode
说明: 查询场景根节点

--]]