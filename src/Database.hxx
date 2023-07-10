#pragma once

#include "Config.hxx"

class Database
{
  public:
    static std::unique_ptr<Database> Create(Config*);

  private:
    static Config* pConfig;

  protected:
    Database(Config*);
};
