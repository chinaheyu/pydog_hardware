#ifndef _ROS_olfaction_msgs_anemometer_h
#define _ROS_olfaction_msgs_anemometer_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace olfaction_msgs
{

  class anemometer : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef const char* _sensor_label_type;
      _sensor_label_type sensor_label;
      typedef float _wind_speed_type;
      _wind_speed_type wind_speed;
      typedef float _wind_direction_type;
      _wind_direction_type wind_direction;

    anemometer():
      header(),
      sensor_label(""),
      wind_speed(0),
      wind_direction(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      uint32_t length_sensor_label = strlen(this->sensor_label);
      varToArr(outbuffer + offset, length_sensor_label);
      offset += 4;
      memcpy(outbuffer + offset, this->sensor_label, length_sensor_label);
      offset += length_sensor_label;
      union {
        float real;
        uint32_t base;
      } u_wind_speed;
      u_wind_speed.real = this->wind_speed;
      *(outbuffer + offset + 0) = (u_wind_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wind_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_wind_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_wind_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wind_speed);
      union {
        float real;
        uint32_t base;
      } u_wind_direction;
      u_wind_direction.real = this->wind_direction;
      *(outbuffer + offset + 0) = (u_wind_direction.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_wind_direction.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_wind_direction.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_wind_direction.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->wind_direction);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t length_sensor_label;
      arrToVar(length_sensor_label, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_sensor_label; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_sensor_label-1]=0;
      this->sensor_label = (char *)(inbuffer + offset-1);
      offset += length_sensor_label;
      union {
        float real;
        uint32_t base;
      } u_wind_speed;
      u_wind_speed.base = 0;
      u_wind_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wind_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_wind_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_wind_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->wind_speed = u_wind_speed.real;
      offset += sizeof(this->wind_speed);
      union {
        float real;
        uint32_t base;
      } u_wind_direction;
      u_wind_direction.base = 0;
      u_wind_direction.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_wind_direction.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_wind_direction.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_wind_direction.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->wind_direction = u_wind_direction.real;
      offset += sizeof(this->wind_direction);
     return offset;
    }

    const char * getType(){ return "olfaction_msgs/anemometer"; };
    const char * getMD5(){ return "a6eda3f54da66470bd27452be26d76e3"; };

  };

}
#endif
