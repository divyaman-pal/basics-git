#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

class BankAccount {
protected:
    std::string accountNumber;
    double balance;
    std::string openDate;
    std::vector<std::string> transactionHistory;

public:
    BankAccount(double initialBalance) : balance(initialBalance) {
        // Generate a random 9-digit account number
        srand(time(NULL));
        accountNumber = std::to_string(rand() % 1000000000 + 100000000);
        
        // Get current date as open date
        time_t now = time(0);
        openDate = ctime(&now);
    }

    virtual void deposit(double amount) {
        balance += amount;
        transactionHistory.push_back("Deposit: " + std::to_string(amount));
    }

    virtual void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            transactionHistory.push_back("Withdrawal: " + std::to_string(amount));
        } else {
            std::cout << "Insufficient funds!" << std::endl;
        }
    }

    virtual void displayStatement() {
        std::cout << "Account Number: " << accountNumber << std::endl;
        std::cout << "Balance: $" << balance << std::endl;
        std::cout << "Transaction History:" << std::endl;
        for (const auto& transaction : transactionHistory) {
            std::cout << transaction << std::endl;
        }
    }

    std::string getAccountNumber() const {
        return accountNumber;
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(double initialBalance, double rate) : BankAccount(initialBalance), interestRate(rate) {
        if (rate > 0.06) {
            interestRate = 0.06; // Cap interest rate at 6%
        }
    }

    double getInterestRate() {
        return interestRate;
    }

    void depositInterest() {
        double interest = balance * interestRate;
        deposit(interest);
    }
};

class CheckingAccount : public BankAccount {
public:
    CheckingAccount(double initialBalance) : BankAccount(initialBalance) {}

    double getInterestRate() {
        throw std::invalid_argument("Checking accounts do not have an interest rate.");
    }
};

class BankAccountHolder {
private:
    std::string name;
    std::string username;
    std::string netBankingPassword;
    std::vector<BankAccount*> accounts;

public:
    BankAccountHolder(std::string n, std::string u, std::string p) : name(n), username(u), netBankingPassword(p) {}

    void createAccount(double initialBalance, bool isSavings) {
        if (isSavings) {
            accounts.push_back(new SavingsAccount(initialBalance, 0.04)); // Example interest rate of 4%
        } else {
            accounts.push_back(new CheckingAccount(initialBalance));
        }
    }

    void deposit(int accountIndex, double amount) {
        accounts[accountIndex]->deposit(amount);
    }

    void withdraw(int accountIndex, double amount) {
        accounts[accountIndex]->withdraw(amount);
    }

    void displayStatement(int accountIndex) {
        accounts[accountIndex]->displayStatement();
    }

    int getNumAccounts() const {
        return accounts.size();
    }

    ~BankAccountHolder() {
        for (auto& account : accounts) {
            delete account;
        }
    }

    std::string getUsername() const {
        return username;
    }

    std::string getPassword() const {
        return netBankingPassword;
    }
};

class BranchManager {
private:
    std::string username;
    std::string password;

    BranchManager() : username("manager"), password("manager123") {}

public:
    static BranchManager& getInstance() {
        static BranchManager instance;
        return instance;
    }

    bool authenticate(std::string user, std::string pass) const {
        return (user == username && pass == password);
    }

    void viewAccountStatement(const BankAccountHolder& accountHolder, int accountIndex) {
        accountHolder.displayStatement(accountIndex);
    }

    void getAccountHolders(const std::vector<BankAccountHolder>& accountHolders) {
        std::cout << "Account Holders:" << std::endl;
        for (const auto& accountHolder : accountHolders) {
            std::cout << "Username: " << accountHolder.getUsername() << std::endl;
        }
    }

    void fastForward(int days) {
        // Logic to fast forward time and calculate interest for savings accounts
    }
};

void signUp(std::vector<BankAccountHolder>& users) {
    std::string username, password, name;
    std::cout << "Sign Up" << std::endl;
    std::cout << "Enter your name: ";
    std::cin >> name;
    std::cout << "Enter your username: ";
    std::cin >> username;
    std::cout << "Enter your password: ";
    std::cin >> password;

    users.push_back(BankAccountHolder(name, username, password));
    std::cout << "Sign Up Successful!" << std::endl;
}

int main() {
    std::vector<BankAccountHolder> users;
    std::string username, password;
    bool loggedIn = false;
    BankAccountHolder* currentUser = nullptr;
    BranchManager& manager = BranchManager::getInstance();

    while (!loggedIn) {
        int choice;
        std::cout << "Welcome to the Bank!" << std::endl;
        std::cout << "1. Sign Up" << std::endl;
        std::cout << "2. Login" << std::endl;
        std::cout << "3. Manager Login" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                signUp(users);
                break;
            case 2: {
                std::cout << "Login" << std::endl;
                std::cout << "Enter your username: ";
                std::cin >> username;
                std::cout << "Enter your password: ";
                std::cin >> password;

                for (auto& user : users) {
                    if (user.getUsername() == username && user.getPassword() == password) {
                        loggedIn = true;
                        currentUser = &user;
                        std::cout << "Login Successful!" << std::endl;
                        break;
                    }
                }
                if (!loggedIn) {
                    std::cout << "Incorrect username or password. Please try again." << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << "Manager Login" << std::endl;
                std::cout << "Enter manager username: ";
                std::cin >> username;
                std::cout << "Enter manager password: ";
                std::cin >> password;

                if (manager.authenticate(username, password)) {
                    std::cout << "Manager Login Successful!" << std::endl;
                    manager.getAccountHolders(users);
                } else {
                    std::cout << "Incorrect manager username or password. Please try again." << std::endl;
                }
                break;
            }
            case 4:
                std::cout << "Exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }

    return 0;
}
