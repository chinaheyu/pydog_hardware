#ifndef _ROS_olfaction_msgs_gas_sensor_h
#define _ROS_olfaction_msgs_gas_sensor_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace olfaction_msgs
{

  class gas_sensor : public ros::Msg
  {
    public:
      typedef std_msgs::Header _header_type;
      _header_type header;
      typedef uint8_t _technology_type;
      _technology_type technology;
      typedef uint8_t _manufacturer_type;
      _manufacturer_type manufacturer;
      typedef uint8_t _mpn_type;
      _mpn_type mpn;
      typedef double _raw_type;
      _raw_type raw;
      typedef uint8_t _raw_units_type;
      _raw_units_type raw_units;
      typedef double _raw_air_type;
      _raw_air_type raw_air;
      typedef double _calib_A_type;
      _calib_A_type calib_A;
      typedef double _calib_B_type;
      _calib_B_type calib_B;
      enum { TECH_UNKNOWN =  0 };
      enum { TECH_MOX =  1 };
      enum { TECH_AEC =  2 };
      enum { TECH_EQ =  50 };
      enum { TECH_PID =  51 };
      enum { TECH_SAW =  52 };
      enum { TECH_TEMP =  100 };
      enum { TECH_HUMIDITY =  101 };
      enum { TECH_NOT_VALID =  255 };
      enum { MANU_UNKNOWN =  0 };
      enum { MANU_FIGARO =  1 };
      enum { MANU_ALPHASENSE =  2 };
      enum { MANU_SGX =  3 };
      enum { MANU_RAE =  50 };
      enum { MANU_HANWEI =  51 };
      enum { MANU_NOT_VALID =  255 };
      enum { MPN_UNKNOWN =  0 };
      enum { MPN_TGS2620 =  50 };
      enum { MPN_TGS2600 =  51 };
      enum { MPN_TGS2611 =  52 };
      enum { MPN_TGS2610 =  53 };
      enum { MPN_TGS2612 =  54 };
      enum { MPN_MINIRAELITE =  70 };
      enum { MPN_NOT_VALID =  255 };
      enum { UNITS_UNKNOWN =  0 };
      enum { UNITS_VOLT =  1 };
      enum { UNITS_AMP =  2 };
      enum { UNITS_PPM =  3 };
      enum { UNITS_PPB =  4 };
      enum { UNITS_OHM =  5 };
      enum { UNITS_CENTIGRADE =  100 };
      enum { UNITS_RELATIVEHUMIDITY =  101 };
      enum { UNITS_NOT_VALID =  255 };

    gas_sensor():
      header(),
      technology(0),
      manufacturer(0),
      mpn(0),
      raw(0),
      raw_units(0),
      raw_air(0),
      calib_A(0),
      calib_B(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->technology >> (8 * 0)) & 0xFF;
      offset += sizeof(this->technology);
      *(outbuffer + offset + 0) = (this->manufacturer >> (8 * 0)) & 0xFF;
      offset += sizeof(this->manufacturer);
      *(outbuffer + offset + 0) = (this->mpn >> (8 * 0)) & 0xFF;
      offset += sizeof(this->mpn);
      union {
        double real;
        uint64_t base;
      } u_raw;
      u_raw.real = this->raw;
      *(outbuffer + offset + 0) = (u_raw.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_raw.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_raw.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_raw.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_raw.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_raw.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_raw.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_raw.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->raw);
      *(outbuffer + offset + 0) = (this->raw_units >> (8 * 0)) & 0xFF;
      offset += sizeof(this->raw_units);
      union {
        double real;
        uint64_t base;
      } u_raw_air;
      u_raw_air.real = this->raw_air;
      *(outbuffer + offset + 0) = (u_raw_air.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_raw_air.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_raw_air.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_raw_air.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_raw_air.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_raw_air.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_raw_air.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_raw_air.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->raw_air);
      union {
        double real;
        uint64_t base;
      } u_calib_A;
      u_calib_A.real = this->calib_A;
      *(outbuffer + offset + 0) = (u_calib_A.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_calib_A.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_calib_A.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_calib_A.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_calib_A.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_calib_A.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_calib_A.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_calib_A.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->calib_A);
      union {
        double real;
        uint64_t base;
      } u_calib_B;
      u_calib_B.real = this->calib_B;
      *(outbuffer + offset + 0) = (u_calib_B.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_calib_B.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_calib_B.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_calib_B.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_calib_B.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_calib_B.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_calib_B.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_calib_B.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->calib_B);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->technology =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->technology);
      this->manufacturer =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->manufacturer);
      this->mpn =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->mpn);
      union {
        double real;
        uint64_t base;
      } u_raw;
      u_raw.base = 0;
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_raw.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->raw = u_raw.real;
      offset += sizeof(this->raw);
      this->raw_units =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->raw_units);
      union {
        double real;
        uint64_t base;
      } u_raw_air;
      u_raw_air.base = 0;
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_raw_air.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->raw_air = u_raw_air.real;
      offset += sizeof(this->raw_air);
      union {
        double real;
        uint64_t base;
      } u_calib_A;
      u_calib_A.base = 0;
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_calib_A.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->calib_A = u_calib_A.real;
      offset += sizeof(this->calib_A);
      union {
        double real;
        uint64_t base;
      } u_calib_B;
      u_calib_B.base = 0;
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_calib_B.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->calib_B = u_calib_B.real;
      offset += sizeof(this->calib_B);
     return offset;
    }

    const char * getType(){ return "olfaction_msgs/gas_sensor"; };
    const char * getMD5(){ return "f19a4378539410bc0da063e4424de564"; };

  };

}
#endif
