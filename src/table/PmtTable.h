#ifndef MPEGTS_ANALYZER_PMT_TABLE
#define MPEGTS_ANALYZER_PMT_TABLE

#include <ostream>
#include <vector>

#include "../Indent.h"
#include "../common.h"
#include "../view/DescriptorView.h"
#include "../view/EsView.h"
#include "RawTable.h"
#include "Table.h"

struct Context;

struct PmtTable final: public Table {
 public:
  class Stream;
  using ConstStreamIt = std::vector<Stream>::const_iterator;

  class Stream {
   public:
    enum StreamClass {
      VIDEO_CLASS,
      AUDIO_CLASS,
      SUBTITLE_CLASS,
      UNKNOWN_CLASS,
    };

    Stream() {}

    uint8_t type() const { return _type; }

    PID pid() const { return _pid; }

    StreamClass stream_class() const { return _stream_class; }

    ConstDescriptorIt desc_begin() const { return _descriptors.begin(); }

    ConstDescriptorIt desc_end() const { return _descriptors.end(); }

    bool is_video();

    bool is_avc();

    bool is_hevc();

    bool is_vvc();

    bool is_audio();

    bool deserialize(const EsView *es);

    template <size_t S>
    void print(Indent<S> indent, std::ostream &os) const;

   private:
    const char *stream_class_to_desc() const;

    uint8_t _type = 0;
    PID _pid = 0;
    StreamClass _stream_class = UNKNOWN_CLASS;

    std::vector<std::shared_ptr<DescriptorView>> _descriptors = {};
  };

  PmtTable() {}

  PmtTable(const PmtTable &) = delete;
  PmtTable(PmtTable &&) = default;

  PmtTable &operator=(const PmtTable &);
  PmtTable &operator=(PmtTable &&) = default;

  ConstDescriptorIt desc_begin() const { return _descriptors.begin(); }

  ConstDescriptorIt desc_end() const { return _descriptors.end(); }

  ConstStreamIt streams_begin() const { return _streams.begin(); }

  ConstStreamIt streams_end() const { return _streams.end(); }

  static constexpr TID ID = 0x02;

 private:
  bool deserialize_payload(const RawTable &raw_table) override;

  static constexpr size_t HDR_SIZE = 12;

  std::vector<DescriptorPtr> _descriptors;
  std::vector<Stream> _streams;
};

template <size_t S>
void PmtTable::Stream::print(Indent<S> indent, std::ostream &os) const {
  os << indent << "stream:" << std::endl;
  os << indent + 1 << "pid: 0x" << std::hex << static_cast<int>(_pid)
     << std::dec << std::endl;
  os << indent + 1 << "type: " << static_cast<int>(_type) << " ("
     << stream_class_to_desc() << ")" << std::endl;
}

void handle_pmt(Context &ctx, const RawTable &raw_table);

#endif
