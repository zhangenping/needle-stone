#include <string>
// ��ʾ����
// ���﴿�����
class Animal {
public:
    virtual ~Animal() = default;

    // ���麯������ȡ������Ŀ
    virtual std::string getSubject() const = 0;
};

// è�ƶ������
class Felidae : public Animal {
public:
    std::string getSubject() const override {
        return "Felidae (è��)";
    }

    // è�����нӿ�
    virtual std::string food() const = 0;
    virtual std::string sound() const = 0;
};

// Ȯ�ƶ������
class Canidae : public Animal {
public:
    std::string getSubject() const override {
        return "Canidae (Ȯ��)";
    }

    // Ȯ�����нӿ�
    virtual std::string food() const = 0;
    virtual std::string sound() const = 0;
};

// �����è��
class Cat : public Felidae {
public:
    std::string food() const override {
        return "Fish and mice";
    }

    std::string sound() const override {
        return "Meow~";
    }
};

// ����Ĺ���
class Dog : public Canidae {
public:
    std::string food() const override {
        return "Bones and meat";
    }

    std::string sound() const override {
        return "Woof! Woof!";
    }
};