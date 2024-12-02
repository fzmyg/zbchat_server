// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message.proto

#include "message.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
#include "google/protobuf/generated_message_tctable_impl.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace message {

inline constexpr LoginRsp::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : error_{0},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR LoginRsp::LoginRsp(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct LoginRspDefaultTypeInternal {
  PROTOBUF_CONSTEXPR LoginRspDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~LoginRspDefaultTypeInternal() {}
  union {
    LoginRsp _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 LoginRspDefaultTypeInternal _LoginRsp_default_instance_;

inline constexpr LoginReq::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : token_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        uid_{0},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR LoginReq::LoginReq(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct LoginReqDefaultTypeInternal {
  PROTOBUF_CONSTEXPR LoginReqDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~LoginReqDefaultTypeInternal() {}
  union {
    LoginReq _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 LoginReqDefaultTypeInternal _LoginReq_default_instance_;

inline constexpr GetChatServerRsp::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : host_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        port_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        token_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        error_{0},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR GetChatServerRsp::GetChatServerRsp(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct GetChatServerRspDefaultTypeInternal {
  PROTOBUF_CONSTEXPR GetChatServerRspDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~GetChatServerRspDefaultTypeInternal() {}
  union {
    GetChatServerRsp _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 GetChatServerRspDefaultTypeInternal _GetChatServerRsp_default_instance_;

inline constexpr GetChatServerReq::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : uid_{0},
        _cached_size_{0} {}

template <typename>
PROTOBUF_CONSTEXPR GetChatServerReq::GetChatServerReq(::_pbi::ConstantInitialized)
    : _impl_(::_pbi::ConstantInitialized()) {}
struct GetChatServerReqDefaultTypeInternal {
  PROTOBUF_CONSTEXPR GetChatServerReqDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~GetChatServerReqDefaultTypeInternal() {}
  union {
    GetChatServerReq _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 GetChatServerReqDefaultTypeInternal _GetChatServerReq_default_instance_;
}  // namespace message
static ::_pb::Metadata file_level_metadata_message_2eproto[4];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_message_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_message_2eproto = nullptr;
const ::uint32_t TableStruct_message_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::message::GetChatServerReq, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::message::GetChatServerReq, _impl_.uid_),
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::message::GetChatServerRsp, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::message::GetChatServerRsp, _impl_.error_),
    PROTOBUF_FIELD_OFFSET(::message::GetChatServerRsp, _impl_.host_),
    PROTOBUF_FIELD_OFFSET(::message::GetChatServerRsp, _impl_.port_),
    PROTOBUF_FIELD_OFFSET(::message::GetChatServerRsp, _impl_.token_),
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::message::LoginReq, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::message::LoginReq, _impl_.uid_),
    PROTOBUF_FIELD_OFFSET(::message::LoginReq, _impl_.token_),
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::message::LoginRsp, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::message::LoginRsp, _impl_.error_),
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        {0, -1, -1, sizeof(::message::GetChatServerReq)},
        {9, -1, -1, sizeof(::message::GetChatServerRsp)},
        {21, -1, -1, sizeof(::message::LoginReq)},
        {31, -1, -1, sizeof(::message::LoginRsp)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::message::_GetChatServerReq_default_instance_._instance,
    &::message::_GetChatServerRsp_default_instance_._instance,
    &::message::_LoginReq_default_instance_._instance,
    &::message::_LoginRsp_default_instance_._instance,
};
const char descriptor_table_protodef_message_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\rmessage.proto\022\007message\"\037\n\020GetChatServe"
    "rReq\022\013\n\003uid\030\001 \001(\005\"L\n\020GetChatServerRsp\022\r\n"
    "\005error\030\001 \001(\005\022\014\n\004host\030\002 \001(\t\022\014\n\004port\030\003 \001(\t"
    "\022\r\n\005token\030\004 \001(\t\"&\n\010LoginReq\022\013\n\003uid\030\001 \001(\005"
    "\022\r\n\005token\030\002 \001(\t\"\031\n\010LoginRsp\022\r\n\005error\030\001 \001"
    "(\0052\207\001\n\rStatusService\022E\n\rGetChatServer\022\031."
    "message.GetChatServerReq\032\031.message.GetCh"
    "atServerRsp\022/\n\005Login\022\021.message.LoginReq\032"
    "\021.message.LoginRsp\"\000b\006proto3"
};
static ::absl::once_flag descriptor_table_message_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_message_2eproto = {
    false,
    false,
    348,
    descriptor_table_protodef_message_2eproto,
    "message.proto",
    &descriptor_table_message_2eproto_once,
    nullptr,
    0,
    4,
    schemas,
    file_default_instances,
    TableStruct_message_2eproto::offsets,
    file_level_metadata_message_2eproto,
    file_level_enum_descriptors_message_2eproto,
    file_level_service_descriptors_message_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_message_2eproto_getter() {
  return &descriptor_table_message_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_message_2eproto(&descriptor_table_message_2eproto);
namespace message {
// ===================================================================

class GetChatServerReq::_Internal {
 public:
};

GetChatServerReq::GetChatServerReq(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:message.GetChatServerReq)
}
GetChatServerReq::GetChatServerReq(
    ::google::protobuf::Arena* arena, const GetChatServerReq& from)
    : GetChatServerReq(arena) {
  MergeFrom(from);
}
inline PROTOBUF_NDEBUG_INLINE GetChatServerReq::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0} {}

inline void GetChatServerReq::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.uid_ = {};
}
GetChatServerReq::~GetChatServerReq() {
  // @@protoc_insertion_point(destructor:message.GetChatServerReq)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void GetChatServerReq::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.~Impl_();
}

PROTOBUF_NOINLINE void GetChatServerReq::Clear() {
// @@protoc_insertion_point(message_clear_start:message.GetChatServerReq)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.uid_ = 0;
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* GetChatServerReq::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 0, 2> GetChatServerReq::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_GetChatServerReq_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
  }, {{
    // int32 uid = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(GetChatServerReq, _impl_.uid_), 63>(),
     {8, 63, 0, PROTOBUF_FIELD_OFFSET(GetChatServerReq, _impl_.uid_)}},
  }}, {{
    65535, 65535
  }}, {{
    // int32 uid = 1;
    {PROTOBUF_FIELD_OFFSET(GetChatServerReq, _impl_.uid_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
  }},
  // no aux_entries
  {{
  }},
};

::uint8_t* GetChatServerReq::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:message.GetChatServerReq)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // int32 uid = 1;
  if (this->_internal_uid() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::
        WriteInt32ToArrayWithField<1>(
            stream, this->_internal_uid(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:message.GetChatServerReq)
  return target;
}

::size_t GetChatServerReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:message.GetChatServerReq)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 uid = 1;
  if (this->_internal_uid() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
        this->_internal_uid());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::google::protobuf::Message::ClassData GetChatServerReq::_class_data_ = {
    GetChatServerReq::MergeImpl,
    nullptr,  // OnDemandRegisterArenaDtor
};
const ::google::protobuf::Message::ClassData* GetChatServerReq::GetClassData() const {
  return &_class_data_;
}

void GetChatServerReq::MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg) {
  auto* const _this = static_cast<GetChatServerReq*>(&to_msg);
  auto& from = static_cast<const GetChatServerReq&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:message.GetChatServerReq)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_uid() != 0) {
    _this->_internal_set_uid(from._internal_uid());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void GetChatServerReq::CopyFrom(const GetChatServerReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:message.GetChatServerReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool GetChatServerReq::IsInitialized() const {
  return true;
}

::_pbi::CachedSize* GetChatServerReq::AccessCachedSize() const {
  return &_impl_._cached_size_;
}
void GetChatServerReq::InternalSwap(GetChatServerReq* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
        swap(_impl_.uid_, other->_impl_.uid_);
}

::google::protobuf::Metadata GetChatServerReq::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_message_2eproto_getter, &descriptor_table_message_2eproto_once,
      file_level_metadata_message_2eproto[0]);
}
// ===================================================================

class GetChatServerRsp::_Internal {
 public:
};

GetChatServerRsp::GetChatServerRsp(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:message.GetChatServerRsp)
}
inline PROTOBUF_NDEBUG_INLINE GetChatServerRsp::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : host_(arena, from.host_),
        port_(arena, from.port_),
        token_(arena, from.token_),
        _cached_size_{0} {}

GetChatServerRsp::GetChatServerRsp(
    ::google::protobuf::Arena* arena,
    const GetChatServerRsp& from)
    : ::google::protobuf::Message(arena) {
  GetChatServerRsp* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);
  _impl_.error_ = from._impl_.error_;

  // @@protoc_insertion_point(copy_constructor:message.GetChatServerRsp)
}
inline PROTOBUF_NDEBUG_INLINE GetChatServerRsp::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : host_(arena),
        port_(arena),
        token_(arena),
        _cached_size_{0} {}

