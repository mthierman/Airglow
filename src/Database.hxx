#pragma once

#include "Config.hxx"

class Database
{
  public:
    static std::unique_ptr<Database> Create(Config*);
    static Config* pConfig;

  protected:
    Database(Config*);
};
