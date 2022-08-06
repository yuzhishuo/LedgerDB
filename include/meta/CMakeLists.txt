

add_library(meta Ledger.cpp
                 Ledgers.cpp
                 User.cpp
                 Users.cpp
                 UsersImpl.cpp
                 LedgersImpl.cpp
                 )
target_link_libraries(meta
                 rocksdb
                 merkle
                 fmt::fmt)