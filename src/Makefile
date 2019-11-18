CXX:= g++ --std=c++14 -O2
CC:= gcc -O2

SRC:=./
E2AP_c:=./E2AP-c
E2SM := ./E2SM
SUBSCR:=$(E2AP_c)/subscription
ASN1C_DEFS:=../asn1c_defs/all-defs
KPI_HANDLER:=./KPI-Message-Handler
KPI_MONITOR:=./KPI-Monitoring

# Logging library and flags
CLOGFLAGS:= `pkg-config mdclog --cflags`
LOG_LIBS:= `pkg-config mdclog --libs`
HIREDIS_LIBS:= `pkg-config hiredis --libs`

# Keep include dirs separate so we have transparency
BASEFLAGS=  -Wall  -std=c++14 $(CLOGFLAGS) 

XAPPFLAGS= -I./
ASN1C_FLAGS=-I$(ASN1C_DEFS) -DASN_DISABLE_OER_SUPPORT
SUBSCRFLAGS=  -I$(SUBSCR)
E2AP_C_FLAGS = -I$(E2AP_c)
E2SM_FLAGS = -I$(E2SM)
KPI_HANDLER_FLAGS = -I$(KPI_HANDLER)
KPI_MONITOR_FLAGS = -I$(KPI_MONITOR)
C_BASEFLAGS=-Wall $(CLOGFLAGS) -DASN_DISABLE_OER_SUPPORT

LIBS= -lrmr_nng -lnng -lpthread -lm $(LOG_LIBS) $(HIREDIS_LIBS)
COV_FLAGS= -fprofile-arcs -ftest-coverage

XAPP_SRC= kpi-xapp.cc \
     xapp_utils.cc \
     message_processor_class.cc \
     get_config.cc

SUBSCR_SRC= $(wildcard $(SUBSCR)/*.cc)
E2AP_C_SRC =$(wildcard $(E2AP_c)/*.cc)
E2SM_SRC = $(wildcard $(E2SM)/*.cc)
ASN1C_SRC= $(wildcard $(ASN1C_DEFS)/*.c)
KPI_HANDLER_SRC = $(wildcard $(KPI_HANDLER)/*.cc)
KPI_MONITOR_SRC = $(wildcard $(KPI_MONITOR)/*.cc)

XAPP_OBJ= ${XAPP_SRC:.cc=.o} 
SUBSCR_OBJ= ${SUBSCR_SRC:.cc=.o} 
E2AP_C_OBJ = $(E2AP_C_SRC:.cc=.o)
E2SM_OBJ = $(E2SM_SRC:.cc=.o)
KPI_HANDLER_OBJ = $(KPI_HANDLER_SRC:.cc=.o)
KPI_MONITOR_OBJ = $(KPI_MONITOR_SRC:.cc=.o)
ASN1C_MODULES = $(ASN1C_SRC:.c=.o)

$(ASN1C_MODULES): export CFLAGS = $(C_BASEFLAGS) $(ASN1C_FLAGS)
$(SUBSCR_OBJ): export CPPFLAGS=$(BASEFLAGS) $(ASN1C_FLAGS) $(E2AP_C_FLAGS) $(SUBSCRFLAGS)
$(E2AP_C_OBJ): export CPPFLAGS = $(BASEFLAGS) $(ASN1C_FLAGS) $(E2AP_C_FLAGS)
$(E2SM_OBJ): export CPPFLAGS = $(BASEFLAGS) $(ASN1C_FLAGS) $(E2SM_FLAGS)
$(KPI_HANDLER_OBJ): export CPPFLAGS = $(BASEFLAGS) $(ASN1C_FLAGS) $(KPI_HANDLER_FLAGS) $(KPI_MONITOR_FLAGS)
$(KPI_MONITOR_OBJ): export CPPFLAGS = $(BASEFLAGS) $(ASN1C_FLAGS) $(KPI_MONITOR_FLAGS)
$(XAPP_OBJ):export CPPFLAGS=$(BASEFLAGS) $(SUBSCRFLAGS) $(E2AP_C_FLAGS) $(ASN1C_FLAGS) $(E2SM_FLAGS) $(XAPPFLAGS) $(KPI_HANDLER_FLAGS) $(KPI_MONITOR_FLAGS)

OBJ= $(SUBSCR_OBJ) $(XAPP_OBJ) $(E2AP_C_OBJ) $(ASN1C_MODULES) $(E2SM_OBJ) $(KPI_HANDLER_OBJ) $(KPI_MONITOR_OBJ)

kpi-xapp: $(OBJ)
	$(CXX) -o $@  $(OBJ) $(LIBS) $(CPPFLAGS) $(CLOGFLAGS) 

install: kpi-xapp
	install  -D kpi-xapp  /usr/local/bin/kpi-xapp

clean:
	-rm *.o $(ASN1C_DEFS)/*.o $(E2AP_c)/*.o $(SUBSCR)/*.o $(E2SM)/*.o $(KPI_HANDLER)/*.o $(KPI_MONITOR)/*.o kpi-xapp

