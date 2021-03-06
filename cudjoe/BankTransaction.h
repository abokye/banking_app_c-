    #ifndef BANKTRANSACTION_H
    #define BANKTRANSACTION_H
     
    #include </usr/include/mysql/mysql.h>
    #include <string>
     
    class BankAccount;
     
    using namespace std;
    class BankTransaction
    {
    public:
       BankTransaction(const string = "localhost",
          const string = "",
       const string = "", const string = "");
       ~BankTransaction();
       void createAccount(BankAccount*);
       void closeAccount(int);
       void deposit(int, double);
       void withdraw(int, double);
       BankAccount* getAccount(int);
       void printAllAccounts();
       void saveAllAccounts();
       void loadAllAccounts();
       void closeAllAccounts();
       void message(string);
     
    private:
       MYSQL* db_conn;
    };
     
    #endif   // BANKTRANSACTION_H
