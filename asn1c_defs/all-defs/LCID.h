/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "X2AP-IEs"
 * 	found in "../../asn_defs/asn1/x2ap-modified-15-05.asn"
 * 	`asn1c -fcompound-names -fno-include-deps -findirect-choice -gen-PER -no-gen-OER`
 */

#ifndef	_LCID_H_
#define	_LCID_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LCID */
typedef long	 LCID_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LCID;
asn_struct_free_f LCID_free;
asn_struct_print_f LCID_print;
asn_constr_check_f LCID_constraint;
ber_type_decoder_f LCID_decode_ber;
der_type_encoder_f LCID_encode_der;
xer_type_decoder_f LCID_decode_xer;
xer_type_encoder_f LCID_encode_xer;
per_type_decoder_f LCID_decode_uper;
per_type_encoder_f LCID_encode_uper;
per_type_decoder_f LCID_decode_aper;
per_type_encoder_f LCID_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _LCID_H_ */
#include <asn_internal.h>