#include <algorithm>
#include <cmath>
#include <memory>
#include <iostream>
#include <regex>
#include <sstream>
#include <string.h>

#include "util.h"

// todo const as much in tdag/util as possible? like range constructor for example (copied from std::pair)

////////////////////////////////////////////////////////////////////////////////
// ustring
////////////////////////////////////////////////////////////////////////////////

ustring toUstr(int n) {
    std::string str = std::to_string(n);
    return ustring(str.begin(), str.end());
}

ustring toUstr(std::string s) {
    return reinterpret_cast<const unsigned char*>(s.c_str());
}

ustring toUstr(unsigned char* p, int len) {
    return ustring(p, len);
}

std::string fromUstr(ustring ustr) {
    std::string str;
    for (unsigned char c : ustr) {
        str += static_cast<char>(c);
    }
    return str;
}

std::ostream& operator <<(std::ostream& os, const ustring ustr) {
    return os << fromUstr(ustr);
}

////////////////////////////////////////////////////////////////////////////////
// Range
////////////////////////////////////////////////////////////////////////////////

template <typename T>
// can't call default constructor for `std::pair` without explicit vals? `0, 0` is supposed to be default
Range<T>::Range() : std::pair<T, T>(0, 0) {}

template <typename T>
Range<T>::Range(const T& start, const T& end) : std::pair<T, T>(start, end) {}

template <typename T>
Range<T> Range<T>::fromStr(std::string str) {
    std::regex re("(.*?)-(.*)");
    std::smatch matches;
    if (!std::regex_search(str, matches, re) || matches.size() != 3) {
        std::cerr << "Error: bad string passed to `Range.Range()`, the world is going to end" << std::endl;
        exit(EXIT_FAILURE);
    }

    Range<T> range;
    range.first = T(std::stoi(matches[1].str()));
    range.second = T(std::stoi(matches[2].str()));
    return range;
}

template <typename T>
T Range<T>::size() {
    return (T)abs(this->second - this->first);
}

template <typename T>
bool Range<T>::contains(Range<T> range) {
    return this->first <= range.first && this->second >= range.second;
}

template <typename T>
bool Range<T>::isDisjointWith(Range<T> range) {
    return this->second < range.first || this->first > range.second;
}

template <typename T>
std::ostream& operator <<(std::ostream& os, const Range<T>& range) {
    return os << range.first << "-" << range.second;
}

template<typename T>
std::string operator +(const std::string& str, const Range<T>& range) {
    std::stringstream ss;
    ss << str << range;
    return ss.str();
}

template <typename T>
ustring toUstr(Range<T> range) {
    return toUstr(range.first) + toUstr("-") + toUstr(range.second);
}

template class Range<Id>;
template class Range<Kw>;

template std::ostream& operator <<(std::ostream& os, const Range<Id>& range);
template std::ostream& operator <<(std::ostream& os, const Range<Kw>& range);

template std::string operator +(const std::string& str, const Range<Id>& range);
template std::string operator +(const std::string& str, const Range<Kw>& range);

template ustring toUstr(Range<Id> range);
template ustring toUstr(Range<Kw> range);

////////////////////////////////////////////////////////////////////////////////
// IEncryptable
////////////////////////////////////////////////////////////////////////////////

template <typename T>
IEncryptable<T>::IEncryptable(T val) {
    this->val = val;
}

template <typename T>
T IEncryptable<T>::get() {
    return this->val;
}

Id::Id(int val) : IEncryptable<int>(val) {}

ustring Id::encode() {
    return ::toUstr(this->val);
}

Id Id::decode(ustring ustr) {
    std::string str(ustr.begin(), ustr.end());
    return Id(std::stoi(str));
}

Id abs(const Id& id) {
    return Id(abs(id.val));
}

void operator ++(Id& id, int _) {
    id.val++;
}

Id operator +(const Id& id1, const Id& id2) {
    return id1.val + id2.val;
}

Id operator +(const Id& id1, const int n) {
    return Id(id1.val + n);
}

Id operator -(const Id& id1, const Id& id2) {
    return id1.val - id2.val;
}

Id operator -(const Id& id1, const int n) {
    return Id(id1.val - n);
}

bool operator ==(const Id& id1, const Id& id2) {
    return id1.val == id2.val;
}

bool operator <(const Id& id1, const Id& id2) {
    return id1.val < id2.val;
}

bool operator >(const Id& id1, const Id& id2) {
    return id1.val > id2.val;
}

bool operator <=(const Id& id1, const Id& id2) {
    return id1.val <= id2.val;
}

bool operator >=(const Id& id1, const Id& id2) {
    return id1.val >= id2.val;
}

std::ostream& operator <<(std::ostream& os, const Id& id) {
    return os << id.val;
}

template <typename T>
ustring toUstr(IEncryptable<T>& iEncryptable) {
    return iEncryptable.encode();
}

// todo temp?

SrciDb1Doc::SrciDb1Doc(KwRange kwRange, IdRange idRange)
        : IEncryptable<std::pair<KwRange, IdRange>>(std::pair<KwRange, IdRange> {kwRange, idRange}) {}

ustring SrciDb1Doc::encode() {
    std::string str = "(" + this->val.first + "," + this->val.second + ")";
    return ::toUstr(str);
}

SrciDb1Doc SrciDb1Doc::decode(ustring ustr) {
    std::string str = ::fromUstr(ustr);
    std::regex re("\\((.*?),(.*?)\\)");
    std::smatch matches;
    if (!std::regex_search(str, matches, re) || matches.size() != 3) {
        std::cerr << "Error: bad string passed to `SrciDb1Doc.fromUstr()`, the world is going to end" << std::endl;
        exit(EXIT_FAILURE);
    }
    KwRange kwRange = KwRange::fromStr(matches[1].str());
    IdRange idRange = IdRange::fromStr(matches[2].str());
    return SrciDb1Doc {kwRange, idRange};
}

std::ostream& operator <<(std::ostream& os, const SrciDb1Doc& srciDb1Doc) {
    return os << "(" << srciDb1Doc.val.first << ", [" << srciDb1Doc.val.second << "])";
}

template class IEncryptable<int>;
template class IEncryptable<std::pair<KwRange, IdRange>>;

template ustring toUstr(IEncryptable<int>& id);
template ustring toUstr(IEncryptable<std::pair<KwRange, IdRange>>& srciDb1Doc);

////////////////////////////////////////////////////////////////////////////////
// Other
////////////////////////////////////////////////////////////////////////////////

template <typename DbDocType, typename DbKwType>
void sortDb(Db<DbDocType, DbKwType>& db) {
    std::sort(db.begin(), db.end(), [](std::tuple<Id, KwRange> entry1, std::tuple<Id, KwRange> entry2) {
        return std::get<0>(entry1) < std::get<0>(entry2);
    });
}

template void sortDb(Db<Id, KwRange>& db);
