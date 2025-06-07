print('-------------------')
-- self 변수가 존재하는지 확인
if self then
    -- self 변수를 통해 C++ 함수 GetName()을 호출!
    local actorName = self:GetName()
    print('This script is attached to an Actor named: ' .. actorName)
else
    print('Error: "self" is not defined!')
end
print('-------------------')