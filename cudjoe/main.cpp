    #include <iostream>
    #include <sstream>
    #include <iomanip>
    
    #include <cstdlib>
    #include </usr/include/mysql/mysql.h>
    #include "/usr/include/mysql-cppconn-8/mysql/jdbc.h"
    #include "/usr/include/mysql-cppconn-8/jdbc/mysql_connection.h"
    #include "/usr/include/mysql-cppconn-8/jdbc/mysql_driver.h"
    #include "/usr/include/mysql-cppconn-8/jdbc/cppconn/exception.h"
    #include "/usr/include/mysql-cppconn-8/jdbc/cppconn/resultset.h"
    #include "/usr/include/mysql-cppconn-8/jdbc/cppconn/statement.h"
     
    #include "BankAccount.h"
    #include "BankTransaction.h"
    #include "BankAccount.cpp"
    #include "BankTransaction.cpp"
     
    using namespace std;
     
    enum Options { PRINT = 1, SAVE, NEW, NEWMANY, WITHDRAW, DEPOSIT,
                   CLOSE, CLOSEALL, END };
     
    int mainMenu()
    {
       cout << "\nMenu Options" << endl
            << "1 - Print All Account"
            << endl << "2 - Save Accounts to a Text File" << endl
            << "3 - Open New Account-One Account" << endl
            << "4 - Open New Account-Multiple Accounts" << endl
            << "5 - Withdraw" << endl << "6 - Deposit"
            << endl << "7 - Close One Account" << endl
            << "8 - Close All Accounts" << endl
            << "9 - End Transaction" << endl;
       int ch;
       cin >> ch;
       return ch;
    }
     
    int main(int argc, char** argv)
    {
       BankTransaction* bt =
          new BankTransaction("localhost", "root",
                              "your root password", "mybank");
     
       int choice;
       int acnu;
       string ffname, llname;
       double ball;
     
       while(1) {
          choice = mainMenu();
          if(choice == END)
             break;
          switch(choice) {
          case PRINT:
             bt->printAllAccounts();
             break;
          case SAVE:
             bt->saveAllAccounts();
             break;
          case NEW:
             cout << "\nEnter account no, first name,last name, balance: "
                  << endl << "? ";
             cin >> acnu;
             cin >> ffname;
             cin >> llname;
             cin >> ball;
             if(acnu < 1) {
                cout << "Invalid account number." << endl;
                break;
             }
             bt->createAccount(new BankAccount(acnu, ffname, llname,
                                               ball));
             break;
          case NEWMANY:
             bt->loadAllAccounts();
             break;
          case WITHDRAW:
             cout << "\nEnter account no, amount to withdraw "
                  << endl << "? ";
             cin >> acnu;
             cin >> ball;
             if(ball < 0) {
                cout << "Invalid amount." << endl;
                break;
             }
             bt->withdraw(acnu, ball);
             break;
          case DEPOSIT:
             cout << "\nEnter account no, amount to deposit "
                  << endl << "? ";
             cin >> acnu;
             cin >> ball;
             if(ball < 0) {
                cout << "Invalid amount." << endl;
                break;
             }
             bt->deposit(acnu, ball);
             break;
         case CLOSE:
             cout << "\nEnter account no to close account "
                  << endl << "? ";
             cin >> acnu;
             bt->closeAccount(acnu);
             break;
         case CLOSEALL:
             bt->closeAllAccounts();
             break;
          default:
             cerr << "Invalid choice!" << endl;
             break;
          }
       }
       return 0;
    }
