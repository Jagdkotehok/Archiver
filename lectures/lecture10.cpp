#include <iostream>
#include <string>

class Printable {
public:
    virtual void Print() const = 0; // pure virtual
};

class Person : public Printable {
public:
    Person(const std::string& name) : name_(name) {
        std::cout << "Person" << std::endl;
    }

    virtual std::string GetInfo() const {
        return "Name: " + name_;
    }

    void Print() const override {
        std::cout << GetInfo() << std::endl;
    }

    virtual ~Person() {
        std::cout << "~Person" << std::endl;
    }

protected:
    std::string name_;
};

class Student : public Person {
public:
    Student(const std::string& name, int group) : Person(name), group_(group) {
        std::cout << "Student" << std::endl;
    }

    std::string GetInfo() const override {
        return Person::GetInfo() + ", Group: " + std::to_string(group_);
    }

    ~Student() {
        std::cout << "~Student" << std::endl;
    }

private:
    int group_ = 0;
};

class Teacher : public Person {
public:
    Teacher(const std::string& name, const std::string& course) : Person(name), course_(course) {
        std::cout << "Teacher" << std::endl;
    }

    std::string GetInfo() const override {
        return Person::GetInfo() + ", Course: " + course_;
    }

    ~Teacher() {
        std::cout << "~Teacher" << std::endl;
    }


private:
    std::string course_;
};

void Print(const Printable& printable) {
    printable.Print();
}

int main() {
    Student student("Ivan", 221);

    Print(student);

    return 0;
}
