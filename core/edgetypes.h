#ifndef _EDGETYPES_H_
#define _EDGETYPES_H_

#include <stdlib.h>
#include <string.h>
#include <statetypes.h>

typedef enum {
  PL_STREET,
  PL_TRIPHOPSCHED,
  PL_TRIPHOP,
  PL_LINK,
  PL_RUBYVALUE,
  PL_NONE,
} edgepayload_t;

// ------------ DECLARATIONS FOR GEOM --------------------------

typedef struct Geom {
	char *data;
}Geom;

Geom*
geomNew (char * geomdata);

void
geomDestroy(Geom* this);


//--------------DECLARATIONS FOR COORDINATES---------------------
typedef struct Coordinates {
   long lat;
   long lon;
}Coordinates;

Coordinates*
coordinatesNew(long latitude,long length);

void 
coordinatesDestroy(Coordinates* this);

Coordinates*
coordinatesDup(Coordinates* this);

//---------------DECLARATIONS FOR STATE CLASS---------------------

typedef struct State {
   long          time;           //seconds since the epoch
   long          weight;
   double        dist_walked;    //meters
   int           num_transfers;
   edgepayload_t prev_edge_type;
   char*         prev_edge_name;
   CalendarDay*  calendar_day;
} State;

State*
stateNew(long time);


void
stateDestroy();

State*
stateDup( State* this );

//---------------DECLARATIONS FOR EDGEPAYLOAD CLASS---------------------

typedef struct EdgePayload {
  edgepayload_t type;
} EdgePayload;

EdgePayload*
epNew();

void
epDestroy( EdgePayload* this );

State*
epWalk( EdgePayload* this, State* param );

State*
epWalkBack( EdgePayload* this, State* param );

EdgePayload*
epCollapse( EdgePayload* this, State* param );

EdgePayload*
epCollapseBack( EdgePayload* this, State* param );



//---------------DECLARATIONS FOR LINK  CLASS---------------------

typedef struct Link {
  edgepayload_t type;
  char* name;
} Link;

Link*
linkNew();

void
linkDestroy(Link* tokill);

inline State*
linkWalk(Link* this, State* param);

inline State*
linkWalkBack(Link* this, State* param);

//---------------DECLARATIONS FOR STREET  CLASS---------------------

typedef struct Street {
   edgepayload_t type;      //type in this case PL_STREET
   char* name;              //Name del street
   double length;           //length metres???
} Street;

Street*
streetNew(const char *name, double length);

void
streetDestroy(Street* tokill);


inline State*
streetWalk(Street* this, State* params);

inline State*
streetWalkBack(Street* this, State* params);

//---------------DECLARATIONS FOR TRIPHOPSCHEDULE and TRIPHOP  CLASSES---------------------

#define INFINITY 100000000
#define SECONDS_IN_WEEK 604800
#define SECONDS_IN_DAY 86400
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60
#define DAYS_IN_WEEK 7

typedef struct TripHopSchedule TripHopSchedule;

typedef struct TripHop {
  edgepayload_t type;
  int depart;
  int arrive;
  int transit;
  char* trip_id;
  TripHopSchedule* schedule;
} TripHop;

struct TripHopSchedule {
  edgepayload_t type;
  int n;
  TripHop* hops;
  ServiceId service_id;
  CalendarDay* calendar;
  int timezone_offset; //number of seconds this schedule is offset from GMT, eg. -8*3600=-28800 for US West Coast
};

TripHopSchedule*
thsNew( int *departs, int *arrives, char **trip_ids, int n, ServiceId service_id, CalendarDay* calendar, int timezone_offset );

void
thsDestroy(TripHopSchedule* this);

inline State*
thsWalk(TripHopSchedule* this, State* params);

inline State*
thsWalkBack(TripHopSchedule* this, State* params);

inline State*
triphopWalk( TripHop* this, State* params );

inline State*
triphopWalkBack( TripHop* this, State* params );

inline TripHop*
thsCollapse( TripHopSchedule* this, State* params );

inline TripHop*
thsCollapseBack( TripHopSchedule* this, State* params );

//convert time, N seconds since the epoch, to seconds since midnight within the span of the service day
inline long
thsSecondsSinceMidnight( TripHopSchedule* this, State* param );

inline TripHop* 
thsGetNextHop(TripHopSchedule* this, long time);

inline TripHop*
thsGetLastHop(TripHopSchedule* this, long time);

void
triphopDestroy(TripHop* this);

#endif
