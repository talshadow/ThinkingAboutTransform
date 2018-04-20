#include <chrono>
#include <cinttypes>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <algorithm>
#include <sstream>

struct MacAdress {
  int mac[6];
};

template <typename  T, size_t N>
constexpr size_t getArraySize(T (&)[N] ) {
  return N;
}
constexpr auto kArrSize = 10000;
constexpr auto kHexValues = "0123456789abcdef";
using DataStruct = std::vector<MacAdress> ;


class TimeCalc{
public:
  using clock=std::chrono::high_resolution_clock;
  using TimeValue = decltype(clock::now());

  TimeCalc():m_value{clock::now()}{}
  void reset() noexcept { m_value = clock::now(); }
  long getDuration() {return (clock::now()-m_value).count();}
private:
  TimeValue m_value;
};

std::vector<MacAdress> fillArray(){
  DataStruct ret;
  ret.reserve(10000);
  std::random_device rd;
  std::default_random_engine rnd(rd());
  std::uniform_int_distribution<int> uniform_distance(1, 256);
  std::generate_n(std::back_insert_iterator<decltype(ret)>(ret),kArrSize,[&]{
    decltype(ret)::value_type mac = {};
    std::generate_n(std::begin(mac.mac),getArraySize(mac.mac),[&]{return uniform_distance(rnd);});
    return mac;}
  );

  return ret;
}

std::string toStringGuide(MacAdress const& addr) {
  std::stringstream strm;
  strm << std::setfill('0') << std::setw(2) <<std::hex << addr.mac[5] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[4] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[3] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[2] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[1] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[0];
  return strm.str();
}

std::string toStringGuideOpt(MacAdress const& addr) {
  std::string mac;
  mac.reserve(18);
  std::ostringstream strm(mac);
  strm << std::setfill('0') << std::setw(2) <<std::hex << addr.mac[5] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[4] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[3] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[2] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[1] << ':'
       << std::setfill('0') << std::setw(2) << std::hex << addr.mac[0];
  return strm.str();
}

constexpr auto buffSize = 19;
std::string toStringSprintf(MacAdress const& addr) {
  char buffer[buffSize]={0};
#define XX "%02" PRIx8
  snprintf(buffer,buffSize,XX ":" XX ":" XX ":" XX ":" XX ":" XX, addr.mac[5],addr.mac[4],addr.mac[3],addr.mac[2],addr.mac[1],addr.mac[0]);
#undef XX
  return std::string(buffer);
}

std::string toStringSprintfOpt(MacAdress const& addr) {
  std::string buffer(buffSize,'\x00');
#define XX "%02" PRIx8
  snprintf(const_cast<char*>(buffer.data()), buffSize ,XX ":" XX ":" XX ":" XX ":" XX ":" XX, addr.mac[5],addr.mac[4],addr.mac[3],addr.mac[2],addr.mac[1],addr.mac[0]);
#undef XX
  return buffer;
}


std::string toString(MacAdress const& addr) {
  auto hi = [&](const uint8_t v){return kHexValues[v & 0xf];};
  auto lo = [&](const uint8_t v){return kHexValues[v >> 4];};
  return std::string {
        lo(static_cast<uint8_t>(addr.mac[5])),hi(static_cast<uint8_t>(addr.mac[5])),':',
        lo(static_cast<uint8_t>(addr.mac[4])),hi(static_cast<uint8_t>(addr.mac[4])),':',
        lo(static_cast<uint8_t>(addr.mac[3])),hi(static_cast<uint8_t>(addr.mac[3])),':',
        lo(static_cast<uint8_t>(addr.mac[2])),hi(static_cast<uint8_t>(addr.mac[2])),':',
        lo(static_cast<uint8_t>(addr.mac[1])),hi(static_cast<uint8_t>(addr.mac[1])),':',
        lo(static_cast<uint8_t>(addr.mac[0])),hi(static_cast<uint8_t>(addr.mac[0]))
  };
}



