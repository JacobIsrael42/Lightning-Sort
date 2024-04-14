//*************************************************
//               BankAccount.cpp
// A class used to test the LightningSorter class.
// Created by Jacob Israel January-19-2014
// Last modified: January-20-2014
// 
//************************************************* 
#include <iostream> 
#include <stdio.h> 
#include <stdlib.h> 
#ifdef _WIN64 
#include <Windows.h>
#endif
#ifdef _WIN32
#include <Windows.h> 
#endif

using namespace std;

class BankAccount{
	
private:
	int accountNumber;
	double balance;
	string firstName;
	string lastName; 
	
public:
	//=============================
	//   Copy Constructor
	//=============================
	BankAccount(BankAccount* ba){
		accountNumber = ba->accountNumber;
		balance = ba->balance;
		firstName = ba->firstName;
		lastName = ba->lastName; 	
	}
	
	//=============================
	//   Constructor
	//=============================
	BankAccount(int acctNum, double startBalance, string first, string last){
		accountNumber = acctNum;
		balance = startBalance;
		firstName = first;
		lastName = last; 	
	}
	
	//=============================
	//   getFirstName
	//=============================
	string getFirstName(){
	   return firstName;
	}
	
	//=============================
	//   getFirstName
	// static function with object reference
	//=============================
	static string getFirstName(BankAccount* ba){
	   return ba->getFirstName();
	}
	
	//=============================
	//   getLastName
	//=============================
	string getLastName(){
	   return lastName;
	}
	
	//=============================
	//   getLastName
	// static function with object reference
	//=============================
	static string getLastName(BankAccount ba){
	   return ba.getLastName();
	}
	
	//=============================
	//   getLastName
	// static function with object pointer reference
	//=============================
	static string getLastName(BankAccount* ba){
	   return ba->getLastName();
	}
	
	//=============================
	//   getBalance
	//=============================
	double getBalance(){
	   return balance;
	}
	
	//=============================
	//   getBalance
	// static function with object reference
	//=============================
	static double getBalance(BankAccount ba){
	   return ba.getBalance();
	}
	
	//=============================
	//   getBalance
	// static function with object pointer reference
	//=============================
	static double getBalance(BankAccount* ba){
	   return ba->getBalance();
	}
	
	//=============================
	//   getAccountNumber
	//=============================
	int getAccountNumber(){
	   return accountNumber;
	}
	
	//=============================
	//   getAccountNumber
   // static function with object reference
	//=============================
	static int getAccountNumber(BankAccount ba){
	   return ba.getAccountNumber();
	}
	
	//=============================
	//   getAccountNumber
   // static function with object pointer reference
	//=============================
	static int getAccountNumber(BankAccount* ba){
	   return ba->getAccountNumber();
	}
	
	//=============================
	//          deposit
	//=============================
	bool deposit(double amount){
		if(amount<=0){
			return false;
		}
		balance += amount;
	   return true;
	}
	
	//=============================
	//       withdraw
	//=============================
	bool withdraw(double amount){
		if(amount > balance || amount <=0 ){
			return false;
		}
		balance -= amount;
	   return true;
	}
	
	//=============================
	//   toString
	//=============================
	string toString(){
		char charArray1[16]; 
		sprintf(charArray1,"%d", getAccountNumber());
		char charArray2[16];
		sprintf(charArray2,"%1.2f", getBalance());	
		
   return getFirstName() + " " + getLastName() + "\t#" + charArray1 + "\t$" + charArray2; 
	}
	
};//end class