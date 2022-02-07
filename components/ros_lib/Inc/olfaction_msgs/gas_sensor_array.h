#ifndef _ROS_olfaction_msgs_gas_sensor_array_h
#define _ROS_olfaction_msgs_gas_sensor_array_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"
#include "olfaction_msgs/gas_sensor.h"

namespace olfaction_msgs
{

  class gas_sensor_array : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      uint32_t sensors_length;
      typedef olfaction_msgs::gas_sensor _sensors_type;
      _sensors_type st_sensors;
      _sensors_type * sensors;

    gas_sensor_array():
      header(),
      sensors_length(0), sensors(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->sensors_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->sensors_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->sensors_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->sensors_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->sensors_length);
      for( uint32_t i = 0; i < sensors_length; i++){
      offset += this->sensors[i].serialize(outbuffer + offset);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t sensors_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      sensors_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      sensors_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      sensors_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->sensors_length);
      if(sensors_lengthT > sensors_length)
        this->sensors = (olfaction_msgs::gas_sensor*)realloc(this->sensors, sensors_lengthT * sizeof(olfaction_msgs::gas_sensor));
      sensors_length = sensors_lengthT;
      for( uint32_t i = 0; i < sensors_length; i++){
      offset += this->st_sensors.deserialize(inbuffer + offset);
        memcpy( &(this->sensors[i]), &(this->st_sensors), sizeof(olfaction_msgs::gas_sensor));
      }
     return offset;
    }

    const char * getType(){ return "olfaction_msgs/gas_sensor_array"; };
    const char * getMD5(){ return "2f2312a6b81ab41737f07bd20ae43828"; };

  };

}
#endif
