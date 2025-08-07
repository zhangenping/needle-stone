-- 定义一个无参数的Hello World函数
function sayHello()
    print("Hello World from Lua function!")
end

-- 定义一个带参数的函数
function greet(name, age)
    local message = string.format("Hello %s, you are %d years old!", name, age)
    print(message)
    return "Greeting successful"  -- 返回一个值
end