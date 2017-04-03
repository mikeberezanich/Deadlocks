// Code derived from http://stackoverflow.com/questions/1385843/simple-deadlock-examples, http://stackoverflow.com/questions/13326861/avoid-deadlock-example/13326948, and Ch7 slides

#include "Bank.h"

using namespace std;

Account::Account(double amount) {
	balance = amount;
	isLocked = false;
}

void Account::withdraw(double amount) {
	balance -= amount;
}

void Account::deposit(double amount) {
	balance += amount;
}

double Account::getBalance() {
	return balance;
}