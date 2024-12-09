#ifndef MPEGTS_ANALYZER_PSI_DEMUXER_H
#define MPEGTS_ANALYZER_PSI_DEMUXER_H

#include "table/RawTable.h"
#include "view/PacketView.h"
#include "view/SectionView.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <unordered_map>

class Context;

class PsiDemuxer {
public:
  using TableHandler = void(Context &, RawTable);

  explicit PsiDemuxer(Context &ctx);

  PsiDemuxer(PsiDemuxer &&) = delete;
  PsiDemuxer(const PsiDemuxer &) = delete;

  PsiDemuxer &operator=(PsiDemuxer &&) = delete;
  PsiDemuxer &operator=(const PsiDemuxer &) = delete;

  void process_packet(const PacketView *packet);

  void add_handler(PID pid, TID tid, std::function<TableHandler> handler);

  void remove_handler(PID pid, TID tid);

private:
  class PidContext;
  struct TidContext;

  PidContext *get_pid(PID id);

  struct TidContext {
  public:
    std::unordered_map<TID_EXT, RawTable> tables;
    std::function<TableHandler> handler;
  };

  class PidContext {
  public:
    explicit PidContext(Context &ctx) : ctx(ctx) {}

    PidContext &operator=(PidContext &&other) = delete;
    PidContext(const PidContext &) = delete;

    PidContext(PidContext &&other) = delete;
    PidContext &operator=(const PidContext &other) = delete;

    void process_packet(const PacketView *packet);

    void add_handler(TID tid, std::function<TableHandler> handler);

    void remove_handler(TID tid);

  private:
    void write_packet_data(const uint8_t *buf, size_t buf_size, bool reset_buf);

    void process_section(const SectionView *section);

    static constexpr size_t BUF_CAPACITY = 4096;

    Context &ctx;
    std::unordered_map<TID, TidContext> tids;

    uint8_t buf[BUF_CAPACITY];
    size_t buf_usage;
    bool end_reached;
    uint8_t cc;
  };

  Context &ctx;
  std::unordered_map<PID, PidContext> pids;
};

#endif
