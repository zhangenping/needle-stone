#include <string>
// 演示函数
// 动物纯虚基类
class Animal {
public:
    virtual ~Animal() = default;

    // 纯虚函数：获取所属科目
    virtual std::string getSubject() const = 0;
};

// 猫科动物基类
class Felidae : public Animal {
public:
    std::string getSubject() const override {
        return "Felidae (猫科)";
    }

    // 猫科特有接口
    virtual std::string food() const = 0;
    virtual std::string sound() const = 0;
};

// 犬科动物基类
class Canidae : public Animal {
public:
    std::string getSubject() const override {
        return "Canidae (犬科)";
    }

    // 犬科特有接口
    virtual std::string food() const = 0;
    virtual std::string sound() const = 0;
};

// 具体的猫类
class Cat : public Felidae {
public:
    std::string food() const override {
        return "Fish and mice";
    }

    std::string sound() const override {
        return "Meow~";
    }
};

// 具体的狗类
class Dog : public Canidae {
public:
    std::string food() const override {
        return "Bones and meat";
    }

    std::string sound() const override {
        return "Woof! Woof!";
    }
};