int main() {
  std::cout << "Let's go!\n";
  constexpr auto CustomGenerator="Custom generator:";
  constexpr auto GuideGenerator="Guide generator:";
  constexpr auto GuideOptGenerator="GuideOpt generator:";
  constexpr auto SprintfGenerator="Sprintf generator:";
  constexpr auto SprintfGeneratorOpt="SprintfOpt generator:";
  std::vector<std::string> answers;
  constexpr auto fieldSize = 40;
  constexpr auto valueFieldSize = 12;
  TimeCalc timer;
  DataStruct Datas = fillArray();
  std::cout << std::setw(fieldSize) << std::left << std::setfill('.')
            << "Finish value generation:"
            << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << timer.getDuration()
            << " Data size: " << Datas.size() << "\n\n\n";
  answers.reserve(Datas.size());

  timer.reset();
  std::transform(Datas.begin(),Datas.end(),std::back_insert_iterator<decltype(answers)>(answers),toString);
  auto toStringTime {timer.getDuration()};
  std::cout << std::setw(fieldSize) << std::left << std::setfill('.')
            << CustomGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << toStringTime << "\nvalue = " << *(--answers.end())<<'\n'
            << "value = " << *(answers.begin())<<"\n\n";
  answers.clear();
  answers.reserve(Datas.size());
  std::cout << "clear answers, current size :" << answers.size() << '\n';

  timer.reset();
  std::transform(Datas.begin(),Datas.end(),std::back_insert_iterator<decltype(answers)>(answers),toStringGuide);
  auto toStringGuideTime {timer.getDuration()};
  std::cout << std::setw(fieldSize) << std::left << std::setfill('.')
            << GuideGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << toStringGuideTime <<  "\nvalue = " << *(--answers.end())<<'\n'
            << "value = " << *(answers.begin())<<"\n\n";
  answers.clear();
  answers.reserve(Datas.size());
  std::cout << "clear answers, current size :" << answers.size() << '\n';

  timer.reset();
  std::transform(Datas.begin(),Datas.end(),std::back_insert_iterator<decltype(answers)>(answers),toStringGuideOpt);
  auto toStringGuideOptTime {timer.getDuration()};
  std::cout << std::setw(fieldSize) << std::left << std::setfill('.')
            << GuideOptGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << toStringGuideOptTime <<  "\nvalue = " << *(--answers.end())<<'\n'
            << "value = " << *(answers.begin())<<"\n\n";
  answers.clear();
  answers.reserve(Datas.size());
  std::cout << "clear answers, current size :" << answers.size() << '\n';

  timer.reset();
  std::transform(Datas.begin(),Datas.end(),std::back_insert_iterator<decltype(answers)>(answers),toStringSprintf);
  auto toStringSprintfTime {timer.getDuration()};
  std::cout << std::setw(fieldSize) << std::left << std::setfill('.')
            << SprintfGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << toStringSprintfTime <<  "\nvalue = " << *(--answers.end())<<'\n'
            << "value = " << *(answers.begin())<<"\n\n";
  answers.clear();
  answers.reserve(Datas.size());
  std::cout << "clear answers, current size :" << answers.size() << '\n';

  timer.reset();
  std::transform(Datas.begin(),Datas.end(),std::back_insert_iterator<decltype(answers)>(answers),toStringSprintfOpt);
  auto toStringSprintfOptTime {timer.getDuration()};
  std::cout << std::setw(fieldSize) << std::left << std::setfill('.')
            << SprintfGeneratorOpt << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << toStringSprintfOptTime <<  "\nvalue = " << *(--answers.end())<<'\n'
            << "value = " << *(answers.begin())<<"\n\n";
  answers.clear();
  answers.reserve(Datas.size());
  std::cout << "clear answers, current size :" << answers.size() << '\n';


  std::cout << std::setw(fieldSize) << std::left << std::setfill('.')
            << CustomGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << 100*toStringTime/toStringTime << "%\n"
            << std::setw(fieldSize) << std::left << std::setfill('.')
            << GuideGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << 100*toStringGuideTime/toStringTime << "%\n"
            << std::setw(fieldSize) << std::left << std::setfill('.')
            << GuideOptGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << 100*toStringGuideOptTime/toStringTime << "%\n"
            << std::setw(fieldSize) << std::left << std::setfill('.')
            << SprintfGenerator << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << 100*toStringSprintfTime/toStringTime << "%\n"
            << std::setw(fieldSize) << std::left << std::setfill('.')
            << SprintfGeneratorOpt << std::setw(valueFieldSize) << std::right << std::setfill('.')
            << 100*toStringSprintfOptTime/toStringTime << "%\n";

  return 0;
}
