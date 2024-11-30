#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Command {
public:
    virtual ~Command() = default;
    virtual void Execute() const = 0;
};

class SimpleCommand : public Command {
private:
    std::string payload_; 

public:
    explicit SimpleCommand(const std::string& payload) : payload_(payload) {}

    void Execute() const override {
        std::cout << "SimpleCommand: Виконуємо просту команду: " << payload_ << ".\n";
    }
};


class Receiver {
public:
    void DoSomething(const std::string& data) const {
        std::cout << "Receiver: Виконую операцію із даними: " << data << ".\n";
    }

    void DoSomethingElse(const std::string& data) const {
        std::cout << "Receiver: Виконую іншу операцію із даними: " << data << ".\n";
    }
};


class ComplexCommand : public Command {
private:
    Receiver* receiver_;      
    std::string action1_;       
    std::string action2_;       

public:
    ComplexCommand(Receiver* receiver, const std::string& action1, const std::string& action2)
        : receiver_(receiver), action1_(action1), action2_(action2) {}

    void Execute() const override {
        std::cout << "ComplexCommand: Делегую роботу отримувачу.\n";
        receiver_->DoSomething(action1_);
        receiver_->DoSomethingElse(action2_);
    }
};


class Invoker {
private:
    std::unique_ptr<Command> onStart_;
    std::unique_ptr<Command> onFinish_;

public:
    void SetOnStart(std::unique_ptr<Command> command) {
        onStart_ = std::move(command);
    }

    void SetOnFinish(std::unique_ptr<Command> command) {
        onFinish_ = std::move(command);
    }

    void DoSomethingImportant() const {
        std::cout << "Invoker: Перед початком роботи...\n";
        if (onStart_) {
            onStart_->Execute();
        }

        std::cout << "Invoker: Виконую основну роботу...\n";

        std::cout << "Invoker: Після завершення роботи...\n";
        if (onFinish_) {
            onFinish_->Execute();
        }
    }
};


int main() {
    Invoker invoker;

    invoker.SetOnStart(std::make_unique<SimpleCommand>("Складати звіт"));

    Receiver receiver;
    invoker.SetOnFinish(std::make_unique<ComplexCommand>(&receiver, "Зберегти звіт", "Відправити звіт"));

    invoker.DoSomethingImportant();

    return 0;
}

