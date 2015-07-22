#ifndef DNA_HPP
#define DNA_HPP

/* BIG FAT WARNING!!!
 *
 * The type-structure of this file is expected to remain consistent for 'atdna'
 * Any changes to the types or namespacing must be reflected in 'atdna/main.cpp'
 */

#include "Global.hpp"
#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"
#include <vector>
#include <memory>

namespace Athena
{
namespace io
{

/* forward-declaration dance for recursively-derived types */

template <size_t sizeVar, Endian VE>
struct Buffer;

template <atInt32 sizeVar, Endian VE>
struct String;

template <atInt32 sizeVar, Endian VE>
struct WString;

template <atInt32 sizeVar, Endian VE>
struct WStringAsString;

/**
 * @brief Base DNA class used against 'atdna'
 *
 * Athena bundles a build-tool called 'atdna'. This tool functions
 * just like the 'clang' compiler, except it emits a full .cpp implementation
 * with all read/write calls necessary to marshal the DNA structure to/from
 * a streamed medium
 */
template <Endian DNAE>
struct DNA
{
    virtual void read(IStreamReader&)=0;
    virtual void write(IStreamWriter&) const=0;

    template <typename T, Endian VE = DNAE>
    using Value = T;

    template <typename T, size_t cntVar, Endian VE = DNAE>
    using Vector = std::vector<T>;

    template <size_t sizeVar>
    using Buffer = struct Athena::io::Buffer<sizeVar, DNAE>;

    template <atInt32 sizeVar = -1>
    using String = struct Athena::io::String<sizeVar, DNAE>;

    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = struct Athena::io::WString<sizeVar, VE>;

    template <atInt32 sizeVar = -1>
    using WStringAsString = struct Athena::io::WStringAsString<sizeVar, DNAE>;

    template <off_t offset, SeekOrigin direction>
    struct Seek {};

    template <size_t align>
    struct Align {};

    struct Delete {};
};

/* Concrete DNA types */

template <size_t sizeVar, Endian VE>
struct Buffer : public DNA<VE>, public std::unique_ptr<atUint8[]>
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {
        reset(new atUint8[sizeVar]);
        reader.readUBytesToBuf(get(), sizeVar);
    }
    inline void write(IStreamWriter& writer) const
    {
        writer.writeUBytes(get(), sizeVar);
    }
};

template <atInt32 sizeVar, Endian VE>
struct String : public DNA<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {*this = reader.readString(sizeVar);}
    inline void write(IStreamWriter& writer) const
    {writer.writeString(*this, sizeVar);}
    inline std::string& operator=(const std::string& __str)
    {return this->assign(__str);}
    inline std::string& operator=(std::string&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE>
struct WString : public DNA<VE>, public std::wstring
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {
        reader.setEndian(VE);
        *this = reader.readWString(sizeVar);
    }
    inline void write(IStreamWriter& writer) const
    {
        writer.setEndian(VE);
        writer.writeWString(*this, sizeVar);
    }
    inline std::wstring& operator=(const std::wstring& __str)
    {return this->assign(__str);}
    inline std::wstring& operator=(std::wstring&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE>
struct WStringAsString : public DNA<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {*this = reader.readWStringAsString(sizeVar);}
    inline void write(IStreamWriter& writer) const
    {writer.writeStringAsWString(*this, sizeVar);}
    inline std::string& operator=(const std::string& __str)
    {return this->assign(__str);}
    inline std::string& operator=(std::string&& __str)
    {this->swap(__str); return *this;}
};

/** Macro to automatically declare read/write methods in subclasses */
#define DECL_DNA \
    void read(Athena::io::IStreamReader&); \
    void write(Athena::io::IStreamWriter&) const; \

/** Macro to automatically declare read/write methods and prevent outputting implementation */
#define DECL_EXPLICIT_DNA \
    void read(Athena::io::IStreamReader&); \
    void write(Athena::io::IStreamWriter&) const; \
    Delete __dna_delete;

/** Macro to supply count variable to atdna and mute it for other compilers */
#ifdef __clang__
#define DNA_COUNT(cnt) sizeof(cnt)
#else
#define DNA_COUNT(cnt) 0
#endif

}
}

#endif // DNA_HPP

