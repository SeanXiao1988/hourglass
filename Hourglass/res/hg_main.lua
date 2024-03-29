function hg_init()
    print("hg_init")
    APPLICATION.setTitle("hourglass v0.1")

    RENDER.setClearColor(0x808080FF)
    
    create_1st_obj()
    --create_background()
    create_animation_tester()
end

function create_pe()
    print("create_pe")
    emitter = ParticleEmitter.new()
    emitter:setTotalParticles(100)
    emitter:setTexture(RENDER.textureLoad("test.png"))
    emitter:setTextureRect(0, 0, 32, 32)
    emitter:setEmitterMode(EMITTER_GRAVITY)

    local mode = emitter:getEmitterModeGravity()
    mode.gravity = {x = 0.0, y = 0.0}
    mode.speed = 60
    mode.speedVar = 20
    emitter:setEmitterModeGravity(mode)
    
    emitter:setAngle(90)
    emitter:setAngleVar(10)
    emitter:setPositionVar({x=20.0, y=20.0})
    emitter:setLifeTime(3)
    emitter:setLifeTimeVar(0.25)
    emitter:setStartSize(64)
    emitter:setStartSizeVar(10)

    emitter:setStartColor({r = 0.76, g = 0.25, b = 0.12, a = 1.0})
    emitter:setEndColor({r = 1.0, g = 1.0, b = 1.0, a = 0.7})

    emitter:setDuration(-1.0)
    emitter:setEmissionRate(20)
    
    return emitter
end

function create_background()
    backg = BackgroundEntity.new()
    backg:setTexture(RENDER.textureLoad("test2.png"))
    backg:setTextureRect(0, 0, 512, 512)
    backg:setWidth(1024)
    backg:setHeight(1024)
    backg:setType(BACKGROUND_SCROLL)
    backg:setScrollSpeedX(500)
    backg:setScrollSpeedY(300)
    SCENEMANAGER.getRoot():attachEntity(backg)
    OBJECTMANAGER.addComponentToObject(objName, backg)
end

function create_1st_obj()
    print("create_1st_obj")

    local objName = "1stObject"
    sceneNode = SCENEMANAGER.createNode("1stObject")
    sceneNode:setXY(100, 100)
    SCENEMANAGER.getRoot():addChild(sceneNode)
    OBJECTMANAGER.addComponentToObject(objName, sceneNode);
    
    pe = create_pe()
    sceneNode:attachEntity(pe)
    pe:fireEmitter()
    OBJECTMANAGER.addComponentToObject(objName, pe)
    
    sceneNode:setXY(128, 128)

    local inputer = InputListener.new()
    OBJECTMANAGER.addComponentToObject(objName, inputer)
    inputer:setScriptCallback(input_callback)
end

function create_animation_tester()
    -- 创建场景节点
    animTester = SCENEMANAGER.createNode("animTester")
    -- 设置节点坐标
    animTester:setXY(400, 300)
    -- 将节点添加至场景根节点
    SCENEMANAGER.getRoot():addChild(animTester)
    
    -- 创建实体
    testEntity = QuadEntity.new()
    -- 设置贴图
    testEntity:setTexture(RENDER.textureLoad("test.png"))
    -- 设置贴图参数
    testEntity:setTextureRect(0, 0, 128, 128)
    -- 将实体添加到场景节点
    animTester:attachEntity(testEntity)

    -- 创建旋转动画，360度，3秒
    local rotAnim = RotationAnimation.new(360, 3)
    -- 动画属性，重置
    rotAnim:setType(ANIMATION_TYPE_RESET);
    -- 将动画添加到节点
    animTester:addAnimation(rotAnim, "rot")
    
    local tranAnim = TranslateAnimation.new(100, 0, 1)
    tranAnim:setType(ANIMATION_TYPE_PINGPONG)
    animTester:addAnimation(tranAnim, "tran")
    animTester:removeAnimation("tran")
end

function input_callback(listener, event)
    if event:eventID() == EVENT_KEYBOARD then
        if event:key() == string.byte("A", 1) and event:action() == INPUT_PRESS then
            local objName = listener:getObjectName()
            local node = OBJECTMANAGER.queryComponent(objName, COMP_SCENE_NODE)
            if node ~= nil then
                x = node:getX()
                x = x - 5.0
                node:setX(x)
            end
        end
    elseif event:eventID() == EVENT_MOUSE then
        local x = event:x()
        local y = event:y()
        --[[
        local node = OBJECTMANAGER.queryComponent(listener:getObjectName(), COMP_SCENE_NODE)
        if node ~= nil then
            node:setXY(x, y)
        end
        --]]
        local node = SCENEMANAGER.getRoot()
        node:setXY(-x, -y)
    end
end


function hg_deinit()
    print("hg_deinit")
end

function print_t(t)
    for i,v in pairs(t) do
        print(i, v)
    end
end