// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: math_service.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_math_5fservice_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_math_5fservice_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3020000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3020003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_math_5fservice_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_math_5fservice_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_math_5fservice_2eproto;
namespace mathservice {
class AddRequest;
struct AddRequestDefaultTypeInternal;
extern AddRequestDefaultTypeInternal _AddRequest_default_instance_;
class AddResponse;
struct AddResponseDefaultTypeInternal;
extern AddResponseDefaultTypeInternal _AddResponse_default_instance_;
class MultiplyRequest;
struct MultiplyRequestDefaultTypeInternal;
extern MultiplyRequestDefaultTypeInternal _MultiplyRequest_default_instance_;
class MultiplyResponse;
struct MultiplyResponseDefaultTypeInternal;
extern MultiplyResponseDefaultTypeInternal _MultiplyResponse_default_instance_;
}  // namespace mathservice
PROTOBUF_NAMESPACE_OPEN
template<> ::mathservice::AddRequest* Arena::CreateMaybeMessage<::mathservice::AddRequest>(Arena*);
template<> ::mathservice::AddResponse* Arena::CreateMaybeMessage<::mathservice::AddResponse>(Arena*);
template<> ::mathservice::MultiplyRequest* Arena::CreateMaybeMessage<::mathservice::MultiplyRequest>(Arena*);
template<> ::mathservice::MultiplyResponse* Arena::CreateMaybeMessage<::mathservice::MultiplyResponse>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace mathservice {

// ===================================================================

class AddRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:mathservice.AddRequest) */ {
 public:
  inline AddRequest() : AddRequest(nullptr) {}
  ~AddRequest() override;
  explicit PROTOBUF_CONSTEXPR AddRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  AddRequest(const AddRequest& from);
  AddRequest(AddRequest&& from) noexcept
    : AddRequest() {
    *this = ::std::move(from);
  }