inline void GetChatServerRsp::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.error_ = {};
}
GetChatServerRsp::~GetChatServerRsp() {
  // @@protoc_insertion_point(destructor:message.GetChatServerRsp)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void GetChatServerRsp::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.host_.Destroy();
  _impl_.port_.Destroy();
  _impl_.token_.Destroy();
  _impl_.~Impl_();
}

PROTOBUF_NOINLINE void GetChatServerRsp::Clear() {
// @@protoc_insertion_point(message_clear_start:message.GetChatServerRsp)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.host_.ClearToEmpty();
  _impl_.port_.ClearToEmpty();
  _impl_.token_.ClearToEmpty();
  _impl_.error_ = 0;
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* GetChatServerRsp::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<2, 4, 0, 46, 2> GetChatServerRsp::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    4, 24,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967280,  // skipmap
    offsetof(decltype(_table_), field_entries),
    4,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_GetChatServerRsp_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
  }, {{
    // string token = 4;
    {::_pbi::TcParser::FastUS1,
     {34, 63, 0, PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.token_)}},
    // int32 error = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(GetChatServerRsp, _impl_.error_), 63>(),
     {8, 63, 0, PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.error_)}},
    // string host = 2;
    {::_pbi::TcParser::FastUS1,
     {18, 63, 0, PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.host_)}},
    // string port = 3;
    {::_pbi::TcParser::FastUS1,
     {26, 63, 0, PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.port_)}},
  }}, {{
    65535, 65535
  }}, {{
    // int32 error = 1;
    {PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.error_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
    // string host = 2;
    {PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.host_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // string port = 3;
    {PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.port_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
    // string token = 4;
    {PROTOBUF_FIELD_OFFSET(GetChatServerRsp, _impl_.token_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\30\0\4\4\5\0\0\0"
    "message.GetChatServerRsp"
    "host"
    "port"
    "token"
  }},
};

::uint8_t* GetChatServerRsp::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:message.GetChatServerRsp)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // int32 error = 1;
  if (this->_internal_error() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::
        WriteInt32ToArrayWithField<1>(
            stream, this->_internal_error(), target);
  }

  // string host = 2;
  if (!this->_internal_host().empty()) {
    const std::string& _s = this->_internal_host();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "message.GetChatServerRsp.host");
    target = stream->WriteStringMaybeAliased(2, _s, target);
  }

  // string port = 3;
  if (!this->_internal_port().empty()) {
    const std::string& _s = this->_internal_port();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "message.GetChatServerRsp.port");
    target = stream->WriteStringMaybeAliased(3, _s, target);
  }

  // string token = 4;
  if (!this->_internal_token().empty()) {
    const std::string& _s = this->_internal_token();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "message.GetChatServerRsp.token");
    target = stream->WriteStringMaybeAliased(4, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:message.GetChatServerRsp)
  return target;
}

