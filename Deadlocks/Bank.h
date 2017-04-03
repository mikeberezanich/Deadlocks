#include <mutex>

using namespace std;

class Account {

private:
	double balance;

public:
	mutex mLock;
	bool isLocked;

	Account(double amount);

	void withdraw(double amount);

	void deposit(double amount);

	double getBalance();
};
