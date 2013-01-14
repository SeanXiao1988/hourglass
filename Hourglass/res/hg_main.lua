function hg_init()

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
    
    trans = TranslateAnimation.new(200, 200, 0.03)
    testAnimator:addAnimation(trans, "trans")

end

function hg_deinit()
    QuadEntity.delete(testAnimator)
end

function print_t(t)
    for i,v in pairs(t) do
        print(i, v)
    end
end