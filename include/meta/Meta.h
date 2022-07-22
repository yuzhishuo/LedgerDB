#include <ledger_engine.pb.h>

template <typename T, typename U> class Meta
{
public:
  void serialize() const
  {
    ledger_engine::Ledger ledger;
    auto serialized = ledger.SerializeAsString();
    // auto deserialized = ledger.ParseFromString(serialized);

    U::instance().store(ledger.name(), serialized);
  }

  void deserialize() const
  {
    ledger_engine::Ledger ledger;
    auto serialized = ledger.SerializeAsString();
    auto deserialized = ledger.ParseFromString(serialized);
  }

  T &operator*() { return value; }

private:
  T value;
};