::size_t GetChatServerRsp::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:message.GetChatServerRsp)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string host = 2;
  if (!this->_internal_host().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_host());
  }

  // string port = 3;
  if (!this->_internal_port().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_port());
  }

  // string token = 4;
  if (!this->_internal_token().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_token());
  }

  // int32 error = 1;
  if (this->_internal_error() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
        this->_internal_error());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::google::protobuf::Message::ClassData GetChatServerRsp::_class_data_ = {
    GetChatServerRsp::MergeImpl,
    nullptr,  // OnDemandRegisterArenaDtor
};
const ::google::protobuf::Message::ClassData* GetChatServerRsp::GetClassData() const {
  return &_class_data_;
}

void GetChatServerRsp::MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg) {
  auto* const _this = static_cast<GetChatServerRsp*>(&to_msg);
  auto& from = static_cast<const GetChatServerRsp&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:message.GetChatServerRsp)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_host().empty()) {
    _this->_internal_set_host(from._internal_host());
  }
  if (!from._internal_port().empty()) {
    _this->_internal_set_port(from._internal_port());
  }
  if (!from._internal_token().empty()) {
    _this->_internal_set_token(from._internal_token());
  }
  if (from._internal_error() != 0) {
    _this->_internal_set_error(from._internal_error());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void GetChatServerRsp::CopyFrom(const GetChatServerRsp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:message.GetChatServerRsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool GetChatServerRsp::IsInitialized() const {
  return true;
}

::_pbi::CachedSize* GetChatServerRsp::AccessCachedSize() const {
  return &_impl_._cached_size_;
}
void GetChatServerRsp::InternalSwap(GetChatServerRsp* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.host_, &other->_impl_.host_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.port_, &other->_impl_.port_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.token_, &other->_impl_.token_, arena);
        swap(_impl_.error_, other->_impl_.error_);
}

::google::protobuf::Metadata GetChatServerRsp::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_message_2eproto_getter, &descriptor_table_message_2eproto_once,
      file_level_metadata_message_2eproto[1]);
}
// ===================================================================

