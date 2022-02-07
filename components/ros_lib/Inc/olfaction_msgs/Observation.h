#ifndef _ROS_olfaction_msgs_Observation_h
#define _ROS_olfaction_msgs_Observation_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Vector3.h"

namespace olfaction_msgs
{

  class Observation : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef const char* _label_type;
      _label_type label;
      typedef geometry_msgs::Point _position_type;
      _position_type position;
      typedef uint8_t _type_type;
      _type_type type;
      typedef float _gas_type;
      _gas_type gas;
      typedef geometry_msgs::Vector3 _wind_type;
      _wind_type wind;
      enum { TYPE_INVALID = 0 };
      enum { TYPE_GAS = 1 };
      enum { TYPE_WIND = 2 };
      enum { TYPE_GAS_WIND = 3 };

    Observation():
      header(),
      label(""),
      position(),
      type(0),
      gas(0),
      wind()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      uint32_t length_label = strlen(this->label);
      varToArr(outbuffer + offset, length_label);
      offset += 4;
      memcpy(outbuffer + offset, this->label, length_label);
      offset += length_label;
      offset += this->position.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->type);
      union {
        float real;
        uint32_t base;
      } u_gas;
      u_gas.real = this->gas;
      *(outbuffer + offset + 0) = (u_gas.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_gas.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_gas.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_gas.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->gas);
      offset += this->wind.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t length_label;
      arrToVar(length_label, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_label; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_label-1]=0;
      this->label = (char *)(inbuffer + offset-1);
      offset += length_label;
      offset += this->position.deserialize(inbuffer + offset);
      this->type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->type);
      union {
        float real;
        uint32_t base;
      } u_gas;
      u_gas.base = 0;
      u_gas.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_gas.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_gas.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_gas.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->gas = u_gas.real;
      offset += sizeof(this->gas);
      offset += this->wind.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "olfaction_msgs/Observation"; };
    const char * getMD5(){ return "69bade18f26a947c564210f213ae07bf"; };

  };

}
#endif
