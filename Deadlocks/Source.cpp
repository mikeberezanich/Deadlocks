// Mike Berezanich
// CMPSC 473
// Project 2

#include <iostream>
#include <thread>
#include <chrono>
#include "Bank.h"

using namespace std;

void deadlockTransfer(Account *from, Account *to, double amount);
void mutualExclusionTransfer(Account *from, Account *to, double amount);
void holdAndWaitTransfer(Account *from, Account *to, double amount);
void noPreemptionTransfer(Account *from, Account *to, double amount);
void circularWaitTransfer(Account *from, Account *to, double amount);

int main() {

	Account *a = new Account(500);
	Account *b = new Account(1000);

	cout << "Account a balance " << a->getBalance() << endl;
	cout << "Account b balance " << b->getBalance() << endl;

	thread t1(holdAndWaitTransfer, a, b, 100);
	thread t2(holdAndWaitTransfer, b, a, 200);

	t1.join();
	t2.join();

	cout << "Account a balance " << a->getBalance() << endl;
	cout << "Account b balance " << b->getBalance() << endl;

	delete a;
	delete b;

	return 0;

}

// This is the function that can deadlock when attempting to run it with multiple threads
void deadlockTransfer(Account *from, Account *to, double amount) {

	from->mLock.lock();
	this_thread::sleep_for(1ms); //if I don't put this in, t1 finishes the entire transfer before the deadlock occurs
	to->mLock.lock();

	from->withdraw(amount);
	to->deposit(amount);

	from->mLock.unlock();
	to->mLock.unlock();

}

// This function does not make use of any mutual exclusion locks (mutexes), therefore it cannot deadlock
void mutualExclusionTransfer(Account *from, Account *to, double amount) {

	from->withdraw(amount);
	to->deposit(amount);

}

// This function prevents deadlocks because it only locks the account that it's updating the balance of
// and then unlocks that account before updating the next
void holdAndWaitTransfer(Account *from, Account *to, double amount) {
	
	from->mLock.lock();
	from->withdraw(amount);
	from->mLock.unlock();
	
	to->mLock.lock();
	to->deposit(amount);
	to->mLock.unlock();

}

// This function locks the from account initially, but then if the to account is already locked by another
// thread, it unlocks the from account and loops. If the to account isn't already locked by another thread,
// then it locks the to account and carries out the transaction
void noPreemptionTransfer(Account *from, Account *to, double amount) {
	
	bool finished = false;

	while (!finished) {
		from->mLock.lock();
		from->isLocked = true;
		if (to->isLocked) {
			from->mLock.unlock();
			from->isLocked = false;
		}
		else {
			to->mLock.lock();
			to->isLocked = true;

			from->withdraw(amount);
			to->deposit(amount);

			from->mLock.unlock();
			from->isLocked = false;
			to->mLock.unlock();
			to->isLocked = false;

			finished = true;
		}
	}

}

// This function prevents deadlock by reordering the from and to accounts so that all threads that
// request a transfer will attempt to lock the accounts in the same order
void circularWaitTransfer(Account *from, Account *to, double amount) {

	if (from < to) {
		from->mLock.lock();
		to->mLock.lock();
	}
	else {
		to->mLock.lock();
		from->mLock.lock();
	}

	from->withdraw(amount);
	to->deposit(amount);

	from->mLock.unlock();
	to->mLock.unlock();

}