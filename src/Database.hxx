#pragma once

#include "Config.hxx"

class Database
{
  public:
    static std::unique_ptr<Database> Create(Config*);

  private:
    Config* pConfig{nullptr};

  protected:
    Database(Config*);
};
