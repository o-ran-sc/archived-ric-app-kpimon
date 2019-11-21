/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-modified-15-05.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#ifndef	_DataTrafficResourceIndication_H_
#define	_DataTrafficResourceIndication_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include "SharedResourceType.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ReservedSubframePattern;
struct ProtocolExtensionContainer;

/* DataTrafficResourceIndication */
typedef struct DataTrafficResourceIndication {
	long	 activationSFN;
	SharedResourceType_t	 sharedResourceType;
	struct ReservedSubframePattern	*reservedSubframePattern;	/* OPTIONAL */
	struct ProtocolExtensionContainer	*iE_Extensions;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DataTrafficResourceIndication_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DataTrafficResourceIndication;

#ifdef __cplusplus
}
#endif

#endif	/* _DataTrafficResourceIndication_H_ */
#include <asn_internal.h>