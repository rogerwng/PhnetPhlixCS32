#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <list>

class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
    bool loaded;
    TreeMultimap<std::string, User*> userData;
    std::list<User*> userStorage;  // used to delete dynamically allocated users bc treemm only responsible for deleting nodes and not objs
};

#endif // USERDATABASE_INCLUDED
