

add_library(meta Ledger.cpp
                 LedgerFactory.cpp
                 User.cpp
                 UserFactory.cpp
                 UserFactoryImpl.cpp
                 LedgerFactoryImpl.cpp
                 )
target_link_libraries(meta
                 rocksdb
                 merkle
                 ledger_engine
                 fmt::fmt)