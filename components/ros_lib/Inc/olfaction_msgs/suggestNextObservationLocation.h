#ifndef _ROS_SERVICE_suggestNextObservationLocation_h
#define _ROS_SERVICE_suggestNextObservationLocation_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "nav_msgs/Odometry.h"

namespace olfaction_msgs
{

static const char SUGGESTNEXTOBSERVATIONLOCATION[] = "olfaction_msgs/suggestNextObservationLocation";

  class suggestNextObservationLocationRequest : public ros::Msg
  {
    public:

    suggestNextObservationLocationRequest()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
     return offset;
    }

    const char * getType(){ return SUGGESTNEXTOBSERVATIONLOCATION; };
    const char * getMD5(){ return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class suggestNextObservationLocationResponse : public ros::Msg
  {
    public:
      typedef nav_msgs::Odometry _odom_type;
      _odom_type odom;

    suggestNextObservationLocationResponse():
      odom()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->odom.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->odom.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return SUGGESTNEXTOBSERVATIONLOCATION; };
    const char * getMD5(){ return "75d9701e058ed9a7608ba3dbc16b5b7e"; };

  };

  class suggestNextObservationLocation {
    public:
    typedef suggestNextObservationLocationRequest Request;
    typedef suggestNextObservationLocationResponse Response;
  };

}
#endif
