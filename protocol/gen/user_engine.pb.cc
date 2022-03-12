// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: user_engine.proto

#include "user_engine.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace user_engine {
constexpr User::User(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : name_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , password_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , public_key_(&::PROTOBUF_NAMESPACE_ID::internal::fixed_address_empty_string)
  , id_(int64_t{0})
  , role_(0)
{}
struct UserDefaultTypeInternal {
  constexpr UserDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~UserDefaultTypeInternal() {}
  union {
    User _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT UserDefaultTypeInternal _User_default_instance_;
}  // namespace user_engine
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_user_5fengine_2eproto[1];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_user_5fengine_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_user_5fengine_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_user_5fengine_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::user_engine::User, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::user_engine::User, id_),
  PROTOBUF_FIELD_OFFSET(::user_engine::User, name_),
  PROTOBUF_FIELD_OFFSET(::user_engine::User, password_),
  PROTOBUF_FIELD_OFFSET(::user_engine::User, public_key_),
  PROTOBUF_FIELD_OFFSET(::user_engine::User, role_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::user_engine::User)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::user_engine::_User_default_instance_),
};

const char descriptor_table_protodef_user_5fengine_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\021user_engine.proto\022\013user_engine\"g\n\004User"
  "\022\n\n\002id\030\001 \001(\003\022\014\n\004name\030\002 \001(\t\022\020\n\010password\030\003"
  " \001(\t\022\022\n\npublic_key\030\004 \001(\t\022\037\n\004role\030\005 \001(\0162\021"
  ".user_engine.Role*8\n\004Role\022\r\n\tREGULATOR\020\000"
  "\022\n\n\006COMMON\020\001\022\014\n\010READONLY\020\002\022\007\n\003DBA\020\003b\006pro"
  "to3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_user_5fengine_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_user_5fengine_2eproto = {
  false, false, 203, descriptor_table_protodef_user_5fengine_2eproto, "user_engine.proto", 
  &descriptor_table_user_5fengine_2eproto_once, nullptr, 0, 1,
  schemas, file_default_instances, TableStruct_user_5fengine_2eproto::offsets,
  file_level_metadata_user_5fengine_2eproto, file_level_enum_descriptors_user_5fengine_2eproto, file_level_service_descriptors_user_5fengine_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_user_5fengine_2eproto_getter() {
  return &descriptor_table_user_5fengine_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_user_5fengine_2eproto(&descriptor_table_user_5fengine_2eproto);
namespace user_engine {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Role_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_user_5fengine_2eproto);
  return file_level_enum_descriptors_user_5fengine_2eproto[0];
}
bool Role_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class User::_Internal {
 public:
};

User::User(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor();
  if (!is_message_owned) {
    RegisterArenaDtor(arena);
  }
  // @@protoc_insertion_point(arena_constructor:user_engine.User)
}
User::User(const User& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_name().empty()) {
    name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_name(), 
      GetArenaForAllocation());
  }
  password_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_password().empty()) {
    password_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_password(), 
      GetArenaForAllocation());
  }
  public_key_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_public_key().empty()) {
    public_key_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_public_key(), 
      GetArenaForAllocation());
  }
  ::memcpy(&id_, &from.id_,
    static_cast<size_t>(reinterpret_cast<char*>(&role_) -
    reinterpret_cast<char*>(&id_)) + sizeof(role_));
  // @@protoc_insertion_point(copy_constructor:user_engine.User)
}

void User::SharedCtor() {
name_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
password_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
public_key_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
    reinterpret_cast<char*>(&id_) - reinterpret_cast<char*>(this)),
    0, static_cast<size_t>(reinterpret_cast<char*>(&role_) -
    reinterpret_cast<char*>(&id_)) + sizeof(role_));
}