class LoginReq::_Internal {
 public:
};

LoginReq::LoginReq(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:message.LoginReq)
}
inline PROTOBUF_NDEBUG_INLINE LoginReq::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from)
      : token_(arena, from.token_),
        _cached_size_{0} {}

LoginReq::LoginReq(
    ::google::protobuf::Arena* arena,
    const LoginReq& from)
    : ::google::protobuf::Message(arena) {
  LoginReq* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_);
  _impl_.uid_ = from._impl_.uid_;

  // @@protoc_insertion_point(copy_constructor:message.LoginReq)
}
inline PROTOBUF_NDEBUG_INLINE LoginReq::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : token_(arena),
        _cached_size_{0} {}

inline void LoginReq::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.uid_ = {};
}
LoginReq::~LoginReq() {
  // @@protoc_insertion_point(destructor:message.LoginReq)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void LoginReq::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.token_.Destroy();
  _impl_.~Impl_();
}

PROTOBUF_NOINLINE void LoginReq::Clear() {
// @@protoc_insertion_point(message_clear_start:message.LoginReq)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.token_.ClearToEmpty();
  _impl_.uid_ = 0;
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* LoginReq::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<1, 2, 0, 30, 2> LoginReq::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    2, 8,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967292,  // skipmap
    offsetof(decltype(_table_), field_entries),
    2,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_LoginReq_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
  }, {{
    // string token = 2;
    {::_pbi::TcParser::FastUS1,
     {18, 63, 0, PROTOBUF_FIELD_OFFSET(LoginReq, _impl_.token_)}},
    // int32 uid = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(LoginReq, _impl_.uid_), 63>(),
     {8, 63, 0, PROTOBUF_FIELD_OFFSET(LoginReq, _impl_.uid_)}},
  }}, {{
    65535, 65535
  }}, {{
    // int32 uid = 1;
    {PROTOBUF_FIELD_OFFSET(LoginReq, _impl_.uid_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
    // string token = 2;
    {PROTOBUF_FIELD_OFFSET(LoginReq, _impl_.token_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kUtf8String | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
    "\20\0\5\0\0\0\0\0"
    "message.LoginReq"
    "token"
  }},
};

::uint8_t* LoginReq::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:message.LoginReq)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // int32 uid = 1;
  if (this->_internal_uid() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::
        WriteInt32ToArrayWithField<1>(
            stream, this->_internal_uid(), target);
  }

  // string token = 2;
  if (!this->_internal_token().empty()) {
    const std::string& _s = this->_internal_token();
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormatLite::SERIALIZE, "message.LoginReq.token");
    target = stream->WriteStringMaybeAliased(2, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:message.LoginReq)
  return target;
}

::size_t LoginReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:message.LoginReq)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string token = 2;
  if (!this->_internal_token().empty()) {
    total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                    this->_internal_token());
  }

  // int32 uid = 1;
  if (this->_internal_uid() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
        this->_internal_uid());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::google::protobuf::Message::ClassData LoginReq::_class_data_ = {
    LoginReq::MergeImpl,
    nullptr,  // OnDemandRegisterArenaDtor
};
const ::google::protobuf::Message::ClassData* LoginReq::GetClassData() const {
  return &_class_data_;
}

