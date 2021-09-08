#ifndef DDSCXX_HELLOWORLDDATA_HPP
#define DDSCXX_HELLOWORLDDATA_HPP

#include <cstdint>
#include <string>

namespace HelloWorldData
{
class Msg
{
private:
 //int32_t picture_width_ = 0;
 unsigned picture_width_ = 0.;
 std::string message_;

public:
  Msg() = default;
  //message constructor
  explicit Msg(unsigned picture_width, const std::string& message) {
      picture_width_ = picture_width;
      message_ = message;
  }

  //function with field "picture_width"
  unsigned picture_width() const { return this->picture_width_; }

  unsigned& picture_width() { return this->picture_width_; }

  void picture_width(unsigned _val_) { this->picture_width_ = _val_; }

  //function with field "message"
  const std::string& message() const { return this->message_; }

  std::string& message() { return this->message_; }

  void message(const std::string& _val_) { this->message_ = _val_; }

  void message(std::string&& _val_) { this->message_ = _val_; }

  //Msg operators 
  bool operator==(const Msg& _other) const
  {
    return picture_width_ == _other.picture_width_ && message_ == _other.message_;
  }

  bool operator!=(const Msg& _other) const
  {
    return !(*this == _other);
  }

};

}

#include "dds/topic/TopicTraits.hpp"
#include "org/eclipse/cyclonedds/topic/TopicTraits.hpp"
#include "org/eclipse/cyclonedds/topic/datatopic.hpp"

namespace org {
namespace eclipse {
namespace cyclonedds {
namespace topic {
/* all traits not explicitly set are defaulted to the values in TopicTraits.hpp */

template <>
class TopicTraits<::HelloWorldData::Msg>
{
public:
  static bool isKeyless()
  {
    return false;
  }

  static const char *getTypeName()
  {
    return "HelloWorldData::Msg";
  }

  static ddsi_sertype *getSerType()
  {
    auto *st = new ddscxx_sertype<::HelloWorldData::Msg>();
    return static_cast<ddsi_sertype*>(st);
  }

  static size_t getSampleSize()
  {
    return sizeof(::HelloWorldData::Msg);
  }

  static bool isSelfContained()
  {
    return false;
  }
};

}
}
}
}

namespace dds {
namespace topic {

template <>
struct topic_type_name<::HelloWorldData::Msg>
{
    static std::string value()
    {
      return org::eclipse::cyclonedds::topic::TopicTraits<::HelloWorldData::Msg>::getTypeName();
    }
};

}
}

REGISTER_TOPIC_TYPE(::HelloWorldData::Msg)

namespace org{
namespace eclipse{
namespace cyclonedds{
namespace core{
namespace cdr{

template<typename T>
void write(T& streamer, const ::HelloWorldData::Msg& instance)
{
  write(streamer, instance.picture_width());
  write_string(streamer, instance.message(), 0);
  
}

template<typename T>
void read(T& streamer, ::HelloWorldData::Msg& instance)
{
  read(streamer, instance.picture_width());
  read_string(streamer, instance.message(), 0);
}

template<typename T>
void move(T& streamer, const ::HelloWorldData::Msg& instance)
{
  move(streamer, instance.picture_width());
  move_string(streamer, instance.message(), 0);
}

template<typename T>
void max(T& streamer, const ::HelloWorldData::Msg& instance) {
  (void)instance;
  streamer.position(SIZE_MAX);
}

template<typename T>
void key_write(T& streamer, const ::HelloWorldData::Msg& instance)
{
  write(streamer, instance.picture_width());
}

template<typename T>
void key_read(T& streamer, ::HelloWorldData::Msg& instance)
{
  read(streamer, instance.picture_width());
}

template<typename T>
void key_move(T& streamer, const ::HelloWorldData::Msg& instance)
{
  move(streamer, instance.picture_width());
}

template<typename T>
void key_max(T& streamer, const ::HelloWorldData::Msg& instance)
{
  max(streamer, instance.picture_width());
}

template<typename T>
void write_swapped(T& streamer, const ::HelloWorldData::Msg& instance)
{
  write_swapped(streamer, instance.picture_width());
  write_string_swapped(streamer, instance.message(), 0);
}

template<typename T>
void read_swapped(T& streamer, ::HelloWorldData::Msg& instance)
{
  read_swapped(streamer, instance.picture_width());
  read_string_swapped(streamer, instance.message(), 0);
}

template<typename T>
void move_swapped(T& streamer, const ::HelloWorldData::Msg& instance)
{
  move_swapped(streamer, instance.picture_width());
  move_string_swapped(streamer, instance.message(), 0);
}

template<typename T>
void max_swapped(T& streamer, const ::HelloWorldData::Msg& instance) {
  (void)instance;
  streamer.position(SIZE_MAX);
}

template<typename T>
void key_write_swapped(T& streamer, const ::HelloWorldData::Msg& instance)
{
  write_swapped(streamer, instance.picture_width());
}

template<typename T>
void key_read_swapped(T& streamer, ::HelloWorldData::Msg& instance)
{
  read_swapped(streamer, instance.picture_width());
}

template<typename T>
void key_move_swapped(T& streamer, const ::HelloWorldData::Msg& instance)
{
  move_swapped(streamer, instance.picture_width());
}

template<typename T>
void key_max_swapped(T& streamer, const ::HelloWorldData::Msg& instance)
{
  max_swapped(streamer, instance.picture_width());
}

} //namespace cdr
} //namespace core
} //namespace cyclonedds
} //namespace eclipse
} //namespace org

#endif // DDSCXX_HELLOWORLDDATA_HPP
