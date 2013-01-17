function hg_init()

    APPLICATION.setTitle("hourglass v0.1")

    RENDER.setClearColor(0x808080FF)
    
    create_1st_obj()

end

function create_1st_obj()
    local objName = "1stObject"
    local sceneNode = SCENEMANAGER.createNode("1stNode")
    sceneNode:setXY(100, 100)
    SCENEMANAGER.getRoot():addChild(sceneNode)
    OBJECTMANAGER.addComponentToObject(objName, sceneNode);
    
    entity = QuadEntity.new()
    entity:setTexture(RENDER.textureLoad("map.png"))
    entity:setTextureRect(0, 0, 256, 256)
    sceneNode:attachEntity(entity)
    
    sceneNode:setXY(128, 128)
    
    local rotAnim = RotationAnimation.new(360, 3)
    rotAnim:setType(ANIMATION_TYPE_RESET);
    sceneNode:addAnimation(rotAnim)
    
    local inputer = InputListener.new()
    OBJECTMANAGER.addComponentToObject(objName, inputer)
    inputer:setScriptCallback("input_callback")
end

function input_callback(listener, event)
    print(event)
end


function hg_deinit()
    QuadEntity.delete(entity)
end

function hg_init2()

    APPLICATION.setTitle("hourglass v0.1")

    RENDER.setClearColor(0x808080FF)

    anime = SPRITEANIMATIONCOMPILER.parseXML("animation.xml")

    shitNode = SCENEMANAGER.createNode("shitNode")

    root = SCENEMANAGER.getRoot()

    root:addChild(shitNode)

    shitNode:setXY(400, 300)

    testAnimator = QuadEntity.new()

    tex = RENDER.textureLoad("map.png")

    testAnimator:setTexture(tex)

    testAnimator:setTextureRect(0, 0, 256, 256)

    shitNode:attachEntity(testAnimator)

    --testAnimator:initAnimation(anime)

    --testAnimator:playAnimation(HASH.make("1stAnimation"))
    
    icomp = shitNode:getEntity()
    print(HASH.stringForHash(icomp:getComponentName()))

    trans1 = TranslateAnimation.new(100, 0, 1)
    trans1:setType(ANIMATION_TYPE_PINGPONG)
    shitNode:addAnimation(trans1, "trans1")

    trans2 = TranslateAnimation.new(0, 100, 1)
    trans2:setType(ANIMATION_TYPE_PINGPONG)
    shitNode:addAnimation(trans2, "trans2")

    rot = RotationAnimation.new(360, 3)
    rot:setType(ANIMATION_TYPE_RESET)
    shitNode:addAnimation(rot, "rot")

    scl = ScaleAnimation.new(0.2, 0.2, 1)
    scl:setType(ANIMATION_TYPE_PINGPONG)
    shitNode:addAnimation(scl, "scale")
    
    alp = AlphaAnimation.new(-200, 1)
    alp:setType(ANIMATION_TYPE_PINGPONG)
    shitNode:addAnimation(alp, "alp")
    
    map = MapData.new()
    map:create(16, 16)

end

function hg_deinit2()
    QuadEntity.delete(testAnimator)
    MapData.delete(map)
end

function print_t(t)
    for i,v in pairs(t) do
        print(i, v)
    end
end