#pragma once

#include "Utility.hxx"

using namespace Utility;

class JSON
{
  public:
    static std::unique_ptr<JSON> Create();
    void Save();
    void Load();

  private:
    JSON();
};
