#pragma once

#include "Config.hxx"

class Database
{
  public:
    static std::unique_ptr<Database> Create(Config*);

  protected:
    Database(Config*);
};
