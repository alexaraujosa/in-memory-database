#ifndef TP_QUERY_COMMON_H
#define TP_QUERY_COMMON_H

#include "catalog/catalogManager.h"
#include "catalog/flightsCatalog.h"
#include "catalog/reservationsCatalog.h"
#include "catalog/usersCatalog.h"
#include "collections/flight.h"
#include "collections/passenger.h"
#include "collections/reservation.h"
#include "collections/user.h"

#include "common.h"
#include "debug.h"

typedef void(*QueryWriter)(int, char, void*, FILE*, int);

#endif