void LoginReq::MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg) {
  auto* const _this = static_cast<LoginReq*>(&to_msg);
  auto& from = static_cast<const LoginReq&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:message.LoginReq)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_token().empty()) {
    _this->_internal_set_token(from._internal_token());
  }
  if (from._internal_uid() != 0) {
    _this->_internal_set_uid(from._internal_uid());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void LoginReq::CopyFrom(const LoginReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:message.LoginReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool LoginReq::IsInitialized() const {
  return true;
}

::_pbi::CachedSize* LoginReq::AccessCachedSize() const {
  return &_impl_._cached_size_;
}
void LoginReq::InternalSwap(LoginReq* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.token_, &other->_impl_.token_, arena);
        swap(_impl_.uid_, other->_impl_.uid_);
}

::google::protobuf::Metadata LoginReq::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_message_2eproto_getter, &descriptor_table_message_2eproto_once,
      file_level_metadata_message_2eproto[2]);
}
// ===================================================================

class LoginRsp::_Internal {
 public:
};

LoginRsp::LoginRsp(::google::protobuf::Arena* arena)
    : ::google::protobuf::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:message.LoginRsp)
}
LoginRsp::LoginRsp(
    ::google::protobuf::Arena* arena, const LoginRsp& from)
    : LoginRsp(arena) {
  MergeFrom(from);
}
inline PROTOBUF_NDEBUG_INLINE LoginRsp::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0} {}

inline void LoginRsp::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.error_ = {};
}
LoginRsp::~LoginRsp() {
  // @@protoc_insertion_point(destructor:message.LoginRsp)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void LoginRsp::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.~Impl_();
}

PROTOBUF_NOINLINE void LoginRsp::Clear() {
// @@protoc_insertion_point(message_clear_start:message.LoginRsp)
  PROTOBUF_TSAN_WRITE(&_impl_._tsan_detect_race);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.error_ = 0;
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

const char* LoginRsp::_InternalParse(
    const char* ptr, ::_pbi::ParseContext* ctx) {
  ptr = ::_pbi::TcParser::ParseLoop(this, ptr, ctx, &_table_.header);
  return ptr;
}


PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 1, 0, 0, 2> LoginRsp::_table_ = {
  {
    0,  // no _has_bits_
    0, // no _extensions_
    1, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967294,  // skipmap
    offsetof(decltype(_table_), field_entries),
    1,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    &_LoginRsp_default_instance_._instance,
    ::_pbi::TcParser::GenericFallback,  // fallback
  }, {{
    // int32 error = 1;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(LoginRsp, _impl_.error_), 63>(),
     {8, 63, 0, PROTOBUF_FIELD_OFFSET(LoginRsp, _impl_.error_)}},
  }}, {{
    65535, 65535
  }}, {{
    // int32 error = 1;
    {PROTOBUF_FIELD_OFFSET(LoginRsp, _impl_.error_), 0, 0,
    (0 | ::_fl::kFcSingular | ::_fl::kInt32)},
  }},
  // no aux_entries
  {{
  }},
};

::uint8_t* LoginRsp::_InternalSerialize(
    ::uint8_t* target,
    ::google::protobuf::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:message.LoginRsp)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  // int32 error = 1;
  if (this->_internal_error() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::
        WriteInt32ToArrayWithField<1>(
            stream, this->_internal_error(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:message.LoginRsp)
  return target;
}

::size_t LoginRsp::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:message.LoginRsp)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 error = 1;
  if (this->_internal_error() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
        this->_internal_error());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::google::protobuf::Message::ClassData LoginRsp::_class_data_ = {
    LoginRsp::MergeImpl,
    nullptr,  // OnDemandRegisterArenaDtor
};
const ::google::protobuf::Message::ClassData* LoginRsp::GetClassData() const {
  return &_class_data_;
}

void LoginRsp::MergeImpl(::google::protobuf::Message& to_msg, const ::google::protobuf::Message& from_msg) {
  auto* const _this = static_cast<LoginRsp*>(&to_msg);
  auto& from = static_cast<const LoginRsp&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:message.LoginRsp)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_error() != 0) {
    _this->_internal_set_error(from._internal_error());
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void LoginRsp::CopyFrom(const LoginRsp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:message.LoginRsp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool LoginRsp::IsInitialized() const {
  return true;
}

::_pbi::CachedSize* LoginRsp::AccessCachedSize() const {
  return &_impl_._cached_size_;
}
void LoginRsp::InternalSwap(LoginRsp* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
        swap(_impl_.error_, other->_impl_.error_);
}

::google::protobuf::Metadata LoginRsp::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_message_2eproto_getter, &descriptor_table_message_2eproto_once,
      file_level_metadata_message_2eproto[3]);
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace message
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"