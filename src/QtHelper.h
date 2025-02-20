#pragma once

#define IS_CONENCTED_OK                                                        \
  Q_ASSERT_X(                                                                  \
      connected,                                                               \
      std::string(std::string(__FILE__) + " line:" + std::to_string(__LINE__)) \
          .c_str(),                                                            \
      "connected is FALSE");
