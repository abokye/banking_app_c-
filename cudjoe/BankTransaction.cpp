    #include <cstdlib>
    #include <sstream>
    #include <iostream>
    #include <iomanip>
    #include <fstream>  
    #include "BankTransaction.h"
    #include "BankAccount.h"
     
    BankTransaction::BankTransaction(const string HOST,
       const string USER, const string PASSWORD,
       const string DATABASE)
    {
       db_conn = mysql_init(NULL);
       if(!db_conn)
          message("MySQL initialization failed! ");
       db_conn = mysql_real_connect(db_conn, HOST.c_str(),
          USER.c_str(), PASSWORD.c_str(), DATABASE.c_str(), 0,
             NULL, 0);
       if(!db_conn)
          message("Connection Error! ");
    }
     
    BankTransaction::~BankTransaction()
    {
       mysql_close(db_conn);
    }
     
    BankAccount* BankTransaction::getAccount(int acno)
    {
       BankAccount* b = NULL;
       MYSQL_RES* rset;
       MYSQL_ROW row;
       stringstream sql;
       sql << "SELECT * FROM Accounts WHERE AccountNum="
          << acno;
     
       if(!mysql_query(db_conn, sql.str().c_str())) {
          b = new BankAccount();
          rset = mysql_use_result(db_conn);
          row = mysql_fetch_row(rset);
          b->setAccountNumber(atoi(row[0]));
          b->setFirstName(row[1]);
          b->setLastName(row[2]);
          b->setBalance(atof(row[3]));
       }
       mysql_free_result(rset);
       return b;
    }
     
    void BankTransaction::withdraw(int acno, double amount)
    {
       BankAccount* b = getAccount(acno);
       if(b != NULL) {
          if(b->getBalance() < amount)
             message("Cannot withdraw. Try lower amount.");
          else {
             b->setBalance(b->getBalance() - amount);
             stringstream sql;
             sql << "UPDATE Accounts SET Balance="
                << b->getBalance()
                << " WHERE AccountNum=" << acno;
             if(!mysql_query(db_conn, sql.str().c_str())) {
                message("Cash withdraw successful.Balance updated.");
             } else {
                message("Cash deposit unsuccessful! Update failed");
             }
          }
       }
    }
     
    void BankTransaction::deposit(int acno, double amount)
    {
       stringstream sql;
       sql << "UPDATE Accounts SET Balance=Balance+" << amount
          << " WHERE AccountNum=" << acno;
       if(!mysql_query(db_conn, sql.str().c_str())) {
          message("Cash deposit successful. Balance updated.");
       } else {
          message("Cash deposit unsuccessful! Update failed");
       }
    }
     
    void BankTransaction::createAccount(BankAccount* ba)
    {
       stringstream ss;
       ss << "INSERT INTO Accounts(AccountNum, FirstName, LastName, Balance)"
          << "values (" << ba->getAccountNumber() << ", '"
             << ba->getFirstName() + "','"
             << ba->getLastName() << "',"
             << ba->getBalance() << ")";
       if(mysql_query(db_conn, ss.str().c_str()))
          message("Failed to create account! ");
       else
          message("Account creation successful.");
    }
     
    void BankTransaction::closeAccount(int acno)
    {
       stringstream ss;
       ss << "DELETE FROM Accounts WHERE AccountNum="
          << acno;
       if(mysql_query(db_conn, ss.str().c_str()))
          message("Failed to close account! ");
       else
          message("Account close successful.");
    }
    
     void BankTransaction::closeAllAccounts()
    {
       stringstream ss;
       ss << "DELETE FROM Accounts";
       if(mysql_query(db_conn, ss.str().c_str()))
          message("Failed to close all accounts! ");
       else
          message("All Accounts closed successful.");
    }
    
    void BankTransaction::message(string msg)
    {
       cout << msg << endl;
    }
    void BankTransaction::printAllAccounts()
    {
       MYSQL_RES* rset;
       MYSQL_ROW rows;
       string sql = "SELECT * FROM Accounts";
       if(mysql_query(db_conn, sql.c_str())) {
          message("Error printing all accounts! ");
          return;
       }
     
       rset = mysql_use_result(db_conn);
     
       cout << left << setw(15) << setfill('-') << left << '+'
            << setw(25) << setfill('-') << left << '+'
            << setw(25)
            << setfill('-') << left << '+' << setw(25)
            << setfill('-')
            << '+' << '+' << endl;
       cout << setfill(' ') << '|' << left << setw(14)
            << "Account Number"
            << setfill(' ') << '|' << setw(24) << "First Name"
            << setfill(' ') << '|' << setw(24) << "Last Name"
            << setfill(' ') << '|' << right << setw(24)
            << "Balance" << '|' << endl;
     
       cout << left << setw(15) << setfill('-') << left
           << '+' << setw(25) << setfill('-') << left << '+'
           << setw(25)
           << setfill('-') << left << '+' << setw(25) << setfill('-')
           << '+' << '+' << endl;
       if(rset) {
          while((rows = mysql_fetch_row(rset))) {
             cout << setfill(' ') << '|' << left << setw(14) << rows[0]
                  << setfill(' ') << '|' << setw(24) << rows[1]
                  << setfill(' ') << '|' << setw(24) << rows[2]
                  << setfill(' ') << '|' << right << setw(24)
                  << rows[3] << '|' << endl;
          }
          cout << left << setw(15) << setfill('-') << left
               << '+' << setw(25) << setfill('-') << left << '+'
               << setw(25)
               << setfill('-') << left << '+' << setw(25)
               << setfill('-')
               << '+' << '+' << endl;
       }
       mysql_free_result(rset);
    }
     void BankTransaction::saveAllAccounts()
    {
       MYSQL_RES* rset;
       MYSQL_ROW rows;
       //ofstream myfile;//create an output file called myfile
	//myfile.open ("pri.txt");//opens a file pri.dat using the output file myfile
	//string sql="SELECT @@global.secure_file_priv";
	//system("mysql --local-infile=1 -u root --database=mybank --password=Innocenti1@ -e \"SELECT * FROM Accounts INTO OUTFILE '/var/lib/mysql-files/pri.txt'\""); 
	string sql= "SELECT * FROM Accounts INTO OUTFILE '/var/lib/mysql-files/pri.txt'";
	system("sudo mv /var/lib/mysql-files/pri.txt .;sudo chmod 777 pri.txt;");
	message("Success saving all accounts to a text file! ");
       if(mysql_query(db_conn, sql.c_str())) {
          message("Error saving all accounts to a text file! ");
          return;
       }
     
       rset = mysql_use_result(db_conn);
     
       /*cout << left << setw(15) << setfill('-') << left << '+'
            << setw(25) << setfill('-') << left << '+'
            << setw(25)
            << setfill('-') << left << '+' << setw(25)
            << setfill('-')
            << '+' << '+' << endl;
       cout << setfill(' ') << '|' << left << setw(14)
            << "Account Number"
            << setfill(' ') << '|' << setw(24) << "First Name"
            << setfill(' ') << '|' << setw(24) << "Last Name"
            << setfill(' ') << '|' << right << setw(24)
            << "Balance" << '|' << endl;
     
       cout << left << setw(15) << setfill('-') << left
           << '+' << setw(25) << setfill('-') << left << '+'
           << setw(25)
           << setfill('-') << left << '+' << setw(25) << setfill('-')
           << '+' << '+' << endl;
       if(rset) {
          while((rows = mysql_fetch_row(rset))) {
             cout << setfill(' ') << '|' << left << setw(14) << rows[0]
                  << setfill(' ') << '|' << setw(24) << rows[1]
                  << setfill(' ') << '|' << setw(24) << rows[2]
                  << setfill(' ') << '|' << right << setw(24)
                  << rows[3] << '|' << endl;
          }
          cout << left << setw(15) << setfill('-') << left
               << '+' << setw(25) << setfill('-') << left << '+'
               << setw(25)
               << setfill('-') << left << '+' << setw(25)
               << setfill('-')
               << '+' << '+' << endl;
       }*/
       mysql_free_result(rset);
       //myfile.close();//closes a file pri.dat using the output file myfile
    }
    void BankTransaction::loadAllAccounts()
    {
       MYSQL_RES* rset;
       MYSQL_ROW rows;
	string sql= "SET GLOBAL local_infile=1";
	system("mysql --local-infile=1 -u root --database=mybank --password=Innocenti1@ -e \"LOAD DATA LOCAL INFILE 'bank_data' INTO TABLE Accounts FIELDS TERMINATED BY '\t' LINES TERMINATED BY '\n' (AccountNum, FirstName, LastName, Balance);\""); 
		cout << "#\t Accounts table updated" << endl;
       if(mysql_query(db_conn, sql.c_str())) {
          message("Error creating multiple accounts! ");
          return;
       }
     
       rset = mysql_use_result(db_conn);
     
       /*cout << left << setw(15) << setfill('-') << left << '+'
            << setw(25) << setfill('-') << left << '+'
            << setw(25)
            << setfill('-') << left << '+' << setw(25)
            << setfill('-')
            << '+' << '+' << endl;
       cout << setfill(' ') << '|' << left << setw(14)
            << "Account Number"
            << setfill(' ') << '|' << setw(24) << "First Name"
            << setfill(' ') << '|' << setw(24) << "Last Name"
            << setfill(' ') << '|' << right << setw(24)
            << "Balance" << '|' << endl;
     
       cout << left << setw(15) << setfill('-') << left
           << '+' << setw(25) << setfill('-') << left << '+'
           << setw(25)
           << setfill('-') << left << '+' << setw(25) << setfill('-')
           << '+' << '+' << endl;
       if(rset) {
          while((rows = mysql_fetch_row(rset))) {
             cout << setfill(' ') << '|' << left << setw(14) << rows[0]
                  << setfill(' ') << '|' << setw(24) << rows[1]
                  << setfill(' ') << '|' << setw(24) << rows[2]
                  << setfill(' ') << '|' << right << setw(24)
                  << rows[3] << '|' << endl;
          }
          cout << left << setw(15) << setfill('-') << left
               << '+' << setw(25) << setfill('-') << left << '+'
               << setw(25)
               << setfill('-') << left << '+' << setw(25)
               << setfill('-')
               << '+' << '+' << endl;
       }*/
       mysql_free_result(rset);
    }
