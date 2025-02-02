#include "athena/FileReader.hpp"

#if __APPLE__ || __FreeBSD__
#include "osx_largefilewrapper.h"
#elif GEKKO
#include "gekko_support.h"
#include "osx_largefilewrapper.h"
#endif

namespace athena::io {
FileReader::FileReader(std::string_view filename, atInt32 cacheSize, bool globalErr)
: m_fileHandle(nullptr), m_cacheData(nullptr), m_offset(0), m_globalErr(globalErr) {
  m_filename = filename;
  open();
  setCacheSize(cacheSize);
}

FileReader::FileReader(std::wstring_view filename, atInt32 cacheSize, bool globalErr)
: m_fileHandle(nullptr), m_cacheData(nullptr), m_offset(0), m_globalErr(globalErr) {
  m_filename = utility::wideToUtf8(filename);
  open();
  setCacheSize(cacheSize);
}

FileReader::~FileReader() {
  if (isOpen())
    close();
}

void FileReader::open() {
  m_fileHandle = fopen(m_filename.c_str(), "rb");

  if (!m_fileHandle) {
    std::string _filename = filename();
    if (m_globalErr)
      atError("File not found '{}'", _filename);
    setError();
    return;
  }

  m_fileSize = utility::fileSize(m_filename);

  // reset error
  m_hasError = false;
}

void FileReader::close() {
  if (!m_fileHandle) {
    if (m_globalErr)
      atError("Cannot close an unopened stream");
    setError();
    return;
  }

  fclose(m_fileHandle);
  m_fileHandle = NULL;
  return;
}

void FileReader::seek(atInt64 pos, SeekOrigin origin) {
  if (!isOpen())
    return;

  // check block position
  if (m_blockSize > 0) {
    switch (origin) {
    case SeekOrigin::Begin:
      m_offset = pos;
      break;
    case SeekOrigin::Current:
      m_offset += pos;
      break;
    case SeekOrigin::End:
      m_offset = length() - pos;
      break;
    }
    if (m_offset > length()) {
      if (m_globalErr)
        atError("Unable to seek in file");
      setError();
      return;
    }

    size_t block = m_offset / m_blockSize;
    if (atInt32(block) != m_curBlock) {
      fseeko64(m_fileHandle, block * m_blockSize, SEEK_SET);
      fread(m_cacheData.get(), 1, m_blockSize, m_fileHandle);
      m_curBlock = atInt32(block);
    }
  } else if (fseeko64(m_fileHandle, pos, int(origin)) != 0) {
    if (m_globalErr)
      atError("Unable to seek in file");
    setError();
  }
}

atUint64 FileReader::position() const {
  if (!isOpen()) {
    if (m_globalErr)
      atError("File not open");
    return 0;
  }

  if (m_blockSize > 0)
    return m_offset;
  else
    return atUint64(ftello64(m_fileHandle));
}

atUint64 FileReader::length() const {
  if (!isOpen()) {
    if (m_globalErr)
      atError("File not open");
    return 0;
  }

  return m_fileSize;
}

atUint64 FileReader::readUBytesToBuf(void* buf, atUint64 len) {
  if (!isOpen()) {
    if (m_globalErr)
      atError("File not open for reading");
    setError();
    return 0;
  }

  if (m_blockSize <= 0)
    return fread(buf, 1, len, m_fileHandle);
  else {
    if (m_offset >= m_fileSize)
      return 0;
    if (m_offset + len >= m_fileSize)
      len = m_fileSize - m_offset;

    size_t block = m_offset / m_blockSize;
    atUint64 cacheOffset = m_offset % m_blockSize;
    atUint64 cacheSize;
    atUint64 rem = len;
    atUint8* dst = (atUint8*)buf;

    while (rem) {
      if (atInt32(block) != m_curBlock) {
        fseeko64(m_fileHandle, block * m_blockSize, SEEK_SET);
        fread(m_cacheData.get(), 1, m_blockSize, m_fileHandle);
        m_curBlock = atInt32(block);
      }

      cacheSize = rem;
      if (atInt32(cacheSize + cacheOffset) > m_blockSize)
        cacheSize = m_blockSize - cacheOffset;

      memmove(dst, m_cacheData.get() + cacheOffset, cacheSize);
      dst += cacheSize;
      rem -= cacheSize;
      cacheOffset = 0;
      ++block;
    }
    m_offset += len;
    return atUint64(dst - reinterpret_cast<atUint8*>(buf));
  }
}

void FileReader::setCacheSize(const atInt32 blockSize) {
  m_blockSize = blockSize;

  atInt32 len = atInt32(length());
  if (m_blockSize > len)
    m_blockSize = len;

  m_curBlock = -1;
  if (m_blockSize > 0)
    m_cacheData.reset(new atUint8[m_blockSize]);
}

} // namespace athena::io
