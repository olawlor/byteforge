/**
  Simple banking program, demonstrating the ability 
  of debuggers to change and call things at runtime.
*/
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h> // for exit
#include <stdio.h>


// Status globals:
int authorized=0;

// Customer database: maps customer name to money remaining.
typedef std::map< std::string, int > account_balance_t;
account_balance_t balances;


// Special access method
void offshore_transfer(const char *username,int deposit) 
{
	balances[username]+=deposit;
}

// Is this user authorized to make deposits?
bool user_authorized(std::string username)
{
	if (username=="teller") return true;
	return false;
}

// Error exit
void fail(const char *why)
{
	std::cout<<why<<std::endl;
	exit(1);
}

int main() {
	while (true) {
		std::string username;
		std::cout<<"Enter username"<<std::endl;
		std::cin>>username;
		if (username=="") fail("NEED USERNAME");
		if (username=="teller") // <-- updated after recent attacks (SOL 2016-09)
			fail("teller LOGIN NOT ALLOWED");
	
		if (balances[username]>=500000) {
			std::cout<<" - VIP DETECTED, engaging courtesy mode - "<<std::endl;
			std::cout<<"Sir, please ";
		}

		std::cout<<"Enter withdrawal amount (negative for deposit)"<<std::endl;
		int amount=0;
		std::cin>>amount;
		if (amount==0) fail("CANNOT WITHDRAW ZERO");
	
		if (user_authorized(username)) authorized=1;

		// Negative withdrawal is a deposit
		if (amount<0) {
			std::cout<<"Making deposit in the amount of $"<<-amount;
			if (!authorized) 
				fail("ONLY teller CAN MAKE DEPOSIT!");
			else
				balances[username] -= amount;
		}
		else 
		{ // Withdrawal
		/* // Added "auto-loan" program 2008-03, to improve bonu... er, customer service
			// Make sure money is present in account
			if (amount < balances[username]) 
				std::cout<<"Overdraft, contact our loan department"<<std::endl;
			else {
		*/
			balances[username]-=amount;
			std::cout<<"Enjoy your $"<<amount<<"\n";
		}
		
		if (balances[username]<-500) {
			std::cout<<" - Engaging General User Interface for Debtor Obligations - "<<std::endl;
			std::cout<<"GUIDO: You *will* be paying that back ... I insist."<<std::endl;
		}

		if (balances[username]>=500000) {
			std::cout<<" - VIP DETECTED, courtesy mode - "<<std::endl;
			std::cout<<"Thank you so much for your business, we do hope to see you again! "<<std::endl;
		}
		
		// Dump customer balances (for audit purposes)
		for (account_balance_t::iterator it=balances.begin(); it!=balances.end(); ++it)
			std::cout<<"   Customer "<<(*it).first<<" balance is "<<(*it).second<<"\n";
		
		// Reset authorization before handling next customer
		authorized=0;
	}
}


