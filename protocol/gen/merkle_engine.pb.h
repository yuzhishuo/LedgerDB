// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: merkle_engine.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_merkle_5fengine_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_merkle_5fengine_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3019000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3019004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_merkle_5fengine_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_merkle_5fengine_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_merkle_5fengine_2eproto;
namespace merkle_engine {
class LeafMeta;
struct LeafMetaDefaultTypeInternal;
extern LeafMetaDefaultTypeInternal _LeafMeta_default_instance_;
}  // namespace merkle_engine
PROTOBUF_NAMESPACE_OPEN
template<> ::merkle_engine::LeafMeta* Arena::CreateMaybeMessage<::merkle_engine::LeafMeta>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace merkle_engine {

// ===================================================================

class LeafMeta final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:merkle_engine.LeafMeta) */ {
 public:
  inline LeafMeta() : LeafMeta(nullptr) {}
  ~LeafMeta() override;
  explicit constexpr LeafMeta(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  LeafMeta(const LeafMeta& from);
  LeafMeta(LeafMeta&& from) noexcept
    : LeafMeta() {
    *this = ::std::move(from);
  }

  inline LeafMeta& operator=(const LeafMeta& from) {
    CopyFrom(from);
    return *this;
  }
  inline LeafMeta& operator=(LeafMeta&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const LeafMeta& default_instance() {
    return *internal_default_instance();
  }
  static inline const LeafMeta* internal_default_instance() {
    return reinterpret_cast<const LeafMeta*>(
               &_LeafMeta_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(LeafMeta& a, LeafMeta& b) {
    a.Swap(&b);
  }
  inline void Swap(LeafMeta* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LeafMeta* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  LeafMeta* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<LeafMeta>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const LeafMeta& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const LeafMeta& from);
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message* to, const ::PROTOBUF_NAMESPACE_ID::Message& from);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LeafMeta* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "merkle_engine.LeafMeta";
  }
  protected:
  explicit LeafMeta(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kHashFieldNumber = 3,
    kIndexFieldNumber = 1,
    kIdFieldNumber = 2,
  };
  // bytes hash = 3;
  void clear_hash();
  const std::string& hash() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_hash(ArgT0&& arg0, ArgT... args);
  std::string* mutable_hash();
  PROTOBUF_NODISCARD std::string* release_hash();
  void set_allocated_hash(std::string* hash);
  private:
  const std::string& _internal_hash() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_hash(const std::string& value);
  std::string* _internal_mutable_hash();
  public:

  // int64 index = 1;
  void clear_index();
  int64_t index() const;
  void set_index(int64_t value);
  private:
  int64_t _internal_index() const;
  void _internal_set_index(int64_t value);
  public:

  // int64 id = 2;
  void clear_id();
  int64_t id() const;
  void set_id(int64_t value);
  private:
  int64_t _internal_id() const;
  void _internal_set_id(int64_t value);
  public:

  // @@protoc_insertion_point(class_scope:merkle_engine.LeafMeta)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr hash_;
  int64_t index_;
  int64_t id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_merkle_5fengine_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LeafMeta

// int64 index = 1;
inline void LeafMeta::clear_index() {
  index_ = int64_t{0};
}
inline int64_t LeafMeta::_internal_index() const {
  return index_;
}
inline int64_t LeafMeta::index() const {
  // @@protoc_insertion_point(field_get:merkle_engine.LeafMeta.index)
  return _internal_index();
}
inline void LeafMeta::_internal_set_index(int64_t value) {
  
  index_ = value;
}
inline void LeafMeta::set_index(int64_t value) {
  _internal_set_index(value);
  // @@protoc_insertion_point(field_set:merkle_engine.LeafMeta.index)
}

// int64 id = 2;
inline void LeafMeta::clear_id() {
  id_ = int64_t{0};
}
inline int64_t LeafMeta::_internal_id() const {
  return id_;
}
inline int64_t LeafMeta::id() const {
  // @@protoc_insertion_point(field_get:merkle_engine.LeafMeta.id)
  return _internal_id();
}
inline void LeafMeta::_internal_set_id(int64_t value) {
  
  id_ = value;
}
inline void LeafMeta::set_id(int64_t value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:merkle_engine.LeafMeta.id)
}

// bytes hash = 3;
inline void LeafMeta::clear_hash() {
  hash_.ClearToEmpty();
}
inline const std::string& LeafMeta::hash() const {
  // @@protoc_insertion_point(field_get:merkle_engine.LeafMeta.hash)
  return _internal_hash();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LeafMeta::set_hash(ArgT0&& arg0, ArgT... args) {
 
 hash_.SetBytes(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:merkle_engine.LeafMeta.hash)
}
inline std::string* LeafMeta::mutable_hash() {
  std::string* _s = _internal_mutable_hash();
  // @@protoc_insertion_point(field_mutable:merkle_engine.LeafMeta.hash)
  return _s;
}
inline const std::string& LeafMeta::_internal_hash() const {
  return hash_.Get();
}
inline void LeafMeta::_internal_set_hash(const std::string& value) {
  
  hash_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* LeafMeta::_internal_mutable_hash() {
  
  return hash_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* LeafMeta::release_hash() {
  // @@protoc_insertion_point(field_release:merkle_engine.LeafMeta.hash)
  return hash_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void LeafMeta::set_allocated_hash(std::string* hash) {
  if (hash != nullptr) {
    
  } else {
    
  }
  hash_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), hash,
      GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (hash_.IsDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited())) {
    hash_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), "", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:merkle_engine.LeafMeta.hash)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace merkle_engine

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_merkle_5fengine_2eproto
