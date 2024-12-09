#include "PsiDemuxer.h"

#include "view/PacketView.h"
#include "view/View.h"

#include <cstring>

PsiDemuxer::PsiDemuxer(Context &ctx) : ctx(ctx) {}

void PsiDemuxer::process_packet(const PacketView *packet) {
  PidContext *pid = get_pid(packet->pid());
  if (pid == nullptr) {
    return;
  }

  pid->process_packet(packet);
}

void PsiDemuxer::add_handler(PID pid, TID tid,
                             std::function<TableHandler> handler) {
  auto pid_ctx = pids.emplace(pid, ctx).first;
  pid_ctx->second.add_handler(tid, handler);
}

PsiDemuxer::PidContext *PsiDemuxer::get_pid(PID id) {
  auto found_pid = pids.find(id);
  if (found_pid == pids.end()) {
    return nullptr;
  }

  return &found_pid->second;
}

void PsiDemuxer::remove_handler(PID pid, TID tid) {
  auto found_pid_ctx = pids.find(pid);
  if (found_pid_ctx == pids.end()) {
    return;
  }

  auto &pid_ctx = found_pid_ctx->second;
  pid_ctx.remove_handler(tid);
}

void PsiDemuxer::PidContext::process_packet(const PacketView *packet) {
  const uint8_t *payload = packet->payload();

  if (packet->pusi()) {
    size_t padding_size = (size_t)(*payload);
    const uint8_t *section_start = payload + 1 + padding_size;

    if (section_start >= packet->end()) {
      return;
    }

    if (packet->is_continuous(cc)) {
      write_packet_data(payload + 1, padding_size, false);
    }

    write_packet_data(section_start, packet->end() - section_start, true);
  } else {
    if (packet->is_continuous(cc)) {
      write_packet_data(payload, packet->end() - payload, false);
    }
  }

  cc = packet->cc();
}

void PsiDemuxer::PidContext::add_handler(TID tid,
                                         std::function<TableHandler> handler) {
  tids.emplace(tid, TidContext{{}, std::move(handler)});
}

void PsiDemuxer::PidContext::remove_handler(TID tid) {
  auto found_tid_ctx = tids.find(tid);
  if (found_tid_ctx == tids.end()) {
    return;
  }

  tids.erase(found_tid_ctx);
}

void PsiDemuxer::PidContext::write_packet_data(const uint8_t *src_buf,
                                               size_t src_buf_size,
                                               bool reset) {
  if (reset) {
    buf_usage = 0;
    end_reached = false;
  }

  if (end_reached) {
    return;
  }

  src_buf_size = src_buf_size < BUF_CAPACITY - buf_usage
                     ? src_buf_size
                     : BUF_CAPACITY - buf_usage;

  std::memcpy(buf + buf_usage, src_buf, src_buf_size);
  buf_usage += src_buf_size;

  for (uint8_t *section_start = buf;
       section_start - buf < buf_usage && section_start[0] != 0xFF;) {
    uint8_t avaiable_space = buf_usage - (section_start - buf);

    const SectionView *section =
        cast_buf_to_view<SectionView>(section_start, avaiable_space);

    if (!section) {
      end_reached = false;
      return;
    }

    section_start += section->size();
    end_reached = true;
    process_section(section);
  }
}

void PsiDemuxer::PidContext::process_section(const SectionView *section) {
  auto found_tid_ctx = tids.find(section->tid());
  if (found_tid_ctx == tids.end()) {
    return;
  }
  TidContext &tid_ctx = found_tid_ctx->second;

  RawTable &raw_table =
      tid_ctx.tables.insert({section->tid_ext(), RawTable()}).first->second;

  if (!raw_table.is_uninit() &&
      raw_table.version() != section->version_number()) {
    auto _ = std::move(raw_table);
  }

  if (!raw_table.add_section(section)) {
    return;
  }

  if (raw_table.is_collected()) {
    tid_ctx.handler(ctx, std::move(raw_table));
  }
}