  inline AddRequest& operator=(const AddRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline AddRequest& operator=(AddRequest&& from) noexcept {
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
  static const AddRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const AddRequest* internal_default_instance() {
    return reinterpret_cast<const AddRequest*>(
               &_AddRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(AddRequest& a, AddRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(AddRequest* other) {
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
  void UnsafeArenaSwap(AddRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  AddRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<AddRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const AddRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const AddRequest& from);
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
  void InternalSwap(AddRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "mathservice.AddRequest";
  }
  protected:
  explicit AddRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kAFieldNumber = 1,
    kBFieldNumber = 2,
  };
  // int32 a = 1;
  void clear_a();
  int32_t a() const;
  void set_a(int32_t value);
  private:
  int32_t _internal_a() const;
  void _internal_set_a(int32_t value);
  public:

  // int32 b = 2;
  void clear_b();
  int32_t b() const;
  void set_b(int32_t value);
  private:
  int32_t _internal_b() const;
  void _internal_set_b(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:mathservice.AddRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t a_;
  int32_t b_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_math_5fservice_2eproto;
};
// -------------------------------------------------------------------

class AddResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:mathservice.AddResponse) */ {
 public:
  inline AddResponse() : AddResponse(nullptr) {}
  ~AddResponse() override;
  explicit PROTOBUF_CONSTEXPR AddResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  AddResponse(const AddResponse& from);
  AddResponse(AddResponse&& from) noexcept
    : AddResponse() {
    *this = ::std::move(from);
  }

  inline AddResponse& operator=(const AddResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline AddResponse& operator=(AddResponse&& from) noexcept {
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
  static const AddResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const AddResponse* internal_default_instance() {
    return reinterpret_cast<const AddResponse*>(
               &_AddResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(AddResponse& a, AddResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(AddResponse* other) {
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
  void UnsafeArenaSwap(AddResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  AddResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<AddResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const AddResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const AddResponse& from);
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
  void InternalSwap(AddResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "mathservice.AddResponse";
  }
  protected:
  explicit AddResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kResultFieldNumber = 1,
  };
  // int32 result = 1;
  void clear_result();
  int32_t result() const;
  void set_result(int32_t value);
  private:
  int32_t _internal_result() const;
  void _internal_set_result(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:mathservice.AddResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t result_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_math_5fservice_2eproto;
};
// -------------------------------------------------------------------

class MultiplyRequest final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:mathservice.MultiplyRequest) */ {
 public:
  inline MultiplyRequest() : MultiplyRequest(nullptr) {}
  ~MultiplyRequest() override;
  explicit PROTOBUF_CONSTEXPR MultiplyRequest(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  MultiplyRequest(const MultiplyRequest& from);
  MultiplyRequest(MultiplyRequest&& from) noexcept
    : MultiplyRequest() {
    *this = ::std::move(from);
  }

  inline MultiplyRequest& operator=(const MultiplyRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline MultiplyRequest& operator=(MultiplyRequest&& from) noexcept {
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
  static const MultiplyRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const MultiplyRequest* internal_default_instance() {
    return reinterpret_cast<const MultiplyRequest*>(
               &_MultiplyRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(MultiplyRequest& a, MultiplyRequest& b) {
    a.Swap(&b);
  }
  inline void Swap(MultiplyRequest* other) {
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
  void UnsafeArenaSwap(MultiplyRequest* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  MultiplyRequest* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<MultiplyRequest>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const MultiplyRequest& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const MultiplyRequest& from);
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
  void InternalSwap(MultiplyRequest* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "mathservice.MultiplyRequest";
  }
  protected:
  explicit MultiplyRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kAFieldNumber = 1,
    kBFieldNumber = 2,
  };
  // int32 a = 1;
  void clear_a();
  int32_t a() const;
  void set_a(int32_t value);
  private:
  int32_t _internal_a() const;
  void _internal_set_a(int32_t value);
  public:

  // int32 b = 2;
  void clear_b();
  int32_t b() const;
  void set_b(int32_t value);
  private:
  int32_t _internal_b() const;
  void _internal_set_b(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:mathservice.MultiplyRequest)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t a_;
  int32_t b_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_math_5fservice_2eproto;
};
// -------------------------------------------------------------------

class MultiplyResponse final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:mathservice.MultiplyResponse) */ {
 public:
  inline MultiplyResponse() : MultiplyResponse(nullptr) {}
  ~MultiplyResponse() override;
  explicit PROTOBUF_CONSTEXPR MultiplyResponse(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  MultiplyResponse(const MultiplyResponse& from);
  MultiplyResponse(MultiplyResponse&& from) noexcept
    : MultiplyResponse() {
    *this = ::std::move(from);
  }

  inline MultiplyResponse& operator=(const MultiplyResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline MultiplyResponse& operator=(MultiplyResponse&& from) noexcept {
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
  static const MultiplyResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const MultiplyResponse* internal_default_instance() {
    return reinterpret_cast<const MultiplyResponse*>(
               &_MultiplyResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    3;

  friend void swap(MultiplyResponse& a, MultiplyResponse& b) {
    a.Swap(&b);
  }
  inline void Swap(MultiplyResponse* other) {
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
  void UnsafeArenaSwap(MultiplyResponse* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  MultiplyResponse* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<MultiplyResponse>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const MultiplyResponse& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom(const MultiplyResponse& from);
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
  void InternalSwap(MultiplyResponse* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "mathservice.MultiplyResponse";
  }
  protected:
  explicit MultiplyResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kResultFieldNumber = 1,
  };
  // int32 result = 1;
  void clear_result();
  int32_t result() const;
  void set_result(int32_t value);
  private:
  int32_t _internal_result() const;
  void _internal_set_result(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:mathservice.MultiplyResponse)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  int32_t result_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_math_5fservice_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AddRequest

// int32 a = 1;
inline void AddRequest::clear_a() {
  a_ = 0;
}
inline int32_t AddRequest::_internal_a() const {
  return a_;
}
inline int32_t AddRequest::a() const {
  // @@protoc_insertion_point(field_get:mathservice.AddRequest.a)
  return _internal_a();
}
inline void AddRequest::_internal_set_a(int32_t value) {
  
  a_ = value;
}
inline void AddRequest::set_a(int32_t value) {
  _internal_set_a(value);
  // @@protoc_insertion_point(field_set:mathservice.AddRequest.a)
}

// int32 b = 2;
inline void AddRequest::clear_b() {
  b_ = 0;
}
inline int32_t AddRequest::_internal_b() const {
  return b_;
}
inline int32_t AddRequest::b() const {
  // @@protoc_insertion_point(field_get:mathservice.AddRequest.b)
  return _internal_b();
}
inline void AddRequest::_internal_set_b(int32_t value) {
  
  b_ = value;
}
inline void AddRequest::set_b(int32_t value) {
  _internal_set_b(value);
  // @@protoc_insertion_point(field_set:mathservice.AddRequest.b)
}

// -------------------------------------------------------------------

// AddResponse

// int32 result = 1;
inline void AddResponse::clear_result() {
  result_ = 0;
}
inline int32_t AddResponse::_internal_result() const {
  return result_;
}
inline int32_t AddResponse::result() const {
  // @@protoc_insertion_point(field_get:mathservice.AddResponse.result)
  return _internal_result();
}
inline void AddResponse::_internal_set_result(int32_t value) {
  
  result_ = value;
}
inline void AddResponse::set_result(int32_t value) {
  _internal_set_result(value);
  // @@protoc_insertion_point(field_set:mathservice.AddResponse.result)
}

// -------------------------------------------------------------------

// MultiplyRequest

// int32 a = 1;
inline void MultiplyRequest::clear_a() {
  a_ = 0;
}
inline int32_t MultiplyRequest::_internal_a() const {
  return a_;
}
inline int32_t MultiplyRequest::a() const {
  // @@protoc_insertion_point(field_get:mathservice.MultiplyRequest.a)
  return _internal_a();
}
inline void MultiplyRequest::_internal_set_a(int32_t value) {
  
  a_ = value;
}
inline void MultiplyRequest::set_a(int32_t value) {
  _internal_set_a(value);
  // @@protoc_insertion_point(field_set:mathservice.MultiplyRequest.a)
}

// int32 b = 2;
inline void MultiplyRequest::clear_b() {
  b_ = 0;
}
inline int32_t MultiplyRequest::_internal_b() const {
  return b_;
}
inline int32_t MultiplyRequest::b() const {
  // @@protoc_insertion_point(field_get:mathservice.MultiplyRequest.b)
  return _internal_b();
}
inline void MultiplyRequest::_internal_set_b(int32_t value) {
  
  b_ = value;
}
inline void MultiplyRequest::set_b(int32_t value) {
  _internal_set_b(value);
  // @@protoc_insertion_point(field_set:mathservice.MultiplyRequest.b)
}

// -------------------------------------------------------------------

// MultiplyResponse

// int32 result = 1;
inline void MultiplyResponse::clear_result() {
  result_ = 0;
}
inline int32_t MultiplyResponse::_internal_result() const {
  return result_;
}
inline int32_t MultiplyResponse::result() const {
  // @@protoc_insertion_point(field_get:mathservice.MultiplyResponse.result)
  return _internal_result();
}
inline void MultiplyResponse::_internal_set_result(int32_t value) {
  
  result_ = value;
}
inline void MultiplyResponse::set_result(int32_t value) {
  _internal_set_result(value);
  // @@protoc_insertion_point(field_set:mathservice.MultiplyResponse.result)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace mathservice

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_math_5fservice_2eproto
