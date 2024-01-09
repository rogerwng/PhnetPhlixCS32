#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

UserDatabase::UserDatabase()
{
    loaded = false;
}

UserDatabase::~UserDatabase()
{
    // TreeMM destructor will delete its nodes, UserDataBase just needs to delete the User's it created, whose pointers are stored in a list
    for (list<User*>::iterator p = userStorage.begin(); p != userStorage.end(); p = p)
    {
        delete *p++;
    }
}

bool UserDatabase::load(const string& filename)
{
    // check if users alr loaded
    if (loaded)
        return false;

    ifstream file(filename);
    
    while (!file.eof())
    {
        string name, email, num;
        stringstream ss;
        int numHist;
        vector<string> history;
        
        getline(file, name);
        getline(file, email);
        getline(file, num);
        ss << num;
        ss >> numHist;
        for (int i = 0; i < numHist; i++)
        {
            string temp;
            getline(file,temp);
            history.push_back(temp);
        }
        
        User* temp = new User(name, email, history);
        userStorage.push_back(temp);
        userData.insert(email, temp);
        
        //
        // TESTING ONLY
        //cout << name << " - " << email << endl;
        // TESTING ONLY
        //
        
        getline(file, name);
    }
    loaded = true;
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<string, User*>::Iterator it = userData.find(email);
    
    if (it.is_valid())
        return it.get_value();
    return nullptr;
}