User::~User() {
  // @@protoc_insertion_point(destructor:user_engine.User)
  if (GetArenaForAllocation() != nullptr) return;
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

inline void User::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  name_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  password_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  public_key_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void User::ArenaDtor(void* object) {
  User* _this = reinterpret_cast< User* >(object);
  (void)_this;
}
void User::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void User::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void User::Clear() {
// @@protoc_insertion_point(message_clear_start:user_engine.User)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  name_.ClearToEmpty();
  password_.ClearToEmpty();
  public_key_.ClearToEmpty();
  ::memset(&id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&role_) -
      reinterpret_cast<char*>(&id_)) + sizeof(role_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* User::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int64 id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string name = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          auto str = _internal_mutable_name();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "user_engine.User.name"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string password = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          auto str = _internal_mutable_password();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "user_engine.User.password"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string public_key = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          auto str = _internal_mutable_public_key();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "user_engine.User.public_key"));
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .user_engine.Role role = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 40)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          _internal_set_role(static_cast<::user_engine::Role>(val));
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* User::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:user_engine.User)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 id = 1;
  if (this->_internal_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(1, this->_internal_id(), target);
  }

  // string name = 2;
  if (!this->_internal_name().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_name().data(), static_cast<int>(this->_internal_name().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "user_engine.User.name");
    target = stream->WriteStringMaybeAliased(
        2, this->_internal_name(), target);
  }

  // string password = 3;
  if (!this->_internal_password().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_password().data(), static_cast<int>(this->_internal_password().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "user_engine.User.password");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_password(), target);
  }

  // string public_key = 4;
  if (!this->_internal_public_key().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_public_key().data(), static_cast<int>(this->_internal_public_key().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "user_engine.User.public_key");
    target = stream->WriteStringMaybeAliased(
        4, this->_internal_public_key(), target);
  }

  // .user_engine.Role role = 5;
  if (this->_internal_role() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      5, this->_internal_role(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:user_engine.User)
  return target;
}

size_t User::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:user_engine.User)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string name = 2;
  if (!this->_internal_name().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_name());
  }

  // string password = 3;
  if (!this->_internal_password().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_password());
  }

  // string public_key = 4;
  if (!this->_internal_public_key().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_public_key());
  }

  // int64 id = 1;
  if (this->_internal_id() != 0) {
    total_size += ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64SizePlusOne(this->_internal_id());
  }

  // .user_engine.Role role = 5;
  if (this->_internal_role() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_role());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData User::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSizeCheck,
    User::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*User::GetClassData() const { return &_class_data_; }

void User::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to,
                      const ::PROTOBUF_NAMESPACE_ID::Message& from) {
  static_cast<User *>(to)->MergeFrom(
      static_cast<const User &>(from));
}


void User::MergeFrom(const User& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:user_engine.User)
  GOOGLE_DCHECK_NE(&from, this);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_name().empty()) {
    _internal_set_name(from._internal_name());
  }
  if (!from._internal_password().empty()) {
    _internal_set_password(from._internal_password());
  }
  if (!from._internal_public_key().empty()) {
    _internal_set_public_key(from._internal_public_key());
  }
  if (from._internal_id() != 0) {
    _internal_set_id(from._internal_id());
  }
  if (from._internal_role() != 0) {
    _internal_set_role(from._internal_role());
  }
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void User::CopyFrom(const User& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:user_engine.User)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool User::IsInitialized() const {
  return true;
}

void User::InternalSwap(User* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &name_, lhs_arena,
      &other->name_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &password_, lhs_arena,
      &other->password_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      &public_key_, lhs_arena,
      &other->public_key_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(User, role_)
      + sizeof(User::role_)
      - PROTOBUF_FIELD_OFFSET(User, id_)>(
          reinterpret_cast<char*>(&id_),
          reinterpret_cast<char*>(&other->id_));
}

::PROTOBUF_NAMESPACE_ID::Metadata User::GetMetadata() const {
  return ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(
      &descriptor_table_user_5fengine_2eproto_getter, &descriptor_table_user_5fengine_2eproto_once,
      file_level_metadata_user_5fengine_2eproto[0]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace user_engine
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::user_engine::User* Arena::CreateMaybeMessage< ::user_engine::User >(Arena* arena) {
  return Arena::CreateMessageInternal< ::user_engine::User >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
