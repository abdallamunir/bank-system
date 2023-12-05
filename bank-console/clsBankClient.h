#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include <vector>
#include <fstream>
#include "clsDate.h"

using namespace std;


    class clsBankClient : public clsPerson
    {
    private:

        enum enMode { EmptyMode = 0, UpdateMode = 1 ,AddNewMode = 2};
        enMode _Mode;

        string _AccountNumber;
        string _PinCode;
        float _AccountBalance;
        bool _MarkedForDelete = false;

        static clsBankClient _ConvertLineToClientObject(string Line, string Separator = "#//#")
        {
            vector<string> vClientData;
            vClientData = clsString::Split(Line, Separator);

            return clsBankClient(enMode::UpdateMode, vClientData[0], vClientData[1], vClientData[2],
                vClientData[3], vClientData[4], vClientData[5], stod(vClientData[6]));

        }

        static string _ConvertClientObjectToLine(clsBankClient Client, string Separator = "#//#")
        {

            string stClientRecord = "";
            stClientRecord += Client.FirstName + Separator;
            stClientRecord += Client.LastName + Separator;
            stClientRecord += Client.Email + Separator;
            stClientRecord += Client.Phone + Separator;
            stClientRecord += Client.AccountNumber() + Separator;
            stClientRecord += Client.PinCode + Separator;
            stClientRecord += to_string(Client.AccountBalance);

            return stClientRecord;

        }

        static  vector <clsBankClient> _LoadClientsDataFromFile()
        {

            vector <clsBankClient> vClients;

            fstream MyFile;
            MyFile.open("Clients.txt", ios::in);//read Mode

            if (MyFile.is_open())
            {

                string Line;


                while (getline(MyFile, Line))
                {

                    clsBankClient Client = _ConvertLineToClientObject(Line);

                    vClients.push_back(Client);
                }

                MyFile.close();

            }

            return vClients;

        }

        static void _SaveClientsDataToFile(vector <clsBankClient> vClients)
        {

            fstream MyFile;
            MyFile.open("Clients.txt", ios::out);//overwrite

            string DataLine;

            if (MyFile.is_open())
            {

                for (clsBankClient C : vClients)
                {
                    DataLine = _ConvertClientObjectToLine(C);
                    MyFile << DataLine << endl;

                }

                MyFile.close();

            }

        }

        void _Update()
        {
            vector <clsBankClient> _vClients;
            _vClients = _LoadClientsDataFromFile();

            for (clsBankClient& C : _vClients)
            {
                if (C.AccountNumber() == AccountNumber())
                {
                    C = *this;
                    break;
                }

            }

            _SaveClientsDataToFile(_vClients);

        }
        void _AddNew()
        {
            _AddDataLineToFile(_ConvertClientObjectToLine(*this));
        }

        void _AddDataLineToFile(string  stDataLine)
        {
            fstream MyFile;
            MyFile.open("Clients.txt", ios::out | ios::app);

            if (MyFile.is_open())
            {

                MyFile <<  stDataLine << endl;

                MyFile.close();
            }

        }

        static clsBankClient _GetEmptyClientObject()
        {
            return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0);
        }

        struct stTransferLogRecord;

        static stTransferLogRecord _ConvertTransferLogLineToRecord(string Line, string Seperator = "#//#")
        {
            stTransferLogRecord TransferLogRecord;

            vector <string> vTrnsferLogRecordLine = clsString::Split(Line, Seperator);
            TransferLogRecord.DateTime = vTrnsferLogRecordLine[0];
            TransferLogRecord.SourceAccountNumber = vTrnsferLogRecordLine[1];
            TransferLogRecord.DestinationAccountNumber = vTrnsferLogRecordLine[2];
            TransferLogRecord.Amount = stod(vTrnsferLogRecordLine[3]);
            TransferLogRecord.srcBalanceAfter = stod(vTrnsferLogRecordLine[4]);
            TransferLogRecord.destBalanceAfter = stod(vTrnsferLogRecordLine[5]);
            TransferLogRecord.UserName = vTrnsferLogRecordLine[6];

            return TransferLogRecord;

        }


        string _PrepareTransferLogRecord(float Amount, clsBankClient DestinationClient,
            string UserName, string Seperator = "#//#")
        {
            string TransferLogRecord = "";
            TransferLogRecord += clsDate::GetSystemDateTimeString() + Seperator;
            TransferLogRecord += AccountNumber() + Seperator;
            TransferLogRecord += DestinationClient.AccountNumber() + Seperator;
            TransferLogRecord += to_string(Amount) + Seperator;
            TransferLogRecord += to_string(AccountBalance) + Seperator;
            TransferLogRecord += to_string(DestinationClient.AccountBalance) + Seperator;
            TransferLogRecord += UserName;
            return TransferLogRecord;
        }

        void _RegisterTransferLog(float Amount, clsBankClient DestinationClient, string UserName)
        {

            string stDataLine = _PrepareTransferLogRecord(Amount, DestinationClient, UserName);

            fstream MyFile;
            MyFile.open("TransferLog.txt", ios::out | ios::app);

            if (MyFile.is_open())
            {

                MyFile << stDataLine << endl;

                MyFile.close();
            }

        }

    public:

        struct stTransferLogRecord
        {
            string DateTime;
            string SourceAccountNumber;
            string DestinationAccountNumber;
            float Amount;
            float srcBalanceAfter;
            float destBalanceAfter;
            string UserName;

        };

        clsBankClient(enMode Mode, string FirstName, string LastName,
            string Email, string Phone, string AccountNumber, string PinCode,
            float AccountBalance) :
            clsPerson(FirstName, LastName, Email, Phone)

        {
            _Mode = Mode;
            _AccountNumber = AccountNumber;
            _PinCode = PinCode;
            _AccountBalance = AccountBalance;

        }

        bool IsEmpty()
        {
            return (_Mode == enMode::EmptyMode);
        }


        void SetAccountNumber(string AccountNumber) // remove after implementing account number autogenerate
        {
            _AccountNumber = AccountNumber;
        }
        string AccountNumber()
        {
            return _AccountNumber;
        }

        void SetPinCode(string PinCode)
        {
            _PinCode = PinCode;
        }

        string GetPinCode()
        {
            return _PinCode;
        }
        __declspec(property(get = GetPinCode, put = SetPinCode)) string PinCode;

        void SetAccountBalance(float AccountBalance)
        {
            _AccountBalance = AccountBalance;
        }

        float GetAccountBalance()
        {
            return _AccountBalance;
        }
        __declspec(property(get = GetAccountBalance, put = SetAccountBalance)) float AccountBalance;

        void Print()
        {
            cout << "\nClient Card:";
            cout << "\n___________________";
            cout << "\nFirstName   : " << FirstName;
            cout << "\nLastName    : " << LastName;
            cout << "\nFull Name   : " << FullName();
            cout << "\nEmail       : " << Email;
            cout << "\nPhone       : " << Phone;
            cout << "\nAcc. Number : " << _AccountNumber;
            cout << "\nPassword    : " << _PinCode;
            cout << "\nBalance     : " << _AccountBalance;
            cout << "\n___________________\n";

        }

        static clsBankClient Find(string AccountNumber)
        {


            fstream MyFile;
            MyFile.open("Clients.txt", ios::in);//read Mode

            if (MyFile.is_open())
            {
                string Line;
                while (getline(MyFile, Line))
                {
                    clsBankClient Client = _ConvertLineToClientObject(Line);
                    if (Client.AccountNumber() == AccountNumber)
                    {
                        MyFile.close();
                        return Client;
                    }

                }

                MyFile.close();

            }

            return _GetEmptyClientObject();
        }

        static clsBankClient Find(string AccountNumber, string PinCode)
        {



            fstream MyFile;
            MyFile.open("Clients.txt", ios::in);//read Mode

            if (MyFile.is_open())
            {
                string Line;
                while (getline(MyFile, Line))
                {
                    clsBankClient Client = _ConvertLineToClientObject(Line);
                    if (Client.AccountNumber() == AccountNumber && Client.PinCode == PinCode)
                    {
                        MyFile.close();
                        return Client;
                    }

                }

                MyFile.close();

            }
            return _GetEmptyClientObject();
        }

        static bool IsClientExist(string AccountNumber)
        {

            clsBankClient Client1 = clsBankClient::Find(AccountNumber);
            return (!Client1.IsEmpty());
        }

        enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1,svFaildAccountNumberExists = 2 };

        enSaveResults Save()
        {

            switch (_Mode)
            {
            case enMode::EmptyMode:
            {

                return enSaveResults::svFaildEmptyObject;
            }

            case enMode::UpdateMode:
            {


                _Update();

                return enSaveResults::svSucceeded;

                break;
            }
            case enMode::AddNewMode:
            {
                //This will add new record to file or database
                if (clsBankClient::IsClientExist(_AccountNumber))
                {
                    return enSaveResults::svFaildAccountNumberExists;
                }
                else
                {
                    _AddNew();

                    //We need to set the mode to update after add new
                    _Mode = enMode::UpdateMode;
                    return enSaveResults::svSucceeded;
                }

                break;
            }
            }



        }

        bool Delete()
        {
            vector <clsBankClient> _vClients;
            _vClients = _LoadClientsDataFromFile();
            for (clsBankClient& C : _vClients)
            {
                if (C.AccountNumber() == _AccountNumber) {
                    C._MarkedForDelete = true;
                    return true;
                }
            }
            _SaveClientsDataToFile(_vClients);
            *this = _GetEmptyClientObject();
            return false;
        }
        static clsBankClient GetAddNewClientObject(string AccountNumber)
        {
            return clsBankClient(enMode::AddNewMode, "", "", "", "", "", "", 0);
        }
       
        static vector <clsBankClient> GetClientsList()
        {
            return _LoadClientsDataFromFile();
        }

        static double GetTotalBalances()
        {
            vector <clsBankClient> vClients = clsBankClient::GetClientsList();

            double TotalBalances = 0;

            for (clsBankClient Client : vClients)
            {

                TotalBalances += Client.AccountBalance;
            }

            return TotalBalances;

        }

        void Deposit(double Amount)
        {
            _AccountBalance += Amount;
            Save();
        }

        bool Withdraw(double Amount)
        {
            if (Amount > _AccountBalance)
            {
                return false;
            }
            else
            {
                _AccountBalance -= Amount;
                Save();
            }
            
        }

        bool Transfer(float Amount, clsBankClient& DestinationClient,string UserName)
        {
            if (Amount > AccountBalance)
            {
                return false;
            }

            Withdraw(Amount);
            DestinationClient.Deposit(Amount);
            _RegisterTransferLog(Amount, DestinationClient, UserName);
            return true;
        }

        static  vector <stTransferLogRecord> GetTransfersLogList()
        {
            vector <stTransferLogRecord> vTransferLogRecord;

            fstream MyFile;
            MyFile.open("TransfersLog.txt", ios::in);//read Mode

            if (MyFile.is_open())
            {

                string Line;

                stTransferLogRecord TransferRecord;

                while (getline(MyFile, Line))
                {

                    TransferRecord = _ConvertTransferLogLineToRecord(Line);

                    vTransferLogRecord.push_back(TransferRecord);

                }

                MyFile.close();

            }

            return vTransferLogRecord;

        }
    };


