/*
 * Copyright 2021 Ingemar Hedvall
 * SPDX-License-Identifier: MIT
 */
#include "fh4block.h"
namespace {
constexpr size_t kIndexNext = 0;
constexpr size_t kIndexMd = 1;
}
namespace mdf::detail {

void Fh4Block::GetBlockProperty(BlockPropertyList &dest) const {
  IBlock::GetBlockProperty(dest);

  dest.emplace_back("Links", "", "",BlockItemType::HeaderItem);
  dest.emplace_back("Next FH", ToHexString(Link(kIndexNext)), "", BlockItemType::LinkItem);
  dest.emplace_back("Comment MD", ToHexString(Link(kIndexMd)), "", BlockItemType::LinkItem);
  dest.emplace_back("", "", "", BlockItemType::BlankItem);

  dest.emplace_back("FH Info", "", "", BlockItemType::HeaderItem);
  timestamp_.GetBlockProperty(dest);
  if (md_comment_) {
    md_comment_->GetBlockProperty(dest);
  }
}

size_t Fh4Block::Read(std::FILE *file) {
  size_t bytes = ReadHeader4(file);
  timestamp_.Init(*this);
  bytes += timestamp_.Read(file);
  ReadMdComment(file, kIndexMd);
  return bytes;
}

size_t Fh4Block::Write(std::FILE *file) {
  const bool update = FilePosition() > 0; // Write or update the values inside the block
  if (update) {
    return block_size_;
  }
  block_type_ = "##FH";
  block_size_ = 24 + (2*8) + 8 + 2 + 2 + 1 + 3;
  link_list_.resize(2,0);

  auto bytes = IBlock::Write(file);
  bytes += timestamp_.Write(file);
  bytes += WriteBytes(file, 3);
  UpdateBlockSize(file, bytes);
  WriteMdComment(file, kIndexMd);
  return bytes;
}

IMetaData *Fh4Block::MetaData() {
  CreateMd4Block();
  return dynamic_cast<IMetaData *>(md_comment_.get());
}

const IMetaData *Fh4Block::MetaData() const {
  return !md_comment_ ? nullptr : dynamic_cast<IMetaData *>(md_comment_.get());
}

int64_t Fh4Block::Index() const {
  return FilePosition();
}

void Fh4Block::Time(uint64_t ns_since_1970) {
  timestamp_.NsSince1970(ns_since_1970);
}

uint64_t Fh4Block::Time() const {
  return timestamp_.NsSince1970();
}

}