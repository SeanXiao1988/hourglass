function hg_init()

    APPLICATION.setTitle("hourglass v0.1")

    RENDER.setClearColor(0x808080FF)
    
    create_1st_obj()

end

function create_1st_obj()

    local objName = "1stObject"
    sceneNode = SCENEMANAGER.createNode("1stNode")
    sceneNode:setXY(100, 100)
    SCENEMANAGER.getRoot():addChild(sceneNode)
    OBJECTMANAGER.addComponentToObject(objName, sceneNode);
    
    entity = QuadEntity.new()
    entity:setTexture(RENDER.textureLoad("test2.png"))
    entity:setTextureRect(0, 0, 512, 512)
    entity:setWidth(256)
    entity:setHeight(256)
    sceneNode:attachEntity(entity)
    
    sceneNode:setXY(128, 128)
    
    local rotAnim = RotationAnimation.new(360, 3)
    rotAnim:setType(ANIMATION_TYPE_RESET);
    sceneNode:addAnimation(rotAnim)
    
    local inputer = InputListener.new()
    OBJECTMANAGER.addComponentToObject(objName, inputer)
    inputer:setScriptCallback(input_callback)

    --[[
    backgroundNode = SCENEMANAGER.createNode("background")
    SCENEMANAGER.getRoot():addChild(backgroundNode)
    backgroundNode:setXY(400, 300)

    backgroundEntity = QuadEntity.new()
    backgroundEntity:setTexture(RENDER.textureLoad("nebula.png"))
    backgroundEntity:setTextureRect(0, 0, 800, 600)
    backgroundNode:attachEntity(backgroundEntity)
    
    OBJECTMANAGER.addComponentToObject("background", backgroundNode);
    --]]
    
    entity.a = 0
    print(entity.a)
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
        --if event:action() == INPUT_MOUSE_LEFT_RELEASE then
            local node = OBJECTMANAGER.queryComponent(listener:getObjectName(), COMP_SCENE_NODE)
            local x = event:x()
            local y = event:y()
            if node ~= nil then
                node:setXY(x, y)
            end
        --end
    end
end


function hg_deinit()
    QuadEntity.delete(entity)
    QuadEntity.delete(backgroundEntity)
end

function print_t(t)
    for i,v in pairs(t) do
        print(i, v)
    end
end