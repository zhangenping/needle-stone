#include "animal.h"
#include <iostream>

// ÑÝÊ¾º¯Êý
void printAnimalInfo(const Animal& animal) {
    std::cout << "Subject: " << animal.getSubject() << std::endl;

    if (const Felidae* felidae = dynamic_cast<const Felidae*>(&animal)) {
        std::cout << "Food: " << felidae->food() << std::endl;
        std::cout << "Sound: " << felidae->sound() << std::endl;
    }
    else if (const Canidae* canidae = dynamic_cast<const Canidae*>(&animal)) {
        std::cout << "Food: " << canidae->food() << std::endl;
        std::cout << "Sound: " << canidae->sound() << std::endl;
    }

    std::cout << "-------------------" << std::endl;
}

//int main() {
//    Cat cat;
//    Dog dog;
//
//    printAnimalInfo(cat);
//    printAnimalInfo(dog);
//
//    return 0;
//}