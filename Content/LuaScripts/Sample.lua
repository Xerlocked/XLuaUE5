print('-------------------')
if self then
    -- 테스트 1: 파라미터가 있는 함수 호출
    local newLabel = "My Lua-Powered Actor"
    print("Setting actor label to: '" .. newLabel .. "'")
    self:SetActorLabel(newLabel)

    -- 잠시 후 라벨이 바뀌었는지 다시 확인
    print("New actor label is: '" .. self:GetActorLabel() .. "'")
    print('--------------------')

    -- 테스트 2: 구조체(FVector)를 리턴하는 함수 호출
    print("Getting actor location...")
    local location = self:K2_GetActorLocation()

    if location then
        print("SUCCESS: Got actor location via reflection!")
        print("Location X: " .. location.x)
        print("Location Y: " .. location.y)
        print("Location Z: " .. location.z)
    else
        print("FAILURE: Could not get actor location.")
    end
else
    print('Error: "self" is not defined!')
end
print('-------------------')